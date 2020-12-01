/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "OglObj.h"
#include <QPainter>

#ifndef TEXTOBJ_H
#define	TEXTOBJ_H

#include <QString>

class TextObj : public OglObj{
public:
    bool inUse = false;
    float pos[3];
    TextObj(QString val, float s = 0, float sc = 0, int resm = 1);
    TextObj();
    TextObj(int val, float s = 0, float sc = 0, int resm = 1);
    TextObj(const TextObj& orig);
    virtual ~TextObj();
    void pushRenderItem();
    void pushRenderItem(float rot);
    void render();
    void render(float rot);
    void setColor(int r, int g, int b);
    void setOColor(int r, int g, int b);
    void setFontName(QString val);
    void setRotOffset(float val);
private:
    QString text;
    QString fontName;
    void init();
    QColor color;
    QColor ocolor;
    bool isOutline = false;
    bool isInit = false;
    float size = 4;
    float scale = 1;
    float rotOffset = 3.14;
    int resMult = 1;
};

#endif	/* TEXTOBJ_H */

