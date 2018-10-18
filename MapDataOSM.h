/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef MAPDATAOSM_H
#define	MAPDATAOSM_H

#include "MapData.h"
#include <unordered_map>
#include <vector>
#include <QVector>

class IghCoordinate;
class LatitudeLongitudeCoordinate;
class PreciseTileCoordinate;
class QImage;
class QColor;
class QPen;
class QPainter;
class QBrush;
class QByteArray;
class QNetworkReply;


class MapDataOSM : public MapData {
    Q_OBJECT
public:
    
    struct Node {
        int64_t id;
        float lat;
        float lon;
        unsigned short type;
        unsigned char val1;
        unsigned char val2;

        Node(){
             //id = 0;
             lon = 0;
             lat = 0;
             type = 0;
             val1 = 0; 
             val2 = 0;
        }   

        Node(int64_t id1, float lat1, float lon1){
             id = id1;
             lon = lon1;
             lat = lat1;
             type = 0;
             val1 = 0; 
             val2 = 0;
        }
    };

    struct Way {
        int64_t id;
        unsigned short type;
        unsigned char val1;
        unsigned char val2;
        QVector<int64_t> ref;
        QVector<float> lat;
        QVector<float> lon;

        Way(int64_t id1){
             id = id1;
             type = 0;
             val1 = 0;
             val2 = 0;
        }

        Way(){
             id = 0;
             type = 0;
             val1 = 0;
             val2 = 0;
        }    
    };
    
    MapDataOSM();
    virtual ~MapDataOSM();
    bool draw(QImage* myImage);
    void load();
    
signals:
    void loaded();
    void statusInfo(QString val);
    
public slots:
    void isData(QNetworkReply* r);
    
private:
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* latlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    QColor* color;
    QColor* roadBorder;
    QPen* p;
    QPainter* gg;
    QBrush* brush;
    
    std::unordered_map<int64_t,Node*> nodes;
    QVector<Way*> ways[10];
    float height, width;
    
    int loadCount;
    int totalLoadCount;
    int rX(float tlon);
    int rY(float tlat);
    void r(int &x, int &y, float lat, float lon);
    void loadData(QByteArray* data = NULL);
    void setColor(int r, int g, int b);
    void setColor(QColor* color);
    void setPenSettings(QPen* pen);
    void get(LatitudeLongitudeCoordinate* min, LatitudeLongitudeCoordinate* max);
};

#endif	/* MAPDATAOSM_H */

