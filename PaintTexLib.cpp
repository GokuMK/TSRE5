
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

    int w = 128;
    int h = 32;
    
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
    
    QString val = texture->pathid.split(".").first();
    
    QPainter p;
    p.begin(&img);
    p.setRenderHint(QPainter::RenderHint::Antialiasing, false);
    QPen niebieski(QColor(50,50,255));
    p.setPen(niebieski); 
    p.setFont(QFont("Arial", 25));
    p.drawText(QRect(0,0,w,h), Qt::AlignCenter, val);
    p.end();
    texture->loaded = true;

    //qDebug() << "2";
    return;
}