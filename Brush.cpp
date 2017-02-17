/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Brush.h"
#include <QImage>

Brush::Brush() {
    this->color[0] = 0;
    this->color[1] = 0;
    this->color[2] = 0;
}

float Brush::getAlpha(int x, int y, int size){
    if(brushshape == 0)
        return 1.0;
    x += size;
    y += size;
    size = size*2;
    float width = brushshape->width();
    float height = brushshape->height();
    float wo = width/size;
    float ho = height/size;
    int wx = x*wo;
    int wy = y*ho;
    if(wx > width)
        wx = width;
    if(wy > height)
        wy = height;
    if(wx < 0)
        wx = 0;
    if(wy < 0)
        wy = 0;
    return
        1.0 - (float)brushshape->bits()[(int)(wy*width+wx)]/255.0;
}

