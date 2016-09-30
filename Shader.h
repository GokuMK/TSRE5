/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHADER_H
#define	SHADER_H

#include <QOpenGLShaderProgram>

class Shader : public QOpenGLShaderProgram {
public:
    Shader();
    virtual ~Shader();
    unsigned int shaderProgram;
    unsigned int vertexPositionAttribute;
    unsigned int textureCoordAttribute;
    unsigned int pMatrixUniform;
    unsigned int mvMatrixUniform;
    unsigned int msMatrixUniform;
    unsigned int samplerUniform;
    unsigned int lod;
    unsigned int sun;
    unsigned int brightness;
    unsigned int skyColor;
    unsigned int skyLight;
    unsigned int shaderAlpha;
    unsigned int shaderAlphaTest;
    unsigned int shaderTextureEnabled;
    unsigned int shaderShapeColor;
    unsigned int shaderEnableNormals;
private:

};

#endif	/* SHADER_H */

