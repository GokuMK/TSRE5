/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
#include "TRnode.h"

TrackItemObj* TRitem::pointer3d = NULL;

TRitem* TRitem::newPlatformItem(int trItemId, float metry) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("platformitem")) return NULL;
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

TRitem* TRitem::newSidingItem(int trItemId, float metry) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("sidingitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 2;
    trit->platformTrItemData = new unsigned int[2];
    trit->platformTrItemData[0] = 0;
    trit->platformTrItemData[1] = 0;
    trit->platformName = "new siding";
    return trit;
}

TRitem* TRitem::newCarspawnerItem(int trItemId, float metry) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("carspawneritem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 1;
    trit->platformTrItemData = new unsigned int[2];
    trit->platformTrItemData[0] = 0;
    trit->platformTrItemData[1] = 0;
    return trit;
}

TRitem* TRitem::newPickupItem(int trItemId, float metry) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("pickupitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 6;
    trit->pickupTrItemData1 = 0;
    trit->pickupTrItemData2 = 0;
    return trit;
}

TRitem* TRitem::newLevelCrItem(int trItemId, float metry) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("levelcritem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 6;
    return trit;
}

TRitem* TRitem::newSoundRegionItem(int trItemId, float metry) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("soundregionitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 6;
    trit->trItemSRData = new float[3];
    return trit;
}

TRitem* TRitem::newHazardItem(int trItemId, float metry) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("hazzarditem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 6;
    return trit;
}

