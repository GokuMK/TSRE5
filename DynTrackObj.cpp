/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "DynTrackObj.h"
#include "ParserX.h"
#include <QDebug>
#include "GLMatrix.h"
#include "TexLib.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include <QOpenGLShaderProgram>
#include "GLUU.h"
#include "TS.h"
#include "TrackItemObj.h"
#include "Game.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

DynTrackObj::DynTrackObj() {
    sectionIdx = -1;
    sections = NULL;
    collideFlags = 39;    
    staticFlags = 0x100000;
}

DynTrackObj::DynTrackObj(const DynTrackObj& o) : WorldObj(o) {
    if(o.sections != NULL){
        sections = new Section[5];
        for (int i = 0; i < 5; i++) {
            sections[i].type = o.sections[i].type;
            sections[i].sectIdx = o.sections[i].sectIdx;
            sections[i].a = o.sections[i].a;
            sections[i].r = o.sections[i].r;
        }
    }
    tex1 = o.tex1;
    tex2 = o.tex2;
    init = false;// o.init;
    elevation = o.elevation;
    if(o.jNodePosn!=NULL){
        jNodePosn = new float[5];
        memcpy(jNodePosn, o.jNodePosn, sizeof(float)*5);
    }
    sidxSelected = o.sidxSelected;
}

WorldObj* DynTrackObj::clone(){
    return new DynTrackObj(*this);
}

DynTrackObj::~DynTrackObj() {
}

bool DynTrackObj::allowNew(){
    return true;
}

void DynTrackObj::deleteVBO(){
    //this->shape.deleteVBO();
    this->init = false;
    shape[0].deleteVBO();
    shape[1].deleteVBO();
}

void DynTrackObj::load(int x, int y) {
    this->tex1 = -1;
    this->init = false;
    //this->shape = ShapeLib::addShape(path, fileName);
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    Mat4::fromRotationTranslation(this->matrix, qDirection, position);
    Mat4::rotate(this->matrix, this->matrix, M_PI, 0, -1, 0);
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 3;
    this->modified = false;
    
    if(sections == NULL){
        sections = new Section[5];
        sections[0].type = 0;
        sections[0].sectIdx = 0;
        sections[0].a = 10;
        sections[0].r = 0;
        for (int iii = 1; iii < 5; iii++) {
            sections[iii].type = iii%2;
            sections[iii].sectIdx = 4294967295;
            sections[iii].a = 0;
            sections[iii].r = 0;
        }
    }
    //sections[1].type = 1;
    //sections[1].sectIdx = 0;
    //sections[1].a = 0.1;
    //sections[1].r = 100;
}



void DynTrackObj::resize(float x, float y, float z){
    if(z < 0)
        this->sidxSelected--;
    if(z > 0)
        this->sidxSelected++;
    if(this->sidxSelected < 0 )
        this->sidxSelected = 0;
    if(this->sidxSelected > 4 )
        this->sidxSelected = 4;

    if(this->sections[this->sidxSelected].sectIdx > 1000000 && x == 0) return;
    
    if(this->sections[this->sidxSelected].sectIdx > 1000000){
        this->sections[this->sidxSelected].sectIdx = 0;
        this->sections[this->sidxSelected].a = 0.1;
        if(this->sections[this->sidxSelected].type == 1) 
            this->sections[this->sidxSelected].r = 100;
    }
    
    

    if(this->sections[this->sidxSelected].type == 1){
        if(x < 0)
            this->sections[this->sidxSelected].a -= 0.01;
        if(x > 0)
            this->sections[this->sidxSelected].a += 0.01;
        if(y < 0)
            this->sections[this->sidxSelected].r -= 5;
        if(y > 0)
            this->sections[this->sidxSelected].r += 5;
        if(this->sections[this->sidxSelected].a > 3.14) this->sections[this->sidxSelected].a = 3.14;
        if(this->sections[this->sidxSelected].a < -3.14) this->sections[this->sidxSelected].a = -3.14;
        if(this->sections[this->sidxSelected].r < 15) this->sections[this->sidxSelected].r = 15;
        
        if(x < 0 && this->sections[this->sidxSelected].a < 0.01 && this->sections[this->sidxSelected].a > -0.01) 
            this->sections[this->sidxSelected].a = -0.01;
        if(x > 0 && this->sections[this->sidxSelected].a > -0.01 && this->sections[this->sidxSelected].a < 0.01) 
            this->sections[this->sidxSelected].a = 0.01;
    } else {
        if(x < 0)
            this->sections[this->sidxSelected].a -= 0.1;
        if(x > 0)
            this->sections[this->sidxSelected].a += 0.1;
        if(this->sections[this->sidxSelected].a < 0) this->sections[this->sidxSelected].a = 0;
    }

    this->modified = true;
    deleteVBO();
}

