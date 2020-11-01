/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <QString>
#include <QVector>

class Brush;

class Texture {
public:
    Texture();
    Texture(QString pathid);
    Texture(const Texture* orig);
    Texture(int x, int y, int bpp, Brush* brush = NULL);
    virtual ~Texture();

    unsigned char* imageData;
    int bpp;
    int imageSize;
    int bytesPerPixel;
    int compressed;
    int width;
    int height;
    int texID;
    int type;
    int typk;
    unsigned int* tex;
    QString pathid;
    QVector<QString> hashid;
    bool loaded = false;
    int ref = 0;
    bool glLoaded = false;
    bool editable = false;
    bool missing = false;
    bool error = false;

    void setEditable();
    bool GLTextures(bool mipmaps = false);
    void update();
    void advancedCrop(float *texCoords, int w = 0, int h = 0);
    void crop(float x1, float y1, float x2, float y2);
    void paint(Brush* brush, float x, float z);
    void sendToUndo(int id);
    void fillData(unsigned char* data);
    unsigned char * getImageData(int width, int height);
    void delVBO();
private:

};

#endif	/* TEXTURE_H */

