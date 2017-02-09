/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef MAPDATAURLIMAGE_H
#define	MAPDATAURLIMAGE_H

#include "MapData.h"
#include <unordered_map>
#include <vector>
#include <QImage>

class LatitudeLongitudeCoordinate;
class QNetworkReply;

class MapDataUrlImage : public MapData {

    Q_OBJECT
public:
    static double Resolution;
    
    struct MapImage {
        double lat;
        double lon;
        
        double minlat;
        double minlon;
        
        double maxlat;
        double maxlon;
        
        int zoom;
        QImage image;
        MapImage();
        MapImage(double tlat, double tlon, int tzoom, unsigned char *data, int length);
        //double getDistaneToCenter();
        void getPixel(double tlat, double tlon, unsigned char* pixel, bool alpha);
        unsigned char getImagePixelFromFloatXY(double x, double y, int color);
        bool isPoint(double tlat, double tlon);
        double distanceToCenter(double tlat, double tlon);
    };

    struct Mercator {
        double MERCATOR_RANGE = 256;
        double pixelOriginX;
        double pixelOriginY;
        double pixelsPerLonDegree;
        double pixelsPerLonRadian;

        Mercator();
        double bound(double value, double opt_min, double opt_max);
        double degreesToRadians(double deg);
        double radiansToDegrees(double rad);
        void fromLatLngToPoint(double tlat, double tlon, double &x, double &y);
        void fromPointToLatLng(double x, double y, double &tlat, double &tlon);
    };

    MapDataUrlImage(double zoom = 17);
    MapDataUrlImage(const MapDataUrlImage& orig);
    virtual ~MapDataUrlImage();
    bool draw(QImage* myImage);
    void load();

signals:
    void loaded();
    void statusInfo(QString val);

public slots:
    void isData(QNetworkReply* r);

private:
    double zoom;
    QVector<MapImage> images;
    int requestCout;
    int totalRequestCout;

    void get(LatitudeLongitudeCoordinate* center);
};

#endif	/* MAPDATAURLIMAGE_H */

