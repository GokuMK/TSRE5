#ifndef TSECTIONDAT_H
#define	TSECTIONDAT_H
#include <unordered_map>
#include "TSection.h"

class TSectionDAT {
public:
    std::unordered_map<int, TSection*> sekcja;
    bool loaded;
    
    TSectionDAT();
    TSectionDAT(const TSectionDAT& orig);
    virtual ~TSectionDAT();
private:
    bool loadGlobal();
    bool loadRoute();
};

#endif	/* TSECTIONDAT_H */