TRitem* TRitem::newCrossOverItem(int trItemId, float metry, int trItemId2, int shapeIdx){
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("crossoveritem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 2;
    trit->crossoverTrItemData = new int[2];
    trit->crossoverTrItemData[0] = trItemId2;
    trit->crossoverTrItemData[1] = shapeIdx;
    return trit;
}

TRitem* TRitem::newSignalItem(int trItemId, float metry, int direction, unsigned int flags, QString type) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("signalitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 2;
    trit->platformTrItemData = new unsigned int[2];
    trit->trSignalType1 = flags;
    trit->trSignalType2 = direction;
    trit->trSignalType3 = 0;
    trit->trSignalType4 = type;
    //qDebug() << "aa ";
    return trit;
}

TRitem* TRitem::newSpeedPostItem(int trItemId, float metry, int speedPostType) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("speedpostitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 6;
    trit->platformTrItemData = new unsigned int[2];
    if (speedPostType == TRitem::SIGN) { // sign
        trit->speedpostTrItemDataLength = 3;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000010;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    if (speedPostType == TRitem::RESUME) { // resume
        trit->speedpostTrItemDataLength = 2;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000011;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    if (speedPostType == TRitem::WARNING) { // warning
        trit->speedpostTrItemDataLength = 3;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000001;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    if (speedPostType == TRitem::MILEPOST) { // milepost
        trit->speedpostTrItemDataLength = 3;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000000;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    //qDebug() << "aa ";
    return trit;
}

void TRitem::setSoundRegionRot(float rot) {
    this->trItemSRData[2] = rot;
}

void TRitem::setSoundRegionData(float rot, float ttype, float val) {
    this->trItemSRData[0] = val;
    this->trItemSRData[1] = ttype;
    this->trItemSRData[2] = rot;
}

void TRitem::setPickupContent(float val) {
    this->pickupTrItemData1 = val;
}

void TRitem::enableSignalSubObjFlag(QString sType) {
    qDebug() << sType;
    //this->trSignalType1 = this->trSignalType1 | (1 << (i + 3));
        if(sType == "NUMBER_PLATE")
            trSignalType1 |= 0b0000010000;
        if(sType == "GRADIENT_PLATE")
            trSignalType1 |= 0b0000100000;
        if(sType == "USER1")
            trSignalType1 |= 0b0001000000;
        if(sType == "USER2")
            trSignalType1 |= 0b0010000000;
        if(sType == "USER3")
            trSignalType1 |= 0b0100000000;
        if(sType == "USER4")
            trSignalType1 |= 0b1000000000;
}

void TRitem::disableSignalSubObjFlag(QString sType) {
    //this->trSignalType1 = this->trSignalType1 & ~(1 << (i + 3));
        if(sType == "NUMBER_PLATE")
            trSignalType1 &= ~(0b0000010000);
        if(sType == "GRADIENT_PLATE")
            trSignalType1 &= ~(0b0000100000);
        if(sType == "USER1")
            trSignalType1 &= ~(0b0001000000);
        if(sType == "USER2")
            trSignalType1 &= ~(0b0010000000);
        if(sType == "USER3")
            trSignalType1 &= ~(0b0100000000);
        if(sType == "USER4")
            trSignalType1 &= ~(0b1000000000);
}

TRitem::TRitem() {
    typeObj = this->tritemobj;
}

TRitem::TRitem(int id) {
    typeObj = this->tritemobj;
    trItemId = id;
}

TRitem::TRitem(const TRitem& o) {
    typeObj = o.typeObj;
    type = o.type;
    
    tdbId = o.tdbId;
    trItemId = o.trItemId;
    trItemSData1 = o.trItemSData1;
    trItemSData2 = o.trItemSData2;
    
    trItemPData = o.trItemPData;
    if(trItemPData != NULL){
        trItemPData = new float[4];
        memcpy(trItemPData, o.trItemPData, sizeof(float[4]));
    }
    trItemRData = o.trItemRData;
    if(trItemRData != NULL){
        trItemRData = new float[5];
        memcpy(trItemRData, o.trItemRData, sizeof(float[5]));
    }

    crossoverTrItemData = o.crossoverTrItemData;
    if(crossoverTrItemData != NULL){
        crossoverTrItemData = new int[2];
        memcpy(crossoverTrItemData, o.crossoverTrItemData, sizeof(int[2]));
    }
    platformTrItemData = o.platformTrItemData;
    if(platformTrItemData != NULL){
        platformTrItemData = new unsigned int[2];
        memcpy(platformTrItemData, o.platformTrItemData, sizeof(unsigned int[2]));
    }
    
    platformName = o.platformName;
    stationName = o.stationName;
    platformMinWaitingTime = o.platformMinWaitingTime;
    platformNumPassengersWaiting = o.platformNumPassengersWaiting;
    
    trItemSRData = o.trItemSRData;
    if(trItemSRData != NULL){
        trItemSRData = new float[3];
        memcpy(trItemSRData, o.trItemSRData, sizeof(float[3]));
    }
    speedpostTrItemData = o.speedpostTrItemData;
    if(speedpostTrItemData != NULL){
        speedpostTrItemData = new float[4];
        memcpy(speedpostTrItemData, o.speedpostTrItemData, sizeof(float[4]));
    }
    
    trSignalType1 = o.trSignalType1;
    trSignalType2 = o.trSignalType2;
    trSignalType3 = o.trSignalType3;
    trSignalType4 = o.trSignalType4;
    trSignalDirs = o.trSignalDirs;
        
    trSignalDir = o.trSignalDir;
    if(trSignalDir != NULL){
        trSignalDir = new int[trSignalDirs * 4];
        memcpy(trSignalDir, o.trSignalDir, sizeof(int[trSignalDirs * 4]));
    }
    trSignalRDir = o.trSignalRDir;
    if(trSignalRDir != NULL){
        trSignalRDir = new float[trSignalDirs * 6];
        memcpy(trSignalRDir, o.trSignalRDir, sizeof(float[trSignalDirs * 6]));
    }
    
    titLoading = o.titLoading;
    pickupTrItemData1 = o.pickupTrItemData1;
    pickupTrItemData2 = o.pickupTrItemData2;
    
    speedpostTrItemDataLength = o.speedpostTrItemDataLength;
    
}

TRitem::~TRitem() {
    if(trItemPData != NULL)
        delete[] trItemPData;
    if(trItemRData != NULL)
        delete[] trItemRData;
    if(crossoverTrItemData != NULL)
        delete[] crossoverTrItemData;
    if(platformTrItemData != NULL)
        delete[] platformTrItemData;
    if(trItemSRData != NULL)
        delete[] trItemSRData;
    if(speedpostTrItemData != NULL)
        delete[] speedpostTrItemData;
    if(trSignalDir != NULL)
        delete[] trSignalDir;
    if(trSignalRDir != NULL)
        delete[] trSignalRDir;
}

bool TRitem::init(QString sh) {
    type = sh;
    trItemPData = NULL;
    trItemRData = NULL;
    crossoverTrItemData = NULL;
    platformTrItemData = NULL;
    trItemSRData = NULL;
    speedpostTrItemData = NULL;
    trSignalDir = NULL;
    //pickupTrItemData = NULL;

    if (sh == "crossoveritem") return true;
    if (sh == "signalitem") return true;
    if (sh == "soundregionitem") return true;
    if (sh == "levelcritem") return true;
    if (sh == "speedpostitem") return true;
    if (sh == "platformitem") return true;
    if (sh == "sidingitem") return true;
    if (sh == "carspawneritem") return true;
    if (sh == "emptyitem") return true;
    if (sh == "hazzarditem") return true;
    if (sh == "pickupitem") return true;
    return false;
}

void TRitem::set(QString sh, FileBuffer* data) {
    //qDebug() << "---"<< sh;
    if (sh == ("tritemid")) {
        trItemId = ParserX::GetUInt(data);
        return;
    }
    if (sh == ("tritemsdata")) {
        trItemSData1 = ParserX::GetNumber(data);
        trItemSData2 = ParserX::GetHex(data);
        return;
    }
    if (sh == ("tritempdata")) {
        trItemPData = new float[4];
        trItemPData[0] = ParserX::GetNumber(data);
        trItemPData[1] = ParserX::GetNumber(data);
        trItemPData[2] = ParserX::GetNumber(data);
        trItemPData[3] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("tritemrdata")) {
        trItemRData = new float[5];
        trItemRData[0] = ParserX::GetNumber(data);
        trItemRData[1] = ParserX::GetNumber(data);
        trItemRData[2] = ParserX::GetNumber(data);
        trItemRData[3] = ParserX::GetNumber(data);
        trItemRData[4] = ParserX::GetNumber(data);
        return;
    }

    // crossover
    if (sh == ("crossovertritemdata")) {
        crossoverTrItemData = new int[2];
        crossoverTrItemData[0] = (int) ParserX::GetNumber(data);
        crossoverTrItemData[1] = (int) ParserX::GetNumber(data);
        return;
    }

    // platform / siding
    if (sh == ("platformtritemdata") || sh == ("sidingtritemdata")) {
        platformTrItemData = new unsigned int[2];
        platformTrItemData[0] = ParserX::GetHex(data);
        platformTrItemData[1] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("platformname") || sh == ("sidingname")) {
        platformName = ParserX::GetString(data);
        return;
    }
    if (sh == ("station")) {
        stationName = ParserX::GetString(data);
        return;
    }
    if (sh == ("platformminwaitingtime")) {
        platformMinWaitingTime = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("platformnumpassengerswaiting")) {
        platformNumPassengersWaiting = ParserX::GetNumber(data);
        return;
    }

    // soundregion
    if (sh == ("tritemsrdata")) {
        trItemSRData = new float[3];
        trItemSRData[0] = ParserX::GetNumber(data);
        trItemSRData[1] = ParserX::GetNumber(data);
        trItemSRData[2] = ParserX::GetNumber(data);
        return;
    }

    // speed
    if (sh == ("speedposttritemdata")) {
        speedpostTrItemData = new float[4];
        bool ok = false;
        speedpostTrItemDataLength = 0;
        speedpostTrItemData[0] = ParserX::GetNumberInside(data, &ok);
        /*
        9 - speed or dot
        8 - mph
        7 - both
        6 - tow
        5 - pas
        4 - number instead speed
        3 - speed instead number
        2 - ?
        0, 1 - speed, warning, resume
        00 - spost; 2 - 3;
        01 - warning 2 - 3;
        10 - speed; 2 - 3
        11 - resume; 1 - 2*/
        if (!ok) return;
        speedpostTrItemDataLength++;
        speedpostTrItemData[1] = ParserX::GetNumberInside(data, &ok);
        if (!ok) return;
        speedpostTrItemDataLength++;
        speedpostTrItemData[2] = ParserX::GetNumberInside(data, &ok);
        if (!ok) return;
        speedpostTrItemDataLength++;
        speedpostTrItemData[3] = ParserX::GetNumberInside(data, &ok);
        if (!ok) return;
        speedpostTrItemDataLength++;
        return;
    }

    // signal
    if (sh == ("trsignaltype")) {
        trSignalType1 = ParserX::GetHex(data);
        //if((trSignalType1 & 6) != 0)
        //    qDebug() << "tst "<< this->trItemId << " "<<trSignalType1;
        trSignalType2 = ParserX::GetNumber(data);
        trSignalType3 = ParserX::GetNumber(data);
        trSignalType4 = ParserX::GetString(data);
        return;
    }

    if (sh == ("trsignaldirs")) {
        trSignalDirs = ParserX::GetNumber(data);
        if (!titLoading) {
            trSignalDir = new int[trSignalDirs * 4];
            for (int i = 0; i < trSignalDirs * 4; i += 4) {
                trSignalDir[i + 0] = ParserX::GetNumber(data);
                trSignalDir[i + 1] = ParserX::GetNumber(data);
                trSignalDir[i + 2] = ParserX::GetNumber(data);
                trSignalDir[i + 3] = ParserX::GetNumber(data);
            }
        } else {
            trSignalRDir = new float[trSignalDirs * 6];
            for (int i = 0; i < trSignalDirs * 6; i += 6) {
                trSignalRDir[i + 0] = ParserX::GetNumber(data);
                trSignalRDir[i + 1] = ParserX::GetNumber(data);
                trSignalRDir[i + 2] = ParserX::GetNumber(data);
                trSignalRDir[i + 3] = ParserX::GetNumber(data);
                trSignalRDir[i + 4] = ParserX::GetNumber(data);
                trSignalRDir[i + 5] = ParserX::GetNumber(data);
            }
        }
        ParserX::SkipToken(data);
        return;
    }

    // pickup
    if (sh == ("pickuptritemdata")) {
        pickupTrItemData1 = ParserX::GetNumber(data);
        pickupTrItemData2 = ParserX::GetHex(data);
        return;
    }

    qDebug() << "=" << sh;
    return;
}

void TRitem::setTrItemRData(float* posT, float* pos) {
    if (this->trItemRData == NULL)
        this->trItemRData = new float[5];
    this->trItemRData[0] = pos[0];
    this->trItemRData[1] = pos[1];
    this->trItemRData[2] = pos[2];
    this->trItemRData[3] = posT[0];
    this->trItemRData[4] = posT[1];
}

void TRitem::setTrItemPData(float* posT, float* pos) {
    if (this->trItemPData == NULL)
        this->trItemPData = new float[4];
    this->trItemPData[0] = pos[0];
    this->trItemPData[1] = pos[2];
    this->trItemPData[2] = posT[0];
    this->trItemPData[3] = posT[1];
}

void TRitem::setSignalRot(float rot) {
    this->trSignalType3 = rot;
}

void TRitem::setSignalDirection(int dir) {
    this->trSignalType2 = dir;
}

/*void TRitem::linkSignal(int from, int to) {
    TDB* tdb = Game::trackDB;
    TRnode* n = tdb->trackNodes[from];
    if(n == NULL) return;
    if(n->typ != 2) {
        qDebug() << "from is no junction";
        return;
    }
    // find to;
    int direction;
    TRnode* n1 = tdb->trackNodes[n->TrPinS[1]];
    TRnode* n2 = tdb->trackNodes[n->TrPinS[2]];
    TRnode* nd;
    if(n1 == NULL) return;
    if(n2 == NULL) return;
    if(n1->TrPinS[0] == to || n1->TrPinS[1] == to){
        direction = 0;
        nd = n1;
    }
    else if(n2->TrPinS[0] == to || n2->TrPinS[1] == to){
        direction = 1;
        nd = n2;
    }
    else {
        qDebug() << "no route from to";
        return;
    }
    trSignalDirs = 1;
    trSignalDir = new int[trSignalDirs * 4];
    trSignalDir[0 + 0] = from;
    trSignalDir[0 + 1] = 1;
    trSignalDir[0 + 2] = direction;
    trSignalDir[0 + 3] = 0;

    // calc position n - nd;
    
    trSignalRDir = new float[trSignalDirs * 6];
    trSignalRDir[0 + 0] = n->UiD[6];
    trSignalRDir[0 + 1] = n->UiD[7];
    trSignalRDir[0 + 2] = n->UiD[8];
    trSignalRDir[0 + 3] = n->UiD[4];
    trSignalRDir[0 + 4] = n->UiD[5];
    trSignalRDir[0 + 5] = n->UiD[10];
}*/

void TRitem::linkSignal(int trackId, int dist) {
    TDB* tdb = Game::trackDB;
    TRnode* n = tdb->trackNodes[trackId];
    if(n == NULL) return;
    if(n->typ != 1) {
        qDebug() << "link is no track";
        return;
    }
    float length = tdb->getVectorSectionLength(trackId);
    int end = 0;
    if(dist*2 > length)
        end = 1;
    int jid = n->TrPinS[end];
    n = tdb->trackNodes[jid];
    if(n->typ != 2 ) {
        qDebug() << "link is no junction";
        return;
    }
    // find to;
    int direction;

    TRnode* n1 = tdb->trackNodes[n->TrPinS[1]];
    TRnode* n2 = tdb->trackNodes[n->TrPinS[2]];

    if(n1 == NULL) return;
    if(n2 == NULL) return;
    if(n->TrPinS[1] == trackId){
        direction = 0;
    }
    else if(n->TrPinS[2] == trackId){
        direction = 1;
    }
    else {
        qDebug() << "no route from to";
        return;
    }
    trSignalDirs = 1;
    trSignalDir = new int[trSignalDirs * 4];
    trSignalDir[0 + 0] = jid;
    trSignalDir[0 + 1] = 1;
    trSignalDir[0 + 2] = direction;
    trSignalDir[0 + 3] = 0;

    // calc position n - nd;
    
    trSignalRDir = new float[trSignalDirs * 6];
    trSignalRDir[0 + 0] = n->UiD[6];
    trSignalRDir[0 + 1] = n->UiD[7];
    trSignalRDir[0 + 2] = n->UiD[8];
    trSignalRDir[0 + 3] = n->UiD[4];
    trSignalRDir[0 + 4] = n->UiD[5];
    trSignalRDir[0 + 5] = n->UiD[10];
}

void TRitem::setSpeedpostRot(float rot) {
    if(this->speedpostTrItemData == NULL)
        return;
    this->speedpostTrItemData[this->speedpostTrItemDataLength - 1] = rot;
}

float TRitem::getSpeedpostRot() {
    if(this->speedpostTrItemData == NULL)
        return 0;
    return this->speedpostTrItemData[this->speedpostTrItemDataLength - 1];
}

QString TRitem::getTrackItemName(){
    if(platformName.length() > 0)
        return platformName;
    
    return type;
}

void TRitem::flipSignal() {
    this->trSignalType2 = abs(this->trSignalType2 - 1);
    this->trSignalType3 += M_PI;
    if (this->trSignalType3 > 2 * M_PI)
        this->trSignalType3 -= 2 * M_PI;
    if (this->trSignalType3 < 0)
        this->trSignalType3 += 2 * M_PI;
}

void TRitem::flipSpeedpost() {
    //this->trSignalType2 = abs(this->trSignalType2-1);
    this->speedpostTrItemData[this->speedpostTrItemDataLength - 1] += M_PI;
    if (this->speedpostTrItemData[this->speedpostTrItemDataLength - 1] > 2 * M_PI)
        this->speedpostTrItemData[this->speedpostTrItemDataLength - 1] -= 2 * M_PI;
    if (this->speedpostTrItemData[this->speedpostTrItemDataLength - 1] < 0)
        this->speedpostTrItemData[this->speedpostTrItemDataLength - 1] += 2 * M_PI;
}

void TRitem::flipSoundRegion(){
    this->trItemSRData[2] += M_PI;
    if (this->trItemSRData[2] > 2 * M_PI)
        this->trItemSRData[2] -= 2 * M_PI;
    if (this->trItemSRData[2] < 0)
        this->trItemSRData[2] += 2 * M_PI;
}



void TRitem::setSpeedpostSpeed(float val) {
    SType stype = getSpeedpostType();
    if (stype == TRitem::RESUME) {
        return;
    }
    if (stype == TRitem::MILEPOST) {
        if(((int)this->speedpostTrItemData[0] >> 3) & 1)
            this->speedpostTrItemData[1] = val;
        return;
    }
    if (stype == TRitem::WARNING || stype == TRitem::SIGN) {
        this->speedpostTrItemData[1] = val;
        return;
    }
}

float TRitem::getSpeedpostSpeed(){
    SType stype = getSpeedpostType();
    if (stype == TRitem::RESUME) {
        return 0;
    }
    if (stype == TRitem::WARNING || stype == TRitem::SIGN) {
        return this->speedpostTrItemData[1];
    }
    if (stype == TRitem::MILEPOST) {
        if(((int)this->speedpostTrItemData[0] >> 3) & 1)
            return this->speedpostTrItemData[1];
    }
    return 0;
}

void TRitem::setSpeedpostSpeedInsteadNumber(bool val){
    if(val){
        if((((int)this->speedpostTrItemData[0] >> 3) & 1) == 1)
            return;
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] | (1 << 3);
        this->speedpostTrItemData[3] = this->speedpostTrItemData[2];
        this->speedpostTrItemData[2] = this->speedpostTrItemData[1];
        this->speedpostTrItemData[1] = 0;
        this->speedpostTrItemDataLength++;
    } else {
        if((((int)this->speedpostTrItemData[0] >> 3) & 1) == 0)
            return;
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] & ~(1 << 3);
        this->speedpostTrItemData[1] = this->speedpostTrItemData[2];
        this->speedpostTrItemData[2] = this->speedpostTrItemData[3];
        this->speedpostTrItemDataLength--;
    }
}

bool TRitem::getSpeedpostSpeedInsteadNumber(){
    return ((((int)this->speedpostTrItemData[0] >> 3) & 1) == 1);
}

bool TRitem::getSpeedpostNumberInsteadSpeed(){
    return ((((int)this->speedpostTrItemData[0] >> 4) & 1) == 1);
}
    
void TRitem::setSpeedpostNumberInsteadSpeed(bool val){
    if(val){
        if((((int)speedpostTrItemData[0] >> 4) & 1) == 1)
            return;
        speedpostTrItemData[0] = (int)speedpostTrItemData[0] | (1 << 4);
        speedpostTrItemData[speedpostTrItemDataLength] = speedpostTrItemData[speedpostTrItemDataLength-1];
        speedpostTrItemData[speedpostTrItemDataLength-1] = 0;
        speedpostTrItemDataLength++;
    } else {
        if((((int)speedpostTrItemData[0] >> 4) & 1) == 0)
            return;
        speedpostTrItemData[0] = (int)speedpostTrItemData[0] & ~(1 << 4);
        speedpostTrItemData[speedpostTrItemDataLength-2] = speedpostTrItemData[speedpostTrItemDataLength-1];
        speedpostTrItemDataLength--;
    }
}
    
float TRitem::getSpeedpostNumber(){
    SType stype = getSpeedpostType();
    if (stype == TRitem::MILEPOST) {
        return this->speedpostTrItemData[speedpostTrItemDataLength - 2];
    }
    if (stype == TRitem::WARNING || stype == TRitem::SIGN || stype == TRitem::RESUME) {
        if(((int)this->speedpostTrItemData[0] >> 4) & 1)
            return this->speedpostTrItemData[speedpostTrItemDataLength - 2];
    }
    return 0;    
}
    
void TRitem::setSpeedpostNumber(float val) {
    SType stype = getSpeedpostType();

    if (stype == TRitem::MILEPOST) {
        this->speedpostTrItemData[speedpostTrItemDataLength - 2] = val;
        return;
    }
    if (stype == TRitem::WARNING || stype == TRitem::SIGN || stype == TRitem::RESUME) {
        if(((int)this->speedpostTrItemData[0] >> 4) & 1)
            this->speedpostTrItemData[speedpostTrItemDataLength - 2] = val;
        return;
    }
}

TRitem::SType TRitem::getSpeedpostType() {
    return (TRitem::SType)((int) (this->speedpostTrItemData[0]) & 0b11);
}

void TRitem::setSpeedPostSpeedUnitId(int val){
    if(val == 1)
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] | (1 << 8);
    if(val == 0)
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] & ~(1 << 8);
}

int TRitem::getSpeedPostSpeedUnitId(){
    return (((int)this->speedpostTrItemData[0] >> 8) & 1);
}

int TRitem::getSpeedPostTrainType(){
    if(((int)this->speedpostTrItemData[0] >> 7) & 1)
        return 2;
    if(((int)this->speedpostTrItemData[0] >> 6) & 1)
        return 1;
    if(((int)this->speedpostTrItemData[0] >> 5) & 1)
        return 0;
    return 0;
}

void TRitem::setSpeedPostTrainType(int val){
    this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] & ~(1 << 7);
    this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] & ~(1 << 6);
    this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] & ~(1 << 5);
    if(val == 0)
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] | (1 << 5);
    if(val == 1)
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] | (1 << 6);
    if(val == 2)
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] | (1 << 7);
}

