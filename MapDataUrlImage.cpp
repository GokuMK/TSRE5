/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#include "MapDataUrlImage.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QImage>
#include <QPainter>
#include "GeoCoordinates.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include "CoordsMkr.h"
#include "GeoCoordinates.h"
#include "GLMatrix.h"
#include <QTime>
#include <algorithm>
#include "Game.h"
#include "MapWindow.h"
#include "UriImageDrawThread.h"

double MapDataUrlImage::Resolution = 640;

MapDataUrlImage::MapDataUrlImage(double zoom) {
    this->zoom = zoom;
}

MapDataUrlImage::MapDataUrlImage(const MapDataUrlImage& orig) {
}

MapDataUrlImage::~MapDataUrlImage() {
}

bool MapDataUrlImage::draw(QImage* myImage) {
    
    myImage->fill(Qt::red);

    PreciseTileCoordinate *aCoords = new PreciseTileCoordinate();
    IghCoordinate* igh = new IghCoordinate();
    qDebug() << this->tileX << " " << this->tileZ;;
    LatitudeLongitudeCoordinate llpoint00;
    LatitudeLongitudeCoordinate llpoint01;
    LatitudeLongitudeCoordinate llpoint10;
    LatitudeLongitudeCoordinate llpoint11;
    aCoords->setTWxyzU(this->tileX, this->tileZ, 0, 0, 0);
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint00);
    aCoords->setTWxyzU(this->tileX, this->tileZ, tileSize, 0, tileSize);
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint11);
    aCoords->setTWxyzU(this->tileX, this->tileZ, 0, 0, tileSize);
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint10);
    aCoords->setTWxyzU(this->tileX, this->tileZ, tileSize, 0, 0);
    igh = Game::GeoCoordConverter->ConvertToInternal(aCoords);
    Game::GeoCoordConverter->ConvertToLatLon(igh, &llpoint01);
    
    int numThreads = 4;
    UriImageDrawThread threads[numThreads];
    int wstep = myImage->width() / numThreads;
    for(int i = 0; i < numThreads; i++){
        threads[i].llpoint00 = &llpoint00;
        threads[i].llpoint01 = &llpoint01;
        threads[i].llpoint10 = &llpoint10;
        threads[i].llpoint11 = &llpoint11;
        threads[i].myImage = myImage;
        threads[i].images = &images;
        threads[i].wmin = i*wstep;
        threads[i].wmax = i*wstep+wstep;
        threads[i].start();
    }
    for(int i = 0; i < numThreads; i++){
        threads[i].wait();
    }

    qDebug() << "image draw end";
    return true;
}

void MapDataUrlImage::load() {
    
    if(Game::imageMapsUrl.length() < 2){
        QMessageBox msgBox;
        msgBox.setText("Enter imageMapsUrl (for example google static maps url) in settings.txt!");
        msgBox.exec();
        return;
    }
    
    images.clear();
    LatitudeLongitudeCoordinate p00;
    
    requestCout = 0;
    double isteps = 5;
    double jsteps = 6;
    if(maxlat > 53.4)
        isteps = 7;
    double tzoom = zoom;
    if(tzoom == 18){
        isteps *= 2;
        jsteps *= 2;
    }
    qDebug() << "level"<<level;
    tzoom -= log2(level);
    qDebug() << tzoom;
    //if(level == 0.5)
    //    zoom = 16;
    //if(level == 0.25)
    //    zoom = 15;
    //if(zoom == 16){
   //     isteps *= 0.5;
    //    jsteps *= 0.5;
    //}
    
    totalRequestCout = (jsteps+1)*(isteps+1);
    
    double latstep = (maxlat - minlat) / isteps;
    double lonstep = (maxlon - minlon) / jsteps;
    qDebug() << "Step" <<latstep <<lonstep;
    
    for(double i = 0; i < isteps+1; i++)
        for(double j = 0; j < jsteps+1; j++){
            p00.Latitude = minlat + i*latstep;
            p00.Longitude = minlon + j*lonstep;
            p00.Latitude = floor(p00.Latitude*10000)/10000;
            p00.Longitude = floor(p00.Longitude*10000)/10000;
            get(&p00, tzoom);
        }

}

