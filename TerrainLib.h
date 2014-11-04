#ifndef TERRAINLIB_H
#define	TERRAINLIB_H

#include <unordered_map>
#include <QString>
#include "GLUU.h"

class Terrain;

class TerrainLib {
public:
    static std::unordered_map<int, Terrain*> terrain;
    
    TerrainLib();
    TerrainLib(const TerrainLib& orig);
    virtual ~TerrainLib();
    static void fillRAW(float** terrainData, float mojex, float mojez);
    static float getHeight(float x, float z, float posx, float posz);
    static float getHeight(float x, float z, float posx, float posz, bool addR);
    static void render(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
private:

};

#endif	/* TERRAINLIB_H */