bool TRitem::getSpeedPostNumberDot(){
    if(((int)this->speedpostTrItemData[0] >> 9) & 1)
        return true;
    return false;
}

void TRitem::setSpeedPostNumberDot(bool val){
    if(val)
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] | (1 << 9);
    else
        this->speedpostTrItemData[0] = (int)this->speedpostTrItemData[0] & ~(1 << 9);
}

void TRitem::addToTrackPos(float d) {
    this->trItemSData1 += d;
}

void TRitem::flipTrackPos(float d) {
    this->trItemSData1 = d - this->trItemSData1;
}

float TRitem::getTrackPosition(){
    return trItemSData1;
}

void TRitem::setTrackPosition(float val){
    trItemSData1 = val;
    refresh();
}

void TRitem::trackPositionAdd(float val){
    trItemSData1 += val;
    refresh();
}

void TRitem::refresh(){
    delete[] drawPosition;
    drawPosition = NULL;
}

void TRitem::render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot, int selectionColor) {
    if (this->type == "emptyitem") {
        //qDebug() << "empty";
        return;
    }
    if (this->type == "") {
        //qDebug() << "undefined";
        return;
    }
    int offy = 0;
    if (tdb->isRoad()) offy++;

    if (drawPosition == NULL) {
        drawPosition = new float[7];
        int id = tdb->findTrItemNodeId(this->trItemId);
        if (id < 0) {
            //qDebug() << "fail id";
            return;
        }
        tdb->getDrawPositionOnTrNode(drawPosition, id, this->trItemSData1);
        //drawPosition[0] += 2048 * (drawPosition[5] - playerT[0]);
        //drawPosition[2] -= 2048 * (-drawPosition[6] - playerT[1]);
    }

    if (fabs(drawPosition[5] - playerT[0]) + fabs(-drawPosition[6] - playerT[1]) > 2) {
        return;
    }

    gluu->mvPushMatrix();
    //if(pos == NULL) return;
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 2048 * (drawPosition[5] - playerT[0]), drawPosition[1] + 2 + offy, -drawPosition[2] + 2048 * (-drawPosition[6] - playerT[1]));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3]);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    if (pointer3d == NULL) {
        pointer3d = new TrackItemObj(1);
    }
    
    if (tdb->isRoad())
        if(!selected)
            pointer3d->setMaterial(0.5, 0.5, 0.5);
        else
            pointer3d->setMaterial(0.7, 0.7, 0.7);
    else
        if(!selected)
            pointer3d->setMaterial(0.0, 0.0, 0.0);
        else
            pointer3d->setMaterial(0.2, 0.2, 0.2);
    if(selectionColor != 0)
        selectionColor |= trItemId;
    pointer3d->render(selectionColor);
    gluu->mvPopMatrix();
}

