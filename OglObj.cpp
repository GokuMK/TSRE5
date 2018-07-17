/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "OglObj.h"
#include "TexLib.h"
#include "Game.h"
#include "GLMatrix.h"

OglObj::OglObj() {
    loaded = false;
    texId = -1;
    materialType = NONE;
    for(int i = 0; i < 6; i++)
        bound[i] = 0;   
}

OglObj::OglObj(const OglObj& orig) {
}

OglObj::~OglObj() {

}

void OglObj::setMaterial(float r, float g, float b) {
    materialType = COLOR;
    if(color == NULL) {
        color = new Vector4f(r, g, b, 1.0);
    } else {
        color->x = r;
        color->y = g;
        color->z = b;
        color->c = 1.0;
    }
}

void OglObj::setMaterial(QString* path) {
    materialType = TEXTURE;
    res = path;
}

void OglObj::deleteVBO(){
    if(loaded){
        VBO.destroy();
        VAO.destroy();
    }
    loaded = false;
}

float* OglObj::mapBuffer(){
    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);
    VBO.bind();
    return (float*)VBO.map(QOpenGLBuffer::ReadWrite);
    VBO.release();
}

void OglObj::unmapBuffer(){
    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);
    VBO.bind();
    VBO.unmap();
    VBO.release();
}

void OglObj::init(float* punkty, int ptr, enum VertexAttr v, int type) {
    //if(loaded){
    //    VBO.destroy();
    //    VAO.destroy();
    //}
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    shapeType = type;
    vAttribures = v;
    if(!loaded){
        VAO.create();
        VBO.create();
    }
    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);
    //VBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    VBO.bind();
    VBO.allocate(punkty, ptr * sizeof (GLfloat));
    
    if (v == V) {
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
    } else if (v == VT) {
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glEnableVertexAttribArray(3);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
        f->glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), reinterpret_cast<void *> (5 * sizeof (GLfloat)));
    } else if (v == VNT) {
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glEnableVertexAttribArray(2);
        f->glEnableVertexAttribArray(3);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof (GLfloat), reinterpret_cast<void *> (6 * sizeof (GLfloat)));
        f->glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof (GLfloat), reinterpret_cast<void *> (8 * sizeof (GLfloat)));
    }
    
    VBO.release();
    length = ptr / v;
    loaded = true;
}

void OglObj::setLineWidth(int val){
    lineWidth = val;
}

void OglObj::render() {
    render(0);
}

void OglObj::setDistanceRange(float min, float max){
    minDistance = min;
    maxDistance = max;
}

void OglObj::render(int selectionColor, float lod) {
    if(!loaded)
        return;
    if(lod > maxDistance || lod < minDistance)
        return;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    GLUU *gluu = GLUU::get();
    
    if(vAttribures == NO_ATTR){
        return;
    } else if(vAttribures == V){
        gluu->disableNormals();
    } else if(vAttribures == VT){
        gluu->disableNormals();
    } else if(vAttribures == VNT){
        gluu->enableNormals();
    }  
    
    if(selectionColor != 0){
        gluu->disableTextures(selectionColor);
    } else if(materialType == TEXTURE){
        gluu->enableTextures();
        if (texId == -2) {
            return;
        } else {
            if (texId == -1) {
                texId = TexLib::addTex(*res);
                //qDebug() << texId << " "<< *res;
            }
            if (TexLib::mtex[texId]->loaded) {
                if (!TexLib::mtex[texId]->glLoaded)
                    TexLib::mtex[texId]->GLTextures();
                gluu->bindTexture(f, TexLib::mtex[texId]->tex[0]);
                //f->glBindTexture(GL_TEXTURE_2D, TexLib::mtex[texId]->tex[0]);
            } else {
            }
        }
    } else if(materialType == COLOR){
        gluu->disableTextures(color);
    } else if(materialType == NONE){
        return;
    }

    if(lineWidth > 0 && lineWidth != Game::oglDefaultLineWidth)
        f->glLineWidth(lineWidth);
    gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]>(gluu->objStrMatrix));
    gluu->currentMsMatrinxHash = 0;//gluu->getMatrixHash(gluu->objStrMatrix);
    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);
    f->glDrawArrays(shapeType, 0, length); /**/
    
    if(lineWidth > 0 && lineWidth != Game::oglDefaultLineWidth)
        f->glLineWidth(Game::oglDefaultLineWidth);
}

int OglObj::getTexId(){
    return this->texId;
}

bool OglObj::getSimpleBorder(float* border){
    border[0] = bound[0];
    border[1] = bound[1];
    border[2] = bound[2];
    border[3] = bound[3];
    border[4] = bound[4];
    border[5] = bound[5];
    return true;
}

void OglObj::setBound(float *b){
    bound[0] = b[0];
    bound[1] = b[1];
    bound[2] = b[2];
    bound[3] = b[3];
    bound[4] = b[4];
    bound[5] = b[5];
}