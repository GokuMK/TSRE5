/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TERRAINLIBQTCLIENT_H
#define TERRAINLIBQTCLIENT_H

#include "TerrainLibQt.h"

class TerrainLibQtClient : public TerrainLibQt {
public:
    TerrainLibQtClient();
    TerrainLibQtClient(const TerrainLibQtClient& orig);
    virtual ~TerrainLibQtClient();
    Terrain* getTerrainByXY(int x, int y, bool load = false);
    void updateTerrainHeightmap(Terrain *t);
    void updateTerrainTFile(Terrain *t);
private:

};

#endif /* TERRAINLIBQTCLIENT_H */

