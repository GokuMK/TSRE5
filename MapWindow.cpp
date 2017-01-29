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
#include "IghCoords.h"
#include "OSMFeatures.h"
#include "MapDataOSM.h"
#include "MapDataUrlImage.h"
#include <QTime>

std::unordered_map<int, QImage*> MapWindow::mapTileImages;

MapWindow::MapWindow() : QDialog() {

    mapServicesCombo.setMaximumWidth(100);
    mapServicesCombo.setStyleSheet("combobox-popup: 0;");
    mapServicesCombo.addItem("OSM Vector", 0);
    mapServices.push_back(new MapDataOSM());
    mapServicesCombo.addItem("Google Satellite", 1);
    mapServices.push_back(new MapDataUrlImage());
    
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
    vlist3->addWidget(&mapServicesCombo,0,0);
    vlist3->addWidget(loadButton,0,1);
    vlist3->addWidget(colorLabel,0,2);
    vlist3->addWidget(colorCombo,0,3);
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

void MapWindow::load(){
    if(aCoords == NULL) aCoords = new PreciseTileCoordinate();
    aCoords->TileX = this->tileX;
    aCoords->TileZ = -this->tileZ;
    qDebug() << this->tileX << " " << -this->tileZ;;
    LatitudeLongitudeCoordinate llpoint[4];
    aCoords->setWxyz(-1024, 0, -1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint[0]);
    aCoords->setWxyz(1024, 0, 1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint[1]);
    aCoords->setWxyz(-1024, 0, 1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint[2]);
    aCoords->setWxyz(1024, 0, -1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint[3]);
    
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
    dane->minlon = minLatlon->Longitude;
    dane->minlat = minLatlon->Latitude;
    dane->maxlon = maxLatlon->Longitude;
    dane->maxlat = maxLatlon->Latitude;
    
    loadButton->setText("Wait ...");
    
    QObject::connect(dane, SIGNAL(loaded()), this, SLOT(reload()));
    QObject::connect(dane, SIGNAL(statusInfo(QString)), this, SLOT(isStatusInfo(QString)));
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
    QImage* myImage = new QImage(4096, 4096, QImage::Format_RGB888);

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

MapWindow::~MapWindow() {
}

