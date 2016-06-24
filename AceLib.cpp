#include "AceLib.h"
#include "ReadFile.h"
#include "FileBuffer.h"
#include "Texture.h"
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QString>

AceLib::AceLib(){
    
}

AceLib::~AceLib(){
    
}

/*===============================================================
===== Wczytywanie tekstury w formacie ACE [RGB,cRGB, DXT, cDXT]
==============================================================*/
//bool AceLib::LoadACE(Texture* texture) {
void AceLib::run() {
    QFile *file = new QFile(texture->pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << "ACE: not exist "<<texture->pathid;
        //return false;
        return;
    }
    FileBuffer* data = ReadFile::read(file);
    //qDebug() << "Date:" << data->length;
    unsigned char* bufor = data->data;
    int offset = -16;
    int typ = 0, dane;
    unsigned char tempt;

    dane = bufor[20 + offset];
    texture->compressed = bufor[32 + offset];

    texture->width = bufor[25 + offset] * 256 + bufor[24 + offset];
    texture->height = bufor[29 + offset] * 256 + bufor[28 + offset];

    if (bufor[36 + offset] == 3) {
        texture->bpp = 24;
        texture->typk = typ = 0;
    }
    if (bufor[36 + offset] == 4) {
        texture->bpp = 32;
        texture->typk = typ = 2;
    }
    if (bufor[36 + offset] == 5) {
        texture->bpp = 32;
        texture->typk = typ = 1;
    }

    //qDebug() << "--"<<texture->width<<":"<<texture->height<<" "<<texture->bpp;

    if ((texture->width <= 1) || (texture->height <= 1) || ((texture->bpp != 24) && (texture->bpp != 32))) {
        qDebug() << "!!!!!!!!!!!!! mega fail tex: " <<  texture->pathid
                <<  " " <<  texture->width
                <<  " " <<  texture->height
                <<  " " <<  texture->bpp
                ;
        return;
    }
    if (/*(texture->width % 2) != 0 || (texture->height % 2) != 0 ||*/ ((texture->width > 8192) && (texture->height > 8192))) {
        qDebug() << "!!!!!!!!!!!!! mega fail tex: " <<  texture->pathid
                <<  " " <<  texture->width
                <<  " " <<  texture->height
                <<  " " <<  texture->bpp
                ;
        return;
    }
    //if (!(texture->width == 16 || texture->width == 32 || texture->width == 64 || texture->width == 128 ||
    //        texture->width == 256 || texture->width == 512 || texture->width == 1024 || texture->width == 2048)) {
    /*    qDebug() << "!!!!!!!!!!!!! mega fail tex 22: " << texture->pathid
                << " " << texture->width
                << " " << texture->height
                << " " << texture->bpp
                ;
        return;
    }*/

    texture->bytesPerPixel = (texture->bpp / 8);
    texture->imageSize = (texture->bytesPerPixel * texture->width * texture->height);
    texture->imageData = new unsigned char[texture->imageSize];
    if (texture->bpp == 24) {
        texture->type = GL_RGB;
    } else {
        texture->type = GL_RGBA;
    }
        
    int ptr = 0;
    if (texture->compressed != 18) {
        int iw = 0, ite;
        if (typ == 0) ptr = 216 + offset;
        if (typ == 1) ptr = 248 + offset;
        if (typ == 2) ptr = 232 + offset;
        if (dane == 0) ptr += texture->height * 4;
        else if (dane == 1) ptr += texture->height * 8 - 4;
        else if (dane == 4) ptr += texture->height * 4;
        else if (dane == 5) ptr += texture->height * 8 - 4;
        //if (dane == 5) {
        //    qDebug() << "ace dane: " << dane << texture->pathid;
        //}

        //console.log("tekstura wtyp = " + typ);
        if (typ == 0) {
            for (int ih = 0; ih<texture->height; ih++) {
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel] = bufor[ptr++];
                }
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 1] = bufor[ptr++];
                }
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 2] = bufor[ptr++];
                }
            }
        }
        if (typ == 1) {
            for (int ih = 0; ih<texture->height; ih++) {
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel] = bufor[ptr++];
                }
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 1] = bufor[ptr++];
                }
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 2] = bufor[ptr++];
                }
                ptr += texture->height / 8;
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 3] = bufor[ptr++];
                }
            }
        }
        if (typ == 2) {
            for (int ih = 0; ih<texture->height; ih++) {
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel] = bufor[ptr++];
                }
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 1] = bufor[ptr++];
                }
                for (iw = 0; iw<texture->width; iw = iw + 1) {
                    texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 2] = bufor[ptr++];
                }
                for (iw = 0; iw<texture->width; ptr++) {
                    for (ite = 0; ite < 8; ite++) {
                        tempt = (unsigned char)(bufor[ptr] << ite);
                        texture->imageData[texture->bytesPerPixel * texture->width * ih + iw * texture->bytesPerPixel + 3] = (unsigned char)(tempt >> 7)*255;
                        iw = iw + 1;
                    }
                }
            }
        }
        texture->loaded = true;
        texture->editable = true;        
    } else {
        //var start = new Date().getTime();
        ptr = 216 + offset;
        int tempp = texture->height;
        if (texture->bpp == 24) ptr += 4;
        else ptr += 20;
        while (tempp >= 1) {
            ptr += 4;
            tempp = tempp / 2;
        }

        unsigned short c[5] = {0,0,0,0,0};
        unsigned char r[4] = {0,0,0,0};
        unsigned char g[4] = {0,0,0,0};
        unsigned char b[4] = {0,0,0,0};
        unsigned char a[5] = {255, 255, 255, 255, 255};
        unsigned char bits[4];

        //for(var u = 0, ppp = ptr; u<100; u++, ptr = ppp)
        for (int ih = 0; ih<texture->height; ih += 4) {
            for (int iw = 0; iw<texture->width; iw += 4) {

                c[0] = bufor[ptr++] + bufor[ptr++]*256;
                c[1] = bufor[ptr++] + bufor[ptr++]*256;
                bits[0] = bufor[ptr++];
                bits[1] = bufor[ptr++];
                bits[2] = bufor[ptr++];
                bits[3] = bufor[ptr++];

                c[4] = c[0] & 0xf800;
                r[0] = ((c[4] >> 11) << 3) + (c[4] >> 13);
                c[4] = c[0] & 0x07e0;
                g[0] = ((c[4] >> 5) << 2) + (c[4] >> 9);
                c[4] = c[0] & 0x1f;
                b[0] = ((c[4] >> 0) << 3) + (c[4] >> 2);

                c[4] = c[1] & 0xf800;
                r[1] = ((c[4] >> 11) << 3) + (c[4] >> 13);
                c[4] = c[1] & 0x07e0;
                g[1] = ((c[4] >> 5) << 2) + (c[4] >> 9);
                c[4] = c[1] & 0x1f;
                b[1] = ((c[4] >> 0) << 3) + (c[4] >> 2);

                if (c[0] <= c[1]) {
                    r[2] = (r[0] + r[1]) / 2;
                    r[3] = 0;
                    g[2] = (g[0] + g[1]) / 2;
                    g[3] = 0;
                    b[2] = (b[0] + b[1]) / 2;
                    b[3] = 0;
                    a[3] = 0;
                } else {
                    r[2] = ((2 * r[0] + r[1]) / 3);
                    r[3] = ((r[0] + 2 * r[1]) / 3);
                    g[2] = ((2 * g[0] + g[1]) / 3);
                    g[3] = ((g[0] + 2 * g[1]) / 3);
                    b[2] = ((2 * b[0] + b[1]) / 3);
                    b[3] = ((b[0] + 2 * b[1]) / 3);
                    a[3] = 255;
                }

                for (int ii = 0; ii < 4; ii++) {
                    for (int jj = 0; jj < 4; jj++) {
                        int o = (bits[ii] >> (jj * 2)) & 0x3;
                        int p = texture->bytesPerPixel * texture->width * (ih + ii) + (iw + jj) * texture->bytesPerPixel + 0;
                        texture->imageData[p] = r[o];
                        texture->imageData[p + 1] = g[o];
                        texture->imageData[p + 2] = b[o];
                        if (texture->bpp == 32)
                            texture->imageData[p + 3] = a[o];
                    }
                }
            }
        }
        texture->loaded = true;
        texture->editable = true;
    }
    //qDebug() << "--";
    delete data;
    file->close();
    //qDebug() << "2";
    return;
}

