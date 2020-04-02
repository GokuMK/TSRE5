/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PlatformObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include "Game.h"
#include "TDB.h"
#include "TRitem.h"
#include "TrackItemObj.h"
#include "OglObj.h"
#include "TS.h"
#include <math.h>
#include <QFile>
#include "FileBuffer.h"
#include "ReadFile.h"
#include "ErrorMessagesLib.h"
#include "ErrorMessage.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PlatformObj::PlatformObj() {
    this->shape = -1;
    this->loaded = false;
    this->staticFlags = 0x100;

    //pointer3d = new TrackItemObj();
    //pointer3dSelected = new TrackItemObj();
}

PlatformObj::PlatformObj(const PlatformObj& o) : WorldObj(o) {
    platformData = o.platformData;
    trItemId[0] = o.trItemId[0];
    trItemId[1] = o.trItemId[1];
    trItemId[2] = o.trItemId[2];
    trItemId[3] = o.trItemId[3];
    trItemIdCount = o.trItemIdCount;
    rotB = o.rotB;
    rotE = o.rotE;
    selectionValue = o.selectionValue;
}

WorldObj* PlatformObj::clone(){
    return new PlatformObj(*this);
}

PlatformObj::~PlatformObj() {
}

void PlatformObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->skipLevel = 1;
    this->modified = false;
    
    setMartix();
}

bool PlatformObj::allowNew(){
    return true;
}

void PlatformObj::set(int sh, FileBuffer* data) {
    if (sh == TS::SidingData || sh == TS::PlatformData) {
        data->off++;
        platformData = data->getUint();
        return;
    }
    if (sh == TS::TrItemId) {
        data->off++;
        trItemId[trItemIdCount++] = data->getUint();
        trItemId[trItemIdCount++] = data->getUint();
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void PlatformObj::set(QString sh, FileBuffer* data) {
    if (sh == ("sidingdata") || sh == ("platformdata")) {
        platformData = ParserX::GetHex(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        return;
    }
     
    WorldObj::set(sh, data);
    return;
}

void PlatformObj::setPosition(int x, int z, float* p){
    TDB* tdb = Game::trackDB;
    if(this->typeID == this->carspawner)
        tdb = Game::roadDB;
    
    int nodeId[4];
    nodeId[1] = tdb->findTrItemNodeId(trItemId[1]);
    nodeId[3] = tdb->findTrItemNodeId(trItemId[3]);
    /*if (nodeId[1] < 0 || nodeId[3] < 0) {
        qDebug() << "fail id";
        return;
    }*/
    float playerT[2];
    playerT[0] = x;
    playerT[1] = z;
    float tpos[3];

    if(tdb->findNearestPositionOnTDB(playerT, p, NULL, tpos) < 0)
        return;

    //tdb- >trackItems[trItemId[selectionValue]]-
    if(nodeId[selectionValue] == tpos[0]){
        tdb->trackItems[trItemId[selectionValue]]->setTrackPosition(tpos[1]);
        tdb->updateTrItemRData(tdb->trackItems[trItemId[selectionValue]]);
    }
    //if(tpos[0] != nodeId[1])
    //    return;

    if(this->selectionValue == 1){
        delete[] drawPositionB;
        drawPositionB = NULL;   
    } else if(this->selectionValue == 3){
        delete[] drawPositionE;
        drawPositionE = NULL;
    }
    delete line;
    line = NULL;
    this->modified = true;
}

void PlatformObj::translate(float px, float py, float pz){
    if(pz == 0) 
        return;
    TDB* tdb = Game::trackDB;
    if(this->typeID == this->carspawner)
        tdb = Game::roadDB;
    
    int id = tdb->findTrItemNodeId(this->trItemId[this->selectionValue]);
    if (id < 0) {
        qDebug() << "fail id";
        return;
    }
    
    float dlugosc = tdb->getVectorSectionLength(id);
    TRitem* trit = tdb->trackItems[this->trItemId[this->selectionValue]];
    if(trit == NULL) 
        return;
    if(pz < 0){
        trit->trackPositionAdd(-1);
        if(trit->getTrackPosition() < 0)
            trit->setTrackPosition(0);
        tdb->updateTrItemRData(trit);
    } else if(pz > 0){
        trit->trackPositionAdd(1);
        if(trit->getTrackPosition() > dlugosc)
            trit->setTrackPosition(dlugosc);
        tdb->updateTrItemRData(trit);
    }
    if(this->selectionValue == 1){
        delete[] drawPositionB;
        drawPositionB = NULL;   
    } else if(this->selectionValue == 3){
        delete[] drawPositionE;
        drawPositionE = NULL;
    }
    delete line;
    line = NULL;
    this->modified = true;
    setMartix();
}

bool PlatformObj::select(int value){
    this->selectionValue = value;
    this->selected = true;
}

bool PlatformObj::isTrackItem(){
    return true;
}

void PlatformObj::initTrItems(float* tpos){
    if(tpos == NULL)
        return;
    int trNodeId = tpos[0];
    float metry = tpos[1];
    
    int isRoad = 0;
    TDB* tdb = Game::trackDB;
    if(this->typeID == this->carspawner){
        isRoad = 1;
        tdb = Game::roadDB;
    }

    int trItemId[2];

    tdb->newPlatformObject(trItemId, trNodeId, metry, this->typeID);

    this->trItemIdCount = 4;
    this->trItemId[0] = isRoad;
    this->trItemId[1] = trItemId[0];
    this->trItemId[2] = isRoad;
    this->trItemId[3] = trItemId[1];
}

void PlatformObj::deleteTrItems(){
    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == 0){
            tdb->deleteTrItem(this->trItemId[i*2+1]);
        } else if(this->trItemId[i*2] == 1) {
            rdb->deleteTrItem(this->trItemId[i*2+1]);
        }
        this->trItemId[i*2+1] = -1;
    }
}

bool PlatformObj::containsTrackItem(int tdbId, int id){
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == tdbId){
            if(this->trItemId[i*2+1] == id)
                return true;
        }
    }
    return false;
}

