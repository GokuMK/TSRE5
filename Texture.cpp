/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Texture.h"
#include "Brush.h"
#include "Undo.h"
#include <QOpenGLShaderProgram>
#include <QString>
#include <QDebug>
#include <QColor>
#include "GLUU.h"
#include "Game.h"

Texture::Texture() {
}

Texture::Texture(QString pathid) {
    this->pathid = pathid;
    this->hashid.push_back(pathid);
    //temp fix for dds/ace loading
    // Openrails uses .dds textures instead of .ace
    QString tType = pathid.toLower().split(".").last();
    if(tType == "dds"){
        hashid.push_back(pathid.left(pathid.length() - 3)+"ace");
    }
}
    
Texture::Texture(int x, int y, int bpp, Brush* brush){
    width = x;
    height = y;
    bpp = bpp;
    bytesPerPixel = (bpp / 8);
    imageSize = (bytesPerPixel * width * height);
    imageData = new unsigned char[imageSize];
    std::fill(imageData, imageData+imageSize, 255);
    if (bpp == 24) {
        type = GL_RGB;
    } else {
        type = GL_RGBA;
    }

    editable = true;
    loaded = true;
}

Texture::Texture(const Texture* orig) {
    qDebug() << "clone tex" << orig->pathid;
    //QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->functions();
    width = orig->width;
    height = orig->height;
    bpp = orig->bpp;
    type = orig->type;
    bytesPerPixel = orig->bytesPerPixel;
    
    //QOpenGLFunctions_3_2_Core *f = new QOpenGLFunctions_3_2_Core();
    if(orig->editable){
        imageData = new unsigned char[bytesPerPixel*width*height];
        memcpy(imageData, orig->imageData, bytesPerPixel*width*height);
        this->editable = true;
    } else {
        imageData = new unsigned char[bytesPerPixel*width*height];
        glBindTexture(GL_TEXTURE_2D, orig->tex[0]);
        glGetTexImage(GL_TEXTURE_2D, 0, orig->type, GL_UNSIGNED_BYTE, imageData);
        this->editable = true;
    }

    tex = new unsigned int[1];
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //delete imageData;
    glLoaded = true;

    /*unsigned int* pex = new unsigned int[1];
    f->glGenBuffers(1, pex);
    f->glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pex[0]);
    f->glBufferData(GL_PIXEL_UNPACK_BUFFER, bytesPerPixel*width*height, 0, GL_STREAM_DRAW_ARB);
    imageData = (unsigned byte*)f->glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    
    
    
    f->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                type, GL_UNSIGNED_BYTE, 0);
    
    f->glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);*/
    loaded = true;
}

void Texture::setEditable(){
    imageData = new unsigned char[bytesPerPixel*width*height];

    //QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()-> functions();
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glGetTexImage(GL_TEXTURE_2D, 0, type, GL_UNSIGNED_BYTE, imageData);
    this->editable = true;
}

unsigned char * Texture::getImageData(int width, int height){
    if(!editable) 
        setEditable();
    
    //qDebug() << width << height << bytesPerPixel;
    unsigned char * out = new unsigned char[width*height*bytesPerPixel];
    
    float scalew = (float)this->width/width;
    float scaleh = (float)this->height/height;
    
    qDebug() << this->width <<" "<< this->height;
    
    int lineWidth = (this->width*bytesPerPixel);
    //if( lineWidth%4 !=0) 
    //    lineWidth = lineWidth + 4 - lineWidth%4;
    //lineWidth /= 4;
    //if(lineWidth*4 < this->width*bytesPerPixel)
    //    lineWidth = lineWidth*4+4;
    //else
    //    lineWidth = lineWidth*4;
    
    for(int i = 0; i < height; i++ )
        for(int j = 0; j < width; j++ ){
            int wsi = scaleh*i;
            int hsi = scalew*j;
            out[i*width*bytesPerPixel + j*bytesPerPixel+0] = imageData[wsi*lineWidth + hsi*bytesPerPixel+0];
            out[i*width*bytesPerPixel + j*bytesPerPixel+1] = imageData[wsi*lineWidth + hsi*bytesPerPixel+1];
            out[i*width*bytesPerPixel + j*bytesPerPixel+2] = imageData[wsi*lineWidth + hsi*bytesPerPixel+2];
            if(bytesPerPixel == 4)
                out[i*width*bytesPerPixel + j*bytesPerPixel+3] = imageData[wsi*lineWidth + hsi*bytesPerPixel+3];
        }
    
    return out;
}

