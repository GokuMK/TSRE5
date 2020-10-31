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
#include <QPainterPath>

void PaintTexLib::run() {
    
    int size = 4;
    int resM = 1;
    QString fontname = "Arial";
    bool isOutline = false;
    
    QStringList data = texture->pathid.split(".");
    QString val = data.first();
    QColor color;
    QColor colorOutline;
    colorOutline.setRgb(0,0,0);
    
    for(int i = 1; i < data.length()-1; i++){
        if(data[i].split(":").first() == "color"){
            color.setNamedColor(data[i].split(":").last());
        }
        if(data[i].split(":").first() == "ocolor"){
            colorOutline.setNamedColor(data[i].split(":").last());
            isOutline = true;
        }
        if(data[i].split(":").first() == "size"){
            size = data[i].split(":").last().toInt();
        }
        if(data[i].split(":").first() == "resm"){
            resM = data[i].split(":").last().toInt();
        }
        if(data[i].split(":").first() == "font"){
            fontname = data[i].split(":").last();
        }
    }
    
    int h = 32*resM;
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
    QFont font(fontname, 24*resM);
    p.setFont(font);
    QPen spen(color);
    QPen apen(colorOutline);
    apen.setWidth(3);
    QBrush abrush(color);
    
    if(!isOutline){
        p.setPen(spen); 
        p.drawText(QRect(0,0,w,h), Qt::AlignCenter, val);
    } else {
        p.setBrush(abrush);
        p.setPen(apen); 
        QFontMetrics fm(font);
        int pwide = fm.width(val);
        int phigh = fm.ascent();
        QPainterPath myPath;
        myPath.addText(w/2-pwide/2,phigh-2, font, val);
        p.drawPath(myPath);
    }
    
    p.end();
    texture->loaded = true;

    //qDebug() << "2";
    return;
}