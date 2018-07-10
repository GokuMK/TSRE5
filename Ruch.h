/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

/* 
 * File:   Ruch.h
 * Author: Goku
 *
 * Created on 13 maja 2015, 23:57
 */

#ifndef RUCH_H
#define	RUCH_H

#include <QString>
#include "TDB.h"
#include "Vector3f.h"
#include "Vector2f.h"
#include <QMap>

class Ruch {
public:
    int onJunction = 0;
    
    Ruch();
    void next(float m);
    void back(float m);
    void toNext(float m);
    void set(int nid, int m, int tdirection, QMap<int, int>* jDirections = NULL);
    float *getCurrentPosition(float *sElev = NULL);
    float getDistanceDownPath();
    int getVectorDirection();
    void trackPassingItems(bool val);
    QString getLastItemName();

private:
    void checkNode(int mSign);
    void checkPassingItems();
    float kierunek = 1;
    float direction = 1;
    int nodeIdx;
    float nodeDist;
    float nodeLength;
    float drawPosition[8];
    float distanceDownPath = 0;
    QMap<int, int>* junctionDirections = NULL;
    
    bool trackItems = false;
    float lastNodeDist = 0;
    int lastItemId = -1;
    QString lastItemName;
};

#endif	/* RUCH_H */