void TRitem::save(QTextStream* out) {
    save(out, false);
}

void TRitem::save(QTextStream* out, bool tit) {

    int l;
    QString tritemsdata = QString::number(this->trItemSData2, 16);
    l = tritemsdata.length();
    for (int i = 0; i < 8 - l; i++) tritemsdata = "0" + tritemsdata;

    QString woff = "";
    if (!tit)
        woff = "	";

    QString flags;
    if (type == "platformitem" || type == "sidingitem") {
        flags = ParserX::MakeFlagsString(this->platformTrItemData[0]);
    }
    if (type == "signalitem") {
        flags = ParserX::MakeFlagsString(this->trSignalType1);
    }

    if (type == "pickupitem") {
        flags = ParserX::MakeFlagsString(this->pickupTrItemData2);
    }

    if (type == "crossoveritem")
        *(out) << woff + "	CrossoverItem (\n";
    if (type == "signalitem")
        *(out) << woff + "	SignalItem (\n";
    if (type == "soundregionitem")
        *(out) << woff + "	SoundRegionItem (\n";
    if (type == "levelcritem")
        *(out) << woff + "	LevelCrItem (\n";
    if (type == "speedpostitem")
        *(out) << woff + "	SpeedPostItem (\n";
    if (type == "platformitem")
        *(out) << woff + "	PlatformItem (\n";
    if (type == "sidingitem")
        *(out) << woff + "	SidingItem (\n";
    if (type == "carspawneritem")
        *(out) << woff + "	CarSpawnerItem (\n";
    if (type == "emptyitem")
        *(out) << woff + "	EmptyItem (\n";
    if (type == "hazzarditem")
        *(out) << woff + "	HazzardItem (\n";
    if (type == "pickupitem")
        *(out) << woff + "	PickupItem (\n";

    *(out) << woff + "		TrItemId ( " << this->trItemId << " )\n";
    if (type != "emptyitem"){
        *(out) << woff + "		TrItemSData ( " << this->trItemSData1 << " " << tritemsdata << " )\n";
    if (this->trItemPData != NULL)
        *(out) << woff + "		TrItemPData ( " << this->trItemPData[0] << " " << this->trItemPData[1] << " " << this->trItemPData[2] << " " << this->trItemPData[3] << " )\n";
    if (this->trItemRData != NULL)
        *(out) << woff + "		TrItemRData ( " << this->trItemRData[0] << " " << this->trItemRData[1] << " " << this->trItemRData[2] << " " << this->trItemRData[3] << " " << this->trItemRData[4] << " )\n";
    }
    if (type == "crossoveritem" && this->crossoverTrItemData != NULL)
        *(out) << woff + "		CrossoverTrItemData ( " << this->crossoverTrItemData[0] << " " << this->crossoverTrItemData[1] << " )\n";

    if (type == "soundregionitem" && this->trItemSRData != NULL)
        *(out) << woff + "		TrItemSRData ( " << this->trItemSRData[0] << " " << this->trItemSRData[1] << " " << this->trItemSRData[2] << " )\n";

    if (type == "speedpostitem" && this->speedpostTrItemData != NULL) {
        *(out) << woff + "		SpeedpostTrItemData (";
        for (int i = 0; i < speedpostTrItemDataLength; i++)
            *(out) << " " << this->speedpostTrItemData[i];
        *(out) << " )\n";
    }
    if (type == "sidingitem") {
        if (this->platformTrItemData != NULL)
            *(out) << woff + "		SidingTrItemData ( " << flags << " " << this->platformTrItemData[1] << " )\n";
        *(out) << woff + "		SidingName ( \"" << this->platformName << "\" )\n";
    }

    if (type == "platformitem") {
        if (this->platformTrItemData != NULL)
            *(out) << woff + "		PlatformTrItemData ( " << flags << " " << this->platformTrItemData[1] << " )\n";
        *(out) << woff + "		PlatformName ( \"" << this->platformName << "\" )\n";
        *(out) << woff + "		Station ( \"" << this->stationName << "\" )\n";
        *(out) << woff + "		PlatformMinWaitingTime ( " << this->platformMinWaitingTime << " )\n";
        *(out) << woff + "		PlatformNumPassengersWaiting ( " << this->platformNumPassengersWaiting << " )\n";
    }

    if (type == "signalitem") {
        *(out) << woff + "		TrSignalType ( " << flags << " " << this->trSignalType2 << " " << this->trSignalType3 << " " << this->trSignalType4 << " )\n";
        if (this->trSignalDir != NULL) {
            *(out) << woff + "		TrSignalDirs ( " << this->trSignalDirs << "\n";
            if (!tit) {
                for (int i = 0; i < this->trSignalDirs * 4; i += 4)
                    *(out) << woff + "			TrSignalDir ( " << this->trSignalDir[i + 0] << " " << this->trSignalDir[i + 1] << " " << this->trSignalDir[i + 2] << " " << this->trSignalDir[i + 3] << " )\n";
            } else {
                for (int i = 0; i < this->trSignalDirs * 6; i += 6)
                    *(out) << woff + "			TrSignalRDir ( " << this->trSignalRDir[i + 0] << " " << this->trSignalRDir[i + 1] << " " << this->trSignalRDir[i + 2] << " " << this->trSignalRDir[i + 3] << " " << this->trSignalRDir[i + 4] << " " << this->trSignalRDir[i + 5] << " )\n";
            }
            *(out) << woff + "		)\n";
        }
    }
    if (type == "pickupitem") {
        *(out) << woff + "		PickupTrItemData ( " << this->pickupTrItemData1 << " " << flags << " )\n";
    }

    *(out) << woff + "	)\n";
}

