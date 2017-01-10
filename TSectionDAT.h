/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    int tsectionMaxIdx = 0;// = 40000;
    int tsectionShapes = 0;// = 40000;
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