void MapDataUrlImage::get(LatitudeLongitudeCoordinate* center, double tzoom) {
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(isData(QNetworkReply*)));
    // the HTTP request
    qDebug() << "wait";
    
    QString imageMapsUrl = Game::imageMapsUrl;
    imageMapsUrl.replace("{lat}", QString::number(center->Latitude));
    imageMapsUrl.replace("{lon}", QString::number(center->Longitude));
    imageMapsUrl.replace("{zoom}", QString::number(tzoom));
    imageMapsUrl.replace("{res}", QString::number(Resolution));
    QNetworkRequest req(QUrl(QString("")+imageMapsUrl));
    qDebug() << req.url();

    QNetworkReply* r = mgr->get(req);
    r->setProperty("centerLat", QVariant(center->Latitude));
    r->setProperty("centerLon", QVariant(center->Longitude));
    r->setProperty("zoom", tzoom);
}

void MapDataUrlImage::isData(QNetworkReply* r) {
    QByteArray data = r->readAll();

    qDebug() << "data " << data.length();
    //qDebug() << r->property("centerLat").toDouble();
    //qDebug() << r->property("centerLon").toDouble();
    //qDebug() << r->property("zoom");
    double lat = r->property("centerLat").toDouble();
    double lon = r->property("centerLon").toDouble();
    int zoom = r->property("zoom").toInt();
    
    requestCout++;
    if (data.length() == 0) {
        emit statusInfo(QString("No data from the network..."));
        QTime cTime = QTime::currentTime().addSecs(5);
        while (QTime::currentTime() < cTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        r->close();
        emit statusInfo(QString("Load"));
    } else if(data.length() < 1000){
            qDebug() << data;
    } else {
        images.push_back(MapImage(lat, lon, zoom, (unsigned char*) data.constData(), data.length()));
        if(requestCout == totalRequestCout){
            emit statusInfo(QString("Load"));
            emit loaded();
        } else {
            emit statusInfo(QString("Wait [")+QString::number(requestCout)+"/"+QString::number(totalRequestCout)+"] ...");
        }
    }
}

MapDataUrlImage::MapImage::MapImage() {
    lat = 0;
    lon = 0;
    zoom = 0;
}

void MapDataUrlImage::MapImage::getPixel(double tlat, double tlon, unsigned char* val, bool alpha){
    double tx = ((double) (tlon - minlon)*((double) Resolution / ((maxlon - minlon))));
    double ty = Resolution - ((double) (tlat - minlat)*((double) Resolution / ((maxlat - minlat))));
    if(tx < 0 || ty < 0 || tx >= Resolution || ty >= Resolution)
        return;
    
    //unsigned int val = 0;
    //*val = 0;
    if(alpha)
        val[3] = (255-MapWindow::isAlpha);
    val[2] = (int)getImagePixelFromFloatXY(tx, ty, 2);
    val[1] = (int)getImagePixelFromFloatXY(tx, ty, 1);
    val[0] = (int)getImagePixelFromFloatXY(tx, ty, 0);
    //*val |= (255-MapWindow::isAlpha) << 24;
    //*val |= (int)getImagePixelFromFloatXY(tx, ty, 0) << 16;
    //*val |= (int)getImagePixelFromFloatXY(tx, ty, 1) << 8;
    //*val |= (int)getImagePixelFromFloatXY(tx, ty, 2);
    //return val;
}

unsigned char MapDataUrlImage::MapImage::getImagePixelFromFloatXY(double x, double y, int color){
    if(x < 1 || y < 1 || x > image.width() - 2 || y > image.height() - 2)
        return *((image.bits()+(int)y*image.width()*3 + (int)x*3 + color));
    
    int tx = floor(x);
    int ty = floor(y);
    x = x - tx;
    y = y - ty;
    
    unsigned char out = 0;
    out += *((image.bits()+(ty)*image.width()*3 + (tx)*3 + color))*(1.0 - x)*(1.0 - y);
    out += *((image.bits()+(ty)*image.width()*3 + (tx+1)*3 + color))*(x)*(1.0 - y);
    out += *((image.bits()+(ty+1)*image.width()*3 + (tx)*3 + color))*(1.0 - x)*(y);
    out += *((image.bits()+(ty+1)*image.width()*3 + (tx+1)*3 + color))*(x)*(y);
    return out;
}

bool MapDataUrlImage::MapImage::isPoint(double tlat, double tlon){
    if(tlon > minlon && tlon < maxlon && tlat > minlat && tlat < maxlat)
        return true;
    return false;
}

double MapDataUrlImage::MapImage::distanceToCenter(double tlat, double tlon){
    return sqrt((tlat - lat)*(tlat - lat) + (tlon - lon)*(tlon - lon));
}

MapDataUrlImage::MapImage::MapImage(double tlat, double tlon, int tzoom, unsigned char* data, int length) {
    lat = tlat;
    lon = tlon;
    zoom = tzoom;
    image = QImage::fromData(data, length);
    image = image.convertToFormat(QImage::Format_RGB888);
    qDebug() << image.width() << image.height();
    
    //qDebug() << "c: "<<lat<<lon;
    double scale = pow(2,zoom);
    Mercator proj;
    double cx, cy;
    proj.fromLatLngToPoint(lat, lon, cx, cy);
    //qDebug() << "cp: "<<cx<<cy;
    //double fix = 0.96;
    double SWx = (cx - (Resolution/2.0)/ scale);
    double SWy = (cy + (Resolution/2.0)/ scale);
    //qDebug() << "c1: "<<SWx<<SWy;
    proj.fromPointToLatLng(SWx, SWy, minlat, minlon);
    //qDebug() << "corner: "<<minlat<<minlon;

    double NEx = (cx + (Resolution/2.0)/ scale);
    double NEy = (cy - (Resolution/2.0)/ scale);
    //qDebug() << "c2: "<<NEx<<NEy;
    proj.fromPointToLatLng(NEx, NEy, maxlat, maxlon);
    //qDebug() << "corner: "<<maxlat<<maxlon;
    //qDebug() << "lat size: "<<maxlat-minlat;

}

MapDataUrlImage::Mercator::Mercator() {
    pixelOriginX = MERCATOR_RANGE / 2.0;
    pixelOriginY = MERCATOR_RANGE / 2.0;
    pixelsPerLonDegree = MERCATOR_RANGE / 360.0;
    pixelsPerLonRadian = MERCATOR_RANGE / (2.0 * M_PI);
};

double MapDataUrlImage::Mercator::bound(double value, double opt_min, double opt_max) {
    value = std::max(value, opt_min);
    value = std::min(value, opt_max);
    return value;
}

double MapDataUrlImage::Mercator::degreesToRadians(double deg) {
    return deg * M_PI / 180.0;
}

double MapDataUrlImage::Mercator::radiansToDegrees(double rad) {
    return rad * 180.0 / M_PI;
}

void MapDataUrlImage::Mercator::fromLatLngToPoint(double tlat, double tlon, double &x, double &y) {
    x = pixelOriginX + tlon * pixelsPerLonDegree;
    double siny = bound(sin(degreesToRadians(tlat)), -0.9999, 0.9999);
    y = pixelOriginY + 0.5 * log((1.0 + siny) / (1.0 - siny)) * -pixelsPerLonRadian;
    return;
};

void MapDataUrlImage::Mercator::fromPointToLatLng(double x, double y, double &tlat, double &tlon) {
    tlon = (x - pixelOriginX) / pixelsPerLonDegree;
    double latRadians = (y - pixelOriginY) / -pixelsPerLonRadian;
    tlat = radiansToDegrees(2.0 * atan(exp(latRadians)) - M_PI / 2.0);
    return;
};