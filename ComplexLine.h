/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef COMPLEXLINE_H
#define	COMPLEXLINE_H

#include <QVector>
#include "TSection.h"

class ComplexLinePoint {
public:
    bool selected = false;
    int shapeType = 0;
    float position[3];
    
    ComplexLinePoint();
    ComplexLinePoint(const ComplexLinePoint& o);
};

class ComplexLine {
public:
    float length;
    QString hash;
    
    ComplexLine();
    virtual ~ComplexLine();
    void init(QVector<TSection> s);
    void init(QVector<ComplexLinePoint> s);
    float getLength();
    QString getHash();
    void getDrawPosition(float* posRot, float distance, float xOffset = 0);
private:
    QVector<TSection> sections;
    QVector<ComplexLinePoint> points;
    void getDrawPositionFromTSection(float* posRot, float distance, float xOffset = 0);
    void getDrawPositionFromPoints(float* posRot, float distance, float xOffset = 0);
};

#endif	/* COMPLEXLINE_H */

