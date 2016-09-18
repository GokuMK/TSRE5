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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

QVector<QString> PlatformObj::CarSpawnerList;

void PlatformObj::LoadCarSpawnerList(){
    QString path = Game::root + "/routes/" + Game::route + "/openrails/extcarspawn.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    ParserX::NextLine(data);

    QString sh = "";
    //ParserX::szukajsekcji1(sh, data);
    //ParserX::GetNumber(data);
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("carspawnerlist")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                qDebug() << sh;
                if (sh == ("listname")) {
                    CarSpawnerList.push_back(ParserX::GetString(data));
                    qDebug() << CarSpawnerList.back();
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }
}

PlatformObj::PlatformObj() {
    this->shape = -1;
    this->loaded = false;

    //pointer3d = new TrackItemObj();
    //pointer3dSelected = new TrackItemObj();
}

PlatformObj::PlatformObj(const PlatformObj& orig) {
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
    
    if(this->typeID == this->carspawner){
        this->carAvSpeed = 20;
        this->carFrequency = 5;
    }
    
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
    if (sh == TS::CarFrequency) {
        data->off++;
        carFrequency = data->getFloat();
        return;
    }
    if (sh == TS::CarAvSpeed) {
        data->off++;
        carAvSpeed = data->getFloat();
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
    if (sh == ("carfrequency")) {
        carFrequency = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("caravspeed")) {
        carAvSpeed = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("ortslistname")) {
        this->carspawnerListName = ParserX::GetString(data);
        return;
    }
     
    WorldObj::set(sh, data);
    return;
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
        trit->trItemSData1 -= 1;
        if(trit->trItemSData1 < 0)
            trit->trItemSData1 = 0;
        tdb->updateTrItemRData(trit);
    } else if(pz > 0){
        trit->trItemSData1 += 1;
        if(trit->trItemSData1 > dlugosc)
            trit->trItemSData1 = dlugosc;
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
        if(this->trItemId[i*2] == 0)
            tdb->deleteTrItem(this->trItemId[i*2+1]);
        else if(this->trItemId[i*2] == 1)
            rdb->deleteTrItem(this->trItemId[i*2+1]);
        this->trItemId[i*2+1] = -1;
    }
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
}
    
float PlatformObj::getLength(){
    TDB* tdb = Game::trackDB;
    TRitem* p1 = tdb->trackItems[this->trItemId[1]];
    TRitem* p2 = tdb->trackItems[this->trItemId[3]];
    if(p1 == NULL || p2 == NULL) return 0;
    return fabs(p2->trItemSData1 - p1->trItemSData1);
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
}

void PlatformObj::setSideRight(bool val){
    if(val)
        this->platformData = this->platformData | 4;
    else 
        this->platformData = this->platformData ^ 4;
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
}

int PlatformObj::getCarNumber(){
    return this->carFrequency;
}

int PlatformObj::getCarSpeed(){
    return this->carAvSpeed;
}

void PlatformObj::setCarNumber(int val){
    this->carFrequency = val;
}

void PlatformObj::setCarSpeed(int val){
    this->carAvSpeed = val;
}


void PlatformObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    //Vector3f *pos = tdb->getDrawPositionOnTrNode(playerT, id, this->trItemSData1);
    if(!this->loaded) 
        return;
    if(Game::viewInteractives) 
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
        bool ok = tdb->getDrawPositionOnTrNode(drawPositionB, id, tdb->trackItems[this->trItemId[1]]->trItemSData1);
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
        bool ok = tdb->getDrawPositionOnTrNode(drawPositionE, id, tdb->trackItems[this->trItemId[3]]->trItemSData1);
        if(!ok){
            qDebug() << "platform fail tdb "<<id;
            this->loaded = false;
            return;
        }
        drawPositionE[0] += 2048 * (drawPositionE[5] - this->x);
        drawPositionE[2] -= 2048 * (-drawPositionE[6] - this->y);
    }

    if(line == NULL){
        if(pointer3d == NULL) pointer3d = new TrackItemObj();
        if(pointer3dSelected == NULL) pointer3dSelected = new TrackItemObj();

        makelineShape();
        
        if(this->typeID == this->platform){
            line->setMaterial(0.0, 1.0, 0.0);
            pointer3d->setMaterial(0.0, 1.0, 0.0);
            pointer3dSelected->setMaterial(0.5, 1.0, 0.5);
        }
        if(this->typeID == this->siding){
            line->setMaterial(1.0, 0.7, 0.0);
            pointer3d->setMaterial(1.0, 0.7, 0.0);
            pointer3dSelected->setMaterial(1.0, 1.0, 0.5);
        }
        if(this->typeID == this->carspawner){
            line->setMaterial(0.4, 0.0, 1.0);
            pointer3d->setMaterial(0.4, 0.0, 1.0);
            pointer3dSelected->setMaterial(0.9, 0.5, 1.0);
        }
    }
    //if(pos == NULL) return;
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();
    
    //float dlugosc = (float) sqrt(pow(drawPositionB[2]-drawPositionE[2], 2) + pow(drawPositionB[0]-drawPositionE[0], 2));
    float aa = (drawPositionE[2]-drawPositionB[2]);
    if(aa != 0) aa = (aa/fabs(aa));
    //float rot = (aa+1)*M_PI/2 + (float)(atan((drawPositionB[0]-drawPositionE[0])/(drawPositionB[2]-drawPositionE[2]))); 
    
    //(-(float)(atan((drawPositionB[1]-drawPositionE[1])/(dlugosc))
    int useSC;
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[3] + rotB*M_PI);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 1) 
        pointer3dSelected->render(selectionColor + 1*131072*8*useSC);
    else
        pointer3d->render(selectionColor + 1*131072*8*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[0] + 0 * (drawPositionE[4] - this->x), drawPositionE[1] + 1, -drawPositionE[2] + 0 * (-drawPositionE[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[3] + rotE*M_PI);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 3) 
        pointer3dSelected->render(selectionColor + 3*131072*8*useSC);
    else
        pointer3d->render(selectionColor + 3*131072*8*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
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
        tdb->getVectorSectionLine(ptr, length, x, y, id);
        
        float beg = tdb->trackItems[this->trItemId[1]]->trItemSData1;
        float end = tdb->trackItems[this->trItemId[3]]->trItemSData1;
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
            if(this->getSideRight() && this->getSideLeft()){
                punkty[len++] = punkty[0];
                punkty[len++] = punkty[1];
                punkty[len++] = punkty[2];
                punkty[len++] = punkty[6];
                punkty[len++] = punkty[7];
                punkty[len++] = punkty[8];
                punkty[len++] = punkty[len-3-7];
                punkty[len++] = punkty[len-2-8];
                punkty[len++] = punkty[len-1-9];
                punkty[len++] = punkty[len-9-10];
                punkty[len++] = punkty[len-8-11];
                punkty[len++] = punkty[len-7-12];
            } else if(this->getSideLeft() || this->getSideRight()){
                punkty[len++] = punkty[0];
                punkty[len++] = punkty[1];
                punkty[len++] = punkty[2];
                punkty[len++] = posB[0];
                punkty[len++] = posB[1]+1;
                punkty[len++] = -posB[2];
                punkty[len++] = punkty[len-3-7];
                punkty[len++] = punkty[len-2-8];
                punkty[len++] = punkty[len-1-9];
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

QString PlatformObj::getCarListName(){
    return carspawnerListName;
}

void PlatformObj::setCarListName(QString val){
    carspawnerListName = val;
}

void PlatformObj::save(QTextStream* out) {
    if (!loaded) return;
    int l;
    QString flags = ParserX::MakeFlagsString(this->staticFlags);
    QString flags2 = ParserX::MakeFlagsString(this->platformData);

    if (type == "siding")
        *(out) << "	Siding (\n";
    if (type == "platform")
        *(out) << "	Platform (\n";
    if (type == "carspawner")
        *(out) << "	CarSpawner (\n";

    *(out) << "		UiD ( " << this->UiD << " )\n";
    if (type == "siding")
        *(out) << "		SidingData ( " << flags2 << " )\n";
    if (type == "platform")
        *(out) << "		PlatformData ( " << flags2 << " )\n";
    if (type == "carspawner") {
        *(out) << "		CarFrequency ( " << this->carFrequency << " )\n";
        *(out) << "		CarAvSpeed ( " << this->carAvSpeed << " )\n";
        if (this->carspawnerListName.length() > 0)
            *(out) << "		ORTSListName ( " << ParserX::AddComIfReq(this->carspawnerListName) << " )\n";
    }
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