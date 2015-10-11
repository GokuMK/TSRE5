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
    int tsectionMaxIdx;// = 40000;
    int tsectionShapes;// = 40000;
    int routeMaxIdx;// = 40000;
    int routeShapes;// = 40000;
    //std::unordered_map<int, TrackShape::SectionIdx*> routeShape;
    
    TSectionDAT();
    TSectionDAT(const TSectionDAT& orig);
    virtual ~TSectionDAT();
    bool isRoadShape(int id);
    void getShapeData(int id);
    bool saveRoute();
private:
    bool loadGlobal();
    bool loadRoute();
};

#endif	/* TSECTIONDAT_H */