void Texture::advancedCrop(float* texCoords, int w, int h){
    if(!editable)
        setEditable();
    
    if(w == 0) 
        w = width;
    if(h == 0) 
        h = height;
    float texCoords2[7];
    texCoords2[1] = texCoords[1]*w;
    texCoords2[2] = texCoords[2]*h;
    texCoords2[3] = texCoords[3]*16.0;//*width;
    texCoords2[4] = texCoords[4]*16.0;//*height;
    texCoords2[5] = texCoords[5]*16.0;//*width;
    texCoords2[6] = texCoords[6]*16.0;//*height;
    
    qDebug() << width << height << bytesPerPixel << "--" << w << h;
    qDebug() << texCoords2[1] << texCoords2[2] << texCoords2[3] << texCoords2[4] << texCoords2[5] << texCoords2[6];
    qDebug() << texCoords[1] << texCoords[2] << texCoords[3] << texCoords[4] << texCoords[5] << texCoords[6];
    
    unsigned char* newData = new unsigned char[w*h*this->bytesPerPixel];    
    
    float ii, jj;
    float widthRatio = (float)width/w;
    float heightTatio = (float)height/h;
    for(int i = 0; i < w; i++)
        for(int j = 0; j < h; j++){
            jj = texCoords2[1] + texCoords2[3]*j + texCoords2[4]*i;
            ii = texCoords2[2] + texCoords2[5]*j + texCoords2[6]*i;
            ii *= widthRatio;
            jj *= heightTatio;

            while(ii >= width)
                ii -= width;
            while(jj >= height)
                jj -= height;
            
            while(ii < 0)
                ii += width;
            while(jj < 0)
                jj += height;
                            
            newData[i*w*bytesPerPixel+j*bytesPerPixel+0] = imageData[(int)ii*width*bytesPerPixel+(int)jj*bytesPerPixel+0];
            newData[i*w*bytesPerPixel+j*bytesPerPixel+1] = imageData[(int)ii*width*bytesPerPixel+(int)jj*bytesPerPixel+1];
            newData[i*w*bytesPerPixel+j*bytesPerPixel+2] = imageData[(int)ii*width*bytesPerPixel+(int)jj*bytesPerPixel+2];
            if(this->bytesPerPixel == 4)
                newData[i*width*bytesPerPixel+j*bytesPerPixel+3] = imageData[(int)ii*width*bytesPerPixel+(int)jj*bytesPerPixel+3];            
        }
        
    qDebug() << "advanced rot finished";
    delete[] this->imageData;
    this->imageData = newData;
    this->width = w;
    this->height = h;
    this->update();
}

void Texture::crop(float x1, float y1, float x2, float y2){
    if(!editable) 
        setEditable();

    qDebug() << x1 <<" "<<y1<<" "<<x2<<" "<<y2;

    if(x1 < x2 && y1 < y2)
        return;   
    
    unsigned char* newData = new unsigned char[this->width*this->height*this->bytesPerPixel];    
    
    if(x1 > x2 && y1 > y2){
        int ii, jj;
        for(int i = 0; i < width; i++)
            for(int j = 0; j < height; j++){
                ii = width - i - 1;
                jj = height - j - 1;
                newData[i*width*bytesPerPixel+j*bytesPerPixel+0] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+0];
                newData[i*width*bytesPerPixel+j*bytesPerPixel+1] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+1];
                newData[i*width*bytesPerPixel+j*bytesPerPixel+2] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+2];
                if(this->bytesPerPixel == 4)
                    newData[i*width*bytesPerPixel+j*bytesPerPixel+3] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+3];            
            }
    }
    
    if(x1 > x2 && y1 < y2){
        int ii, jj;
        for(int i = 0; i < width; i++)
            for(int j = 0; j < height; j++){
                ii = j;
                jj = width - i - 1;
                newData[i*width*bytesPerPixel+j*bytesPerPixel+0] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+0];
                newData[i*width*bytesPerPixel+j*bytesPerPixel+1] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+1];
                newData[i*width*bytesPerPixel+j*bytesPerPixel+2] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+2];
                if(this->bytesPerPixel == 4)
                    newData[i*width*bytesPerPixel+j*bytesPerPixel+3] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+3];            
            }
        ii = this->height;
        this->height = this->width;
        this->width = ii;
    }

    if(x1 < x2 && y1 > y2){
        int ii, jj;
        for(int i = 0; i < width; i++)
            for(int j = 0; j < height; j++){
                ii = height - j - 1;
                jj = i;
                newData[i*width*bytesPerPixel+j*bytesPerPixel+0] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+0];
                newData[i*width*bytesPerPixel+j*bytesPerPixel+1] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+1];
                newData[i*width*bytesPerPixel+j*bytesPerPixel+2] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+2];
                if(this->bytesPerPixel == 4)
                    newData[i*width*bytesPerPixel+j*bytesPerPixel+3] = imageData[ii*width*bytesPerPixel+jj*bytesPerPixel+3];
            }
        ii = this->height;
        this->height = this->width;
        this->width = ii;
    }
    
    delete[] this->imageData;
    this->imageData = newData;
    
    this->update();
}

