/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SimpleHud.h"
#include "GLMatrix.h"
#include "GLUU.h"
#include "FileBuffer.h"
#include "TextObj.h"

SimpleHud::SimpleHud() {

    
    speedText = new TextObj("Speed:", 4, 0.05, 3);
    speedText->pos[0] = -0.90;
    speedText->pos[1] = 0.95;
    speedText->setRotOffset(0);
    speedText->setColor(255,255,0);
    speedText->setOColor(0,0,0);
    speedText->setFontName("Ni7seg");
    
    distanceText = new TextObj("Distance:", 6, 0.05, 3);
    distanceText->pos[0] = -0.85;
    distanceText->pos[1] = 0.80;
    distanceText->setRotOffset(0);
    distanceText->setColor(255,255,0);    
    distanceText->setOColor(0,0,0);
    distanceText->setFontName("Ni7seg");
    
    kmText = new TextObj("km", 3, 0.05, 3);
    kmText->setRotOffset(0);
    kmText->setColor(255,255,0);
    kmText->setOColor(0,0,0);
    kmText->setFontName("Ni7seg");
    
    kmsText = new TextObj("km/s", 3, 0.05, 3);
    kmsText->setRotOffset(0);
    kmsText->setColor(255,255,0);
    kmsText->setOColor(0,0,0);
    kmsText->setFontName("Ni7seg");
    
    for(int i = 0; i < 10; i++){
        numberText.push_back(new TextObj(i, 4, 0.1, 4));
        numberText.back()->setRotOffset(0);
        numberText.back()->setColor(255,255,0);
        numberText.back()->setOColor(0,0,0);
        numberText.back()->setFontName("Ni7seg");
    }
    numberText.push_back(new TextObj(",", 4, 0.1, 4));
    numberText.back()->setRotOffset(0);
    numberText.back()->setColor(255,255,0);
    numberText.back()->setOColor(0,0,0);
    numberText.back()->setFontName("Ni7seg");
    
    
    
    
    gradientObj = new OglObj();
    
    float *punkty = new float[54];
    int ptr = 0;
    int i = 0;

    
    float alpha = -0.01;
    punkty[ptr++] = -1.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.01;
    punkty[ptr++] = 0.01;
    punkty[ptr++] = alpha;

    punkty[ptr++] = -1.0;
    punkty[ptr++] = 0.7;
    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.01;
    punkty[ptr++] = 0.99;
    punkty[ptr++] = alpha;

    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.7;
    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.99;
    punkty[ptr++] = 0.99;
    punkty[ptr++] = alpha;

    punkty[ptr++] = -0.5;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.99;
    punkty[ptr++] = 0.01;
    punkty[ptr++] = alpha;

    punkty[ptr++] = -1.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.01;
    punkty[ptr++] = 0.01;
    punkty[ptr++] = alpha;

    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.7;
    punkty[ptr++] = -0.5;
    punkty[ptr++] = 0.99;
    punkty[ptr++] = 0.99;
    punkty[ptr++] = alpha;
    QString resmtext;

    gradientObj->setMaterial(new QString(QString("tsre_appdata/")+Game::AppDataVersion+"/gradient.png"));
    gradientObj->init(punkty, ptr, gradientObj->VT, GL_TRIANGLES);
    delete[] punkty;
}

SimpleHud::SimpleHud(const SimpleHud& orig) {
}

SimpleHud::~SimpleHud() {
}

void SimpleHud::setAttribute(QString name, float val){
    attributes[name] = val;
}

void SimpleHud::render(){
    GLUU* gluu = GLUU::get();
    
    gradientObj->render();
    
    speedText->render();
    distanceText->render();
    
    int speed = attributes["speed"] * 3.6;
    int distance = attributes["dist"] / 100;
    int s1 = speed % 10;
    speed /=10;
    int s2 = speed % 10;
    speed /=10;
    int s3 = speed % 10;
    int d1 = distance % 10;
    distance /=10;
    int d2 = distance % 10;
    distance /=10;
    int d3 = distance % 10;
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, -0.96, 0.85, 0);
    numberText[s3]->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.07, 0.0, 0);
    numberText[s2]->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.07, 0.0, 0);
    numberText[s1]->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.12, 0.0, 0);
    kmsText->render();
    
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, -0.26, -0.15, 0);
    numberText[d3]->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.07, 0.0, 0);
    numberText[d2]->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.05, 0.0, 0);
    numberText[10]->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.05, 0.0, 0);
    numberText[d1]->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.08, 0.0, 0);
    kmText->render();
    gluu->mvPopMatrix();
    
}