void DynTrackObj::set(int sh, FileBuffer* data) {
    //qDebug() << "dyntrack: "<<sh;
    if (sh == TS::SectionIdx) {
        data->off++;
        sectionIdx = data->getUint();
        return;
    }
    if (sh == TS::Elevation) {
        data->off++;
        elevation = data->getFloat();
        return;
    }
    if (sh == TS::JNodePosn) {
        data->off++;
        jNodePosn = new float[5];
        jNodePosn[0] = data->getFloat();
        jNodePosn[1] = data->getFloat();
        jNodePosn[2] = data->getFloat();
        jNodePosn[3] = data->getFloat();
        jNodePosn[4] = data->getFloat();
        return;
    }
    if (sh == TS::TrackSections) {
        if(sections == NULL) sections = new Section[5];
        data->off++;
        
        for (int iii = 0; iii < 5; iii++) {
            data->off+=18;
            sections[iii].type = data->getUint();
            sections[iii].sectIdx = data->getUint();
            sections[iii].a = data->getFloat();
            sections[iii].r = data->getFloat();
        }
        return;
    }
    //qDebug() <<"A";
    WorldObj::set(sh, data);
    return;
}

void DynTrackObj::set(QString sh, FileBuffer* data) {
    if (sh == ("sectionidx")) {
        //qDebug() << ParserX::GetNumber(data);
        sectionIdx = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("elevation")) {
        elevation = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("tracksections")) {
        if(sections == NULL) sections = new Section[5];
        for (int iii = 0; iii < 5; iii++) {
            sections[iii].type = ParserX::GetNumber(data);
            sections[iii].sectIdx = ParserX::GetUInt(data);
            sections[iii].a = ParserX::GetNumber(data);
            sections[iii].r = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
        }
        return;
    }
    if (sh == ("jnodeposn")) {
        jNodePosn = new float[5];
        jNodePosn[0] = ParserX::GetNumber(data);
        jNodePosn[1] = ParserX::GetNumber(data);
        jNodePosn[2] = ParserX::GetNumber(data);
        jNodePosn[3] = ParserX::GetNumber(data);
        jNodePosn[4] = ParserX::GetNumber(data);
        return;
    }    
    //qDebug() <<"A";
    WorldObj::set(sh, data);
    return;
}

void DynTrackObj::set(QString sh, float* val) {
    if(sh == "dyntrackdata"){
        for (int iii = 0; iii < 5; iii++) {
            sections[iii].a = val[iii*2];
            sections[iii].r = val[iii*2+1];
            if(iii%2 == 0){
                sections[iii].sectIdx = 0;
                if(sections[iii].a == 0 && iii > 0)
                    sections[iii].sectIdx = 4294967295;
            } else {
                sections[iii].sectIdx = 0;
                if(sections[iii].a < 0.01 && sections[iii].a > -0.01)
                    sections[iii].sectIdx = 4294967295;
                if(sections[iii].r < 0.1)
                    sections[iii].sectIdx = 4294967295;
            }
        }
    }
    //sections[0].sectIdx = 4294967295;
    //sections[0].a = 0;
    //sections[0].r = 0;
    this->modified = true;
    deleteVBO();
}

void DynTrackObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) 
        return;

    Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);

    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    if(Game::showWorldObjPivotPoints){
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.9,0.9,0.7);
        }
        pointer3d->render(selectionColor);
    }

    drawShape(selectionColor);
    
    if(selected){
        drawBox();
    }    
};

void DynTrackObj::drawShape(int selectionColor) {
    if (!init) {
        genShape();
        init = true;
    } else {
        shape[0].render(selectionColor);
        shape[1].render(selectionColor);
    }
}

