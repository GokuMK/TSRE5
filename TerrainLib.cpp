#include "TerrainLib.h"
#include "Terrain.h"
#include "GLMatrix.h"
#include <QOpenGLShaderProgram>
#include <math.h>
#include "Game.h"

std::unordered_map<int, Terrain*> TerrainLib::terrain;

TerrainLib::TerrainLib() {
}

TerrainLib::TerrainLib(const TerrainLib& orig) {
}

TerrainLib::~TerrainLib() {
}

bool TerrainLib::isLoaded(int x, int z) {
    Terrain *tTile;
    //try {
    tTile = terrain[((x)*10000 + z)];
    //} catch (const std::out_of_range& oor) {
    if (tTile == NULL)
        return false;
    //}
    if (tTile->loaded) {
        return true;
    }
    return false;
}

bool TerrainLib::load(int x, int z) {
    Terrain* tTile = terrain[x*10000 + z];
    if (tTile == NULL) {
        terrain[x*10000 + z] = new Terrain(x, z);
    }
    tTile = terrain[x*10000 + z];
    if (tTile == NULL)
        return false;
    if (tTile->loaded) {
        return true;
    }
    return false;
}

void TerrainLib::save(){
    if (!Game::writeEnabled) return;
    qDebug() << "save terrain";
    for (auto it = terrain.begin(); it != terrain.end(); ++it) {
        //console.log(obj.type);
        Terrain* tTile = (Terrain*) it->second;
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            tTile->save();
            tTile->setModified(false);
        }
    }
}

bool TerrainLib::reload(int x, int z) {
    Terrain* tTile;// = terrain[x*10000 + z];
    //if (tTile == NULL) {
    terrain[x*10000 + z] = new Terrain(x, z);
    //}
    tTile = terrain[x*10000 + z];
    if (tTile == NULL)
        return false;
    if (tTile->loaded) {
        return true;
    }
    return false;
}

float TerrainLib::getHeight(float x, float z, float posx, float posz) {
    return TerrainLib::getHeight(x, z, posx, posz, false);
}

void TerrainLib::refresh(int x, int z) {
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->refresh();
}

void TerrainLib::setHeight(float x, float z, float posx, float posz, float h) {
    Game::check_coords(x, z, posx, posz);
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    
    //float value = terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8];
    terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8] = h;
    terr->setModified(true);
}

void TerrainLib::setHeight256(int x, int z, int posx, int posz, float h) {
    Game::check_coords(x, z, posx, posz);
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    
    //float value = terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8];
    terr->terrainData[posz+128][posx+128] = h;
    terr->setModified(true);
}

float TerrainLib::getHeight(float x, float z, float posx, float posz, bool addR) {
    Game::check_coords(x, z, posx, posz);

    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return -1;
    if (terr->loaded == false) return -1;

    if ((posx + 1024) / 8 + 1 > 256 || (posz + 1024) / 8 + 1 > 256)
        return terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8];

    float tx = (posx / 8) - (float) floor(posx / 8);
    float tz = (posz / 8) - (float) floor(posz / 8);
    //float min = 1-Math.abs(tx+tz-1);

    float roznica = 0;

    if (addR) {
        roznica = 0.25 * (terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8] +
                terr->terrainData[(int) (posz + 1024) / 8 + 1][(int) (posx + 1024) / 8 + 1] +
                terr->terrainData[(int) (posz + 1024) / 8 + 1][(int) (posx + 1024) / 8] +
                terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8 + 1]) -
                0.5f * (terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8] +
                terr->terrainData[(int) (posz + 1024) / 8 + 1][(int) (posx + 1024) / 8 + 1]);
    }
    return (
            terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8]*(1.0 - tx)*(1.0 - tz) +
            terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8 + 1]*(tx)*(1.0 - tz) +
            terr->terrainData[(int) (posz + 1024) / 8 + 1][(int) (posx + 1024) / 8]*(1.0 - tx)*(tz) +
            terr->terrainData[(int) (posz + 1024) / 8 + 1][(int) (posx + 1024) / 8 + 1]*(tx)*(tz)
            + fabs(roznica));

    //} catch (const std::out_of_range& oor) {
    ///    return -1;
    //}
    //return -1;
}

void TerrainLib::setTerrainTexture(Brush* brush, int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setTexture(brush, x, z, posx, posz);
}

int TerrainLib::getTexture(int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return -1;
    if (terr->loaded == false) return -1;
    return terr->getTexture(x, z, posx, posz);
}

