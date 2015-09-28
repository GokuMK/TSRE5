/* 
 * File:   TextObj.cpp
 * Author: Goku
 * 
 * Created on 14 maja 2015, 21:43
 */

#include "TextObj.h"
#include "GLMatrix.h"
#include "GLUU.h"

TextObj::TextObj(int val) : OglObj() {
    this->text.setNum(val, 10);
    this->init();
}

TextObj::TextObj() : OglObj() {
    this->text = " ";
    this->init();
}

void TextObj::init(){
    float *punkty = new float[48];
    int ptr = 0;
    int i = 0;

    punkty[ptr++] = -2;
    punkty[ptr++] = 1;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;

    punkty[ptr++] = -2;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 1.00;

    punkty[ptr++] = 2;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 1.0;

    punkty[ptr++] = 2;
    punkty[ptr++] = 1;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;

    punkty[ptr++] = -2;
    punkty[ptr++] = 1;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;

    punkty[ptr++] = 2;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 1.0;

    this->setMaterial(new QString(text+".:paintTex"));
    OglObj::init(punkty, ptr, this->VNT, GL_TRIANGLES);
    delete[] punkty;
}

TextObj::TextObj(const TextObj& orig) {
}

TextObj::~TextObj() {
}

void TextObj::render() {
    render(0.0);
}

void TextObj::render(float rot) {

    GLUU* gluu = GLUU::get();
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, pos[0], pos[1], pos[2]);
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, rot+3.14);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));

    OglObj::render();

    gluu->mvPopMatrix();
}
