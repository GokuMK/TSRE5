#ifndef ROUTE_H
#define	ROUTE_H
#include <unordered_map>
#include <QString>
#include "GLUU.h"
#include "TDB.h"
#include "WorldObj.h"
#include "Ref.h"

class Tile;

class Route {
public:
    std::unordered_map<int, Tile*> tile;
    bool loaded;
    Ref *ref;
    Route();
    Route(const Route& orig);
    virtual ~Route();
    WorldObj* getObj(int x, int z, int uid);
    void save();
    WorldObj* placeObject(int x, int z, float* p);
    WorldObj* placeObject(int x, int z, float* p, float* q);
    WorldObj* placeObject(int x, int z, float* p, float* q, Ref::RefItem* r);
    void transalteObj(int x, int z, float px, float py, float pz, int uid);
    void render(GLUU *gluu, float* playerT, float* playerW, float* target, float fov, bool selection);
private:
    TDB *trackDB;
};

#endif	/* ROUTE_H */

