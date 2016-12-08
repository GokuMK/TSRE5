/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "RulerObj.h"
#include "GLMatrix.h"
#include "TrackObj.h"
#include "GLMatrix.h"
#include "TrackItemObj.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include "Game.h"

bool RulerObj::TwoPointRuler = false;

RulerObj::RulerObj() {
    this->shape = -1;
    this->loaded = false;
    this->modified = false;
}

bool RulerObj::allowNew(){
    return true;
}

RulerObj::RulerObj(const RulerObj& orig) {
}

RulerObj::~RulerObj() {
}

void RulerObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 1;
    this->box.loaded = false;
    setMartix();
    //this->point3d = new TrackItemObj();
    //this->point3d->setMaterial(1,1,1);
    //this->point3dSelected = new TrackItemObj();
    //this->point3dSelected->setMaterial(0.5,0.5,0.5);

    if(this->points.size() == 0){
        Point point;
        Vec3::copy(point.position, this->position);
        this->points.push_back(point);
        if(TwoPointRuler){
            Point point2;
            Vec3::copy(point2.position, this->position);
            point2.position[2] += 1;
            this->points.push_back(point2);
            selectionValue = 1;
        }
    }
}

void RulerObj::set(QString sh, QString val){
    
    WorldObj::set(sh, val);
    return;
}

void RulerObj::set(QString sh, FileBuffer* data) {
    if (sh == ("points")) {
        int pointCount = ParserX::GetNumber(data);
        for(int i=0; i< pointCount; i++){
            Point point;
            point.position[0] = ParserX::GetNumber(data);
            point.position[1] = ParserX::GetNumber(data);
            point.position[2] = -ParserX::GetNumber(data);
            points.push_back(point);
        }
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void RulerObj::setPosition(int x, int z, float* p){
    if(selectionValue > 0){
        points[selectionValue].position[0] = -2048*(this->x-x) + p[0];
        points[selectionValue].position[1] = p[1];
        points[selectionValue].position[2] = -2048*(this->y-z) + p[2];
    } else {
        Point point;
        point.position[0] = -2048*(this->x-x) + p[0];
        point.position[1] = p[1];
        point.position[2] = -2048*(this->y-z) + p[2];
        this->points.push_back(point);
    }
    this->modified = true;
    if(line3d != NULL)
        line3d->deleteVBO();
}

void RulerObj::refreshLength(){
    length = 0;
    for(int i = 0; i < points.size() - 1; i++){
        length += Vec3::distance(points[i].position, points[i+1].position);
    }
}

float RulerObj::getLength(){
    return length;
}

void RulerObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if (renderMode == GLUU::RENDER_SHADOWMAP) return;
    if (!loaded) return;
    if (jestPQ < 2) return;

    if(Game::showWorldObjPivotPoints){
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.9,0.9,0.7);
        }
        pointer3d->render(selectionColor);
    }
    
    if(point3d == NULL){
        point3d = new OglObj();
        point3d->setMaterial(1,1,1);
        point3d->setLineWidth(8);
        float *punkty = new float[6];
        int ptr = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 10;
        punkty[ptr++] = 0;
        point3d->init(punkty, ptr, point3d->V, GL_LINES);
        
        point3dSelected = new OglObj();
        point3dSelected->setLineWidth(8);
        point3dSelected->setMaterial(0.5,0.5,0.5);
        ptr = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 10;
        punkty[ptr++] = 0;
        point3dSelected->init(punkty, ptr, point3dSelected->V, GL_LINES);
        delete[] punkty;
    }
    if(line3d == NULL){
        line3d = new OglObj();
        line3d->setLineWidth(2);
        line3d->setMaterial(1,1,1);
    }
    if(!line3d->loaded){
        float *punkty = new float[points.size()*6*2]; 
        int ptr = 0;
        
        for(int i = 0; i < points.size() - 1; i++){
            punkty[ptr++] = points[i].position[0];
            punkty[ptr++] = points[i].position[1]+10;
            punkty[ptr++] = points[i].position[2];
            punkty[ptr++] = points[i+1].position[0];
            punkty[ptr++] = points[i+1].position[1]+10;
            punkty[ptr++] = points[i+1].position[2];
        }
        line3d->init(punkty, ptr, line3d->V, GL_LINES);
        delete[] punkty;
        refreshLength();
    }
    
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    line3d->render(selectionColor);
    
    for(int i = 0; i < points.size(); i++){
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, points[i].position[0], points[i].position[1], points[i].position[2]);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        int useSC = (float)selectionColor/(float)(selectionColor+0.000001);
        if(this->selected && this->selectionValue == i) 
            point3dSelected->render(selectionColor + (i&0xF)*131072*8*useSC);
        else
            point3d->render(selectionColor + (i&0xF)*131072*8*useSC);
        //    pointer3d->render(selectionColor + (i+1)*131072*8*useSC);
        gluu->mvPopMatrix();
    }
};

bool RulerObj::select(int value){
    this->selectionValue = value;
    this->selected = true;
}


void RulerObj::save(QTextStream* out){
    if (!loaded) return;
    if (jestPQ < 2) return;
    
*(out) << "	Ruler (\n";

*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		Points ( " << points.size()<<" \n";
for(int i = 0; i < points.size(); i++)
*(out) << "			Point ( "<<points[i].position[0]<<" "<<points[i].position[1]<<" "<<-points[i].position[2]<<" )\n";
*(out) << "		)\n";
*(out) << "	)\n";
}