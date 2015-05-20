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
    int routeMaxIdx = 40000;
    int routeShapes = 40000;
    //std::unordered_map<int, TrackShape::SectionIdx*> routeShape;
    
    TSectionDAT();
    TSectionDAT(const TSectionDAT& orig);
    virtual ~TSectionDAT();
    bool isRoadShape(int id);
    void getShapeData(int id);
private:
    bool loadGlobal();
    bool loadRoute();
    bool saveRoute();
};

#endif	/* TSECTIONDAT_H */