void PlatformObj::getTrackItemIds(QVector<int> &ids, int tdbId){
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == tdbId){
            ids.push_back(this->trItemId[i*2+1]);
        }
    }
}

int PlatformObj::getTrackBegItemId(){
    if(this->trItemIdCount < 2)
        return -1;
    return this->trItemId[1];
}

QString PlatformObj::getStationName(){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return "";
    return trit->stationName;
}

QString PlatformObj::getPlatformName(){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return "";
    return trit->platformName;
}
int PlatformObj::getPlatformMinWaitingTime(){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return 0;
    return trit->platformMinWaitingTime;
}
int PlatformObj::getPlatformNumPassengersWaiting(){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return 0;
    return trit->platformNumPassengersWaiting;
}
void PlatformObj::setStationName(QString name){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->stationName = name;
    id = this->trItemId[3];
    trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->stationName = name;
    this->modified = true;
}
void PlatformObj::setPlatformName(QString name){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->platformName = name;
    id = this->trItemId[3];
    trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->platformName = name;
    this->modified = true;
}
void PlatformObj::setPlatformMinWaitingTime(int val){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->platformMinWaitingTime = val;
    id = this->trItemId[3];
    trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->platformMinWaitingTime = val;
    this->modified = true;
}
void PlatformObj::setPlatformNumPassengersWaiting(int val){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->platformNumPassengersWaiting = val;
    id = this->trItemId[3];
    trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->platformNumPassengersWaiting = val;
    this->modified = true;
}
    
float PlatformObj::getLength(){
    TDB* tdb = Game::trackDB;
    TRitem* p1 = tdb->trackItems[this->trItemId[1]];
    TRitem* p2 = tdb->trackItems[this->trItemId[3]];
    if(p1 == NULL || p2 == NULL) return 0;
    return fabs(p2->getTrackPosition() - p1->getTrackPosition());
}

bool PlatformObj::getSideLeft(){
    return ((this->platformData & 2) == 2);
}

bool PlatformObj::getSideRight(){
    return ((this->platformData & 4) == 4);
}

bool PlatformObj::getDisabled(){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return false;
    return ((trit->platformTrItemData[0] & 1) == 1);
}
void PlatformObj::setSideLeft(bool val){
    if(val)
        this->platformData = this->platformData | 2;
    else 
        this->platformData = this->platformData ^ 2;
    delete line;
    line = NULL;
    this->modified = true;
}

void PlatformObj::setSideRight(bool val){
    if(val)
        this->platformData = this->platformData | 4;
    else 
        this->platformData = this->platformData ^ 4;
    delete line;
    line = NULL;
    this->modified = true;
}

