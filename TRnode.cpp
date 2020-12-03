/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TRnode.h"
#include "GLMatrix.h"
#include <math.h>
#include "Game.h"
#include <QString>
#include <QDebug>

TRnode::TRnode() {
    typ = -1;
    TrP1 = 0;
    TrP1 = 0;
    iTri = 0;
    trItemRef = nullptr;
    args[0] = args[1] = args[2] = 0;
    TrPinS[0] = TrPinS[1] = TrPinS[2] = 0;
    TrPinK[0] = TrPinK[1] = TrPinK[2] = 0;
}

TRnode::TRnode(const TRnode& o) {
    typ = o.typ;
    memcpy(args, o.args, sizeof(int[3]));
    memcpy(UiD, o.UiD, sizeof(float[12]));
    iTrv = o.iTrv;
    if(iTrv > 0){
        trVectorSection = new TRSect[iTrv];
        for(int i = 0; i < iTrv; i++){
            memcpy(trVectorSection[i].param, o.trVectorSection[i].param, sizeof(float[16]));
        }
    }
    iTri = o.iTri;
    if(iTri > 0){
        trItemRef = new int[iTri];
        memcpy(trItemRef, o.trItemRef, sizeof(int[iTri]));
    }
    TrP1 = o.TrP1;
    TrP2 = o.TrP2;
    memcpy(TrPinS, o.TrPinS, sizeof(int[3]));
    memcpy(TrPinK, o.TrPinK, sizeof(int[3]));
}

TRnode::~TRnode() {
    if(trVectorSection != NULL)
        delete[] trVectorSection;
        
    if(trItemRef != NULL)
        delete[] trItemRef;
}

Vector2i* TRnode::getTile() {
    if (typ == 1) return new Vector2i((int) trVectorSection[0].param[8], (int) trVectorSection[0].param[9]);
    else return new Vector2i((int) UiD[4], -(int) UiD[5]);
}

bool TRnode::isEnd() {
    if (typ == 1) return false;
    return true;
}

bool TRnode::equals(TRnode* r) {
    if (typ != r->typ)
        return false;
    if (typ == 0) {
        if (UiD[4] != r->UiD[4])
            return false;
        if (UiD[5] != r->UiD[5])
            return false;
        float len[3];
        len[0] = UiD[6] - r->UiD[6];
        len[1] = UiD[7] - r->UiD[7];
        len[2] = UiD[8] - r->UiD[8];
        
        if (fabs(Vec3::length(len)) < 0.17)
            return true;
    }
    return false;
}

bool TRnode::equalsIgnoreType(TRnode* r) {
    if (typ == 1)
        return false;
    else {
        if (UiD[4] != r->UiD[4])
            return false;
        if (UiD[5] != r->UiD[5])
            return false;
        float len[3];
        len[0] = UiD[6] - r->UiD[6];
        len[1] = UiD[7] - r->UiD[7];
        len[2] = UiD[8] - r->UiD[8];
        
        if (fabs(Vec3::length(len)) < 0.17)
            return true;
    }
    return false;
}

int TRnode::podmienTrPin(int stare, int nowe) {
        for(int j = 0; j < 3; j++)
            if(TrPinS[j] == stare){
                TrPinS[j] = nowe;
                return j;
            }
        return 0;
    }

int TRnode::setTrPinK(int id, int nowe) {
        for(int j = 0; j < 3; j++)
            if(TrPinS[j] == id){
                TrPinK[j] = nowe;
                return j;
            }
        return 0;
    }

float TRnode::getVectorSectionXRot(int id){
    if(id >= iTrv - 1)
        return trVectorSection[id].param[13];
    float pos1[3], pos2[3];
    pos1[0] = trVectorSection[id].param[10];
    pos1[1] = trVectorSection[id].param[11];
    pos1[2] = trVectorSection[id].param[12];
    pos2[0] = trVectorSection[id+1].param[10];
    pos2[1] = trVectorSection[id+1].param[11];
    pos2[2] = trVectorSection[id+1].param[12];
    
    pos2[0] += 2048*(trVectorSection[id+1].param[8]-trVectorSection[id].param[8]);
    pos2[2] += 2048*(trVectorSection[id+1].param[9]-trVectorSection[id].param[9]);

    float dlugosc = Vec3::distance(pos1, pos2);
    return (float)(asin((pos1[1]-pos2[1])/(dlugosc))); 
}

void TRnode::addPositionOffset(float offsetXYZ[]){
    if(this->typ == 0 || this->typ == 2){
        int x = UiD[4], z = UiD[5];
        float pos[3];
        pos[0] = UiD[6] + offsetXYZ[0];
        pos[1] = UiD[7] + offsetXYZ[1];
        pos[2] = UiD[8] + offsetXYZ[2];
        //qDebug() << "old tile" << wObj->x << wObj->y;
        while(pos[0] > 1024 || pos[0] < -1024 || pos[2] > 1024 || pos[2] < -1024 ){
            Game::check_coords(x, z, pos);
        }
        UiD[4] = x;
        UiD[5] = z;
        UiD[6] = pos[0];
        UiD[7] = pos[1];
        UiD[8] = pos[2];
    } else if(this->typ == 1){
        for(int i = 0; i < iTrv; i++){
            int x = this->trVectorSection[i].param[8], z = this->trVectorSection[i].param[9];
            float pos[3];
            pos[0] = this->trVectorSection[i].param[10] + offsetXYZ[0];
            pos[1] = this->trVectorSection[i].param[11] + offsetXYZ[1];
            pos[2] = this->trVectorSection[i].param[12] + offsetXYZ[2];
            //qDebug() << "old tile" << wObj->x << wObj->y;
            while(pos[0] > 1024 || pos[0] < -1024 || pos[2] > 1024 || pos[2] < -1024 ){
                Game::check_coords(x, z, pos);
            }
            this->trVectorSection[i].param[8] = x;
            this->trVectorSection[i].param[9] = z;
            this->trVectorSection[i].param[10] = pos[0];
            this->trVectorSection[i].param[11] = pos[1];
            this->trVectorSection[i].param[12] = pos[2];
        }
    }
}

void TRnode::addTrackNodeItemOffset(unsigned int trackNodeOffset, unsigned int trackItemOffset){
    for(int i = 0; i < this->TrP1+this->TrP2; i++)
        this->TrPinS[i] += trackNodeOffset;
    
    if(trItemRef == NULL)
        return;
    for(int i = 0; i < this->iTri; i++)
        trItemRef[i] += trackItemOffset;
    

    
}