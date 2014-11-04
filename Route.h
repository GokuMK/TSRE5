#ifndef ROUTE_H
#define	ROUTE_H
#include <unordered_map>
#include <QString>
#include "GLUU.h"
#include "TDB.h"

class Tile;

class Route {
public:
    std::unordered_map<int, Tile*> tile;
    bool loaded;
    Route();
    Route(const Route& orig);
    virtual ~Route();
    void render(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
private:
    TDB *trackDB;
};

#endif	/* ROUTE_H */

