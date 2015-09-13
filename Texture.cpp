#include "Texture.h"
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

void Texture::paint(float x, float z){
    if(!editable) 
        setEditable();
    
    int tx = x*width;
    int tz = z*height;
    for(int i = -10; i < 10; i++)
        for(int j = -10; j < 10; j++){
            if(tx+i >= width) continue;
            if(tz+j >= height) continue;
            if(tx+i < 0) continue;
            if(tz+j < 0) continue;
            if(sqrt(i*i + j*j) > 10) continue;
            imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel] = 0;
            imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+1] = 0;
            imageData[(tx+i)*width*bytesPerPixel + (tz + j)*bytesPerPixel+2] = 0;
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