void TerrainLib::paintTexture(Brush* brush, int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->paintTexture(brush, x, z, posx, posz);
}

void TerrainLib::paintHeightMap(int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    //terr->paintTexture(x, z, posx, posz);
    
    int px = (posx + 1024)/8;
    int pz = (posz + 1024)/8;
    
    float h = 0;
    for(int i = -5; i < 5; i++)
        for(int j = -5; j < 5; j++){
            if(px+i >= 256) continue;
            if(pz+j >= 256) continue;
            if(px+i < 0) continue;
            if(pz+j < 0) continue;
            h = (fabs(i) + fabs(j))/2.0;
            terr->terrainData[pz+j][px+i] += (10.0 - h)/5.0;
        }
    terr->setModified(true);
    terr->refresh();
}

void TerrainLib::fillRAW(float** terrainData, int mojex, int mojez) {
    Terrain *tTile;
    //try {
    tTile = terrain[((mojex + 1)*10000 + mojez)];
    //} catch (const std::out_of_range& oor) {
    if (tTile == NULL) {
        terrain[(mojex + 1)*10000 + mojez] = new Terrain(mojex + 1, mojez);
    }
    tTile = terrain[(mojex + 1)*10000 + mojez];
    if (tTile->loaded) {
        for (int i = 0; i < 256; i++) {
            terrainData[i][256] = tTile->terrainData[i][0];
        }
    }

    //try {
    tTile = terrain[((mojex)*10000 + mojez + 1)];
    //} catch (const std::out_of_range& oor) {
    if (tTile == NULL) {
        terrain[(mojex)*10000 + mojez + 1] = new Terrain(mojex, mojez + 1);
    }
    tTile = terrain[(mojex)*10000 + mojez + 1];
    if (tTile->loaded) {
        for (int i = 0; i < 256; i++) {
            terrainData[256][i] = tTile->terrainData[0][i];
        }
    }

    //try {
    tTile = terrain[((mojex + 1)*10000 + (mojez + 1))];
    //} catch (const std::out_of_range& oor) {
    if (tTile == NULL) {
        terrain[(mojex + 1)*10000 + mojez + 1] = new Terrain(mojex + 1, mojez + 1);
    }
    tTile = terrain[(mojex + 1)*10000 + mojez + 1];
    if (tTile->loaded) {
        terrainData[256][256] = tTile->terrainData[0][0];
    }/**/
}

void TerrainLib::render(GLUU *gluu, float * playerT, float* playerW, float* target, float fov) {
    int mintile = -2;
    int maxtile = 2;

    //TerrainLib.render(playerT, playerW); 

    /*for (auto it = terrain.begin(); it != terrain.end(); ++it) {
        //console.log(obj.type);
        Terrain* obj = (Terrain*) it->second;
        if(obj == NULL) continue;
        obj->inUse = false;
    }*/
    
    gluu->m_program->setUniformValue(gluu->shaderAlpha, 0.0f);

    Terrain *tTile;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            tTile = terrain[((playerT[0] + i)*10000 + playerT[1] + j)];
            
            if (tTile == NULL) {
                terrain[(playerT[0] + i)*10000 + playerT[1] + j] = new Terrain(playerT[0] + i, playerT[1] + j);
            }
            //} catch (const std::out_of_range& oor) {
            //    terrain[(playerT[0] + i)*10000 + playerT[1] + j] = new Terrain(playerT[0] + i, playerT[1] + j);
            //}
            tTile = terrain[(playerT[0] + i)*10000 + playerT[1] + j];
            tTile->inUse = true;
            if (tTile->loaded == false) continue;
            //tTile->inUse = true;
            if (tTile->loaded) {
                float lodx = 2048 * i - playerW[0];
                float lodz = 2048 * j - playerW[2];
                gluu->mvPushMatrix();
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
                gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
                tTile->render(lodx, lodz, playerT, playerW, target, fov);
                gluu->mvPopMatrix();
            }
        }
    }
    
    mintile = -3;
    maxtile = 3;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            tTile = terrain[((playerT[0] + i)*10000 + playerT[1] + j)];
            if (tTile != NULL)
                tTile->inUse = true;
        }
    }
    
    for (auto it = terrain.begin(); it != terrain.end(); ++it) {
        //console.log(obj.type);
        Terrain* obj = (Terrain*) it->second;
        if(obj == NULL) continue;
        if(!obj->inUse && obj->loaded){
           //console.log("a"+this.tile[key]);
           delete obj;
           terrain[(int)it->first] = NULL;
       } else {
           obj->inUse = false;
       }
    }
}