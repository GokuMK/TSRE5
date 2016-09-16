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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

TrackObj::TrackObj() {
    this->shape = -1;
    this->loaded = false;
    this->sectionIdx = 0;
    this->elevation = 0;
    this->staticFlags = 0x200180;
    this->collideFlags = 0;
    this->modified = false;
}

TrackObj::TrackObj(const TrackObj& orig) {
}

TrackObj::~TrackObj() {
}

void TrackObj::load(int x, int y) {
    this->shape = Game::currentShapeLib->addShape(resPath +"/"+ fileName);
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
    setMartix();
}

bool TrackObj::allowNew(){
    return true;
}

void TrackObj::rotate(float x, float y, float z){
    this->tRotation[0] += x;
    this->tRotation[1] += y;
    if(matrix3x3 != NULL) matrix3x3 = NULL;
    
    //if(this->endp[3] == 1){
    qDebug() << "rot";
    //Mat4::translate(this->matrix, this->matrix, this->endp);
    float vect2[3];
    float vect[3];
    float quat[4];
    quat[0] = 0;
    quat[1] = 0;
    quat[2] = 0;
    quat[3] = 1;
    
    if(x!=0) Quat::rotateX(this->qDirection, this->qDirection, x*this->endp[3]);
    if(y!=0) Quat::rotateY(this->qDirection, this->qDirection, y*this->endp[3]);
    if(z!=0) Quat::rotateZ(this->qDirection, this->qDirection, z*this->endp[3]);    
    
    
    //Quat::rotateY(quat, quat, -this->endp[4]);
    
    vect[0] = 0; vect[1] = 0; vect [2] = 10;
    Vec3::transformQuat(vect, vect, this->qDirection);
    float elevation = tan((vect[1]/10.0));
    //Quat::rotateX(quat, quat, elevation);
    //Vec3::transformQuat(reinterpret_cast<float*>(&vect), this->endp, this->qDirection);
    //Vec3::transformQuat(reinterpret_cast<float*>(&vect), this->endp, this->qDirection);
    quat[0] = this->qDirection[0];
    quat[1] = this->qDirection[1];
    quat[2] = this->qDirection[2];
    quat[3] = this->qDirection[3];
    //Quat::rotateY(quat, quat, endp[4] + M_PI);
    
    //Vec3::transformQuat(reinterpret_cast<float*>(&vect), this->endp, quat);
    /*//vect2[0] = vect[0];
    //vect2[2] = vect[2];
    //Vec3::transformQuat(vect, vect, quat);
    
    qDebug() << this->endp[0] << " "<< vect[0] << " " << vect2[0];
    qDebug() << this->endp[2] << " "<< vect[2] << " " << vect2[2];
    */
    quat[0] = 0;
    quat[1] = 0;
    quat[2] = 0;
    quat[3] = 1;
    Quat::rotateY(quat, quat, endp[4]);
    //Quat::rotateX(quat, quat,-elevation);
    Vec3::transformQuat(reinterpret_cast<float*>(&vect), this->endp, this->qDirection);
    Vec3::transformQuat(reinterpret_cast<float*>(&vect2), this->endp, quat);
    /*vect[0] = -(this->endp[0] - vect2[0]);
    //vect[1] = 0;
    vect[2] = (this->endp[2] + vect2[2]);
    Vec3::transformQuat(vect, vect, this->qDirection)*/
    qDebug() << this->endp[0] << " "<< vect[0] << " " << vect2[0];
    qDebug() << this->endp[2] << " "<< vect[2] << " " << vect2[2];
    
    //Vec3::transformQuat(vect, vect, quat);
    //Vec3::transformQuat(this->endp, this->endp, quat);
    
    vect[0] = (vect2[0] - vect[0]);
    //vect[1] = 0;
    vect[2] = (vect2[2] - vect[2]);
    
    this->position[0] = this->placedAtPosition[0] + vect[0];
    this->position[1] = this->placedAtPosition[1] - vect[1];
    this->position[2] = this->placedAtPosition[2] + vect[2];
    setMartix();
    

    
    //if(this->endp[3] == 1){
    //Mat4::translate(this->matrix, this->matrix, -this->endp[0], -this->endp[1], -this->endp[2]);
    //}
    
    this->modified = true;
    setMartix();
}

void TrackObj::set(QString sh, int val) {
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
    if (sh == TS::CollideFunction) {
        data->off++;
        collideFunction = data->getUint();
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
        jNodePosn = new float[5];
        jNodePosn[0] = ParserX::GetNumber(data);
        jNodePosn[1] = ParserX::GetNumber(data);
        jNodePosn[2] = ParserX::GetNumber(data);
        jNodePosn[3] = ParserX::GetNumber(data);
        jNodePosn[4] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("collidefunction")) {
        collideFunction = ParserX::GetNumber(data);
        return;
    }
    
    WorldObj::set(sh, data);
    return;
}

QString TrackObj::getShapePath(){
    if (!loaded) return "";
    if (shape < 0) return "";
    return Game::currentShapeLib->shape[shape]->pathid+"|"+Game::currentShapeLib->shape[shape]->texPath;
}

void TrackObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;
    if (shape < 0) return;
    if (jestPQ < 2) return;
    //GLUU* gluu = GLUU::get();
    //if((this.position===undefined)||this.qDirection===undefined) return;

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
        if (Game::currentShapeLib->shape[shape]->loaded)
            size = Game::currentShapeLib->shape[shape]->size;
    }

    Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    if(selectionColor != 0){
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
    } else {
        gluu->enableTextures();
    }
        
    Game::currentShapeLib->shape[shape]->render();
    
    if(selected){
        drawBox();
    }
};

bool TrackObj::getBorder(float* border){
    if (shape < 0) return false;
    if (!Game::currentShapeLib->shape[shape]->loaded)
        return false;
    float* bound = Game::currentShapeLib->shape[shape]->bound;
    border[0] = bound[0];
    border[1] = bound[1];
    border[2] = bound[2];
    border[3] = bound[3];
    border[4] = bound[4];
    border[5] = bound[5];
    return true;
}

Ref::RefItem* TrackObj::getRefInfo(){
    Ref::RefItem* r = new Ref::RefItem();
    r->type = this->type;
    r->filename = this->fileName;
    r->value = this->sectionIdx;
    return r;
}

int TrackObj::getDefaultDetailLevel(){
    return -2;
}

void TrackObj::save(QTextStream* out){
    if (!loaded) return;
    if (jestPQ < 2) return;
    
*(out) << "	TrackObj (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		SectionIdx ( "<<this->sectionIdx<<" )\n";
*(out) << "		Elevation ( "<<this->elevation<<" )\n";
if(this->jNodePosn!=NULL)
*(out) << "		JNodePosn ( "<<this->jNodePosn[0]<<" "<<this->jNodePosn[1]<<" "<<this->jNodePosn[2]<<" "<<this->jNodePosn[3]<<" "<<this->jNodePosn[4]<<" )\n";
*(out) << "		CollideFlags ( "<<this->collideFlags<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
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
