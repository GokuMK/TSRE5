/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "RenderItem.h"
#include "Vector3f.h"
#include "Vector4f.h"

RenderItem::RenderItem() {
}

RenderItem::RenderItem(const RenderItem& orig) {
}

RenderItem::~RenderItem() {
    //delete mvMatrix;
}

void RenderItem::setVertexAttributes(VertexAttr attr){
    vertexAttr = attr;
    if(vertexAttr == V){
        normalsEnabled = 0;
    } else if(vertexAttr == VT){
        normalsEnabled = 0;
    } else if(vertexAttr == VNT){
        normalsEnabled = 1;
    } else if(vertexAttr == VNTA){
        normalsEnabled = 1;
    }  
}

void RenderItem::disableTextures(int color){
    int wColor = (int)(color/65536);
    int sColor = (int)(color - wColor*65536)/256;
    int bColor = (int)(color - wColor*65536 - sColor*256);
    disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
}

void RenderItem::disableTextures(Vector4f* color){
    texturesEnabled = 0;
    colorX = color->x;
    colorY = color->y;
    colorZ = color->z;
    colorA = color->c;
}

void RenderItem::disableTextures(Vector3f* color){
    texturesEnabled = 0;
    colorX = color->x;
    colorY = color->y;
    colorZ = color->z;
}

void RenderItem::disableTextures(float x, float y, float z, float a){
    texturesEnabled = 0;
    colorX = x;
    colorY = y;
    colorZ = z;
    colorA = a;
}

void RenderItem::enableTextures(unsigned int addr){
    texAddr = addr;
}