void DynTrackObj::genShape() {
    float* pd = new float[55000];
    float* sk = new float[55000];

    int ptr = 0;
    int str = 0;
    
    Vector2f tx;
    Vector2f ty;
    Vector2f offpos(0.0, 0.0);
    Vector2f b1;
    Vector2f a1;
    Vector2f a;
    Vector2f b;
    Vector2f dl;
    
    float offrot = 0;
    GLUU *gluu = GLUU::get();
    float alpha = -gluu->alphaTest;
    
    for (int i = 0; i < 5; i++) {
        if (sections[i].sectIdx > 100000000) continue;
        //prosta
        if (sections[i].type == 0) {
            //podklady

            b.set(2.5, 0.0);
            b.rotate(offrot, 0);
            a1.set(0.0, sections[i].a);
            a1.rotate(offrot, 0);

            pd[ptr++] = offpos.x + b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x - b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x - b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].a;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x + b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].a;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x + b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x - b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].a;
            pd[ptr++] = alpha;
            //szyny

            tx.set(-0.717500, 0.0);
            ty.set(-0.867500, 0.0);

            
            for (int jj = 0; jj < 2; jj++) {
                tx.rotate(offrot, 0);
                ty.rotate(offrot, 0);

                sk[str++] = offpos.x + tx.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2270;
                sk[str++] = alpha;

                sk[str++] = offpos.x + ty.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.1330;
                sk[str++] = alpha;

                sk[str++] = offpos.x + ty.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.1330;
                sk[str++] = alpha;

                sk[str++] = offpos.x + tx.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.2270;
                sk[str++] = alpha;

                sk[str++] = offpos.x + tx.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2270;
                sk[str++] = alpha;

                sk[str++] = offpos.x + ty.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.1330;
                sk[str++] = alpha;
                
                tx.set(0.867500, 0.0);
                ty.set(0.717500, 0.0);
            }
            ///
            tx.set(0.717500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;
            
            tx.set(-0.717500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;
            
            tx.set(-0.867500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            tx.set(0.867500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;
            
            offpos.x += a1.x;
            offpos.y += a1.y;
        }
        //krzywa
        if(sections[i].type==1){
            float kierunek = 1;
            if(sections[i].a > 0){
                kierunek = -1;
            }
            float aa = 0;
            float angle;
            float angle2;
            for(angle = 0, angle2 = 0; angle2>sections[i].a*kierunek; angle-=0.03*kierunek,angle2-=0.03){
                if(sections[i].a*kierunek-angle2<-0.03) 
                    aa = -0.03*kierunek;
                else 
                    aa = (sections[i].a*kierunek-angle2)*kierunek;
                //podklady
                b1.set(-2.5, 0.0);
                a1.set(2.5, 0.0);
                a.set(-2.5, 0.0);
                b.set(2.5, 0.0);
                a1.rotate(angle, sections[i].r*kierunek);
                b1.rotate(angle, sections[i].r*kierunek);           
                a.rotate(angle+aa, sections[i].r*kierunek);
                b.rotate(angle+aa, sections[i].r*kierunek);
                a1.rotate(offrot, 0);
                b1.rotate(offrot, 0);
                a.rotate(offrot, 0);
                b.rotate(offrot, 0);
                dl.set(0.0, 0.0);
                dl.rotate(aa, sections[i].r*kierunek); 
                float dlugosc = dl.getDlugosc();
                    
                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                pd[ptr++] = alpha;
                    
                pd[ptr++] = offpos.x+b1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0;
                pd[ptr++] = alpha;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                pd[ptr++] = alpha;
                  
                pd[ptr++] = offpos.x+b.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0 + 0.2*dlugosc;
                pd[ptr++] = alpha;

                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                pd[ptr++] = alpha;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                pd[ptr++] = alpha;
                
                //szyny
                   
                tx.set(-0.717500, 0.0);
                ty.set(-0.867500, 0.0);
              
                for( int jj = 0; jj < 2; jj++){
                    a.set(tx.x, 0.0);
                    b.set(ty.x, 0.0);
                    tx.rotate(angle, sections[i].r*kierunek);
                    ty.rotate(angle, sections[i].r*kierunek);
                    a.rotate(angle+aa, sections[i].r*kierunek);
                    b.rotate(angle+aa, sections[i].r*kierunek);
                    a.rotate(offrot, 0);
                    b.rotate(offrot, 0);      
                    tx.rotate(offrot, 0); 
                    ty.rotate(offrot, 0); 
                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+ty.x; sk[str++] = 0.325000; sk[str++] = offpos.y+ty.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.1330;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+a.x; sk[str++] =  0.325000; sk[str++] =  offpos.y+a.y;
                        sk[str++] = 0.0; sk[str++] =  1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] =  0.2270;
                        sk[str++] = alpha;

                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                        sk[str++] = alpha;
                    ty.set(0.717500, 0.0);
                    tx.set(0.867500, 0.0);
                }
                ///
                tx.set(0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                tx.set(-0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;

                tx.set(-0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;

                tx.set(0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;
            }
            a.set(0.0, 0.0);
            a.rotate(sections[i].a, sections[i].r*kierunek);
            a.rotate(offrot, 0);
            offpos.x+=a.x; offpos.y+=a.y;
            offrot+=sections[i].a;
        }
    }
    //qDebug() << ptr << "" << str;
    
        bound[0] = -9999;
        bound[1] = 9999;
        bound[2] = -9999;
        bound[3] = 9999;
        bound[4] = -9999;
        bound[5] = 9999;
        for (int i = 0; i < ptr ; i+=9) {
            if(pd[i+0] < bound[1]) bound[1] = pd[i+0];
            if(pd[i+1] < bound[3]) bound[3] = pd[i+1];
            if(pd[i+2] < bound[5]) bound[5] = pd[i+2];
            if(pd[i+0] > bound[0]) bound[0] = pd[i+0];
            if(pd[i+1] > bound[2]) bound[2] = pd[i+1];
            if(pd[i+2] > bound[4]) bound[4] = pd[i+2];
        }
        for (int i = 0; i < str ; i+=9) {
            if(sk[i+0] < bound[1]) bound[1] = sk[i+0];
            if(sk[i+1] < bound[3]) bound[3] = sk[i+1];
            if(sk[i+2] < bound[5]) bound[5] = sk[i+2];
            if(sk[i+0] > bound[0]) bound[0] = sk[i+0];
            if(sk[i+1] > bound[2]) bound[2] = sk[i+1];
            if(sk[i+2] > bound[4]) bound[4] = sk[i+2];
        }

    QString* texturePath = new QString(resPath.toLower()+"/acleantrack1.ace");
    shape[0].setMaterial(texturePath);
    texturePath = new QString(resPath.toLower()+"/acleantrack2.ace");
    shape[1].setMaterial(texturePath);
    shape[0].init(pd, ptr, OglObj::VNT, GL_TRIANGLES );
    shape[1].init(sk, str, OglObj::VNT, GL_TRIANGLES );
    delete[] pd;
    delete[] sk;
    
}

bool DynTrackObj::getSimpleBorder(float* border){
    border[0] = bound[0];
    border[1] = bound[1];
    border[2] = bound[2];
    border[3] = bound[3];
    border[4] = bound[4];
    border[5] = bound[5];
    return true;
}

bool DynTrackObj::getBoxPoints(QVector<float>& points){
        float bound[6];
        if (!getSimpleBorder((float*)&bound)) return false;
        
        for(int i=0; i<2; i++)
            for(int j=4; j<6; j++){
                points.push_back(bound[i]);
                points.push_back(bound[2]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
                points.push_back(bound[3]);
                points.push_back(bound[j]);
            }
        for(int i=0; i<2; i++)
            for(int j=2; j<4; j++){
                points.push_back(bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[4]);
                points.push_back(bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[5]);
            }
        for(int i=4; i<6; i++)
            for(int j=2; j<4; j++){
                points.push_back(bound[0]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
                points.push_back(bound[1]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
            }
        return true;
}

int DynTrackObj::getDefaultDetailLevel(){
    return -12;
}

void DynTrackObj::save(QTextStream* out){
    if (!loaded) return;
    
*(out) << "	Dyntrack (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		TrackSections (\n";
for(int i = 0; i < 5; i++){
*(out) << "			TrackSection (\n";
*(out) << "				SectionCurve ( "<<this->sections[i].type<<" ) "<<this->sections[i].sectIdx<<" "<<this->sections[i].a<<" "<<this->sections[i].r<<"\n";
*(out) << "			)\n";
}
*(out) << "		)\n";
*(out) << "		SectionIdx ( "<<this->sectionIdx<<" )\n";
*(out) << "		Elevation ( "<<this->elevation<<" )\n";
if(this->jNodePosn!=NULL)
*(out) << "		JNodePosn ( "<<this->jNodePosn[0]<<" "<<this->jNodePosn[1]<<" "<<this->jNodePosn[2]<<" "<<this->jNodePosn[3]<<" "<<this->jNodePosn[4]<<" )\n";
*(out) << "		CollideFlags ( "<<this->collideFlags<<" )\n";
*(out) << "		StaticFlags ( "<<ParserX::MakeFlagsString(this->staticFlags)<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}
