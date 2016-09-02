/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "MapLib.h"
#include "MapWindow.h"
#include <QDebug>
#include <QString>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QColor>

MapLib::MapLib() {
}

void MapLib::run(){
    qDebug() << texture->pathid;
    QString hashName = texture->pathid.split(".")[0];
    int hash = hashName.toInt();
    qDebug() << hash;
    QImage *img;
    if(MapWindow::mapTileImages[hash] == NULL){
        img = new QImage(16, 16, QImage::Format_RGB888);    
        img->fill(QColor(255,0,0));
    }else{
        img = MapWindow::mapTileImages[hash];
    }
    
    texture->width = img->width();
    texture->height = img->height();
    texture->bytesPerPixel = 3;
    texture->type = GL_RGB;
    texture->imageData = new unsigned char[texture->width*texture->height*texture->bytesPerPixel];//img.bits();
    int lineWidth = (texture->width*texture->bytesPerPixel);
    if( lineWidth%4 == 0){
        memcpy(texture->imageData, img->bits(), texture->width*texture->height*texture->bytesPerPixel);
    } 
    else if( lineWidth%4 != 0){
        lineWidth = lineWidth + 4 - lineWidth%4;
        for(int i = 0; i < texture->height; i++)
            memcpy(texture->imageData + i*texture->width*texture->bytesPerPixel, img->bits() + i*lineWidth, texture->width*texture->bytesPerPixel);
    }
    texture->loaded = true;
    texture->editable = true;
    
    return;
}