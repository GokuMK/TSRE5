#ifndef TRNODE_H
#define	TRNODE_H

#include "Vector2i.h"

class TRnode {
public:
    struct TRSect {
        float param[16];
    };
    int typ;
    int args[3];
    float UiD[12];
    int iTrv;
    TRSect *trVectorSection;
    int iTri;
    int *trItemRef;
    int TrP1;
    int TrP2;
    int TrPinS[3];
    int TrPinK[3];
    
    TRnode();
    TRnode(const TRnode& orig);
    virtual ~TRnode();
    Vector2i* getTile();
    bool isEnd();
    bool equals(TRnode* r);
    bool equalsIgnoreType(TRnode* r);
    int podmienTrPin(int stare, int nowe);
    int setTrPinK(int id, int nowe);
private:

};

#endif	/* TRNODE_H */

