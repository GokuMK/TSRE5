/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GLUU_H
#define	GLUU_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include "Vector4f.h"
#include "Vector3f.h"
#include "Shader.h"
//QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)


class GLUU {
public:
    Shader *currentShader;
    QHash<QString, Shader*> shaders;
    
    float alpha;
    float alphaTest;
    
    float skyc[4]{230.0/255.0,248.0/255,255.0/255.0, 1.0};
    //float skyc[4]{200.0/255.0,218.0/255,225.0/255.0, 1.0};
    float sky[3]{1.0, 1.0, 1.0};
    
    float* mvMatrixStack[1000];
    int imvMatrixStack;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    //QMatrix4x4 m_proj;
    //QMatrix4x4 m_camera;
    //QMatrix4x4 m_world;
    float* pMatrix;
    float* mvMatrix;
    float* objStrMatrix;
    static GLUU *get();
    GLUU();
    virtual ~GLUU();
    void initShader();
    void setMatrixUniforms();
    void mvPushMatrix();
    void mvPopMatrix();
    float degToRad(float degrees);
    void disableTextures(Vector4f* color);
    void disableTextures(Vector3f* color);
    void disableTextures(float x, float y, float z, float a);
    void enableTextures();
    void disableNormals();
    void enableNormals();
private:
    const char* getShader(QString shaderScript, QString type);
    bool textureEnabled;
    bool normalsEnabled;
    Vector4f shapeColor;
};

#endif	/* GLUU_H */

