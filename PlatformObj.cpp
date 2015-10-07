#include "PlatformObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include "Game.h"
#include "TDB.h"
#include "TrackItemObj.h"
#include "OglObj.h"
#include <Math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

void PlatformObj::set(QString sh, FileBuffer* data) {
    if (sh == ("sidingdata") || sh == ("platformdata")) {
        platformData = ParserX::parsuj16(data);
        return;
    }
    if (sh == ("carfrequency")) {
        carFrequency = ParserX::parsujr(data);
        return;
    }
    if (sh == ("caravspeed")) {
        carAvSpeed = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
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

    } else if(pz > 0){
        trit->trItemSData1 += 1;
        if(trit->trItemSData1 > dlugosc)
            trit->trItemSData1 = dlugosc;

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
    int metry = tpos[1];
    
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
    if(this->typeID == this->carspawner){
        tdb = Game::roadDB;
    }
    tdb->deleteTrItem(this->trItemId[1]);
    tdb->deleteTrItem(this->trItemId[3]);
    
    this->trItemId[1] = -1;
    this->trItemId[3] = -1;
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
    if(!val) return;
    this->platformData = this->platformData | 2;
}
void PlatformObj::setSideRight(bool val){
    if(!val) return;
    this->platformData = this->platformData | 4;
}
void PlatformObj::setDisabled(bool val){
    if(!val) return;
    TDB* tdb = Game::trackDB;
    int id = this->trItemId[1];
    TRitem* trit = tdb->trackItems[id];
    if(trit == NULL) return;
    trit->platformTrItemData[0] = trit->platformTrItemData[0] | 1;
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
    this->renderTritems(gluu, selectionColor);
};

void PlatformObj::renderTritems(GLUU* gluu, int selectionColor){
    
    if (drawPositionB == NULL) {
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 1) {
            qDebug() << "fail id "<<id;
            this->loaded = false;
            return;
        }
        //qDebug() << "id: "<< this->trItemId[1] << " "<< id;
        //qDebug() << "d: "<< tdb->trackItems[this->trItemId[1]]->trItemSData1;
        drawPositionB = new float[7];
        tdb->getDrawPositionOnTrNode(drawPositionB, id, tdb->trackItems[this->trItemId[1]]->trItemSData1);
        drawPositionB[0] += 2048 * (drawPositionB[5] - this->x);
        drawPositionB[2] -= 2048 * (-drawPositionB[6] - this->y);
    }
    if (drawPositionE == NULL) {
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[3]);
        if (id < 1) {
            qDebug() << "fail id";
            this->loaded = false;
            return;
        }
        drawPositionE = new float[7];
        tdb->getDrawPositionOnTrNode(drawPositionE, id, tdb->trackItems[this->trItemId[3]]->trItemSData1);
        
        drawPositionE[0] += 2048 * (drawPositionE[5] - this->x);
        drawPositionE[2] -= 2048 * (-drawPositionE[6] - this->y);
    }

    if(line == NULL){
        if(pointer3d == NULL) pointer3d = new TrackItemObj();
        if(pointer3dSelected == NULL) pointer3dSelected = new TrackItemObj();
        line = new OglObj();
        float *punkty = new float[6];
        int ptr = 0;
        int i = 0;

        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = drawPositionE[0]-drawPositionB[0];
        punkty[ptr++] = drawPositionE[1]-drawPositionB[1];
        punkty[ptr++] = -drawPositionE[2]+drawPositionB[2];
        
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
        
        line->init(punkty, ptr, line->V, GL_LINES);
        delete[] punkty;
    }
    //if(pos == NULL) return;
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();
    
    //float dlugosc = (float) sqrt(pow(drawPositionB[2]-drawPositionE[2], 2) + pow(drawPositionB[0]-drawPositionE[0], 2));
    float aa = (drawPositionE[2]-drawPositionB[2]);
    if(aa != 0) aa = (aa/fabs(aa));
    float rot = (aa+1)*M_PI/2 + (float)(atan((drawPositionB[0]-drawPositionE[0])/(drawPositionB[2]-drawPositionE[2]))); 
    
    //(-(float)(atan((drawPositionB[1]-drawPositionE[1])/(dlugosc))
    int useSC;
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, -rot);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 1) 
        pointer3dSelected->render(selectionColor + 1*65536*16*useSC);
    else
        pointer3d->render(selectionColor + 1*65536*16*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[0] + 0 * (drawPositionE[4] - this->x), drawPositionE[1] + 1, -drawPositionE[2] + 0 * (-drawPositionE[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, -rot + M_PI);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 3) 
        pointer3dSelected->render(selectionColor + 3*65536*16*useSC);
    else
        pointer3d->render(selectionColor + 3*65536*16*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    line->render();
    gluu->mvPopMatrix();
};

void PlatformObj::save(QTextStream* out) {
    if (!loaded) return;
    int l;
    QString flags;
    flags = QString::number(this->staticFlags, 16);
    l = flags.length();
    for (int i = 0; i < 8 - l; i++)
        flags = "0" + flags;

    QString flags2;
    flags2 = QString::number(this->platformData, 16);
    l = flags2.length();
    for (int i = 0; i < 8 - l; i++)
        flags2 = "0" + flags2;

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