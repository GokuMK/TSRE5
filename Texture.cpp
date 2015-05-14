#include "Texture.h"
#include <QOpenGLShaderProgram>
#include <QString>
#include <QDebug>

Texture::Texture() {
}

Texture::Texture(const Texture& orig) {
}

Texture::~Texture() {
}

bool Texture::GLTextures() {
    tex = new unsigned int[1];
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete imageData;
    glLoaded = true;
    return true;
}

void Texture::delVBO() {
    //System.out.println("==== usuwam texture!");
    glLoaded = false;
    loaded = false;
    //gl.glDeleteTextures(1, tex, 0);
}

