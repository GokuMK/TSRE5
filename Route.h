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
    int startTileX;
    int startTileY;
    float startpX;
    float startpZ;
    Ref *ref;
    TDB *trackDB;
    Route();
    Route(const Route& orig);
    virtual ~Route();
    WorldObj* getObj(int x, int z, int uid);
    void save();
    void saveTrk();
    void createNewPaths();
    void createNew();
    void newTile(int x, int z);
    void reloadTile(int x, int z);
    void removeTrackFromTDB(WorldObj* selectedObj);
    WorldObj* placeObject(int x, int z, float* p);
    WorldObj* placeObject(int x, int z, float* p, float* q);
    WorldObj* placeObject(int x, int z, float* p, float* q, Ref::RefItem* r);
    void addToTDB(WorldObj* obj, float* post, float* pos);
    void newPositionTDB(WorldObj* obj, float* post, float* pos);
    void transalteObj(int x, int z, float px, float py, float pz, int uid);
    void render(GLUU *gluu, float* playerT, float* playerW, float* target, float playerRot, float fov, bool selection);
private:
    void loadTrk();
};

#endif	/* ROUTE_H */

