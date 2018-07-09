/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TrackObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include "TS.h"
#include <QDebug>
#include "Game.h"
#include "TDB.h"
#include "TrackItemObj.h"
#include "TSectionDAT.h"
#include "ProceduralShape.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

TrackObj::TrackObj() {
    this->shape = -1;
    this->loaded = false;
    this->sectionIdx = 0;
    this->elevation = 0;
    this->staticFlags = 0x200180;
    this->collideFlags = 39;
    this->modified = false;
}

TrackObj::TrackObj(const TrackObj& o) : WorldObj(o) {
    for(int i = 0; i < o.jNodePosn.size(); i++ ){
        jNodePosn.push_back(std::array<float,5>());
        jNodePosn.back()[0] = o.jNodePosn[i][0];
        jNodePosn.back()[1] = o.jNodePosn[i][1];
        jNodePosn.back()[2] = o.jNodePosn[i][2];
        jNodePosn.back()[3] = o.jNodePosn[i][3];
        jNodePosn.back()[4] = o.jNodePosn[i][4];
    }

    elevation = o.elevation;
}

WorldObj* TrackObj::clone(){
    return new TrackObj(*this);
}

TrackObj::~TrackObj() {
}

void TrackObj::load(int x, int y) {
    this->shape = Game::currentShapeLib->addShape(resPath +"/"+ fileName);
    this->shapePointer = Game::currentShapeLib->shape[this->shape];
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 3;
    this->endp = new float[5];
    this->endp[0] = 0;
    this->endp[1] = 0;
    this->endp[2] = 0;
    this->endp[3] = 1;
    this->endp[4] = 0;
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    this->firstPosition[0] = this->position[0];
    this->firstPosition[1] = this->position[1];
    this->firstPosition[2] = this->position[2];
    
    this->roadShape = Game::trackDB->tsection->isRoadShape(sectionIdx);
    setMartix();
}

bool TrackObj::allowNew(){
    return true;
}

void TrackObj::setTemplate(QString name){
    templateName = name;
    templateDisabled = false;
    modified = true;
    reload();
}

void TrackObj::reload(){
    if(!Game::proceduralTracks || roadShape ) {
        Game::currentShapeLib->shape[shape]->reload();
    } else {
        proceduralShapeInit = false;
        procShape.clear();
    }
}

void TrackObj::setElevation(float prom){
    float * q = qDirection;
    float vect[3];
    vect[0] = 0; vect[1] = 0; vect [2] = 1000;
    Vec3::transformQuat(vect, vect, q);
    rotate(asin((vect[1]*this->endp[3]+prom)/1000.0),0,0);
}

void TrackObj::rotate(float x, float y, float z){
    this->tRotation[0] += x;
    this->tRotation[1] += y;
    if(matrix3x3 != NULL) matrix3x3 = NULL;

    qDebug() << "rot" << x << y << z;
    float vect2[3];
    float vect[3];
    float quat[4];
    Quat::fill(quat);
    
    if(x!=0) Quat::rotateX(this->qDirection, this->qDirection, x*this->endp[3]);
    if(y!=0) Quat::rotateY(this->qDirection, this->qDirection, y*this->endp[3]);
    if(z!=0) Quat::rotateZ(this->qDirection, this->qDirection, z*this->endp[3]);    
    
    Vec3::set(vect, 0, 0, 10);
    Vec3::transformQuat(vect, vect, this->qDirection);
    
    Quat::fill(quat);
    Quat::rotateY(quat, quat, endp[4]);

    Vec3::transformQuat(reinterpret_cast<float*>(&vect), this->endp, this->qDirection);
    Vec3::transformQuat(reinterpret_cast<float*>(&vect2), this->endp, quat);

    qDebug() << this->endp[0] << " "<< vect[0] << " " << vect2[0];
    qDebug() << this->endp[2] << " "<< vect[2] << " " << vect2[2];
    
    vect[0] = (vect2[0] - vect[0]);
    vect[2] = (vect2[2] - vect[2]);
    
    this->position[0] = this->placedAtPosition[0] + vect[0];
    this->position[1] = this->placedAtPosition[1] - vect[1];
    this->position[2] = this->placedAtPosition[2] + vect[2];
    
    this->modified = true;
    setMartix();
}