void PlatformObj::setDisabled(bool val){
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return;
    if(val)
        trit->platformTrItemData[0] = trit->platformTrItemData[0] | 1;
    else
        trit->platformTrItemData[0] = trit->platformTrItemData[0] ^ 1;
    delete line;
    line = NULL;
    this->modified = true;
}

void PlatformObj::checkForErrors(){
    if(!this->loaded) 
        return;
    
    TDB* tdb = Game::trackDB;
    if(tdb == NULL)
        return;
    
    for(int i = 0; i < 2; i++){
        int trItemId = this->trItemId[i*2+1];
        
        // Check If Track Item match this object.
        TRitem *item = tdb->trackItems[trItemId];
        if(item == NULL){
            ErrorMessage *e = new ErrorMessage("error", 
                "World", 
                QString("Object '") + type + "' - reference to trackItem not found. UiD: " + QString::number(UiD) + ". ItemID: " + trItemId,
                 "World Object requires Interactive Item that does not exist in Track DB. \n"
            );
            e->setLocationXYZ(x, -y, position[0], position[1], -position[2]);
            e->setObject((GameObj*)this);
            ErrorMessagesLib::PushErrorMessage(e);
            continue;
        }
        
        // Check Track Node Location
        int id = tdb->findTrItemNodeId(trItemId);
        if (id < 1) {
            
            return;
        }
        
        // Check World Location
        float data[7];
        bool ok = tdb->getDrawPositionOnTrNode(data, id, item->getTrackPosition());
        if(!ok){
            
            return;
        }
    }
}

void PlatformObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if(!this->loaded) 
        return;
    
    if(Game::showWorldObjPivotPoints){
        gluu->mvPushMatrix();
        Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.9,0.9,0.7);
        }
        pointer3d->render(selectionColor);
        gluu->mvPopMatrix();
    }
    
    if(Game::viewInteractives && renderMode != gluu->RENDER_SHADOWMAP) 
        this->renderTritems(gluu, selectionColor);
};

void PlatformObj::renderTritems(GLUU* gluu, int selectionColor){
    
    if (drawPositionB == NULL) {
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 1) {
            qDebug() << "platform fail id "<<id;
            this->loaded = false;
            return;
        }
        //qDebug() << "id: "<< this->trItemId[1] << " "<< id;
        //qDebug() << "d: "<< tdb->trackItems[this->trItemId[1]]->trItemSData1;
        drawPositionB = new float[7];
        bool ok = tdb->getDrawPositionOnTrNode(drawPositionB, id, tdb->trackItems[this->trItemId[1]]->getTrackPosition());
        if(!ok){
            qDebug() << "platform fail tdb "<<id;
            this->loaded = false;
            return;
        }
        drawPositionB[0] += 2048 * (drawPositionB[5] - this->x);
        drawPositionB[2] -= 2048 * (-drawPositionB[6] - this->y);
    }
    if (drawPositionE == NULL) {
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[3]);
        if (id < 1) {
            qDebug() << "platform fail id";
            this->loaded = false;
            return;
        }
        drawPositionE = new float[7];
        bool ok = tdb->getDrawPositionOnTrNode(drawPositionE, id, tdb->trackItems[this->trItemId[3]]->getTrackPosition());
        if(!ok){
            qDebug() << "platform fail tdb "<<id;
            this->loaded = false;
            return;
        }
        drawPositionE[0] += 2048 * (drawPositionE[5] - this->x);
        drawPositionE[2] -= 2048 * (-drawPositionE[6] - this->y);
    }

    if(line == NULL){
        if(spointer3d == NULL) spointer3d = new TrackItemObj();
        if(spointer3dSelected == NULL) spointer3dSelected = new TrackItemObj();

        makelineShape();
        
        if(this->typeID == this->platform){
            line->setMaterial(0.0, 1.0, 0.0);
            spointer3d->setMaterial(0.0, 1.0, 0.0);
            spointer3dSelected->setMaterial(0.5, 1.0, 0.5);
        }
        if(this->typeID == this->siding){
            line->setMaterial(1.0, 0.7, 0.0);
            spointer3d->setMaterial(1.0, 0.7, 0.0);
            spointer3dSelected->setMaterial(1.0, 1.0, 0.5);
        }
        if(this->typeID == this->carspawner){
            line->setMaterial(0.4, 0.0, 1.0);
            spointer3d->setMaterial(0.4, 0.0, 1.0);
            spointer3dSelected->setMaterial(0.9, 0.5, 1.0);
        }
    }
    //if(pos == NULL) return;
    //gluu->setMatrixUniforms();
    //float dlugosc = (float) sqrt(pow(drawPositionB[2]-drawPositionE[2], 2) + pow(drawPositionB[0]-drawPositionE[0], 2));
    float aa = (drawPositionE[2]-drawPositionB[2]);
    if(aa != 0) aa = (aa/fabs(aa));
    //float rot = (aa+1)*M_PI/2 + (float)(atan((drawPositionB[0]-drawPositionE[0])/(drawPositionB[2]-drawPositionE[2]))); 
    
    //(-(float)(atan((drawPositionB[1]-drawPositionE[1])/(dlugosc))
    int useSC;
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[5] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[6] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[3] + rotB*M_PI);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 1) 
        spointer3dSelected->render(selectionColor | 1*useSC);
    else
        spointer3d->render(selectionColor | 1*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[0] + 0 * (drawPositionE[5] - this->x), drawPositionE[1] + 1, -drawPositionE[2] + 0 * (-drawPositionE[6] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[3] + rotE*M_PI);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 3) 
        spointer3dSelected->render(selectionColor | 3*useSC);
    else
        spointer3d->render(selectionColor | 3*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    if(selectionColor == 0)
        line->render();
    gluu->mvPopMatrix();
};

