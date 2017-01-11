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
class Brush;

class Texture {
public:
    Texture();
    Texture(const Texture* orig);
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
     bool loaded = false;
     int ref = 0;
     bool glLoaded = false;
     bool editable = false;

    Texture(QString pathid) {
        this->pathid = pathid;
    }

    bool GLTextures();
    void update();
    void crop(float x1, float y1, float x2, float y2);
    void paint(Brush* brush, float x, float z);
    void sendToUndo(int id);
    void fillData(unsigned char* data);
    unsigned char * getImageData(int width, int height);
    void delVBO();
private:
    
    void setEditable();
};

#endif	/* TEXTURE_H */

