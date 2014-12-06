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
    int TrP1;
    int TrP2;
    int TrPinS[3];
    int TrPinK[3];
    
    TRnode();
    TRnode(const TRnode& orig);
    virtual ~TRnode();
    Vector2i* getTile();
    bool isEnd();
private:

};

#endif	/* TRNODE_H */

