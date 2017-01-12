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
#include <math.h>
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
        float lenx = UiD[6] - r->UiD[6];
        float leny = UiD[7] - r->UiD[7];
        float lenz = UiD[8] - r->UiD[8];
        if (fabs(lenx) < 0.1 && fabs(leny) < 0.1 && fabs(lenz) < 0.1)
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
        float lenx = UiD[6] - r->UiD[6];
        float leny = UiD[7] - r->UiD[7];
        float lenz = UiD[8] - r->UiD[8];
        if (fabs(lenx) < 0.1 && fabs(leny) < 0.1 && fabs(lenz) < 0.1)
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
    }

int TRnode::setTrPinK(int id, int nowe) {
        for(int j = 0; j < 3; j++)
            if(TrPinS[j] == id){
                TrPinK[j] = nowe;
                return j;
            }
    }