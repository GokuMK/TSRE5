/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TERRAINLIB_H
#define	TERRAINLIB_H

#include <unordered_map>
#include <QString>
#include "GLUU.h"

class Terrain;
class Brush;
class HeightWindow;
class QuadTree;

class TerrainLib {
public:
    static std::unordered_map<int, Terrain*> terrain;
    
    TerrainLib();
    TerrainLib(const TerrainLib& orig);
    virtual ~TerrainLib();
    static void fillRAW(float** terrainData, int mojex, int mojez);
    static float getHeight(int x, int z, float posx, float posz);
    static float getHeight(int x, int z, float posx, float posz, bool addR);
    static void getRotation(float *rot, int x, int z, float posx, float posz);
    static void setHeight(int x, int z, float posx, float posz, float h);
    static void fillHeightMap(int x, int z, float *data);
    static int setHeight256(int x, int z, int posx, int posz, float h);
    static int setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE);
    static void setHeightFromGeoGui(int x, int z, float* p);
    static bool isLoaded(int x, int z);
    static QSet<int> paintHeightMap(Brush* brush, int x, int z, float* p);
    static void paintTexture(Brush* brush, int x, int z, float* p);
    static void lockTexture(Brush* brush, int x, int z, float* p);
    static void setTerrainTexture(Brush* brush, int x, int z, float* p);
    static void toggleGaps(int x, int z, float* p);
    static void setWaterDraw(int x, int z, float* p);
    static void setWaterLevelGui(int x, int z, float* p);
    static void setFixedTileHeight(Brush* brush, int x, int z, float* p);
    static void setDraw(int x, int z, float* p);
    static void setTileBlob(int x, int z, float* p);
    static void setTerrainToTrackObj(Brush* brush, float* punkty, int length, int x, int z, float* matrix);
    static int getTexture(int x, int z, float* p);
    static bool load(int x, int z);
    static void getUnsavedInfo(std::vector<QString> &items);
    static void save();
    static void refresh(int x, int z);
    static bool reload(int x, int z);
    static void LoadQuadTree();
    static void createNewRouteTerrain(int x, int z);
    static void saveEmpty(int x, int z);
    static void render(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
    static void renderEmpty(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
    static void renderShadowMap(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
private:
    static HeightWindow* heightWindow;
    static QuadTree* quadTree;
};

#endif	/* TERRAINLIB_H */

