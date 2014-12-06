#ifndef TSECTIONDAT_H
#define	TSECTIONDAT_H
#include <unordered_map>
#include "TSection.h"
#include "TrackShape.h"

class TSectionDAT {
public:
    std::unordered_map<int, TSection*> sekcja;
    std::unordered_map<int, TrackShape*> shape;
    bool loaded;
    
    
    
    TSectionDAT();
    TSectionDAT(const TSectionDAT& orig);
    virtual ~TSectionDAT();
private:
    bool loadGlobal();
    bool loadRoute();
};

#endif	/* TSECTIONDAT_H */