void AceLib::save(QString path, Texture* t){
    path.replace("//", "/");
    QFile *file = new QFile(path);
    qDebug() << "zapis .ace "<<path;
    if (!file->open(QIODevice::WriteOnly))
        return;
    QDataStream write(file);
    write.setByteOrder(QDataStream::LittleEndian);
    write.setFloatingPointPrecision(QDataStream::SinglePrecision);
    
    const char header[] = {
        0x53,0x49,0x4D,0x49,0x53,0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40
    };
    //header
    write.writeRawData(header, 16);
    write << (qint32)1;
    //options
    write << (qint32)0;
    //width
    write << (qint32)t->width;
    //height
    write << (qint32)t->height;
    //pixel format
    write << (qint32)14;
    //channels
    write << (qint32)3;
    //0
    write << (qint32)0;
    //empty strings
    for(int i = 0; i < 31; i++)
        write << (qint32)0;
    
    //channels
    write << (qint32)8;
    write << (qint32)0;
    write << (qint32)3;
    write << (qint32)0;
    write << (qint32)8;
    write << (qint32)0;
    write << (qint32)4;
    write << (qint32)0;
    write << (qint32)8;
    write << (qint32)0;
    write << (qint32)5;
    write << (qint32)0;
    //200
    int offset = t->height*4 + 200;
    for(int i = 0; i < t->height; i++){
        write << (qint32)offset + i*t->width*3*4;
    }
    //data
    for(int i = 0; i < t->height; i++){
        for (int j = 0; j<t->width; j++) {
            write << (qint8)t->imageData[3*t->width*i + j*3];
        }
        for (int j = 0; j<t->width; j++) {
            write << (qint8)t->imageData[3*t->width*i + j*3+1];
        }
        for (int j = 0; j<t->width; j++) {
            write << (qint8)t->imageData[3*t->width*i + j*3+2];
        }
    }
    
    write.unsetDevice();
    file->close();
}