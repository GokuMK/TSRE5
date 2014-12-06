#include "TerrainLib.h"
#include "Terrain.h"
#include "GLMatrix.h"
#include <QOpenGLShaderProgram>
#include <math.h>

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
    if(tTile == NULL)
        return false;
    //}
    if (tTile->loaded) {
        return true;
    }
    return false;
}

float TerrainLib::getHeight(float x, float z, float posx, float posz) {
    return TerrainLib::getHeight(x, z, posx, posz, false);
}

float TerrainLib::getHeight(float x, float z, float posx, float posz, bool addR) {
    //return 0;
    if (posx < -1024) {
        x--;
        posx += 2048;
    }
    if (posx > 1024) {
        x++;
        posx -= 2048;
    }
    if (posz < -1024) {
        z--;
        posz += 2048;
    }
    if (posz > 1024) {
        z++;
        posz -= 2048;
    }

    Terrain *terr;
    //try {
        terr = terrain[(x * 10000 + z)];
        if(terr == NULL) return -1;
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

void TerrainLib::fillRAW(float** terrainData, float mojex, float mojez) {
    Terrain *tTile;
    //try {
        tTile = terrain[((mojex + 1)*10000 + mojez)];
    //} catch (const std::out_of_range& oor) {
    if(tTile == NULL){
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
    if(tTile == NULL){
        terrain[(mojex)*10000 + mojez + 1] = new Terrain(mojex, mojez + 1);
    }
    tTile = terrain[(mojex)*10000 + mojez + 1];
    if (tTile->loaded) {
        for (int i = 0; i < 256; i++) {
            terrainData[256][i] = tTile->terrainData[0][i];
        }
    }

    //try {
        tTile = terrain[((mojex + 1)*10000 + mojez + 1)];
    //} catch (const std::out_of_range& oor) {
    if(tTile == NULL){
        terrain[(mojex + 1)*10000 + mojez + 1] = new Terrain(mojex + 1, mojez + 1);
    }
    tTile = terrain[(mojex + 1)*10000 + mojez + 1];
    if (tTile->loaded) {
        terrainData[256][256] = tTile->terrainData[0][0];
    }
}

void TerrainLib::render(GLUU *gluu, float * playerT, float* playerW, float* target, float fov) {
    int mintile = -2;
    int maxtile = 2;

    //TerrainLib.render(playerT, playerW); 

    //for (var key in this.tile){
    //    this.tile[key].inUse = false;
    // }
    gluu->m_program->setUniformValue(gluu->shaderAlpha, 0.0f);

    Terrain *tTile;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            //try {
                tTile = terrain[((playerT[0] + i)*10000 + playerT[1] + j)];
                if(tTile == NULL){
                    terrain[(playerT[0] + i)*10000 + playerT[1] + j] = new Terrain(playerT[0] + i, playerT[1] + j);
                }
            //} catch (const std::out_of_range& oor) {
            //    terrain[(playerT[0] + i)*10000 + playerT[1] + j] = new Terrain(playerT[0] + i, playerT[1] + j);
            //}
            tTile = terrain[(playerT[0] + i)*10000 + playerT[1] + j];
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
    /*
    for (var key in this.tile){
       if(this.tile[key] === undefined) continue;
       if(this.tile[key] === null) continue;
       //console.log(this.tile[key].inUse);
       if(!this.tile[key].inUse){
           //console.log("a"+this.tile[key]);
           this.tile[key] = undefined;
       } else {
           this.tile[key].inUse = false;
       }
    }*/
}