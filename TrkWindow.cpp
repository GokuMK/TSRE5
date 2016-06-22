#include "TrkWindow.h"
#include <QDebug>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "Trk.h"
#include "GuiFunct.h"
#include "TexLib.h"
#include "Texture.h"
#include "Game.h"
#include <QImage>

TrkWindow::TrkWindow() : QDialog(){
    setWindowFlags(Qt::WindowStaysOnTopHint);
    //QPushButton *loadButton = new QPushButton("Load", this);
    //QImage myImage(800, 800, QImage::Format_RGB888);
    //myImage->load("F:/2.png");
    imageGraphic.setFixedSize(640,450);
    imageLoad.setFixedSize(640,450);
    //imageLabel->setContentsMargins(0,0,0,0);
    //imageLabel->setPixmap(QPixmap::fromImage(myImage));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *main2Layout = new QHBoxLayout;
    mainLayout->addItem(main2Layout);
    mainLayout->addWidget(&description);
    
    QVBoxLayout *tab1 = new QVBoxLayout;
    QGridLayout *settings = new QGridLayout;
    settings->setSpacing(2);
    settings->setContentsMargins(3,0,1,0);    
    int row = 0;
    idName.setMinimumWidth(200);
    
    settings->addWidget(GuiFunct::newTQLabel("Route names"), row++, 0);
    settings->addWidget(new QLabel("Route ID: "), row++, 0);
    settings->addWidget(new QLabel("File names: "), row++, 0);
    settings->addWidget(new QLabel("Display name: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Electricity"), row++, 0);
    settings->addWidget(new QLabel("Electrified: "), row++, 0);
    settings->addWidget(new QLabel("Wire Height: "), row++, 0);
    settings->addWidget(new QLabel("Max Voltage: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Start Tile"), row++, 0);
    settings->addWidget(new QLabel("Tile X: "), row++, 0);
    settings->addWidget(new QLabel("Tile Z: "), row++, 0);
    settings->addWidget(new QLabel("Position X: "), row++, 0);
    settings->addWidget(new QLabel("Position Z: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Other"), row++, 0);
    settings->addWidget(new QLabel("Speed Limit (km/h): "), row++, 0);
    settings->addWidget(new QLabel("Restricted Speed (km/h): "), row++, 0);
    settings->addWidget(new QLabel("Speed in Miles: "), row++, 0);
    settings->addWidget(new QLabel("Terrain Error Scale: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Environment"), row++, 0);
    settings->addWidget(&envName, row++, 0);
    QObject::connect(&envName, SIGNAL(activated(QString)), this, SLOT(envNameEnabled(QString)));
    envName.setStyleSheet("combobox-popup: 0;");
    settings->addWidget(GuiFunct::newTQLabel("Description"), row++, 0);
    row = 0;
    row++;
    settings->addWidget(&idName, row++, 1);
    idName.setEnabled(false);
    settings->addWidget(&routeName, row++, 1);
    routeName.setEnabled(false);
    settings->addWidget(&displayName, row++, 1);
    row++;
    settings->addWidget(&electrified, row++, 1);
    electrified.setStyleSheet("combobox-popup: 0;");
    electrified.addItem("No", 0);
    electrified.addItem("Yes", 1);
    settings->addWidget(&overheadWireHeight, row++, 1);
    overheadWireHeight.setRange(0, 10000);
    overheadWireHeight.setSingleStep(0.5);
    settings->addWidget(&maxLineVoltage, row++, 1);
    maxLineVoltage.setRange(0, 100000);
    maxLineVoltage.setSingleStep(100);
    row++;
    settings->addWidget(&startTileX, row++, 1);
    settings->addWidget(&startTileZ, row++, 1);
    settings->addWidget(&startpX, row++, 1);
    settings->addWidget(&startpZ, row++, 1);
    row++;
    settings->addWidget(&speedLimit, row++, 1);
    speedLimit.setRange(0, 1000);
    speedLimit.setSingleStep(5);
    settings->addWidget(&tempRestrictedSpeed, row++, 1);
    tempRestrictedSpeed.setRange(0, 1000);
    tempRestrictedSpeed.setSingleStep(5);
    settings->addWidget(&milepostUnitsKilometers, row++, 1);
    milepostUnitsKilometers.setStyleSheet("combobox-popup: 0;");
    milepostUnitsKilometers.addItem("No", 0);
    milepostUnitsKilometers.addItem("Yes", 1);
    settings->addWidget(&terrainErrorScale, row++, 1);
    terrainErrorScale.setRange(0, 8);
    terrainErrorScale.setSingleStep(0.1);
    row++;
    settings->addWidget(&envValue, row++, 1);
    tab1->addItem(settings);
    QVBoxLayout *tab2 = new QVBoxLayout;
    QHBoxLayout *ibuttons = new QHBoxLayout;
    ibuttons->addWidget(&iList);
    iList.setStyleSheet("combobox-popup: 0;");
    iList.addItem("Load Image", 0);
    iList.addItem("Details Image", 1);
    iList.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    ibuttons->addWidget(&iCopy);
    iCopy.setText(" Copy ");
    iPaste.setText(" Paste ");
    ibuttons->addWidget(&iPaste);    
    tab2->addItem(ibuttons);
    tab2->addWidget(&imageLoad);
    //settings->addWidget(GuiFunct::newTQLabel("Details Image"), row++, 0);
    //tab2->addWidget(&imageGraphic);
    main2Layout->addItem(tab1);
    main2Layout->addItem(tab2);
    description.setMinimumHeight(200);
    //mainLayout->addWidget(loadButton);
    //mainLayout->addWidget(imageLabel);
    ibuttons = new QHBoxLayout;
    QPushButton *bok = new QPushButton("OK");
    QObject::connect(bok, SIGNAL(released()), this, SLOT(bokEnabled()));
    QPushButton *bcancel = new QPushButton("Cancel");
    QObject::connect(bcancel, SIGNAL(released()), this, SLOT(bcancelEnabled()));
    ibuttons->addWidget(bok);
    ibuttons->addWidget(bcancel);
    mainLayout->addItem(ibuttons);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);
    
    //QObject::connect(loadButton, SIGNAL(released()),
    //                  this, SLOT(load()));
}

