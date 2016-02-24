#include "MapWindow.h"
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include "CoordsMkr.h"
#include "IghCoords.h"
#include "Features.h"

std::unordered_map<int, QImage*> MapWindow::mapTileImages;

MapWindow::MapWindow() : QDialog() {
    loadButton = new QPushButton("Load", this);
    QImage myImage(800, 800, QImage::Format_RGB888);
    //myImage->load("F:/2.png");
    imageLabel = new QLabel("");
    imageLabel->setContentsMargins(0,0,0,0);
    imageLabel->setPixmap(QPixmap::fromImage(myImage));
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(loadButton);
    QLabel *colorLabel = new QLabel("Color: ");
    colorLabel->setMaximumWidth(50);
    QComboBox *colorCombo = new QComboBox();
    colorCombo->setMaximumWidth(100);
    colorCombo->setStyleSheet("combobox-popup: 0;");
    colorCombo->addItem("standard", 0);
    colorCombo->addItem("invert", 1);
    QGridLayout *vlist3 = new QGridLayout;
    vlist3->setSpacing(2);
    vlist3->setContentsMargins(3,0,1,0);    
    vlist3->addWidget(loadButton,0,0);
    vlist3->addWidget(colorLabel,0,1);
    vlist3->addWidget(colorCombo,0,2);
    mainLayout->addItem(vlist3);
    mainLayout->addWidget(imageLabel);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);
    
    
    /*    QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item(QPixmap("F:/2.png"));
    scene.addItem(&item);
    view.show();*/
    //load();
    //dane.draw(myImage);
    //imageLabel->setPixmap(QPixmap::fromImage(*myImage));
    
    QObject::connect(loadButton, SIGNAL(released()),
                      this, SLOT(loadOSM()));
    
    //
        
    QObject::connect(colorCombo, SIGNAL(activated(QString)),
                      this, SLOT(colorComboActivated(QString)));
}

int MapWindow::exec() {
    this->setWindowTitle("Tile: " + QString::number(this->tileX) + " " + QString::number(-this->tileZ));
    return QDialog::exec();
} 

void MapWindow::colorComboActivated(QString val){
    if(val == "standard")
        this->invert = false;
    if(val == "invert")
        this->invert = true;
    reload();
}

