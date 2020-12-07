/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#include "TerrainClient.h"
#include "RouteEditorClient.h"
#include "TerrainInfo.h"

TerrainClient::TerrainClient(){
    
}

TerrainClient::TerrainClient(TerrainInfo *ti){
    mojex = ti->cx;
    mojez = -ti->cy;
    name = ti->name;
    lowTile = ti->low;
    load();
}

TerrainClient::TerrainClient(float x, float y){
    mojex = x;
    mojez = y;
    name = getTileName((int) x, (int) -y);
    load();
}

TerrainClient::TerrainClient(const TerrainClient& orig) {
    
}

void TerrainClient::loadTFile(FileBuffer *data){
    this->tfile = new TFile();
    this->tfile->load(data);
    loadingProgress++;
    load();
}

void TerrainClient::loadRAWFile(FileBuffer *data){
    loadingProgress++;
    this->readRAWFloat(data);
    load();
}

void TerrainClient::updateTFile(){
    Game::serverClient->updateTerrainTFile(this);
}

void TerrainClient::loadFFile(FileBuffer *data){
    loadingProgress++;
    if(data->off == data->length){
        load();
        return;
    }
    jestF = true;
    this->readF(data);
    load();
}

void TerrainClient::load(){
    int esdAlternativeTexture = 0x01;
    QString seasonPath;
    QString path;
    
    switch(loadingProgress){
        case 0:
            typeObj = this->terrainobj;
            loaded = false;
            isOgl = false;
            modified = false;
            wTexid = -1;
            for (int i = 0; i < 256; i++) {
                texid[i] = -1;
                texid2[i] = -1;
                hidden[i] = false;
                texModified[i] = false;
                texLocked[i] = false;
                uniqueTex[i] = false;
                selectedPatchs[i] = false;
            }
            VBO = new QOpenGLBuffer();
            VAO = new QOpenGLVertexArrayObject();

            esdAlternativeTexture = 0x01;
            if((esdAlternativeTexture & Game::TextureFlags[Game::season]) != 0)
                seasonPath = Game::season.toLower() + "/";

            if(Game::season == "Winter" || Game::season == "AutumnSnow" || Game::season == "WinterSnow" || Game::season == "SpringSnow" ){
                if(esdAlternativeTexture & Game::TextureFlags["Snow"] != 0)
                    seasonPath = "snow/";
                if(esdAlternativeTexture & Game::TextureFlags["SnowTrack"] != 0)
                    seasonPath = "snow/";
            }

            texturepath = Game::root + "/routes/" + Game::route + "/terrtex/"+seasonPath;
            rootTexturepath = Game::root + "/routes/" + Game::route + "/terrtex/";
            path = Game::root + "/routes/" + Game::route + "/" + TileDir[(int)lowTile] + "/";

            Game::serverClient->sendUtf16Message("request_terrain_tfile( "+QString::number(mojex)+" "+QString::number(mojez)+" )");
            break;
            //tfile = new TFile();

            //QString filename = getTileName((int) x, (int) -y);
            //QString filename = getTileNameExperimental2((int) x, (int) -y);
            //qDebug() << filename << x << -y;
            //if (!tfile->readT((path + name + ".t"))) {
            //    //qDebug() << " t fail" << name;
            //    return;
            //}
        case 1:
            if(tfile->sampleYbuffer == NULL)
                return;
            Game::serverClient->sendUtf16Message("request_terrain_yfile( "+QString::number(mojex)+" "+QString::number(mojez)+" )");
            if(tfile->sampleFbuffer != NULL){
                Game::serverClient->sendUtf16Message("request_terrain_ffile( "+QString::number(mojex)+" "+QString::number(mojez)+" )");
            } else {
                loadingProgress++;
            }break;
        case 2:
            break;
        case 3:
            //qDebug() << " ok";

            //QString name = this->getTileName(mojex, -mojez);
            QString name2;
            //int samples = 
            int patches = tfile->patchsetNpatches;
            for (int u = 0; u < patches; u++){
                for (int y = 0; y < patches; y++) {
                    name2 = name + "_" + QString::number(y) + "_" + QString::number(u) + ".ace";
                    //qDebug() << name2 << (int) tfile->tdata[(y * 16 + u)*13 + 0 + 6];
                    //qDebug() << tfile->materialsCount;
                    //qDebug() << tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0];
                    //qDebug() << name << patches << tfile->materialsCount << (int) tfile->tdata[(y * patches + u)*13 + 0 + 6];
                    if (tfile->materialsCount <= (int) tfile->tdata[(y * patches + u)*13 + 0 + 6])
                        continue;

                    if (name2 == *tfile->materials[(int) tfile->tdata[(y * patches + u)*13 + 0 + 6]].tex[0])
                        this->uniqueTex[y*patches+u] = true;
                }
            }

            loaded = true;
            break;
    }
}

TerrainClient::~TerrainClient() {
}

