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
#include "HGTfile.h"

std::unordered_map<int, HGTfile*> HeightWindow::hqtFiles;

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
}

int HeightWindow::exec() {
    this->setWindowTitle("Tile: " + QString::number(this->tileX) + " " + QString::number(-this->tileZ));
    return QDialog::exec();
} 

void HeightWindow::load(){
if(aCoords == NULL) aCoords = new PreciseTileCoordinate();
    aCoords->TileX = this->tileX;
    aCoords->TileZ = this->tileZ;
    qDebug() << this->tileX << " " << this->tileZ;;
    minlat = minlon = 9999;
    maxlat = maxlon = -9999;
    
    std::unordered_map<int, bool> fileLat;
    std::unordered_map<int, bool> fileLon;
    
    for(int i = -1024; i <= 1024; i+=1024)
        for(int j = -1024; j <=1024; j+=1024){
            aCoords->setWxyz(i, 0, j);
            igh = MstsCoordinates::ConvertToIgh(aCoords);
            mLatlon = MstsCoordinates::ConvertToLatLon(igh);
            fileLat[(int)floor(mLatlon->Latitude)] = true;
            fileLon[(int)floor(mLatlon->Longitude)] = true;
        }
    
    QImage* image = NULL;
    bool fail;
    for (auto itlat = fileLat.begin(); itlat != fileLat.end(); ++itlat ){
        for (auto itlon = fileLon.begin(); itlon != fileLon.end(); ++itlon ){
            qDebug() << "lat " << itlat->first << " lon " << itlon->first;
            if(this->hqtFiles[itlat->first*1000+itlon->first] == NULL){
                this->hqtFiles[itlat->first*1000+itlon->first] = new HGTfile();
                fail = this->hqtFiles[itlat->first*1000+itlon->first]->load(itlat->first, itlon->first);
            }
            if(!this->hqtFiles[itlat->first*1000+itlon->first]->isLoaded())
                fail = false;
            if(!fail) {
                QMessageBox msgBox;
                msgBox.setText("Failed to load "+this->hqtFiles[itlat->first*1000+itlon->first]->pathid);
                msgBox.exec();
                return;
            }
        }
    }
    drawTile(image);
    imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(800,800,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    //qDebug() << "lat " << minLatlon->Latitude << " " << maxLatlon->Latitude;
    //qDebug() << "lon " << minLatlon->Longitude << " " << maxLatlon->Longitude;
}

void HeightWindow::drawTile(QImage* &image){
    qDebug() << "draw tile";
    image = new QImage(terrainResolution, terrainResolution, QImage::Format_RGB888);
    //float val;
    int step = 2048/terrainResolution;
    
    terrainData = new float*[terrainResolution];
    minVal = 999;
    maxVal = -999;
    for (int i = 0; i < terrainResolution; i++) {
        terrainData[i] = new float[terrainResolution];
        for (int j = 0; j < terrainResolution; j++) {
            aCoords->setWxyz(-1024 + i*step, 0, -1024 + j*step);
            igh = MstsCoordinates::ConvertToIgh(aCoords);
            mLatlon = MstsCoordinates::ConvertToLatLon(igh);
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
    this->ok = true;
}

HeightWindow::~HeightWindow() {
    
}