void MapWindow::loadOSM(){
    if(aCoords == NULL) aCoords = new PreciseTileCoordinate();
    aCoords->TileX = this->tileX;
    aCoords->TileZ = -this->tileZ;
    qDebug() << this->tileX << " " << -this->tileZ;;
    aCoords->setWxyz(-1024, 0, -1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    minLatlon = MstsCoordinates::ConvertToLatLon(igh);
    aCoords->setWxyz(1024, 0, 1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    maxLatlon = MstsCoordinates::ConvertToLatLon(igh);
    qDebug() << "lat " << minLatlon->Latitude << " " << maxLatlon->Latitude;
    qDebug() << "lon " << minLatlon->Longitude << " " << maxLatlon->Longitude;
    aCoords->setWxyz(-1024, 0, 1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    minLatlon = MstsCoordinates::ConvertToLatLon(igh);
    aCoords->setWxyz(1024, 0, -1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    maxLatlon = MstsCoordinates::ConvertToLatLon(igh);
    qDebug() << "lat " << minLatlon->Latitude << " " << maxLatlon->Latitude;
    qDebug() << "lon " << minLatlon->Longitude << " " << maxLatlon->Longitude;
    get();
}

void MapWindow::get(){
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(isData(QNetworkReply*)));
    // the HTTP request
    qDebug() << "wait";
    QNetworkRequest req( QUrl( QString("http://www.openstreetmap.org/api/0.6/map?bbox="
    +QString::number(minLatlon->Longitude)
    +","
    +QString::number(minLatlon->Latitude)
    +","
    +QString::number(maxLatlon->Longitude)
    +","
    +QString::number(maxLatlon->Latitude)
    ) ) );
    loadButton->setText("Wait ...");
    mgr->get(req);
}

void MapWindow::isData(QNetworkReply* r){
    QByteArray data = r->readAll();
    qDebug() << "data " << data.length();    
    loadButton->setText("Load");
    load(&data);
    reload();
}

void MapWindow::reload(){
    if(dane.nodes.size() == 0) return;
    if(dane.tileX != this->tileX) return;
    if(dane.tileZ != -this->tileZ) return;
    QImage* myImage = new QImage(4096, 4096, QImage::Format_RGB888);
    dane.draw(myImage);
    //myImage->save(QString::number(dane.tileX)+"_"+QString::number(dane.tileZ)+"_d.png");
    if(this->invert)
        myImage->invertPixels(QImage::InvertRgba); 
    //myImage->save(QString::number(dane.tileX)+"_"+QString::number(dane.tileZ)+"_i.png");
    
    imageLabel->setPixmap(QPixmap::fromImage(*myImage).scaled(800,800,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    int hash = (int)(this->tileX)*10000+(int)(this->tileZ);
    if(MapWindow::mapTileImages[hash] != NULL)
        delete MapWindow::mapTileImages[hash];
    MapWindow::mapTileImages[hash] = myImage;    
}

void MapWindow::load(QByteArray* data){
    if(data == NULL){
        QFile file("F:/OSM/tczew.osm");
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "no file" << file.errorString();
            exit(0);
        }
        qDebug() <<  "file";
        QByteArray data2 = file.readAll();
        data = &data2;
    } 
    
    int inode = 0;
    int iway = 0;
    bool node = true;
    bool way = false;
    bool bounds = false;
    int iii = 0, uuu = 0;
    
    Node* tnode;
    Way* tway;
    
    QXmlStreamReader reader((*data));
    reader.readNext();
    QString name;
    QXmlStreamAttributes attr;
    while (!reader.isEndDocument()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            attr = reader.attributes();
            
            if (name.toUpper() == ("NODE")) {
                node = true;
                if (inode++ % 100000 == 0) qDebug() << "n " << inode;
                tnode = new Node(
                        attr.value("id").toULongLong(),
                        attr.value("lat").toFloat(),
                        attr.value("lon").toFloat()
                        );
            } else if (name.toUpper() == ("WAY")) {
                way = true;
                if (iway++ % 100000 == 0) qDebug() << "w " << iway;
                tway = new Way(attr.value("id").toULongLong());
                tway->ref.clear();
            } else if (name.toUpper() == ("ND") && way) {
                tway->ref.push_back ((attr.value("ref").toLongLong()));
            } else if (name.toUpper() == ("TAG")&&(way || node)) {
                //adres
                if (attr.value("k").startsWith("ADDR", Qt::CaseInsensitive)) {
                }
                //nazwa
                else if (attr.value("k").startsWith("NAME", Qt::CaseInsensitive)) {
                }
                //drogi
                else if (attr.value("k").startsWith("ONEWAY", Qt::CaseInsensitive)) {}
                else if (attr.value("k").startsWith("MAXSPEED", Qt::CaseInsensitive)) {}
                else if (attr.value("k").startsWith("SURFACE", Qt::CaseInsensitive))  {}
                else if (attr.value("k").startsWith("BRIDGE", Qt::CaseInsensitive)) {
                    if (way) tway->val2 = 7;
                }
                else if (attr.value("k").startsWith("TUNNEL", Qt::CaseInsensitive)) {
                    if (way) tway->val2 = 6;
                }
                //miejsca
                else if (attr.value("k").startsWith("AMENITY", Qt::CaseInsensitive)) {
                    //System.out.println(attr.getValue("v").toUpperCase());
                    //if(node) {
                    //    uuu++;
                    //    dane.miejsca.add(new Miejsce(tnode.lat, tnode.lon, attr.getValue("v").toUpperCase().replace("_", "")));
                    //}
                }
                //bariery
                else if (attr.value("k").startsWith("BARRIER", Qt::CaseInsensitive)) {}
                //las
                else if (attr.value("k").startsWith("WOOD", Qt::CaseInsensitive)) {}
                //sport
                else if (attr.value("k").startsWith("SPORT", Qt::CaseInsensitive)) {}
                else {
                    //inne budynki 
                    if (attr.value("k").startsWith("BUILDING", Qt::CaseInsensitive)) {
                        if (way) tway->type = (short) Features::LIST["BUILDING_YES"];
                        iii++;
                    }
                    //jakies gowna co nie chce
                    //if(attr.getValue("k").toUpperCase().startsWith("SOURCE", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("CREATED", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("EWMAPA", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("BUILDING:", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("REF", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("WIKI", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("ACCESS", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("NOTE", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("TRACKTYPE", 0)) return;
                    //inne -> enum
                    QString fname = "";
                    fname += attr.value("k").toString() + "_" + attr.value("v").toString();
                    fname = fname.toUpper();
                    if (Features::LIST[fname.toStdString()] != 0) {
                        if (way) tway->type = (short) Features::LIST[fname.toStdString()];
                        if (node) tnode->type = (short) Features::LIST[fname.toStdString()];
                        iii++; //System.out.println(fname);
                    } else {
                        //if(!fname.startsWith("source", Qt::CaseInsensitive)
                        //    && !fname.startsWith("created", Qt::CaseInsensitive)
                        //    )
                        //    qDebug() << "fail: " << fname;
                    }
                }
            } else if (name.toUpper() == ("BOUNDS")) {
                bounds = true;
                dane.minlat = attr.value("minlat").toFloat();
                dane.minlon = attr.value("minlon").toFloat();
                dane.maxlat = attr.value("maxlat").toFloat();
                dane.maxlon = attr.value("maxlon").toFloat();
                qDebug() << "minmax";
                qDebug() << dane.minlat << " " << dane.maxlat;
                qDebug() << dane.minlon << " " << dane.maxlon;
            }
        } else if (reader.isEndElement()) {
            name = reader.name().toString();
            if (name.toUpper() == ("NODE")) {
                dane.nodes[tnode->id] = tnode;
                node = false;
            }
            if (name.toUpper() == ("WAY")) {
                //qDebug() << Features::LAYER[tway->type];
                int tlayer = Features::LAYER[tway->type];
                if(tlayer > 9) tlayer = 9;
                if(tway->val2==7) 
                    dane.ways[9].push_back(tway);
                else
                    dane.ways[9-tlayer].push_back(tway);
                way = false;
            }
            if (name.toUpper() == ("BOUNDS")) {
                bounds = false;
            }
        } else if (reader.isCharacters()) {

        }
        
        reader.readNext();
    }
    qDebug() << "node/way: " << inode << "/" << iway;
    dane.tileX = this->tileX;
    dane.tileZ = -this->tileZ;
    dane.minlon = this->minLatlon->Longitude;
    dane.minlat = this->minLatlon->Latitude;
    dane.maxlon = this->maxLatlon->Longitude;
    dane.maxlat = this->maxLatlon->Latitude;
}

MapWindow::~MapWindow() {
}

