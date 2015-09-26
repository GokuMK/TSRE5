#include "TRnode.h"
#include <math.h>

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

TRnode::TRnode(const TRnode& orig) {
}

TRnode::~TRnode() {
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