int TrkWindow::exec() {
    if(this->trk == NULL)
        return 0;
    this->setWindowTitle("Route settings");
    
    this->idName.setText(trk->idName);
    this->routeName.setText(trk->routeName);
    this->displayName.setText(trk->displayName);
    this->maxLineVoltage.setValue(trk->maxLineVoltage);
    this->speedLimit.setValue(trk->speedLimit*3.6);
    this->tempRestrictedSpeed.setValue(trk->tempRestrictedSpeed*3.6);
    this->overheadWireHeight.setValue(trk->overheadWireHeight);
    if(trk->electrified){
        this->electrified.setCurrentIndex(1);
    }else{
        this->electrified.setCurrentText(0);
    }
    
    if(trk->milepostUnitsKilometers){
        this->milepostUnitsKilometers.setCurrentIndex(0);
    }else{
        this->milepostUnitsKilometers.setCurrentIndex(1);
    }
    
    this->startTileX.setText(QString::number(trk->startTileX));
    this->startTileZ.setText(QString::number(trk->startTileZ));
    this->startpX.setText(QString::number(trk->startpX));
    this->startpZ.setText(QString::number(trk->startpZ));
    
    this->terrainErrorScale.setValue(trk->terrainErrorScale);
    
    QString txt = trk->description;
    txt.replace("\\n","\n");
    this->description.setPlainText(txt);
    for (auto it = trk->environment.begin(); it != trk->environment.end(); ++it)
        this->envName.addItem(QString::fromStdString((*it).first));
    this->envValue.setText(trk->environment[this->envName.itemText(0).toStdString()]);
    
    Texture * tex1 = NULL;
    if(TexLib::mtex[trk->imageLoadId] != NULL)
        if(TexLib::mtex[trk->imageLoadId]->loaded){
            tex1 = TexLib::mtex[trk->imageLoadId];
            unsigned char * out = tex1->getImageData(640,450);
            if(tex1->bytesPerPixel == 3)
                imageLoad.setPixmap(QPixmap::fromImage(QImage(out,640,450,QImage::Format_RGB888)));
            if(tex1->bytesPerPixel == 4)
                imageLoad.setPixmap(QPixmap::fromImage(QImage(out,640,450,QImage::Format_RGBA8888)));   
            delete[] out;
    }
    //int imageLoadId = TexLib::addTex(Game::root+"/routes/"+idName+"/"+imageLoad);
    
    
    
    return QDialog::exec();
} 

void TrkWindow::envNameEnabled(QString item){
    this->envValue.setText(trk->environment[item.toStdString()]);
}

void TrkWindow::bokEnabled(){
    trk->setModified(true);
    trk->displayName = displayName.text();
    trk->startTileX = startTileX.text().toInt();
    trk->startTileZ = startTileZ.text().toInt();
    trk->startpX = startpX.text().toFloat();
    trk->startpZ = startpZ.text().toFloat();
    trk->description = description.toPlainText();
    trk->description.replace("\n","\\n");
    
    trk->electrified = electrified.currentIndex();
    trk->overheadWireHeight = overheadWireHeight.value();
    trk->maxLineVoltage = maxLineVoltage.value();
    if(milepostUnitsKilometers.currentIndex() == 1)
        trk->milepostUnitsKilometers = false;
    else
        trk->milepostUnitsKilometers = true;
    
    trk->tempRestrictedSpeed = tempRestrictedSpeed.value()/3.6;
    trk->speedLimit = speedLimit.value()/3.6;
    close();
}

void TrkWindow::bcancelEnabled(){
    close();
}

TrkWindow::~TrkWindow() {
}

