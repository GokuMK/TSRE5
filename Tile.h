#ifndef TILE_H
#define	TILE_H

#include <QString>
#include <unordered_map>
#include "WorldObj.h"
#include "Ref.h"

class Tile {
public:
    struct ViewDbSphere{
        int vDbId;
	float position[3];
	float radius;
    };
    int vDbIdCount;
    ViewDbSphere* viewDbSphere;
    
    int loaded;
    bool inUse;
    bool modified;
    int x;
    int z;
    //obiekty = new Array();
    int jestObiektow;
    std::unordered_map<int, WorldObj*> obiekty;
    Tile();
    Tile(int xx, int zz);
    Tile(const Tile& orig);
    virtual ~Tile();
    static void saveEmpty(int x, int z);
    static QString getNameXY(int e);
    void load();
    WorldObj* getObj(int uid);
    WorldObj* placeObject(float* p, Ref::RefItem* itemData);
    WorldObj* placeObject(float* p, float* q, Ref::RefItem* itemData);
    void transalteObj(float px, float py, float pz, int uid);
    void initNew();
    void render();
    void render(float *  playerT, float* playerW, float* target, float fov, bool selection);
    void save();
    
private:
    int maxUiD = 0;
    QString* viewDbSphereRaw = NULL;
    void wczytajObiekty();
    bool createObj(WorldObj** nowy, QString sh);
};

#endif	/* TILE_H */

