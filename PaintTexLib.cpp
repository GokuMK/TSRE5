/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PaintTexLib.h"
#include "ReadFile.h"
#include "FileBuffer.h"
#include "Texture.h"
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QImage>
#include <QPainter>

void PaintTexLib::run() {
    
    int size = 4;
    
    QStringList data = texture->pathid.split(".");
    QString val = data.first();
    QColor color;
    for(int i = 1; i < data.length()-1; i++){
        if(data[i].split(":").first() == "color")
            color.setNamedColor(data[i].split(":").last());
        if(data[i].split(":").first() == "size")
            size = data[i].split(":").last().toInt();
    }
    
    int h = 32;
    int w = size*h;
    
    texture->width = w;
    texture->height = h;

    texture->bpp = 32;
    texture->typk = 0;

    texture->bytesPerPixel = (texture->bpp / 8);
    texture->imageSize = (texture->bytesPerPixel * texture->width * texture->height);
    texture->imageData = new unsigned char[texture->imageSize];
    std::fill(texture->imageData, texture->imageData+texture->imageSize, 0);
    texture->type = GL_RGBA;

    
    QImage img(texture->imageData, w, h, QImage::Format_RGBA8888);
    
    QPainter p;
    p.begin(&img);
    p.setRenderHint(QPainter::RenderHint::Antialiasing, false);
    QPen niebieski(color);
    p.setPen(niebieski); 
    p.setFont(QFont("Arial", 25));
    p.drawText(QRect(0,0,w,h), Qt::AlignCenter, val);
    p.end();
    texture->loaded = true;

    //qDebug() << "2";
    return;
}