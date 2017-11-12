/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */
#ifndef TERRAINLIBQT_H
#define	TERRAINLIBQT_H

#include "TerrainLib.h"

class TerrainLibQt : public TerrainLib {
public:
    TerrainLibQt();
    TerrainLibQt(const TerrainLibQt& orig);
    Terrain* getTerrainByXY(int x, int y, bool load = false);
    virtual ~TerrainLibQt();
    void fillRaw(float** terrainData, int mojex, int mojez);
    float getHeight(int x, int z, float posx, float posz);
    float getHeight(int x, int z, float posx, float posz, bool addR);
    void getRotation(float *rot, int x, int z, float posx, float posz);
    void setHeight(int x, int z, float posx, float posz, float h);
    void fillHeightMap(int x, int z, float *data);
    void fillWaterLevels(float *w, int mojex, int mojez);
    void setWaterLevels(float *w, int mojex, int mojez);
    int setHeight256(int x, int z, int posx, int posz, float h);
    int setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE);
    void setHeightFromGeoGui(int x, int z, float* p);
    void setHeightFromGeo(int x, int z, float* p);
    bool isLoaded(int x, int z);
    QSet<int> paintHeightMap(Brush* brush, int x, int z, float* p);
    void paintTexture(Brush* brush, int x, int z, float* p);
    void lockTexture(Brush* brush, int x, int z, float* p);
    void setTerrainTexture(Brush* brush, int x, int z, float* p);
    void toggleGaps(int x, int z, float* p, float direction);
    void toggleWaterDraw(int x, int z, float* p, float direction);
    void setWaterLevelGui(int x, int z, float* p);
    void makeTextureFromMap(int x, int z, float* p);
    void removeTileTextureFromMap(int x, int z, float* p);
    void setFixedTileHeight(Brush* brush, int x, int z, float* p);
    void toggleDraw(int x, int z, float* p);
    void setTileBlob(int x, int z, float* p);
    void setTextureToTrackObj(Brush* brush, float* punkty, int length, int x, int z);
    void setTerrainToTrackObj(Brush* brush, float* punkty, int length, int x, int z, float* matrix);
    int getTexture(int x, int z, float* p);
    bool load(int x, int z);
    void getUnsavedInfo(std::vector<QString> &items);
    void save();
    void refresh(int x, int z);
    bool reload(int x, int z);
    void loadQuadTree();
    void createNewRouteTerrain(int x, int z);
    void saveEmpty(int x, int z);
    void render(GLUU *gluu, float* playerT, float* playerW, float* target, float fov, int renderMode);
    void renderLo(GLUU *gluu, float* playerT, float* playerW, float* target, float fov, int renderMode);
    void renderWater(GLUU *gluu, float* playerT, float* playerW, float* target, float fov, int renderMode, int layer);
    void renderEmpty(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
    void renderShadowMap(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
private:
    QuadTree* quadTree;
    QuadTree* quadTreeLo;
    QHash<QString, TerrainInfo*> terrainQt;
    QHash<QString, TerrainInfo*> terrainQtLo;
    
    void spiralLoop(int n, int &x, int &y);
};

#endif	/* TERRAINLIBQT_H */

