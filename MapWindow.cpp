/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "MapWindow.h"
#include <QDebug>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "CoordsMkr.h"
#include "GeoCoordinates.h"
#include "OSMFeatures.h"
#include "MapDataOSM.h"
#include "MapDataUrlImage.h"
#include <QTime>
#include "Game.h"

std::unordered_map<int, QImage*> MapWindow::mapTileImages;
int MapWindow::isAlpha = 0;

MapWindow::MapWindow() : QDialog() {

    mapServicesCombo.setMaximumWidth(130);
    mapServicesCombo.setStyleSheet("combobox-popup: 0;");
    mapServicesCombo.addItem("OSM Vector", 0);
    mapServices.push_back(new MapDataOSM());
    mapServicesCombo.addItem("Raster Images Z17", 1);
    mapServices.push_back(new MapDataUrlImage(17));
    mapServicesCombo.addItem("Raster Images Z18", 2);
    mapServices.push_back(new MapDataUrlImage(18));
    
    loadButton = new QPushButton("Load", this);
    QPushButton *saveButton = new QPushButton("Save to disk", this);
    saveButton->setFixedWidth(100);
    QImage myImage(800, 800, QImage::Format_RGB888);
    //myImage->load("F:/2.png");
    imageLabel = new QLabel("");
    imageLabel->setContentsMargins(0,0,0,0);
    imageLabel->setPixmap(QPixmap::fromImage(myImage));
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(loadButton);
    QLabel *colorLabel = new QLabel("Color: ");
    colorLabel->setMaximumWidth(40);
    QComboBox *colorCombo = new QComboBox();
    colorCombo->setMaximumWidth(100);
    colorCombo->setStyleSheet("combobox-popup: 0;");
    colorCombo->addItem("standard", 0);
    colorCombo->addItem("invert", 1);
    QLabel *alphaLabel = new QLabel("Alpha: ");
    alphaBox.setMaximumWidth(40);
    alphaBox.setMaximum(255);
    alphaBox.setMinimum(0);
    alphaBox.setValue(255-MapWindow::isAlpha);
    alphaLabel->setMaximumWidth(50);
    QGridLayout *vlist3 = new QGridLayout;
    vlist3->setSpacing(2);
    vlist3->setContentsMargins(3,0,1,0);
    vlist3->addWidget(&mapServicesCombo,0,0);
    vlist3->addWidget(alphaLabel,0,1);
    vlist3->addWidget(&alphaBox,0,2);
    vlist3->addWidget(loadButton,0,3);
    vlist3->addWidget(colorLabel,0,4);
    vlist3->addWidget(colorCombo,0,5);
    vlist3->addWidget(saveButton,0,6);
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
                      this, SLOT(load()));
    
    QObject::connect(saveButton, SIGNAL(released()),
                      this, SLOT(saveToDisk()));
        
    QObject::connect(colorCombo, SIGNAL(activated(QString)),
                      this, SLOT(colorComboActivated(QString)));
    
    QObject::connect(&alphaBox, SIGNAL(valueChanged(int)),
                      this, SLOT(alphaBoxActivated(int)));
}

int MapWindow::exec() {
    this->setWindowTitle("Tile: " + QString::number(this->tileX) + " " + QString::number(-this->tileZ));
    return QDialog::exec();
} 

void MapWindow::alphaBoxActivated(int val){
    MapWindow::isAlpha = 255-val;
}

void MapWindow::colorComboActivated(QString val){
    if(val == "standard")
        this->invert = false;
    if(val == "invert")
        this->invert = true;
    reload();
}

