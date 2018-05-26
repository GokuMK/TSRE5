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

class ComplexLine {
public:
    ComplexLine();
    virtual ~ComplexLine();
    QVector<TSection> sections;
    float length;
    void init(QVector<TSection> s);
    float getLength();
    void getDrawPosition(float* posRot, float distance, float xOffset = 0);
private:

};

#endif	/* COMPLEXLINE_H */

