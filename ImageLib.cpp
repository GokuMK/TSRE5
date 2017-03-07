/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ImageLib.h"
#include <QDebug>
#include <QString>
#include <QImage>
#include <QOpenGLShaderProgram>

bool ImageLib::IsThread = true;

ImageLib::ImageLib() {
}

void ImageLib::run(){
    QImage img(texture->pathid);    
    if(img.isNull()) {
        //qDebug() << "IMG: not exist "<<texture->pathid;
        return;
    }
    
    
    if(img.hasAlphaChannel()){
        texture->bytesPerPixel = 4;
        texture->type = GL_RGBA;
        img = img.convertToFormat(QImage::Format_RGBA8888);
    } else {
        texture->bytesPerPixel = 3;
        texture->type = GL_RGB;
        img = img.convertToFormat(QImage::Format_RGB888);
    }
    texture->width = img.width();
    texture->height = img.height();
    
    texture->imageData = new unsigned char[texture->width*texture->height*texture->bytesPerPixel];//img.bits();
    int lineWidth = (texture->width*texture->bytesPerPixel);
    if( lineWidth%4 == 0){
        memcpy(texture->imageData, img.bits(), texture->width*texture->height*texture->bytesPerPixel);
    } 
    else if( lineWidth%4 != 0){
        lineWidth = lineWidth + 4 - lineWidth%4;
        for(int i = 0; i < texture->height; i++)
            memcpy(texture->imageData + i*texture->width*texture->bytesPerPixel, img.bits() + i*lineWidth, texture->width*texture->bytesPerPixel);
    }
    
    //if( lineWidth%4 !=0) 
    //    lineWidth = lineWidth + 4 - lineWidth%4;
    //memcpy(texture->imageData, img.bits(), texture->width*texture->height*texture->bytesPerPixel);
    
    texture->loaded = true;
    texture->editable = true;
    
    return;
}