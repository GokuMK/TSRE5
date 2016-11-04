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

TRitem* TRitem::newSignalItem(int trItemId, float metry, unsigned int flags, QString type) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("signalitem")) return NULL;
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

TRitem* TRitem::newSpeedPostItem(int trItemId, float metry, int speedPostType) {
    TRitem* trit = new TRitem(trItemId);
    if (!trit->init("speedpostitem")) return NULL;
    trit->trItemSData1 = metry;
    trit->trItemSData2 = 6;
    trit->platformTrItemData = new unsigned int[2];
    if (speedPostType == 0) { // sign
        trit->speedpostTrItemDataLength = 3;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000010;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    if (speedPostType == 1) { // resume
        trit->speedpostTrItemDataLength = 2;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000011;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    if (speedPostType == 2) { // warning
        trit->speedpostTrItemDataLength = 3;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000001;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    if (speedPostType == 3) { // milepost
        trit->speedpostTrItemDataLength = 3;
        trit->speedpostTrItemData = new float[4];
        trit->speedpostTrItemData[0] = 0b10000000;
        trit->speedpostTrItemData[1] = 0;
        trit->speedpostTrItemData[2] = 0;
        trit->speedpostTrItemData[3] = 0;
    }
    qDebug() << "aa ";
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

void TRitem::enableSignalSubObjFlag(int i) {
    this->trSignalType1 = this->trSignalType1 | (1 << (i + 3));
}

void TRitem::disableSignalSubObjFlag(int i) {
    this->trSignalType1 = this->trSignalType1 ^ (1 << (i + 3));
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
        10 - speed or dot
        9 - mph
        8 - both
        7 - tow
        6 - pas
        5 - number instead speed
        4 - set speed
        3 - ?
        1, 2 - speed, warning, resume
        00 - spost; 2 - 3;
        01 - warning 2 - 3;
        10 - speed; 2 - 3
        11 - resume; 1 - 2*/
        //if(speedpostTrItemData[0] !=384)qDebug() << ":sp " << speedpostTrItemData[0];
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

void TRitem::linkSignal(int from, int to) {
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
}

void TRitem::setSpeedpostRot(float rot) {
    this->speedpostTrItemData[this->speedpostTrItemDataLength - 1] = rot;
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

void TRitem::setSpeedpostSpeed(float val) {
    SType stype = getSpeedpostType();
    if (stype == TRitem::RESUME) {
        return;
    }
    if (stype == TRitem::MILEPOST) {
        return;
    }
    if (stype == TRitem::WARNING || stype == TRitem::SIGN) {
        this->speedpostTrItemData[1] = val;
        return;
    }
}

void TRitem::setSpeedpostNum(float val) {
    SType stype = getSpeedpostType();
    if (stype == TRitem::RESUME) {
        return;
    }
    if (stype == TRitem::MILEPOST) {
        this->speedpostTrItemData[1] = val;
        return;
    }
    if (stype == TRitem::WARNING || stype == TRitem::SIGN) {

        //this->speedpostTrItemData[1] = val;
        return;
    }
}

TRitem::SType TRitem::getSpeedpostType() {
    return (TRitem::SType)((int) (this->speedpostTrItemData[0]) & 0b11);
}

void TRitem::addToTrackPos(float d) {
    this->trItemSData1 += d;
}

void TRitem::flipTrackPos(float d) {
    this->trItemSData1 = d - this->trItemSData1;
}

void TRitem::render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot) {
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
        pointer3d->setMaterial(0.5, 0.5, 0.5);
    else
        pointer3d->setMaterial(0, 0, 0);
    pointer3d->render();
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