void MapWindow::load(){
    if(aCoords == NULL) aCoords = new PreciseTileCoordinate();
    qDebug() << "MapLoad" << this->tileX << " " << -this->tileZ;;
    LatitudeLongitudeCoordinate llpoint[4];
    qDebug() << this->tileX << -this->tileZ << 0 << 0 << 0;
    aCoords->setTWxyzU(this->tileX, -this->tileZ, 0, 0, 0);
    qDebug() << aCoords->TileX << aCoords->TileZ << aCoords->wX << 0 << aCoords->wZ;
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint[0]);
    qDebug() << this->tileX << -this->tileZ << tileSize << 0 << tileSize;
    aCoords->setTWxyzU(this->tileX, -this->tileZ, tileSize, 0, tileSize);
    qDebug() << aCoords->TileX << aCoords->TileZ << aCoords->wX << 0 << aCoords->wZ;
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint[1]);
    qDebug() << this->tileX << -this->tileZ << 0 << 0 << tileSize;
    aCoords->setTWxyzU(this->tileX, -this->tileZ, 0, 0, tileSize);
    qDebug() << aCoords->TileX << aCoords->TileZ << aCoords->wX << 0 << aCoords->wZ;
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint[2]);
    qDebug() << this->tileX << -this->tileZ << tileSize << 0 << 0;
    aCoords->setTWxyzU(this->tileX, -this->tileZ, tileSize, 0, 0);
    qDebug() << aCoords->TileX << aCoords->TileZ << aCoords->wX << 0 << aCoords->wZ;
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint[3]);
    
    minLatlon = new LatitudeLongitudeCoordinate(999,999);
    maxLatlon = new LatitudeLongitudeCoordinate(-999,-999);
    for(int i = 0; i < 4; i++){
        if(llpoint[i].Latitude < minLatlon->Latitude)
            minLatlon->Latitude = llpoint[i].Latitude;
        if(llpoint[i].Longitude < minLatlon->Longitude)
            minLatlon->Longitude = llpoint[i].Longitude;
        if(llpoint[i].Latitude > maxLatlon->Latitude)
            maxLatlon->Latitude = llpoint[i].Latitude;
        if(llpoint[i].Longitude > maxLatlon->Longitude)
            maxLatlon->Longitude = llpoint[i].Longitude;
    }
    
    dane = mapServices[mapServicesCombo.currentIndex()];
    
    dane->tileX = this->tileX;
    dane->tileZ = -this->tileZ;
    dane->level = tileSize/2048.0;
    dane->tileSize = tileSize;
    dane->minlon = minLatlon->Longitude;
    dane->minlat = minLatlon->Latitude;
    dane->maxlon = maxLatlon->Longitude;
    dane->maxlat = maxLatlon->Latitude;
    
    loadButton->setText("Wait ...");
    
    if(dane->init == false){
        dane->init = true;
        QObject::connect(dane, SIGNAL(loaded()), this, SLOT(reload()));
        QObject::connect(dane, SIGNAL(statusInfo(QString)), this, SLOT(isStatusInfo(QString)));
    }
    dane->load();
}

void MapWindow::isStatusInfo(QString val){
    loadButton->setText(val);
}

void MapWindow::reload(){
    if(dane == NULL)
        return;
    if(dane->tileX != this->tileX) return;
    if(dane->tileZ != -this->tileZ) return;
    QImage* myImage = NULL;
    int res = Game::mapImageResolution;
    if(MapWindow::isAlpha > 0)
        myImage = new QImage(res, res, QImage::Format_RGBA8888);
    else
        myImage = new QImage(res, res, QImage::Format_RGB888);

    if(!dane->draw(myImage)){
        delete myImage;
        return;
    }
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

void MapWindow::saveToDisk(){
    int hash = (int)(this->tileX)*10000+(int)(this->tileZ);
    if(MapWindow::mapTileImages[hash] == NULL)
        return;
    QString path = Game::root + "/routes/" + Game::route + "/terrain_maps/";
    path.replace("//","/");
    if (!QDir(path).exists()) {
        QDir().mkdir(path);
    }
    MapWindow::mapTileImages[hash]->save(path+QString::number(hash)+".png", "PNG");
}

bool MapWindow::LoadMapFromDisk(int x, int z){
    int hash = x*10000+z;
    QString path = Game::root + "/routes/" + Game::route + "/terrain_maps/"+QString::number(hash)+".png";
    QFile file(path);
    if(!file.exists())
        return false;
    QImage image(path);
    QImage *img = NULL;
    if(MapWindow::isAlpha == 0){
        img = new QImage(image.convertToFormat(QImage::Format_RGB888));
    } else {
        img = new QImage(image.convertToFormat(QImage::Format_RGBA8888));
        for(double i = 0; i < img->height(); i++)
            for(double j = 0; j < img->width(); j++){
                unsigned int pix = img->pixel(i, j);
                pix &= 0x00FFFFFF;
                pix |= (255-MapWindow::isAlpha) << 24;
                img->setPixel(i, j, pix);
        }
    }
    MapWindow::mapTileImages[hash] = img;
}

MapWindow::~MapWindow() {
}

