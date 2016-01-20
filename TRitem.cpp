#include "TRitem.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include "GLMatrix.h"
#include "TDB.h"
#include "Vector3f.h"
#include "Game.h"
#include "SigCfg.h"
#include <math.h>
#include "SignalShape.h"
#include <QString>
#include <QDebug>
#include "TrackItemObj.h"

TrackItemObj* TRitem::pointer3d = NULL; 

TRitem* TRitem::newPlatformItem(int trItemId, float metry){
    TRitem* trit = new TRitem(trItemId);
    if(!trit->init("platformitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 2;
    trit->platformTrItemData = new unsigned int[2];
    trit->platformTrItemData[0] = 0;
    trit->platformTrItemData[1] = 0;
    trit->platformName = "new platform";
    trit->stationName = "new station";
    trit->platformMinWaitingTime = 180;
    trit->platformNumPassengersWaiting = 7;
    return trit;
}

TRitem* TRitem::newSidingItem(int trItemId, float metry){
    TRitem* trit = new TRitem(trItemId);
    if(!trit->init("sidingitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 2;
    trit->platformTrItemData = new unsigned int[2];
    trit->platformTrItemData[0] = 0;
    trit->platformTrItemData[1] = 0;
    trit->platformName = "new siding";
    return trit;
}

TRitem* TRitem::newCarspawnerItem(int trItemId, float metry){
    TRitem* trit = new TRitem(trItemId);
    if(!trit->init("carspawneritem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 1;
    trit->platformTrItemData = new unsigned int[2];
    trit->platformTrItemData[0] = 0;
    trit->platformTrItemData[1] = 0;
    return trit;
}


TRitem* TRitem::newPickupItem(int trItemId, float metry){
    TRitem* trit = new TRitem(trItemId);
    if(!trit->init("pickupitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 6;
    trit->pickupTrItemData1 = 0;
    trit->pickupTrItemData2 = 0;
    return trit;
}

TRitem* TRitem::newSignalItem(int trItemId, float metry, unsigned int flags, QString type){
    TRitem* trit = new TRitem(trItemId);
    if(!trit->init("signalitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 2;
    trit->platformTrItemData = new unsigned int[2];
    trit->trSignalType1 = flags;
    trit->trSignalType2 = 1;
    trit->trSignalType3 = 0;
    trit->trSignalType4 = type;
    qDebug() << "aa ";
    return trit;
}

void TRitem::setPickupContent(float val){
    this->pickupTrItemData1 = val;
}

void TRitem::enableSignalSubObj(int i){
    this->trSignalType1 = this->trSignalType1 | (1 << (i+3) );
}
void TRitem::disableSignalSubObj(int i){
    this->trSignalType1 = this->trSignalType1 ^ (1 << (i+3) );
}


TRitem::TRitem() {
}

TRitem::TRitem(int id) {
    this->trItemId = id;
}

TRitem::TRitem(const TRitem& orig) {
}

TRitem::~TRitem() {
}

bool TRitem::init(QString sh){
    type = sh;
    trItemPData = NULL;
    trItemRData = NULL;
    crossoverTrItemData = NULL;
    platformTrItemData = NULL;
    trItemSRData = NULL;
    speedpostTrItemData = NULL;
    trSignalDir = NULL;
    //pickupTrItemData = NULL;
    
    if(sh == "crossoveritem") return true;
    if(sh == "signalitem") return true;
    if(sh == "soundregionitem") return true;
    if(sh == "levelcritem") return true;
    if(sh == "speedpostitem") return true;
    if(sh == "platformitem") return true;
    if(sh == "sidingitem") return true;
    if(sh == "carspawneritem") return true;    
    if(sh == "emptyitem") return true;
    if(sh == "hazzarditem") return true;    
    if(sh == "pickupitem") return true;    
    return false;
}

void TRitem::set(QString sh, FileBuffer* data) {
    //qDebug() << "---"<< sh;
    if (sh == ("tritemid")) {
        trItemId = ParserX::parsujUint(data);
        return;
    }
    if (sh == ("tritemsdata")) {
        trItemSData1 = ParserX::parsujr(data);
        trItemSData2 = ParserX::parsuj16(data);
        return;
    }
    if (sh == ("tritempdata")) {
        trItemPData = new float[4];
        trItemPData[0] = ParserX::parsujr(data);
        trItemPData[1] = ParserX::parsujr(data);
        trItemPData[2] = ParserX::parsujr(data);
        trItemPData[3] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemrdata")) {
        trItemRData = new float[5];
        trItemRData[0] = ParserX::parsujr(data);
        trItemRData[1] = ParserX::parsujr(data);
        trItemRData[2] = ParserX::parsujr(data);
        trItemRData[3] = ParserX::parsujr(data);
        trItemRData[4] = ParserX::parsujr(data);
        return;
    }
    
    // crossover
    if (sh == ("crossovertritemdata")) {
        crossoverTrItemData = new int[2];
        crossoverTrItemData[0] = (int) ParserX::parsujr(data);
        crossoverTrItemData[1] = (int) ParserX::parsujr(data);
        return;
    }
    
    // platform / siding
    if (sh == ("platformtritemdata") || sh == ("sidingtritemdata")) {
        platformTrItemData = new unsigned int[2];
        platformTrItemData[0] = ParserX::parsuj16(data);
        platformTrItemData[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("platformname") || sh == ("sidingname")) {
        platformName = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("station")) {
        stationName = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("platformminwaitingtime")) {
        platformMinWaitingTime = ParserX::parsujr(data);
        return;
    }
    if (sh == ("platformnumpassengerswaiting")) {
        platformNumPassengersWaiting = ParserX::parsujr(data);
        return;
    }
    
    // soundregion
    if (sh == ("tritemsrdata")) {
        trItemSRData = new float[3];
        trItemSRData[0] = ParserX::parsujr(data);
        trItemSRData[1] = ParserX::parsujr(data);
        trItemSRData[2] = ParserX::parsujr(data);
        return;
    }
    
    // speed
    if (sh == ("speedposttritemdata")){
        speedpostTrItemData = new float[3];
        bool ok = false;
        speedpostTrItemData[0] = ParserX::parsujrInside(data, ok);
        if(!ok) return;
        speedpostTrItemDataLength++;
        speedpostTrItemData[1] = ParserX::parsujrInside(data, ok);
        if(!ok) return;
        speedpostTrItemDataLength++;
        speedpostTrItemData[2] = ParserX::parsujrInside(data, ok);
        if(!ok) return;
        speedpostTrItemDataLength++;
        return;
    }
    
    // signal
    if (sh == ("trsignaltype")) {
        trSignalType1 = ParserX::parsuj16(data);
        //if((trSignalType1 & 6) != 0)
        //    qDebug() << "tst "<< this->trItemId << " "<<trSignalType1;
        trSignalType2 = ParserX::parsujr(data);
        trSignalType3 = ParserX::parsujr(data);
        trSignalType4 = ParserX::odczytajtc(data);
        return;
    }
    
    if (sh == ("trsignaldirs")) {
        trSignalDirs = ParserX::parsujr(data);
        if(!titLoading){
            trSignalDir = new int[trSignalDirs*4];
            for(int i = 0; i < trSignalDirs*4; i+=4){
                trSignalDir[i+0] = ParserX::parsujr(data);
                trSignalDir[i+1] = ParserX::parsujr(data);
                trSignalDir[i+2] = ParserX::parsujr(data);
                trSignalDir[i+3] = ParserX::parsujr(data);
            }
        }else{
            trSignalRDir = new float[trSignalDirs*6];
            for(int i = 0; i < trSignalDirs*6; i+=6){
                trSignalRDir[i+0] = ParserX::parsujr(data);
                trSignalRDir[i+1] = ParserX::parsujr(data);
                trSignalRDir[i+2] = ParserX::parsujr(data);
                trSignalRDir[i+3] = ParserX::parsujr(data);
                trSignalRDir[i+4] = ParserX::parsujr(data);
                trSignalRDir[i+5] = ParserX::parsujr(data);
            }
        }
        ParserX::pominsekcje(data);
        return;
    }
    
    // pickup
    if (sh == ("pickuptritemdata")) {
        pickupTrItemData1 = ParserX::parsujr(data);
        pickupTrItemData2 = ParserX::parsuj16(data);
        return;
    }
    
    qDebug() << "=" << sh;
    return;
}

void TRitem::setTrItemRData(float* posT, float* pos){
    if(this->trItemRData == NULL)
        this->trItemRData = new float[5];
    this->trItemRData[0] = pos[0];
    this->trItemRData[1] = pos[1];
    this->trItemRData[2] = pos[2];
    this->trItemRData[3] = posT[0];
    this->trItemRData[4] = posT[1];
}

void TRitem::setTrItemPData(float* posT, float* pos){
    if(this->trItemRData == NULL)
        this->trItemRData = new float[4];
    this->trItemRData[0] = pos[0];
    this->trItemRData[1] = pos[2];
    this->trItemRData[2] = posT[0];
    this->trItemRData[3] = posT[1];
}

void TRitem::setSignalRot(float rot){
    this->trSignalType3 = rot;
}

void TRitem::setSignalDirection(int dir){
    this->trSignalType2 = dir;
}

void TRitem::flipSignal(){
    this->trSignalType2 = abs(this->trSignalType2-1);
    this->trSignalType3 += M_PI;
    if(this->trSignalType3 > 2*M_PI)
        this->trSignalType3 -= 2*M_PI;
    if(this->trSignalType3 < 0)
        this->trSignalType3 += 2*M_PI;
}

void TRitem::addToTrackPos(float d){
    this->trItemSData1 += d;
}
    
void TRitem::flipTrackPos(float d){
    this->trItemSData1 = d - this->trItemSData1;
}
    
void TRitem::render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot){
    if(this->type == "emptyitem"){
        //qDebug() << "empty";
        return;
    }
    
        if(drawPosition == NULL){
            drawPosition = new float[6];
            int id = tdb->findTrItemNodeId(this->trItemId);
            if(id < 0) {
                //qDebug() << "fail id";
                return;
            }
            tdb->getDrawPositionOnTrNode(drawPosition, id, this->trItemSData1);
            //drawPosition[0] += 2048 * (drawPosition[5] - playerT[0]);
            //drawPosition[2] -= 2048 * (-drawPosition[6] - playerT[1]);
        }

        if( fabs(drawPosition[5] - playerT[0]) + fabs(-drawPosition[6] - playerT[1]) > 2){
            return;
        }
    
        gluu->mvPushMatrix();
        //if(pos == NULL) return;
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 2048*(drawPosition[5] - playerT[0]), drawPosition[1]+2, -drawPosition[2] + 2048*(-drawPosition[6] - playerT[1]));
        Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3]);
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0,0,0);
        }
        pointer3d->render();
        gluu->mvPopMatrix();
}

void TRitem::save(QTextStream* out){
    save(out, false);
}

void TRitem::save(QTextStream* out, bool tit){

int l;
QString tritemsdata = QString::number(this->trItemSData2, 16);
l = tritemsdata.length();
for(int i=0; i<8-l; i++) tritemsdata = "0"+tritemsdata;

QString woff = "";
if(!tit)
    woff = "	";

QString flags;
if(type == "platformitem" || type == "sidingitem"){
    flags = QString::number(this->platformTrItemData[0], 16);
    l = flags.length();
    for(int i=0; i<8-l; i++) flags = "0"+flags;
}
if(type == "signalitem"){
    flags = QString::number(this->trSignalType1, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++) flags = "0"+flags;
}

if(type == "pickupitem"){
    flags = QString::number(this->pickupTrItemData2, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++) flags = "0"+flags;
}

if(type == "crossoveritem")
*(out) << woff+"	CrossoverItem (\n";
if(type == "signalitem")
*(out) << woff+"	SignalItem (\n";
if(type == "soundregionitem")
*(out) << woff+"	SoundRegionItem (\n";
if(type == "levelcritem")
*(out) << woff+"	LevelCrItem (\n";
if(type == "speedpostitem")
*(out) << woff+"	SpeedPostItem (\n";
if(type == "platformitem")
*(out) << woff+"	PlatformItem (\n";
if(type == "sidingitem")
*(out) << woff+"	SidingItem (\n";
if(type == "carspawneritem")
*(out) << woff+"	CarSpawnerItem (\n";
if(type == "emptyitem")
*(out) << woff+"	EmptyItem (\n";
if(type == "hazzarditem")
*(out) << woff+"	HazzardItem (\n";
if(type == "pickupitem")
*(out) << woff+"	PickupItem (\n";

*(out) << woff+"		TrItemId ( "<<this->trItemId<<" )\n";
if(type != "emptyitem")
*(out) << woff+"		TrItemSData ( "<<this->trItemSData1<<" "<< tritemsdata <<" )\n";
if(this->trItemPData != NULL)
*(out) << woff+"		TrItemPData ( "<<this->trItemPData[0]<<" "<< this->trItemPData[1]<<" "<< this->trItemPData[2]<<" "<< this->trItemPData[3]<<" )\n";
if(this->trItemRData != NULL)
*(out) << woff+"		TrItemRData ( "<<this->trItemRData[0]<<" "<< this->trItemRData[1]<<" "<< this->trItemRData[2]<<" "<< this->trItemRData[3]<<" "<< this->trItemRData[4] <<" )\n";

if(type == "crossoveritem" && this->crossoverTrItemData != NULL)
*(out) << woff+"		CrossoverTrItemData ( "<<this->crossoverTrItemData[0]<<" "<< this->crossoverTrItemData[1] <<" )\n";

if(type == "soundregionitem" && this->trItemSRData != NULL)
*(out) << woff+"		TrItemSRData ( "<<this->trItemSRData[0]<<" "<< this->trItemSRData[1] <<" "<< this->trItemSRData[2] <<" )\n";

if(type == "speedpostitem" && this->speedpostTrItemData != NULL){
*(out) << woff+"		SpeedpostTrItemData (";
for(int i = 0; i < speedpostTrItemDataLength; i++)
    *(out) <<" "<<this->speedpostTrItemData[i];
    *(out) <<" )\n";
}
if(type == "sidingitem"){
if(this->platformTrItemData != NULL)
*(out) << woff+"		SidingTrItemData ( "<<flags<<" "<< this->platformTrItemData[1]<<" )\n";
*(out) << woff+"		SidingName ( \""<<this->platformName<<"\" )\n";
}

if(type == "platformitem"){
if(this->platformTrItemData != NULL)
*(out) << woff+"		PlatformTrItemData ( "<<flags<<" "<< this->platformTrItemData[1]<<" )\n";
*(out) << woff+"		PlatformName ( \""<<this->platformName<<"\" )\n";
*(out) << woff+"		Station ( \""<<this->stationName<<"\" )\n";
*(out) << woff+"		PlatformMinWaitingTime ( "<<this->platformMinWaitingTime<<" )\n";
*(out) << woff+"		PlatformNumPassengersWaiting ( "<<this->platformNumPassengersWaiting<<" )\n";
}

if(type == "signalitem"){
*(out) << woff+"		TrSignalType ( "<<flags<<" "<<this->trSignalType2<<" "<<this->trSignalType3<<" "<<this->trSignalType4<<" )\n";
if(this->trSignalDir != NULL){
    *(out) << woff+"		TrSignalDirs ( "<<this->trSignalDirs<<"\n";
    if(!tit){
        for(int i = 0; i < this->trSignalDirs*4; i+=4)
        *(out) << woff+"			TrSignalDir ( "<<this->trSignalDir[i+0]<<" "<< this->trSignalDir[i+1]<<" "<< this->trSignalDir[i+2]<<" "<< this->trSignalDir[i+3]<<" )\n";
    } else {
        for(int i = 0; i < this->trSignalDirs*6; i+=6)
        *(out) << woff+"			TrSignalRDir ( "<<this->trSignalRDir[i+0]<<" "<< this->trSignalRDir[i+1]<<" "<< this->trSignalRDir[i+2]<<" "<< this->trSignalRDir[i+3]<<" "<< this->trSignalRDir[i+4]<<" "<< this->trSignalRDir[i+5]<<" )\n";
    }
    *(out) << woff+"		)\n";
}
}
if(type == "pickupitem"){
*(out) << woff+"		PickupTrItemData ( "<<this->pickupTrItemData1<<" "<<flags<<" )\n";
}

*(out) << woff+"	)\n";
}