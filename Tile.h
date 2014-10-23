#ifndef TILE_H
#define	TILE_H

#include <QString>
#include <unordered_map>
#include "WorldObj.h"

class Tile {
public:
    int loaded;
    bool inUse;
    int x;
    int z;
    //obiekty = new Array();
    int jestObiektow;
    std::unordered_map<int, WorldObj*> obiekty;
    Tile();
    Tile(int xx, int zz);
    Tile(const Tile& orig);
    virtual ~Tile();
    void wczytajObiekty();
    QString getNameXY(int e);
    void load();
    void render();
    void render(float *  playerT, float* playerW, float* target, float fov);
    
private:

};

#endif	/* TILE_H */

