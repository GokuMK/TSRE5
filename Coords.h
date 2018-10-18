/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef COORDS_H
#define	COORDS_H

#include <QString>
#include <unordered_map>
#include <QMap>
#include <QPair>
#include <QVector>

class OglObj;
class GLUU;
class TextObj;

class Coords {
public:
    struct Marker {
        QString name;
        float lat;
        float lon;
        int type;
        OglObj* oglObj = NULL;
        QVector<int> tileX;
        QVector<int> tileZ;
        QVector<int> x;
        QVector<int> y;
        QVector<int> z;
    };
    //struct Line {
//
    //};
    
    QVector<Marker> markerList;
    std::unordered_map<std::string, TextObj*> nameGl;
    //std::vector<Line> lineList;
    
    bool loaded = false;
    
    Coords();
    Coords(QString path);
    virtual ~Coords();
    virtual void render(GLUU* gluu, float * playerT, float* playerW, float playerRot);
    virtual void getTileList(QMap<int, QPair<int, int>*> &tileList, int radius = 0, int step = 1);
protected:
    OglObj* simpleMarkerObjP = NULL;
    OglObj* simpleMarkerObjL = NULL;
private:

};

#endif	/* COORDS_H */