QString TRitem::speedpostTypeName(SType val){
    if(val == MILEPOST)
        return QString("milepost");
    if(val == WARNING)
        return QString("warning");
    if(val == SIGN)
        return QString("speedsign");
    if(val == RESUME)
        return QString("resume");
    return "";
}

void TRitem::addPositionOffset(float offsetXYZ[]){
    int x, z;
    //float pos[3];
    if(this->trItemRData != NULL){
        trItemRData[0] += offsetXYZ[0];
        trItemRData[1] += offsetXYZ[1];
        trItemRData[2] += offsetXYZ[2];
        x = trItemRData[3];
        z = trItemRData[4];
        while(trItemRData[0] > 1024 || trItemRData[0] < -1024 || trItemRData[2] > 1024 || trItemRData[2] < -1024 ){
            Game::check_coords(x, z, trItemRData);
        }
        trItemRData[3] = x;
        trItemRData[4] = z;
    }
    if(this->trItemPData != NULL){
        trItemPData[0] += offsetXYZ[0];
        trItemPData[1] += offsetXYZ[2];
        x = trItemPData[2];
        z = trItemPData[3];
        while(trItemPData[0] > 1024 || trItemPData[0] < -1024 || trItemPData[1] > 1024 || trItemPData[1] < -1024 ){
            Game::check_coords(x, z, trItemPData[0], trItemPData[1]);
        }
        trItemPData[2] = x;
        trItemPData[3] = z;
    }
    if(this->trSignalRDir != NULL){
        trSignalRDir[0] += offsetXYZ[0];
        trSignalRDir[1] += offsetXYZ[1];
        trSignalRDir[2] += offsetXYZ[2];
        x = trSignalRDir[3];
        z = trSignalRDir[4];
        while(trSignalRDir[0] > 1024 || trSignalRDir[0] < -1024 || trSignalRDir[2] > 1024 || trSignalRDir[2] < -1024 ){
            Game::check_coords(x, z, trSignalRDir);
        }
        trSignalRDir[3] = x;
        trSignalRDir[4] = z;
    }
}

void TRitem::addTrackNodeItemOffset(unsigned int trackNodeOffset, unsigned int trackItemOffset){
    this->trItemSData2 += trackNodeOffset;
    this->trItemId += trackItemOffset;
    /*if(trItemRef == NULL)
        return;
    for(int i = 0; i < this->iTri; i++)
        trItemRef[i] += trackItemOffset;
    
    for(int i = 0; i < this->TrP1+this->TrP2; i++)
        this->TrPinS[i] += trackNodeOffset;*/
    if(this->trSignalDirs == 1){
        if(this->trSignalDir != NULL){
            this->trSignalDir[0] += trackNodeOffset;
        }
    }
    if(this->platformTrItemData != NULL)
        this->platformTrItemData[1] += trackItemOffset;
    if(this->crossoverTrItemData != NULL){
        this->crossoverTrItemData[0] += trackItemOffset;
    }
}