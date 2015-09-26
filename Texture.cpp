#include "Texture.h"
#include "Brush.h"
#include <QOpenGLShaderProgram>
#include <QString>
#include <QDebug>
#include <QOpenGLFunctions>

Texture::Texture() {
}

Texture::Texture(const Texture* orig) {
    width = orig->width;
    height = orig->height;
    bpp = orig->bpp;
    type = orig->type;
    bytesPerPixel = orig->bytesPerPixel;
    
    //QOpenGLFunctions_3_2_Core *f = new QOpenGLFunctions_3_2_Core();

    imageData = new unsigned char[bytesPerPixel*width*height];
    glBindTexture(GL_TEXTURE_2D, orig->tex[0]);
    glGetTexImage(GL_TEXTURE_2D, 0, orig->type, GL_UNSIGNED_BYTE, imageData);
    this->editable = true;
    
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
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glGetTexImage(GL_TEXTURE_2D, 0, type, GL_UNSIGNED_BYTE, imageData);
    this->editable = true;
}

unsigned char * Texture::getImageData(int width, int height){
    if(!editable) 
        setEditable();
    
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
    
    for(int i = -brush->size; i < brush->size; i++)
        for(int j = -brush->size; j < brush->size; j++){
            txi = tx+i;
            tzj = tz+j;
            if(tx+i >= height) continue;
            if(tz+j >= width) continue;
            if(tx+i < 0) continue;
            if(tz+j < 0) continue;
            if(sqrt(i*i + j*j) > brush->size) continue;
            
            talpha = (brush->alpha)*(1.0-(float)sqrt(i*i + j*j)/brush->size);
            txi*=1;
            tzj*=1;
            
            if(tex != NULL){
                
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
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel] + (talpha)*(0);
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+1] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+1] + (talpha)*(0);
                imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+2] 
                        = (1-talpha)*imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+2] + (talpha)*(0);
            }
        }
}

void Texture::update(){
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
}

Texture::~Texture() {
}

bool Texture::GLTextures() {
    if(!loaded) return false;
    
    tex = new unsigned int[1];
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
    
    //glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, width, height);
    //f->glTexSubImage2D(GL_TEXTURE_2D, 0​, 0, 0, width​, height​, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    //f->glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete imageData;
    this->editable = false;
    glLoaded = true;
    return true;
}


void Texture::delVBO() {
    //System.out.println("==== usuwam texture!");
    glLoaded = false;
    loaded = false;
    //gl.glDeleteTextures(1, tex, 0);
}

