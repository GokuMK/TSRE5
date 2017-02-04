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
#include "IghCoords.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include "CoordsMkr.h"
#include "IghCoords.h"
#include "GLMatrix.h"
#include <QTime>
#include <algorithm>
#include "Game.h"

double MapDataUrlImage::Zoom = 17;
double MapDataUrlImage::Resolution = 640;

MapDataUrlImage::MapDataUrlImage() {
}

MapDataUrlImage::MapDataUrlImage(const MapDataUrlImage& orig) {
}

MapDataUrlImage::~MapDataUrlImage() {
}

bool MapDataUrlImage::draw(QImage* myImage) {

    myImage->fill(Qt::red);
    
    PreciseTileCoordinate *aCoords = new PreciseTileCoordinate();
    IghCoordinate* igh = new IghCoordinate();
    aCoords->TileX = this->tileX;
    aCoords->TileZ = this->tileZ;
    qDebug() << this->tileX << " " << this->tileZ;;
    LatitudeLongitudeCoordinate llpoint00;
    LatitudeLongitudeCoordinate llpoint01;
    LatitudeLongitudeCoordinate llpoint10;
    LatitudeLongitudeCoordinate llpoint11;
    aCoords->setWxyz(-1024, 0, -1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint00);
    aCoords->setWxyz(1024, 0, 1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint11);
    aCoords->setWxyz(-1024, 0, 1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint10);
    aCoords->setWxyz(1024, 0, -1024);
    igh = MstsCoordinates::ConvertToIgh(aCoords);
    MstsCoordinates::ConvertToLatLon(igh, &llpoint01);

    //QPainter *gg = new QPainter();
    //gg->begin(myImage);
    //gg->setRenderHint(QPainter::RenderHint::Antialiasing, false);
    //gg->drawImage(0, 0, images.first().image);

    double tx, tz;
    double tlat, tlon;
    double mindist;
    int minId;
    double tdist;
    for(double i = 0; i < myImage->height(); i++){
        for(double j = 0; j < myImage->width(); j++){
            tx = i/myImage->height();
            tz = j/myImage->width();
            
            tlat = llpoint00.Latitude*(1.0 - tx)*(1.0 - tz) +
                llpoint01.Latitude*(tx)*(1.0 - tz) +
                llpoint10.Latitude*(1.0 - tx)*(tz) +
                llpoint11.Latitude*(tx)*(tz);
            tlon = llpoint00.Longitude*(1.0 - tx)*(1.0 - tz) +
                llpoint01.Longitude*(tx)*(1.0 - tz) +
                llpoint10.Longitude*(1.0 - tx)*(tz) +
                llpoint11.Longitude*(tx)*(tz);
            
            minId = -1;
            mindist = 999999;
            for(int u = 0; u < images.length(); u++){
                //if(images[u].isPoint(tlat, tlon)){
                //    myImage->setPixelColor(i, j, images[u].getPixel(tlat, tlon));
                //    break;
                //}
                //if(images[u].isPoint(tlat, tlon)){
                tdist = images[u].distanceToCenter(tlat, tlon);
                if( tdist < mindist ){
                    minId = u;
                    mindist = tdist;
                }
                //}
            }
            if(minId >= 0 && minId < images.length())
                myImage->setPixelColor(i, j, images[minId].getPixel(tlat, tlon));
        }
    }
    
    //gg->end();
    return true;
}

void MapDataUrlImage::load() {
    images.clear();
    LatitudeLongitudeCoordinate p00;
    
    requestCout = 0;
    double isteps = 5;
    double jsteps = 6;
    if(maxlat > 53.4)
        isteps = 7;
    
    //if(maxlat > 55.0)
    //    jsteps = 6;

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
            get(&p00);
        }

}

void MapDataUrlImage::get(LatitudeLongitudeCoordinate* center) {
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(isData(QNetworkReply*)));
    // the HTTP request
    qDebug() << "wait";
    
    QNetworkRequest req(QUrl(QString("http://maps.googleapis.com/maps/api/staticmap?center=") 
    + QString::number(center->Latitude) + "," 
    + QString::number(center->Longitude) + QString("&zoom=")
    + QString::number(Zoom) + QString("&size=")
    + QString::number(Resolution) + QString("x")
    + QString::number(Resolution) + QString("&maptype=satellite&key=")
    + Game::GoogleMapsKey));
    qDebug() << req.url();

    QNetworkReply* r = mgr->get(req);
    r->setProperty("centerLat", QVariant(center->Latitude));
    r->setProperty("centerLon", QVariant(center->Longitude));
    r->setProperty("zoom", Zoom);

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
        }
    }
}

MapDataUrlImage::MapImage::MapImage() {
    lat = 0;
    lon = 0;
    zoom = 0;
}

unsigned int MapDataUrlImage::MapImage::getPixel(double tlat, double tlon){
    int tx = ((double) (tlon - minlon)*((double) Resolution / ((maxlon - minlon))));
    int ty = Resolution - ((double) (tlat - minlat)*((double) Resolution / ((maxlat - minlat))));
    if(tx < 0 || ty < 0 || tx >= Resolution || ty >= Resolution)
        return 0;
    
    unsigned int val = 0;
    //val |= *(image.bits()+tx*lineWidth + ty*3 + 0);
    //val |= *(image.bits()+tx*lineWidth + ty*3 + 1) >> 0xFF;
    val |= *((image.bits()+ty*image.width()*3 + tx*3 + 0)) << 16;
    val |= *((image.bits()+ty*image.width()*3 + tx*3 + 1)) << 8;
    val |= *((image.bits()+ty*image.width()*3 + tx*3 + 2));
    return val;
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