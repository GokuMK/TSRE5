/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "HeightWindow.h"
#include <QDebug>
#include <QFile>
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
#include "GeoHgtFile.h"
#include "GeoTiffFile.h"
#include "UnsavedDialog.h"

std::unordered_map<int, GeoTerrainFile*> HeightWindow::hqtFiles;

HeightWindow::HeightWindow() : QDialog() {
    QPushButton *loadButton = new QPushButton("Load", this);
    QImage myImage(800, 800, QImage::Format_RGB888);
    //myImage->load("F:/2.png");
    imageLabel = new QLabel("");
    imageLabel->setContentsMargins(0,0,0,0);
    imageLabel->setPixmap(QPixmap::fromImage(myImage));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(loadButton);
    mainLayout->addWidget(imageLabel);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);
    
    QObject::connect(loadButton, SIGNAL(released()),
                      this, SLOT(load()));
    
    igh = new IghCoordinate();
    mLatlon = new LatitudeLongitudeCoordinate();
}

int HeightWindow::exec() {
    this->setWindowTitle("Tile: " + QString::number(this->tileX) + " " + QString::number(-this->tileZ));
    return QDialog::exec();
} 

void HeightWindow::CheckForMissingGeodataFiles(QMap<int,QPair<int,int>*>& tileList){
    PreciseTileCoordinate *tCoords = new PreciseTileCoordinate();
    IghCoordinate *tigh = new IghCoordinate();
    LatitudeLongitudeCoordinate *tLatlon = new LatitudeLongitudeCoordinate();
    QMapIterator<int, QPair<int, int>*> i(tileList);
    
    QMap<QString, bool> missingFiles;
    bool fail;
    
    while (i.hasNext()) {
        i.next();
        if(i.value() == NULL)
            continue;

        tCoords->TileX = i.value()->first;
        tCoords->TileZ = i.value()->second;
        tCoords->setWxyz(0, 0, 0);
        tigh = MstsCoordinates::ConvertToIgh(tCoords, tigh);
        tLatlon = MstsCoordinates::ConvertToLatLon(tigh, tLatlon);

        //qDebug() << "lat " << itlat->first << " lon " << itlon->first;
        if(hqtFiles[(int)floor(tLatlon->Latitude)*1000+(int)floor(tLatlon->Longitude)] == NULL){
            hqtFiles[(int)floor(tLatlon->Latitude)*1000+(int)floor(tLatlon->Longitude)] = new GeoHgtFile();
            //hqtFiles[tLatlon->Latitude*1000+tLatlon->Longitude] = new GeoTiffFile();
            fail = hqtFiles[(int)floor(tLatlon->Latitude)*1000+(int)floor(tLatlon->Longitude)]->load((int)floor(tLatlon->Latitude), (int)floor(tLatlon->Longitude));
        }
        if(!hqtFiles[(int)floor(tLatlon->Latitude)*1000+(int)floor(tLatlon->Longitude)]->isLoaded())
            fail = false;
        //qDebug() << hqtFiles[tLatlon->Latitude*1000+tLatlon->Longitude]->pathid;
        if(!fail) {
            missingFiles[hqtFiles[(int)floor(tLatlon->Latitude)*1000+(int)floor(tLatlon->Longitude)]->pathid] = true;
        }
    }
    
    if(missingFiles.count() > 0){
        QMapIterator<QString, bool> i2(missingFiles);
        UnsavedDialog missingDialog;
        missingDialog.setWindowTitle("Missing files?");
        missingDialog.setMsg("Missing terrain heightmap files. ");
        missingDialog.hideButtons();

        while (i2.hasNext()) {
            i2.next();
            missingDialog.items.addItem(i2.key());
            qDebug() << i2.key();
        }
        missingDialog.exec();
    }
    
}

