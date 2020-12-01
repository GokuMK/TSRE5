/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TextObj.h"
#include "GLMatrix.h"
#include "GLUU.h"
#include "Renderer.h"

TextObj::TextObj(int val, float s, float sc, int resm) : OglObj() {
    this->text.setNum(val, 10);
    if(s > 0)
        size = s;
    if(sc > 0)
        scale = sc;
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    resMult = resm;
}

TextObj::TextObj(QString val, float s, float sc, int resm) : OglObj() {
    this->text = val;
    if(s > 0)
        size = s;
    if(sc > 0)
        scale = sc;
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    resMult = resm;
}

TextObj::TextObj() : OglObj() {
    this->text = " ";
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
}

void TextObj::setColor(int r, int g, int b){
    color.setRgb(r, g, b);
}

void TextObj::setOColor(int r, int g, int b){
    ocolor.setRgb(r, g, b);
    isOutline = true;
}

void TextObj::setFontName(QString val){
    fontName = val;
}

void TextObj::setRotOffset(float val){
    rotOffset = val;
}

void TextObj::init(){
    GLUU* gluu = GLUU::get();
    float *punkty = new float[54];
    int ptr = 0;
    int i = 0;

    float alpha = -gluu->alphaTest;
    
    punkty[ptr++] = (-size / 2)*scale;
    punkty[ptr++] = 1*scale;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (-size / 2)*scale;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 1.00;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (size / 2)*scale;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (size / 2)*scale;
    punkty[ptr++] = 1*scale;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (-size / 2)*scale;
    punkty[ptr++] = 1*scale;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (size / 2)*scale;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = alpha;
    QString args;
    if(resMult > 1)
        args += ".resm:"+QString::number((int)resMult);
    if(fontName.length() > 0)
        args += ".font:"+fontName;
    if(isOutline)
        args += ".ocolor:"+ocolor.name();
    args += ".size:"+QString::number((int)size);
    args += ".color:"+color.name();
    this->setMaterial(new QString(text+args+".:paintTex"));
    OglObj::init(punkty, ptr, RenderItem::VT, GL_TRIANGLES);
    delete[] punkty;
    isInit = true;
}

TextObj::TextObj(const TextObj& orig) {
}

TextObj::~TextObj() {
}

void TextObj::render() {
    render(0.0);
}

void TextObj::pushRenderItem() {
    pushRenderItem(0);
}

void TextObj::pushRenderItem(float rot) {
    if(!isInit)
        init();

    Game::currentRenderer->mvPushMatrix();
    Mat4::translate(Game::currentRenderer->mvMatrix, Game::currentRenderer->mvMatrix, pos[0], pos[1], pos[2]);
    Mat4::rotateY(Game::currentRenderer->mvMatrix, Game::currentRenderer->mvMatrix, rot+rotOffset);

    OglObj::pushRenderItem();

    Game::currentRenderer->mvPopMatrix();
}

void TextObj::render(float rot) {
    if(!isInit)
        init();

    GLUU* gluu = GLUU::get();
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, pos[0], pos[1], pos[2]);
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, rot+rotOffset);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));

    OglObj::render();

    gluu->mvPopMatrix();
}