void TrackObj::set(QString sh, long long int val) {
    if (sh == ("sectionidx")) {
        sectionIdx = val;
        return;
    }
    if (sh == ("ref_value")) {
        sectionIdx = val;
        return;
    }
    WorldObj::set(sh, val);
    this->modified = true;
}

void TrackObj::set(QString sh, QString val){ 
    if (sh == ("filename")) {
        fileName = val;
        return;
    }
    if (sh == ("ref_filename")) {
        fileName = val;
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void TrackObj::set(int sh, FileBuffer* data) {
    if (sh == TS::FileName) {
        data->off++;
        int slen = data->getShort()*2;
        fileName = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
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
        jNodePosn.push_back(std::array<float,5>());
        jNodePosn.back()[0] = data->getInt();
        jNodePosn.back()[1] = data->getInt();
        jNodePosn.back()[2] = data->getFloat();
        jNodePosn.back()[3] = data->getFloat();
        jNodePosn.back()[4] = data->getFloat();
        return;
    }
    
    WorldObj::set(sh, data);
    return;
}

void TrackObj::set(QString sh, FileBuffer* data) {
    if (sh == ("filename")) {
        fileName = ParserX::GetString(data);
        return;
    }
    if (sh == ("sectionidx")) {
        sectionIdx = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("elevation")) {
        elevation = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("jnodeposn")) {
        jNodePosn.push_back(std::array<float,5>());
        jNodePosn.back()[0] = ParserX::GetNumber(data);
        jNodePosn.back()[1] = ParserX::GetNumber(data);
        jNodePosn.back()[2] = ParserX::GetNumber(data);
        jNodePosn.back()[3] = ParserX::GetNumber(data);
        jNodePosn.back()[4] = ParserX::GetNumber(data);
        return;
    }
    
    WorldObj::set(sh, data);
    return;
}

QString TrackObj::getShapePath(){
    if (!loaded) return "";
    if (shapePointer == 0) return "";
    return shapePointer->pathid+"|"+shapePointer->texPath;
}

bool TrackObj::isSimilar(WorldObj* obj){
    if(obj->typeID != this->typeID)
        return false;
    if(obj->fileName == this->fileName)
        return true;
    return false;
}

void TrackObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if (!loaded) return;
    if (shape < 0) return;
    if (jestPQ < 2) return;
    if (renderMode == gluu->RENDER_SHADOWMAP && Game::mstsShadows) {
        if(this->getShadowType() != WorldObj::ShadowDynamic )
            return;
    }

    if (size > 0) {
        if ((lod > size + 150)) {
            float v1[2];
            v1[0] = pos[0] - (target[0]);
            v1[1] = pos[2] - (target[2]);
            float v2[2];
            v2[0] = posx;
            v2[1] = posz;
            float iloczyn = v1[0] * v2[0] + v1[1] * v2[1];
            float d1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
            float d2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
            float zz = iloczyn / (d1 * d2);
            if (zz > 0) return;

            float ccos = cos(fov) + zz;
            float xxx = sqrt(2 * d2 * d2 * (1 - ccos));
            //if((ccos > 0) && (xxx > 200+50)) return;
            if ((ccos > 0) && (xxx > size) && (skipLevel == 1)) return;
        }
    } else {
        //if (!Game::proceduralTracks)
            if (Game::currentShapeLib->shape[shape]->loaded)
                size = Game::currentShapeLib->shape[shape]->size;
    }

    Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    if(Game::showWorldObjPivotPoints){
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.9,0.9,0.7);
        }
        pointer3d->render(selectionColor);
    }
    
    if(selectionColor != 0){
        gluu->disableTextures(selectionColor);
    } else {
        gluu->enableTextures();
    }
    
    if(!Game::proceduralTracks || roadShape || templateDisabled ) {
        Game::currentShapeLib->shape[shape]->render();
    } else {
        if (!proceduralShapeInit) {
            if(templateName == "DISABLED"){
                templateDisabled = true;
                return;
            }
            TrackShape *tsh = Game::trackDB->tsection->shape[sectionIdx];
            QMap<int, float> angles;
            if(Game::useSuperelevation)
                Game::trackDB->fillTrackAngles(x, -y, UiD, angles);
            ProceduralShape::GetShape(templateName, procShape, tsh, angles);
            proceduralShapeInit = true;
        } else {
            for(int i = 0; i < procShape.size(); i++){
                procShape[i]->render(selectionColor);
            }
        }
    }
    
    if(selected){
        drawBox();
    }
};

