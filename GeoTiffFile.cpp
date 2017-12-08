/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GeoTiffFile.h"
#include "FileBuffer.h"
#include <math.h>
#include <QDebug>
#include <QFile>
#include "ReadFile.h"
#include <QImage>
#include <QPainter>
#include "Game.h"

GeoTiffFile::GeoTiffFile() {
}

GeoTiffFile::~GeoTiffFile() {
}

bool GeoTiffFile::load(int lat, int lon){
    QString plat = "N";
    if(lat < 0){
        plat = "S";
        lat = -lat;
    }
    QString plon = "E";    
    if(lon < 0){
        plon = "W";
        lon = -lon;
    }

    QString slat = QString::number(lat);
    QString slon = QString::number(lon);
    while(slat.length() < 2)
        slat = "0"+slat;
    while(slon.length() < 3)
        slon = "0"+slon;
    
    this->pathid = Game::geoPath + "/" + plat + slat + plon + slon + ".tiff";
    this->pathid.replace("//", "/");
    qDebug() << this->pathid;
    //qDebug() << "Wczytam teren RAW: " << fSfile;
    QFile file(this->pathid);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() <<"TIFF not found: "<< this->pathid;
        return false;
    }
    return false;
    
    FileBuffer* data = ReadFile::readRAW(&file);
    this->rowSize = sqrt(data->length/2);
    qDebug() << this->rowSize;
    terrainData = new short int*[rowSize];
    //int u = 0;
    int avg = 0;
    short int val;
    for (int i = 0; i < rowSize; i++) {
        terrainData[i] = new short int[rowSize];
        for (int j = 0; j < rowSize; j++) {
            val = (data->get() << 8) | (data->get());
            terrainData[i][j] = val;
            avg += terrainData[i][j];
        }
    }
    qDebug() << avg / (rowSize*rowSize);
    delete data;
    loaded = true;
    return true;
}

bool GeoTiffFile::isLoaded(){
    return loaded;
}

void GeoTiffFile::draw(QImage* &image){
    qDebug() << this->rowSize;
    return;
    image = new QImage(this->rowSize, this->rowSize, QImage::Format_RGB888);
    short int val;
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < rowSize; j++) {
            val = this->terrainData[j][i] * 4;
                if(val < 0) val = 0;
                if(val > 255) val = 255;
                image->setPixel(i, j, qRgb(val,val,val));
        }
    }
}

float GeoTiffFile::getHeight(float lat, float lon){
    return 0;
    float latO = lat - floor(lat);
    float lonO = lon - floor(lon);
    int latI = rowSize*latO;
    int lonI = rowSize*lonO;

        float tx = (float)rowSize*latO - latI;
        float tz = (float)(rowSize)*lonO - lonI;
        
        if(latI > rowSize - 1)
            latI = rowSize - 1;
        if(lonI > rowSize - 2)
            lonI = rowSize - 2;
        if(latI < 1)
            latI = 1;
        if(lonI < 0)
            lonI = 0;
        
        return 
            this->terrainData[rowSize-latI][lonI+1]*(1.0 - tx)*(tz) +
            this->terrainData[rowSize-latI-1][lonI]*(tx)*(1.0 - tz) +
            this->terrainData[rowSize-latI-1][lonI+1]*(tx)*(tz) +
            this->terrainData[rowSize-latI][lonI]*(1.0 - tx)*(1.0 - tz) +
                0;
}