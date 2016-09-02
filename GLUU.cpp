/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GLUU.h"
//#include "zconf.h"
#include "GLMatrix.h"
#include "ReadFile.h"
#include "Game.h"
#include "Vector4f.h"
#include <QDebug>
#include <QFile>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLUU* GLUU::get() {
    static GLUU* gluu = new GLUU();
    return gluu;
}

GLUU::GLUU() {
    imvMatrixStack = 0;
    alphaTest = 0.3;
    pMatrix = new float[16];
    mvMatrix = new float[16];
    objStrMatrix = new float[16];
}

GLUU::~GLUU() {

}

const char* GLUU::getShader(QString shaderScript, QString type) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    unsigned int shader;
    if (type == "fs") {
        shader = f->glCreateShader(GL_FRAGMENT_SHADER);
    } else if (type == "vs") {
        shader = f->glCreateShader(GL_VERTEX_SHADER);
    } else {
        return "";
    }
    QFile* shaderData = new QFile("shaders/"+shaderScript+"."+type);
    if (!shaderData->open(QIODevice::ReadOnly))
        return "";
    return (const char*) ReadFile::readRAW(shaderData)->data;
}

void GLUU::initShader() {
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, getShader("vertexShaderSource", "vs"));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, getShader("fragmentShaderSource", "fs"));
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("aTextureCoord", 1);
    m_program->link();

    m_program->bind();
    pMatrixUniform = m_program->uniformLocation("uPMatrix");
    mvMatrixUniform = m_program->uniformLocation("uMVMatrix");
    msMatrixUniform = m_program->uniformLocation("uMSMatrix");
    lod = m_program->uniformLocation("lod");
    sun = m_program->uniformLocation("sun");
    brightness = m_program->uniformLocation("brightness");
    skyColor = m_program->uniformLocation("skyColor");
    skyLight = m_program->uniformLocation("sky");
    shaderAlpha = m_program->uniformLocation("isAlpha");
    shaderAlphaTest = m_program->uniformLocation("alphaTest");
    shaderTextureEnabled = m_program->uniformLocation("textureEnabled");
    shaderShapeColor = m_program->uniformLocation("shapeColor");
    //m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    //m_lightPosLoc = m_program->uniformLocation("lightPos");
    // Light position is fixed.
    //m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->release();
}

void GLUU::mvPushMatrix() {
    mvMatrixStack[imvMatrixStack++] = Mat4::clone(mvMatrix);
}

void GLUU::mvPopMatrix() {
    if (--imvMatrixStack < 0) return;
    delete[] mvMatrix;
    mvMatrix = mvMatrixStack[imvMatrixStack];
}

void GLUU::setMatrixUniforms() {
    m_program->setUniformValue(pMatrixUniform, *reinterpret_cast<float(*)[4][4]> (pMatrix));
    m_program->setUniformValue(mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (mvMatrix));
    m_program->setUniformValue(msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (objStrMatrix));
    
    m_program->setUniformValue(lod, Game::objectLod);
    m_program->setUniformValue(sun, 1.0f);
    m_program->setUniformValue(brightness, 1.0f);
    m_program->setUniformValue(skyColor, skyc[0],skyc[1],skyc[2],skyc[3]);
    m_program->setUniformValue(skyLight, sky[0],sky[1],sky[2],sky[3]);
    m_program->setUniformValue(shaderAlpha, alpha);
    m_program->setUniformValue(shaderAlphaTest, alphaTest);
    textureEnabled = true;
    m_program->setUniformValue(shaderTextureEnabled, 1.0f);
};

float GLUU::degToRad(float degrees) {
    return degrees * M_PI / 180.0;
}

void GLUU::disableTextures(Vector4f* color){
    m_program->setUniformValue(shaderShapeColor, color->x, color->y, color->z, color->c);
    if(!this->textureEnabled) return;
    this->textureEnabled = false;
    m_program->setUniformValue(shaderTextureEnabled, 0.0f);
}

void GLUU::disableTextures(Vector3f* color){
    m_program->setUniformValue(shaderShapeColor, color->x, color->y, color->z, 1.0);
    if(!this->textureEnabled) return;
    this->textureEnabled = false;
    m_program->setUniformValue(shaderTextureEnabled, 0.0f);
}

void GLUU::disableTextures(float x, float y, float z, float a){
    m_program->setUniformValue(shaderShapeColor, x, y, z, a);
    if(!this->textureEnabled) return;
    this->textureEnabled = false;
    m_program->setUniformValue(shaderTextureEnabled, 0.0f);
}

void GLUU::enableTextures(){
    if(this->textureEnabled) return;
    this->textureEnabled = true;
    m_program->setUniformValue(shaderTextureEnabled, 1.0f);
}