void PlatformObj::makelineShape(){
        line = new OglObj();
        float *ptr, *punkty;// = new float[6];
        int length, len = 0;
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        tdb->getVectorSectionLine(ptr, length, x, y, id, true);
        
        float beg = tdb->trackItems[this->trItemId[1]]->getTrackPosition();
        float end = tdb->trackItems[this->trItemId[3]]->getTrackPosition();
        float posB[3], posE[3];
        int side = 0;
        if(end < beg){
            rotE = 1;
            rotB = 0;
            side = 1;
            float t = end;
            end = beg;
            beg = t;
            posE[0] = drawPositionB[0];
            posE[1] = drawPositionB[1];
            posE[2] = drawPositionB[2];
            posB[0] = drawPositionE[0];
            posB[1] = drawPositionE[1];
            posB[2] = drawPositionE[2];     
        } else {
            rotE = 0;
            rotB = 1;
            side = -1;
            posB[0] = drawPositionB[0];
            posB[1] = drawPositionB[1];
            posB[2] = drawPositionB[2];
            posE[0] = drawPositionE[0];
            posE[1] = drawPositionE[1];
            posE[2] = drawPositionE[2];
        }
        //for(int i = 0; i < length; i+=12){
        //    qDebug() << ptr[i+5] << "-"<<ptr[i+11];
        //}
        float quat90[4]{0.707,0,-0.707,0};
        if(end - beg > 4){
            punkty = new float[length+6];
            //qDebug() << beg <<" "<<end;
            float tp[3], tp1[3], tp2[3], tp3[3];
            bool endd = false;
            for(int i = 0; i < length; i+=12){
                if(ptr[i+5] < beg) continue;
                if(ptr[i+5+12] > end)
                    endd = true;
                
                if(len == 0)
                    Vec3::set(tp1, posB[0], posB[1]+1, -posB[2]);
                else 
                    Vec3::set(tp1, ptr[i+0], ptr[i+1]+1, ptr[i+2]);
                
                if(endd)
                    Vec3::set(tp2, posE[0], posE[1]+1, -posE[2]); 
                else
                    Vec3::set(tp2, ptr[i+6], ptr[i+7]+1, ptr[i+8]);
                
                Vec3::sub(tp, tp2, tp1);
                Vec3::normalize(tp, tp);
                Vec3::transformQuat(tp3, tp, quat90);
                if(!this->getSideRight() && !this->getSideLeft()){
                    punkty[len++] = tp1[0];
                    punkty[len++] = tp1[1];
                    punkty[len++] = tp1[2];
                    punkty[len++] = tp2[0];
                    punkty[len++] = tp2[1];
                    punkty[len++] = tp2[2];
                }
                if(this->getSideRight()){
                    punkty[len++] = tp1[0] + tp3[0]*side;
                    punkty[len++] = tp1[1] + tp3[1]*side;
                    punkty[len++] = tp1[2] - tp3[2]*side;
                    punkty[len++] = tp2[0] + tp3[0]*side;
                    punkty[len++] = tp2[1] + tp3[1]*side;
                    punkty[len++] = tp2[2] - tp3[2]*side;
                }
                if(this->getSideLeft()){
                    punkty[len++] = tp1[0] - tp3[0]*side;
                    punkty[len++] = tp1[1] - tp3[1]*side;
                    punkty[len++] = tp1[2] + tp3[2]*side;
                    punkty[len++] = tp2[0] - tp3[0]*side;
                    punkty[len++] = tp2[1] - tp3[1]*side;
                    punkty[len++] = tp2[2] + tp3[2]*side;
                }
                if(endd) break;
            }
            int lastItem = len;
            if(this->getSideRight() && this->getSideLeft()){
                punkty[len++] = punkty[0];
                punkty[len++] = punkty[1];
                punkty[len++] = punkty[2];
                punkty[len++] = punkty[6];
                punkty[len++] = punkty[7];
                punkty[len++] = punkty[8];
                punkty[len++] = punkty[lastItem-3];
                punkty[len++] = punkty[lastItem-2];
                punkty[len++] = punkty[lastItem-1];
                punkty[len++] = punkty[lastItem-9];
                punkty[len++] = punkty[lastItem-8];
                punkty[len++] = punkty[lastItem-7];
            } else if(this->getSideLeft() || this->getSideRight()){
                punkty[len++] = punkty[0];
                punkty[len++] = punkty[1];
                punkty[len++] = punkty[2];
                punkty[len++] = posB[0];
                punkty[len++] = posB[1]+1;
                punkty[len++] = -posB[2];
                punkty[len++] = punkty[lastItem-3];
                punkty[len++] = punkty[lastItem-2];
                punkty[len++] = punkty[lastItem-1];
                punkty[len++] = posE[0];
                punkty[len++] = posE[1]+1;
                punkty[len++] = -posE[2];
            }
        } else {
            punkty = new float[6];
            len = 6;
            punkty[0] = posB[0];
            punkty[1] = posB[1]+1;
            punkty[2] = -posB[2];
            punkty[len-3] = posE[0];
            punkty[len-2] = posE[1]+1;
            punkty[len-1] = -posE[2];
        }
        
        //qDebug() << "len "<<len;
        //punkty[ptr++] = 0;
        //punkty[ptr++] = 0;
        //punkty[ptr++] = 0;
        //punkty[ptr++] = drawPositionE[0]-drawPositionB[0];
        //punkty[ptr++] = drawPositionE[1]-drawPositionB[1];
        //punkty[ptr++] = -drawPositionE[2]+drawPositionB[2];
        line->init(punkty, len, line->V, GL_LINES);
        delete[] ptr;
        delete[] punkty;
}

