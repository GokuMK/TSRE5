#include "TRnode.h"

TRnode::TRnode() {
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