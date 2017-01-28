/* 
 * File:   MapDataUrlImage.cpp
 * Author: Goku
 * 
 * Created on 28 stycznia 2017, 03:27
 */

#include "MapDataUrlImage.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QImage>
#include <QPainter>
#include "IghCoords.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include "CoordsMkr.h"
#include "IghCoords.h"
#include <QTime>

MapDataUrlImage::MapDataUrlImage() {
}

MapDataUrlImage::MapDataUrlImage(const MapDataUrlImage& orig) {
}

MapDataUrlImage::~MapDataUrlImage() {
}

bool MapDataUrlImage::draw(QImage* myImage){
    return false;
}

void MapDataUrlImage::load(){
    
}