int PlatformObj::getDefaultDetailLevel(){
    return -7;
}

void PlatformObj::save(QTextStream* out) {
    if (!loaded) return;
    int l;
    QString flags = ParserX::MakeFlagsString(this->staticFlags);
    QString flags2 = ParserX::MakeFlagsString(this->platformData);
    if(Game::useOnlyPositiveQuaternions)
        Quat::makePositive(this->qDirection);
    
    if (type == "siding")
        *(out) << "	Siding (\n";
    if (type == "platform")
        *(out) << "	Platform (\n";
    *(out) << "		UiD ( " << this->UiD << " )\n";
    if (type == "siding")
        *(out) << "		SidingData ( " << flags2 << " )\n";
    if (type == "platform")
        *(out) << "		PlatformData ( " << flags2 << " )\n";
    *(out) << "		TrItemId ( " << this->trItemId[0] << " " << this->trItemId[1] << " )\n";
    *(out) << "		TrItemId ( " << this->trItemId[2] << " " << this->trItemId[3] << " )\n";
    *(out) << "		StaticFlags ( " << flags << " )\n";
    *(out) << "		Position ( " << this->position[0] << " " << this->position[1] << " " << -this->position[2] << " )\n";
    *(out) << "		QDirection ( " << this->qDirection[0] << " " << this->qDirection[1] << " " << -this->qDirection[2] << " " << this->qDirection[3] << " )\n";
    *(out) << "		VDbId ( " << this->vDbId << " )\n";
    if (this->staticDetailLevel > -1)
        *(out) << "		StaticDetailLevel ( " << this->staticDetailLevel << " )\n";
    *(out) << "	)\n";
}