void Texture::sendToUndo(int id){
    if(!editable) 
        setEditable();
    Undo::PushTextureData(id, imageData, bytesPerPixel*width*height);
}

void Texture::fillData(unsigned char* data){
    if(imageData == NULL)
        return;
    memcpy(imageData, data, bytesPerPixel*width*height);
    update();
}

void Texture::paint(Brush* brush, float x, float z){
    if(!editable) 
        setEditable();
    
    Texture* tex = brush->tex;
    
    if(tex != NULL){
        if(!tex->loaded) tex = NULL;
        else if(!tex->editable)
            tex->setEditable();
    }
    
    int tx = x*width;
    int tz = z*height;
    
    int txi, tzj;
    
    float talpha = 0;
    
    int size = (brush->size*this->width)/512;
    if(size < 1)
        size = 1;
    //size = (size/512);
    
    for(int i = -size; i < size; i++)
        for(int j = -size; j < size; j++){
            txi = tx+i;
            tzj = tz+j;
            if(tx+i >= height) continue;
            if(tz+j >= width) continue;
            if(tx+i < 0) continue;
            if(tz+j < 0) continue;
            //if(sqrt(i*i + j*j) > size) continue;
            
            talpha = (brush->alpha)*brush->getAlpha(i, j, size);
            //talpha = (brush->alpha)*(1.0-(float)sqrt(i*i + j*j)/size);
            txi*=1;
            tzj*=1;
            
            if(tex != NULL && brush->useTexture){
                
                if(tzj >= tex->width){
                    tzj = tzj%tex->width;
                }
                if(txi >= tex->height){
                    txi = txi%tex->height;
                }
                
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel] + (talpha)*tex->imageData[(txi)*tex->width*tex->bytesPerPixel + (tzj)*tex->bytesPerPixel];
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+1] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+1] + (talpha)*tex->imageData[(txi)*tex->width*tex->bytesPerPixel + (tzj)*tex->bytesPerPixel+1];
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+2] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+2] + (talpha)*tex->imageData[(txi)*tex->width*tex->bytesPerPixel + (tzj)*tex->bytesPerPixel+2];
            } else {
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel] + (talpha)*(brush->color[0]);
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+1] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+1] + (talpha)*(brush->color[1]);
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+2] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+2] + (talpha)*(brush->color[2]);
            }
        }
}

void Texture::update(){
    //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
}

Texture::~Texture() {
}

bool Texture::GLTextures(bool mipmaps) {
    if(!loaded) return false;
    
    if(Game::AASamples > 0 && Game::AARemoveBorder)
        if(type == GL_RGBA){
            for (int i = 0; i < height; i++)
                imageData[i*width*bytesPerPixel + (width-1)*bytesPerPixel + 3] = 0;
            for (int i = 0; i < height; i++)
                imageData[i*width*bytesPerPixel + 3] = 0;
            for (int i = 0; i < width; i++)
                imageData[(height-1)*width*bytesPerPixel + i*bytesPerPixel + 3] = 0;
            for (int i = 0; i < width; i++)
                imageData[i*bytesPerPixel + 3] = 0;
        }

    tex = new unsigned int[1];
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
    
    //f->glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, width, height);
    //f->glTexSubImage2D(GL_TEXTURE_2D, 0​, 0, 0, width​, height​, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    if(mipmaps){
        f->glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR );
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR );
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] imageData;
    imageData = NULL;
    this->editable = false;
    glLoaded = true;
    return true;
}


void Texture::delVBO() {
    //System.out.println("==== usuwam texture!");
    glLoaded = false;
    loaded = false;
    editable = false;
    missing = false;
    error = false;
    //gl.glDeleteTextures(1, tex, 0);
}