void TrackObj::fillJNodePosn(){
    TDB* tdb = Game::trackDB;
    this->modified = tdb->fillJNodePosn(this->x, this->y, this->UiD, &this->jNodePosn);
}

bool TrackObj::getSimpleBorder(float* border){
    if (shapePointer == 0) return false;
    if (!shapePointer->loaded)
        return false;
    float* bound = shapePointer->bound;
    border[0] = bound[0];
    border[1] = bound[1];
    border[2] = bound[2];
    border[3] = bound[3];
    border[4] = bound[4];
    border[5] = bound[5];
    return true;
}

bool TrackObj::getBoxPoints(QVector<float>& points){
    if (shapePointer == 0) return false;
    if (!shapePointer->loaded)
        return false;
    return shapePointer->getBoxPoints(points);
    /*
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
        return true;*/
}

Ref::RefItem* TrackObj::getRefInfo(){
    Ref::RefItem* r = new Ref::RefItem();
    r->type = this->type;
    r->filename = this->fileName;
    r->value = this->sectionIdx;
    r->staticFlags = this->staticFlags;
    return r;
}

int TrackObj::getDefaultDetailLevel(){
    if (!loaded) return -2;
    if (shapePointer == 0) return -2;
    int esdDLevel = shapePointer->esdDetailLevel;
    if(esdDLevel >= 0) return esdDLevel;
    return -2;
}

int TrackObj::getCollisionType(){
    bool enabled = ((this->collideFlags & 2) >> 1) == 0;
    if(enabled){
        if(this->collideFunction == 1 )
            return 2;
        else 
            return 1;
    } else {
        return 0;
    }
}

void TrackObj::save(QTextStream* out){
    if (!loaded) return;
    if (jestPQ < 2) return;
    
*(out) << "	TrackObj (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		SectionIdx ( "<<this->sectionIdx<<" )\n";
*(out) << "		Elevation ( "<<this->elevation<<" )\n";
if(this->jNodePosn.size() != 0)
    for(int i = 0; i < this->jNodePosn.size(); i++ ){
        *(out) << "		JNodePosn ( "<<this->jNodePosn[i][0]<<" "<<this->jNodePosn[i][1]<<" "<<this->jNodePosn[i][2]<<" "<<this->jNodePosn[i][3]<<" "<<this->jNodePosn[i][4]<<" )\n";
    }
*(out) << "		CollideFlags ( "<<this->collideFlags<<" )\n";
if(this->collideFunction > 0 )
*(out) << "		CollideFunction ( "<<this->collideFunction<<" )\n";
if(this->templateName != "" && this->templateName != "DEFAULT")
*(out) << "		ShapeTemplate ( "<<ParserX::AddComIfReq(this->templateName)<<" )\n";
*(out) << "		FileName ( "<<ParserX::AddComIfReq(this->fileName)<<" )\n";
*(out) << "		StaticFlags ( "<<ParserX::MakeFlagsString(this->staticFlags)<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
if(this->matrix3x3!=NULL)
*(out) << "		Matrix3x3 ( "<<this->matrix3x3[0]<<" "<<this->matrix3x3[1]<<" "<<this->matrix3x3[2]<<" "<<this->matrix3x3[3]<<" "<<this->matrix3x3[4]<<" "<<this->matrix3x3[5]<<" "<<this->matrix3x3[6]<<" "<<this->matrix3x3[7]<<" "<<this->matrix3x3[8]<<" )\n";
else                        
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}
