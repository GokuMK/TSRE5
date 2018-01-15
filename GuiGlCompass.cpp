/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GuiGlCompass.h"
#include "GLMatrix.h"
#include "GLUU.h"
#include "FileBuffer.h"

GuiGlCompass::GuiGlCompass() {
    
    static QString texString = QString("tsre_appdata/")+Game::AppDataVersion+"/compass.png";
    
    GLUU* gluu = GLUU::get();
    float *punkty = new float[54];
    int ptr = 0;
    int i = 0;

    float alpha = -gluu->alphaTest;
    float sizex = 0.3;
    float sizey = 0.1;
    
    punkty[ptr++] = (-sizex / 2);
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (-sizex / 2);
    punkty[ptr++] = 1.0-sizey;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (sizex / 2);
    punkty[ptr++] = 1.0-sizey;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.4;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (sizex / 2);
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.4;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (-sizex / 2);
    punkty[ptr++] = 1.0;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = 0.0;
    punkty[ptr++] = alpha;

    punkty[ptr++] = (sizex / 2);
    punkty[ptr++] = 1.0-sizey;
    punkty[ptr++] = 0;
    punkty[ptr++] = 0.4;
    punkty[ptr++] = 1.0;
    punkty[ptr++] = alpha;
    
    this->setMaterial(&texString);
    OglObj::init(punkty, ptr, this->VT, GL_TRIANGLES);
    
    delete[] punkty;
}

GuiGlCompass::~GuiGlCompass() {
}

void GuiGlCompass::render(){
    render(0);
}

void GuiGlCompass::render(float a){
    
    float *data = mapBuffer();
    a = a / (M_PI*2);
    data[3] = a-0.2;
    data[3+6] = a-0.2;
    data[3+12] = a+0.2;
    data[3+18] = a+0.2;
    data[3+24] = a-0.2;
    data[3+30] = a+0.2;

    unmapBuffer();
    
    OglObj::render();
}