void HeightWindow::load(bool gui){
    if(aCoords == NULL) 
        aCoords = new PreciseTileCoordinate();
    aCoords->TileX = this->tileX;
    aCoords->TileZ = this->tileZ;
    qDebug() << this->tileX << " " << this->tileZ;;
    minlat = minlon = 9999;
    maxlat = maxlon = -9999;
    
    std::unordered_map<int, bool> fileLat;
    std::unordered_map<int, bool> fileLon;
    
    for(int i = 0; i <= terrainSize; i+= 0.5*terrainSize)
        for(int j = 0; j <=terrainSize; j+= 0.5*terrainSize){
            aCoords->setWxyzU(i, 0, j);
            igh = MstsCoordinates::ConvertToIgh(aCoords, igh);
            mLatlon = MstsCoordinates::ConvertToLatLon(igh, mLatlon);
            fileLat[(int)floor(mLatlon->Latitude)] = true;
            fileLon[(int)floor(mLatlon->Longitude)] = true;
        }
    
    QImage* image = NULL;
    bool fail;
    for (auto itlat = fileLat.begin(); itlat != fileLat.end(); ++itlat ){
        for (auto itlon = fileLon.begin(); itlon != fileLon.end(); ++itlon ){
            qDebug() << "lat " << itlat->first << " lon " << itlon->first;
            if(this->hqtFiles[itlat->first*1000+itlon->first] == NULL){
                this->hqtFiles[itlat->first*1000+itlon->first] = new GeoHgtFile();
                //this->hqtFiles[itlat->first*1000+itlon->first] = new GeoTiffFile();
                fail = this->hqtFiles[itlat->first*1000+itlon->first]->load(itlat->first, itlon->first);
            }
            if(!this->hqtFiles[itlat->first*1000+itlon->first]->isLoaded())
                fail = false;
            if(!fail) {
                if(gui){
                    QMessageBox msgBox;
                    msgBox.setText("Failed to load "+this->hqtFiles[itlat->first*1000+itlon->first]->pathid);
                    msgBox.exec();
                }
                return;
            }
        }
    }
    
    drawTile(image, gui);
    
    if(gui){
        imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(800,800,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
    delete image;

    //qDebug() << "lat " << minLatlon->Latitude << " " << maxLatlon->Latitude;
    //qDebug() << "lon " << minLatlon->Longitude << " " << maxLatlon->Longitude;
}

void HeightWindow::drawTile(QImage* &image, bool gui){
    qDebug() << "draw tile";
    if(gui)
        image = new QImage(terrainResolution, terrainResolution, QImage::Format_RGB888);
    int step = (terrainSize)/terrainResolution;
    
    if(terrainData != NULL){
        for (int i = 0; i < terrainResolution; i++) {
            delete[] terrainData[i];
        }
        delete[] terrainData;
    }
    /*if(terrainData == NULL){
        terrainData = new float*[terrainResolution];
        for (int i = 0; i < terrainResolution; i++) {
            terrainData[i] = new float[terrainResolution];
        }
    }*/
    
    terrainData = new float*[terrainResolution];
    minVal = 999;
    maxVal = -999;
    for (int i = 0; i < terrainResolution; i++) {
        terrainData[i] = new float[terrainResolution];
        for (int j = 0; j < terrainResolution; j++) {
            aCoords->setWxyzU(i*step, 0, j*step);
            igh = MstsCoordinates::ConvertToIgh(aCoords, igh);
            mLatlon = MstsCoordinates::ConvertToLatLon(igh, mLatlon);
            if(hqtFiles[(int)floor(mLatlon->Latitude)*1000+(int)floor(mLatlon->Longitude)] == NULL){
                qDebug() << "fail";
                continue;
            }
            terrainData[i][j] = hqtFiles[(int)floor(mLatlon->Latitude)*1000+(int)floor(mLatlon->Longitude)]->getHeight(mLatlon->Latitude, mLatlon->Longitude);
            if(terrainData[i][j] < minVal)
                minVal = terrainData[i][j];
            if(terrainData[i][j] > maxVal)
                maxVal = terrainData[i][j];
        }
    }
    qDebug() << "minmax" << minVal << " "<<maxVal;
    
    if(gui){
        int val;
        float s = (maxVal - minVal) / 255;
        for (int i = 0; i < terrainResolution; i++) {
            for (int j = 0; j < terrainResolution; j++) {
                val = (this->terrainData[i][j] - minVal)/s;
                //val = (this->terrainData[i][j])*10+50;
                if(val < 0) val = 0;
                if(val > 255) val = 255;
                image->setPixel(i, j, qRgb(val,val,val));
            }
        }
    }
    this->ok = true;
}

HeightWindow::~HeightWindow() {
    
}

