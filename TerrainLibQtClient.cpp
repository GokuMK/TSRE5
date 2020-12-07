/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TerrainLibQtClient.h"
#include "TerrainInfo.h"
#include "TerrainClient.h"
#include "QuadTree.h"
#include "RouteEditorClient.h"

TerrainLibQtClient::TerrainLibQtClient() {
}

TerrainLibQtClient::TerrainLibQtClient(const TerrainLibQtClient& orig) {
}

TerrainLibQtClient::~TerrainLibQtClient() {
}

void TerrainLibQtClient::updateTerrainHeightmap(Terrain *t){
    Game::serverClient->updateTerrainHeightmap(t);
}

void TerrainLibQtClient::updateTerrainTFile(Terrain *t){
    Game::serverClient->updateTerrainTFile(t);
}
Terrain* TerrainLibQtClient::getTerrainByXY(int x, int y, bool load) {
    if(currentQuadTree == NULL)
        currentQuadTree = quadTree;
    if(currentQt == NULL)
        currentQt = &terrainQt;
    
    
    //QString terrainName = currentQuadTree->getMyName((int) x, -y);
    unsigned int terrainNameId = currentQuadTree->getMyNameId((int) x, -y);

    if (terrainNameId == 0)
        return NULL;
    if ((*currentQt)[terrainNameId] != NULL) {
        if((*currentQt)[terrainNameId]->t != NULL)
            return (*currentQt)[terrainNameId]->t;
    }
    if (load) {
        (*currentQt)[terrainNameId] = new TerrainInfo();
        currentQuadTree->fillTerrainInfo(x, -y, (*currentQt)[terrainNameId]);
        //qDebug() << terrainNameId;
        (*currentQt)[terrainNameId]->t = new TerrainClient((*currentQt)[terrainNameId]);
        return (*currentQt)[terrainNameId]->t;
    }

    return NULL;
}