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
class TerrainInfo;

class TerrainLib {
public:
    TerrainLib();
    TerrainLib(const TerrainLib& orig);
    virtual ~TerrainLib();
    virtual Terrain* getTerrainByXY(int x, int y, bool load = false);
    virtual void fillRaw(Terrain *cTerr, int mojex, int mojez);
    virtual float getHeight(int x, int z, float posx, float posz);
    virtual float getHeight(int x, int z, float posx, float posz, bool addR);
    virtual void getRotation(float *rot, int x, int z, float posx, float posz);
    virtual void setHeight(int x, int z, float posx, float posz, float h);
    virtual void fillHeightMap(int x, int z, float *data);
    virtual void fillWaterLevels(float *w, int mojex, int mojez);
    virtual void setWaterLevels(float *w, int mojex, int mojez);
    virtual Terrain* setHeight256(int x, int z, int posx, int posz, float h);
    virtual Terrain* setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE);
    virtual void setHeightFromGeoGui(int x, int z, float* p);
    virtual void setHeightFromGeo(int x, int z, float* p);
    virtual bool isLoaded(int x, int z);
    virtual QSet<Terrain*> paintHeightMap(Brush* brush, int x, int z, float* p);
    virtual void paintTexture(Brush* brush, int x, int z, float* p);
    virtual void lockTexture(Brush* brush, int x, int z, float* p);
    virtual void setTerrainTexture(Brush* brush, int x, int z, float* p);
    virtual void toggleGaps(int x, int z, float* p, float direction);
    virtual void toggleWaterDraw(int x, int z, float* p, float direction);
    virtual void setWaterLevelGui(int x, int z, float* p);
    virtual void makeTextureFromMap(int x, int z, float* p);
    virtual void removeTileTextureFromMap(int x, int z, float* p);
    virtual void setFixedTileHeight(Brush* brush, int x, int z, float* p);
    virtual void toggleDraw(int x, int z, float* p);
    virtual void setTileBlob(int x, int z, float* p);
    virtual void setTextureToTrackObj(Brush* brush, float* punkty, int length, int x, int z);
    virtual void setTerrainToTrackObj(Brush* brush, float* punkty, int length, int x, int z, float* matrix);
    virtual int getTexture(int x, int z, float* p);
    virtual bool load(int x, int z);
    virtual void getUnsavedInfo(std::vector<QString> &items);
    virtual void save();
    virtual void refresh(int x, int z);
    virtual bool reload(int x, int z);
    virtual void loadQuadTree();
    virtual void createNewRouteTerrain(int x, int z);
    virtual void saveEmpty(int x, int z);
    virtual void render(GLUU *gluu, float* playerT, float* playerW, float* target, float fov, int renderMode);
    virtual void renderLo(GLUU *gluu, float* playerT, float* playerW, float* target, float fov, int renderMode);
    virtual void renderWater(GLUU *gluu, float* playerT, float* playerW, float* target, float fov, int renderMode, int layer);
    virtual void renderEmpty(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
    virtual void renderShadowMap(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
    
protected:
    HeightWindow* heightWindow = NULL;
};

#endif	/* TERRAINLIB_H */

