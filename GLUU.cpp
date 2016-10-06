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
    pShadowMatrix = new float[16];
    mvMatrix = new float[16];
    objStrMatrix = new float[16];
}

GLUU::~GLUU() {

}

const char* GLUU::getShader(QString shaderScript, QString type) {
    QFile* shaderData = new QFile("shaders/"+shaderScript+"."+type);
    if (!shaderData->open(QIODevice::ReadOnly))
        return "";
    return (const char*) ReadFile::readRAW(shaderData)->data;
}

void GLUU::initShader() {
    QVector<QString> shaderNames;
    shaderNames.push_back("StandardFog");
    shaderNames.push_back("StandardBloom");
    shaderNames.push_back("Shadows");
    
    for(int i = 0; i < shaderNames.size(); i++ ){
        shaders[shaderNames[i]] = new Shader();
        shaders[shaderNames[i]]->addShaderFromSourceCode(QOpenGLShader::Vertex, getShader(shaderNames[i], "vs"));
        shaders[shaderNames[i]]->addShaderFromSourceCode(QOpenGLShader::Fragment, getShader(shaderNames[i], "fs"));
        currentShader = shaders[shaderNames[i]];
        currentShader->bindAttributeLocation("vertex", 0);
        currentShader->bindAttributeLocation("aTextureCoord", 1);
        currentShader->bindAttributeLocation("normal", 2);
        currentShader->link();

        currentShader->bind();
        currentShader->pMatrixUniform = currentShader->uniformLocation("uPMatrix");
        currentShader->pShadowMatrixUniform = currentShader->uniformLocation("uShadowPMatrix");
        currentShader->mvMatrixUniform = currentShader->uniformLocation("uMVMatrix");
        currentShader->msMatrixUniform = currentShader->uniformLocation("uMSMatrix");
        currentShader->lod = currentShader->uniformLocation("lod");
        currentShader->sun = currentShader->uniformLocation("sun");

        currentShader->skyColor = currentShader->uniformLocation("skyColor");

        currentShader->shaderAlpha = currentShader->uniformLocation("isAlpha");
        currentShader->shaderAlphaTest = currentShader->uniformLocation("alphaTest");
        currentShader->shaderTextureEnabled = currentShader->uniformLocation("textureEnabled");
        currentShader->shaderShapeColor = currentShader->uniformLocation("shapeColor");
        currentShader->shaderEnableNormals = currentShader->uniformLocation("enableNormals");
        currentShader->shaderDiffuseColor = currentShader->uniformLocation("diffuseColor");
        currentShader->shaderAmbientColor = currentShader->uniformLocation("ambientColor");
        currentShader->shaderSpecularColor = currentShader->uniformLocation("specularColor");
        currentShader->shaderLightDirection = currentShader->uniformLocation("lightDirection");
        currentShader->shaderSecondTexEnabled = currentShader->uniformLocation("secondTexEnabled");
        unsigned int tex1 = currentShader->uniformLocation("uSampler");
        currentShader->setUniformValue(tex1, 0);
        unsigned int tex2 = currentShader->uniformLocation("uSampler2");
        currentShader->setUniformValue(tex2, 1);
        unsigned int tex3 = currentShader->uniformLocation("shadow1");
        currentShader->setUniformValue(tex3, 2);
        currentShader->release();
    }
        //currentShader = shaders["StandardFog"];
    currentShader = shaders["StandardBloom"];
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
    currentShader->setUniformValue(currentShader->pMatrixUniform, *reinterpret_cast<float(*)[4][4]> (pMatrix));
    currentShader->setUniformValue(currentShader->pShadowMatrixUniform, *reinterpret_cast<float(*)[4][4]> (pShadowMatrix));
    currentShader->setUniformValue(currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (mvMatrix));
    currentShader->setUniformValue(currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (objStrMatrix));
    
    currentShader->setUniformValue(currentShader->lod, Game::objectLod);
    currentShader->setUniformValue(currentShader->skyColor, skyc[0],skyc[1],skyc[2],skyc[3]);
    currentShader->setUniformValue(currentShader->shaderDiffuseColor, 0.7,0.7,0.7,0.7);
    currentShader->setUniformValue(currentShader->shaderAmbientColor, 0.3,0.3,0.3,0.3);
    currentShader->setUniformValue(currentShader->shaderSpecularColor, 1.0,1.0,1.0,1.0);
    currentShader->setUniformValue(currentShader->shaderLightDirection, -1.0,2.0,1.0);
    currentShader->setUniformValue(currentShader->shaderAlpha, alpha);
    currentShader->setUniformValue(currentShader->shaderAlphaTest, alphaTest);
    textureEnabled = true;
    normalsEnabled = true;
    currentShader->setUniformValue(currentShader->shaderTextureEnabled, 1.0f);
    currentShader->setUniformValue(currentShader->shaderEnableNormals, 1.0f);
    currentShader->setUniformValue(currentShader->shaderSecondTexEnabled, 0.0f);
};

float GLUU::degToRad(float degrees) {
    return degrees * M_PI / 180.0;
}

void GLUU::disableTextures(Vector4f* color){
    currentShader->setUniformValue(currentShader->shaderShapeColor, color->x, color->y, color->z, color->c);
    if(!this->textureEnabled) return;
    this->textureEnabled = false;
    currentShader->setUniformValue(currentShader->shaderTextureEnabled, 0.0f);
}

void GLUU::disableTextures(Vector3f* color){
    currentShader->setUniformValue(currentShader->shaderShapeColor, color->x, color->y, color->z, 1.0);
    if(!this->textureEnabled) return;
    this->textureEnabled = false;
    currentShader->setUniformValue(currentShader->shaderTextureEnabled, 0.0f);
}

void GLUU::disableTextures(float x, float y, float z, float a){
    currentShader->setUniformValue(currentShader->shaderShapeColor, x, y, z, a);
    if(!this->textureEnabled) return;
    this->textureEnabled = false;
    currentShader->setUniformValue(currentShader->shaderTextureEnabled, 0.0f);
}

void GLUU::enableTextures(){
    if(this->textureEnabled) return;
    this->textureEnabled = true;
    currentShader->setUniformValue(currentShader->shaderTextureEnabled, 1.0f);
}

void GLUU::disableNormals(){
    if(!this->normalsEnabled) return;
    this->normalsEnabled = false;
    currentShader->setUniformValue(currentShader->shaderEnableNormals, 0.0f);
}

void GLUU::enableNormals(){
    if(this->normalsEnabled) return;
    this->normalsEnabled = true;
    currentShader->setUniformValue(currentShader->shaderEnableNormals, 1.0f);
}
