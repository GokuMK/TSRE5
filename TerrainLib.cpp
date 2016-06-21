#include "TerrainLib.h"
#include "Terrain.h"
#include "GLMatrix.h"
#include <QOpenGLShaderProgram>
#include <set>
#include <math.h>
#include "Game.h"
#include "Brush.h"
#include "HeightWindow.h"
#include "QuadTree.h"

HeightWindow* TerrainLib::heightWindow = NULL;
std::unordered_map<int, Terrain*> TerrainLib::terrain;
QuadTree* TerrainLib::quadTree = NULL;

TerrainLib::TerrainLib() {
}

TerrainLib::TerrainLib(const TerrainLib& orig) {
}

TerrainLib::~TerrainLib() {
}

void TerrainLib::loadQuadTree(){
    quadTree = new QuadTree();
    quadTree->load();
}

void TerrainLib::createNewRouteTerrain(int x, int z){
    Terrain::saveEmpty(x, z);
    quadTree = new QuadTree();
    quadTree->createNew(x, z);
}

void TerrainLib::saveEmpty(int x, int z){
    Terrain::saveEmpty(x, z);
    quadTree->addTile(x, z);
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

void TerrainLib::getUnsavedInfo(std::vector<QString> &items){
    if (!Game::writeEnabled) return;
    for (auto it = terrain.begin(); it != terrain.end(); ++it) {
        //console.log(obj.type);
        Terrain* tTile = (Terrain*) it->second;
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            items.push_back("[T] "+QString::number(tTile->mojex)+" "+QString::number(-tTile->mojez));
        }
    }
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

float TerrainLib::getHeight(int x, int z, float posx, float posz) {
    return TerrainLib::getHeight(x, z, posx, posz, false);
}

void TerrainLib::refresh(int x, int z) {
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->refresh();
}

void TerrainLib::setHeight(int x, int z, float posx, float posz, float h) {
    Game::check_coords(x, z, posx, posz);
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    
    //float value = terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8];
    terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8] = h;
    terr->setErrorBias(posx, posz, 0);
    terr->setModified(true);
}

int TerrainLib::setHeight256(int x, int z, int posx, int posz, float h){
    return setHeight256(x, z, posx, posz, h, 0, 0);
}

int TerrainLib::setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE) {
    Game::check_coords(x, z, posx, posz);
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];

    if (terr == NULL) return -1;
    if (terr->loaded == false) return -1;
    
    //float value = terr->terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8];
    if(diffC == 0 && diffE == 0){
        terr->terrainData[(posz+1024)/8][(posx+1024)/8] = h;
    } else {
        if(terr->terrainData[(posz+1024)/8][(posx+1024)/8] < h)
            if(terr->terrainData[(posz+1024)/8][(posx+1024)/8] < h - diffE) 
                terr->terrainData[(posz+1024)/8][(posx+1024)/8] = h - diffE;
        if(terr->terrainData[(posz+1024)/8][(posx+1024)/8] > h)
            if(terr->terrainData[(posz+1024)/8][(posx+1024)/8] > h + diffC) 
                terr->terrainData[(posz+1024)/8][(posx+1024)/8] = h + diffC;
    }
    terr->setErrorBias(posx, posz, 0);
    terr->setModified(true);
    
    return x * 10000 + z;
}

float TerrainLib::getHeight(int x, int z, float posx, float posz, bool addR) {
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

void TerrainLib::setHeightFromGeoGui(int x, int z, float* p){
    if(heightWindow == NULL)
        heightWindow = new HeightWindow();
    
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;

    heightWindow->tileX = x;
    heightWindow->tileZ = -z;
    heightWindow->ok = false;
    heightWindow->terrainResolution = 256;
    heightWindow->exec();
    if(heightWindow->ok){
        qDebug() << "ok";
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                terr->terrainData[i][j] = heightWindow->terrainData[j][i];
            }
        }
        terr->setModified(true);
        terr->refresh();
        terr = terrain[(x * 10000 + z + 1)];
        if (terr != NULL) terr->refresh();
        terr = terrain[(x * 10000 + z - 1)];
        if (terr != NULL) terr->refresh();
        terr = terrain[((x+1) * 10000 + z)];
        if (terr != NULL) terr->refresh();
        terr = terrain[((x-1) * 10000 + z)];
        if (terr != NULL) terr->refresh();
    }
}

void TerrainLib::setTerrainToTrackObj(Brush* brush, float* punkty, int length, int tx, int tz, float* matrix){
    std::set<int> uterr;
    // calculating plane equation
    float p1[3];
    float p2[3];
    float p3[3];
    
    p1[0] = punkty[0];
    p1[1] = punkty[1];
    p1[2] = punkty[2];
    p2[0] = punkty[length-3];
    p2[1] = punkty[length-2];
    p2[2] = punkty[length-1];
    p3[0] = 10;
    p3[1] = 0;
    p3[2] = 10;
    Vec3::transformMat4(p3, p3, matrix);
    qDebug() << p1[0] << " " << p1[1] <<" " << p1[2];
    qDebug() << p2[0] << " " << p2[1] <<" " << p2[2];
    qDebug() << p3[0] << " " << p3[1] <<" " << p3[2];
    Vector3f vec1, vec2, vec3;
    vec1.x = p2[0] - p1[0]; vec1.y = p2[1] - p1[1]; vec1.z = p2[2] - p1[2];
    vec2.x = p3[0] - p1[0]; vec2.y = p3[1] - p1[1]; vec2.z = p3[2] - p1[2];

    //Vector3f::cross(vec3, vec1, vec2);
    vec3.x = vec1.y * vec2.z - vec1.z * vec2.y;
    vec3.y = vec1.z * vec2.x - vec1.x * vec2.z;
    vec3.z = vec1.x * vec2.y - vec1.y * vec2.x;
    qDebug() << vec1.x << " " << vec1.y <<" " << vec1.z;
    qDebug() << vec2.x << " " << vec2.y <<" " << vec2.z;
    qDebug() << vec3.x << " " << vec3.y <<" " << vec3.z;
    float vec3d = vec3.x*p1[0] + vec3.y*p1[1] + vec3.z*p1[2];
    vec3.x /= vec3.y;
    vec3.z /= vec3.y;
    vec3d /= vec3.y;
    
    // end of calculating plane equation
    
    for(int i = 0; i < length; i+=3 ){
        float h = vec3d - vec3.x*punkty[i] - vec3.z*punkty[i+2];
        qDebug() << punkty[i] << " " << punkty[i+1] <<" " << punkty[i+2] <<" "<<h <<"";
    }
    //qDebug() << p1[0] << " " << p1[1] <<" "<<p1[2] <<"";
    //qDebug() << p2[0] << " " << p2[1] <<" "<<p2[2] <<"";
    //qDebug() << p3[0] << " " << p3[1] <<" "<<p3[2] <<"";
    
    // use equation
    int xxf, zzf;
    int xxc, zzc;
    int xx, zz;
    float h; 

    float diffC = 0;
    float diffE = 0;
    int iis, jjs;
    for(int ii = -brush->eRadius; ii < brush->eRadius; ii++)
        for(int jj = -brush->eRadius; jj < brush->eRadius; jj++)
            for(int i = 0; i< length; i+=3){
                xx = floor((float)punkty[i]/8.0);
                zz = floor((float)punkty[i+2]/8.0);
                xx += ii;
                zz += jj;
                if(ii <= -brush->eSize) 
                    iis = ii+brush->eSize-1;
                else if(ii >= brush->eSize) 
                    iis = ii-brush->eSize;
                else
                    iis = 0;
                if(jj <= -brush->eSize) 
                    jjs = jj+brush->eSize-1;
                else if(jj >= brush->eSize) 
                    jjs = jj-brush->eSize;
                else
                    jjs = 0;
                h = vec3d - vec3.x*xx*8 - vec3.z*zz*8;
                if(sqrt(ii*ii + jj*jj) > brush->eRadius) continue;
                diffC = sqrt(iis*iis + jjs*jjs)*brush->eCut;
                diffE = sqrt(iis*iis + jjs*jjs)*brush->eEmb;
                //qDebug() << diffC <<" "<<diffE;
                uterr.insert(setHeight256(tx, tz, xx*8, zz*8, h, diffC, diffE));
            }
    
    /*for(int j = 0; j < brush->eSize; j++)
        for(int i = 0; i< length; i+=3){
            xxf = floor((float)punkty[i]/8.0 -1*j);
            zzf = floor((float)punkty[i+2]/8.0 -1*j);
            xxc = ceil((float)punkty[i]/8.0 +1*j);
            zzc = ceil((float)punkty[i+2]/8.0 +1*j);
            //xx+=ii;
            //zz+=jj;
            h = vec3d - vec3.x*xxf*8 - vec3.z*zzf*8;
            uterr.insert(setHeight256(tx, tz, xxf*8, zzf*8, h));
            h = vec3d - vec3.x*xxc*8 - vec3.z*zzf*8;
            uterr.insert(setHeight256(tx, tz, xxc*8, zzf*8, h));
            h = vec3d - vec3.x*xxf*8 - vec3.z*zzc*8;
            uterr.insert(setHeight256(tx, tz, xxf*8, zzc*8, h));
            h = vec3d - vec3.x*xxc*8 - vec3.z*zzc*8;
            uterr.insert(setHeight256(tx, tz, xxc*8, zzc*8, h));
            //qDebug() << xx << " " << zz << " " << h;
        }*/
    
    Terrain *terr;
    for (std::set<int>::iterator it = uterr.begin(); it != uterr.end(); ++it) {
        if(*it == -1) continue;
        //console.log(obj.type);
        terr = terrain[(int)*it];
        terr->setModified(true);
        terr->refresh();
    }
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

void TerrainLib::setWaterDraw(int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setWaterDraw(x, z, posx, posz);
}

void TerrainLib::setTileBlob(int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setTileBlob();
}

void TerrainLib::setWaterLevelGui(int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setWaterLevelGui();
}

void TerrainLib::setDraw(int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setDraw(x, z, posx, posz);
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

void TerrainLib::lockTexture(Brush* brush, int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->lockTexture(brush, x, z, posx, posz);
}

void TerrainLib::setFixedTileHeight(Brush* brush, int x, int z, float* p){
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++) {
            terr->terrainData[i][j] = brush->hFixed;
        }
    terr->refresh();
    terr->setModified(true);
}

void TerrainLib::paintHeightMap(Brush* brush, int x, int z, float* p){
    
    std::set<int> uterr;
    
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    //terr->paintTexture(x, z, posx, posz);
    
    //int px = (posx + 1024)/8;
    //int pz = (posz + 1024)/8;
    int px = posx;
    int pz = posz;
    float size = brush->size;
    float h = 0;
    float rd = 0;
    float hAvg = 0;
    int tx, tz;
    int tpx, tpz;
    int count = 0;
    
    h = brush->alpha*brush->direction*10.0;
    if(brush->hType == 1){
        terr->terrainData[(pz+1024)/8][(px+1024)/8] += h;
        //float rh = brush->alpha*brush->direction*10.0;
        rd = terr->terrainData[(pz+1024)/8][(px+1024)/8];
    }
    if(brush->hType == 2){
        hAvg = brush->hFixed;
    }
    if(brush->hType == 3){
        for(int i = -size; i < size; i++)
            for(int j = -size; j < size; j++){
                tpx = px+i*8;
                tpz = pz+j*8;
                tx = x;
                tz = z;
                Game::check_coords(tx, tz, tpx, tpz);
                terr = terrain[(tx * 10000 + tz)];
                if (terr == NULL) continue;
                if (!terr->loaded) continue;
                tpx = (tpx + 1024)/8;
                tpz = (tpz + 1024)/8;
                hAvg += terr->terrainData[tpz][tpx];
                count++;
            }
        hAvg /= count;
    }
    
    for(int i = -size; i < size; i++)
        for(int j = -size; j < size; j++){
            if(brush->hType == 1)
                if(i == 0 && j == 0) continue;
            //if(px+i >= 256) continue;
            //if(pz+j >= 256) continue;
            //if(px+i < 0) continue;
            //if(pz+j < 0) continue;
            //h = (fabs(i) + fabs(j))/2.0;
            tx = x;
            tz = z;
            tpx = px+i*8;
            tpz = pz+j*8;
            Game::check_coords(tx, tz, tpx, tpz);
            terr = terrain[(tx * 10000 + tz)];
            if (terr == NULL) continue;
            if (!terr->loaded) continue;
            uterr.insert(tx * 10000 + tz);
            
            if(sqrt(i*i + j*j) > size) continue;
            h = (float)(size - (sqrt(i*i + j*j)))/size;
            h = h*brush->alpha*brush->direction*10.0;
            
            terr->setErrorBias(tpx, tpz, 0);
            tpz = (tpz + 1024)/8;
            tpx = (tpx + 1024)/8;
            if(brush->hType == 0){
                    terr->terrainData[tpz][tpx] += h;
            } else if(brush->hType == 1){
                if(h < 0){
                    if(terr->terrainData[tpz][tpx] > rd)
                        terr->terrainData[tpz][tpx] += h;
                }
                if(h > 0){
                    if(terr->terrainData[tpz][tpx] < rd)
                        terr->terrainData[tpz][tpx] += h;
                }
            } else if(brush->hType == 2 || brush->hType == 3){
                if(terr->terrainData[tpz][tpx] >hAvg){
                    terr->terrainData[tpz][tpx] -= h*brush->direction;
                    if(terr->terrainData[tpz][tpx] < hAvg)
                        terr->terrainData[tpz][tpx] = hAvg;
                }
                if(terr->terrainData[tpz][tpx] < hAvg){
                    terr->terrainData[tpz][tpx] += h*brush->direction;
                    if(terr->terrainData[tpz][tpx] > hAvg)
                        terr->terrainData[tpz][tpx] = hAvg;
                }
            }
        }
    
    for (std::set<int>::iterator it = uterr.begin(); it != uterr.end(); ++it) {
        //console.log(obj.type);
        terr = terrain[(int)*it];
        terr->setModified(true);
        terr->refresh();
    }
    //terr->setModified(true);
    //terr->refresh();
    
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
    int mintile = -Game::tileLod;
    int maxtile = Game::tileLod;

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
            tTile = terrain[(((int)playerT[0] + i)*10000 + (int)playerT[1] + j)];
            
            if (tTile == NULL) {
                terrain[((int)playerT[0] + i)*10000 + (int)playerT[1] + j] = new Terrain((int)playerT[0] + i, (int)playerT[1] + j);
            }
            //} catch (const std::out_of_range& oor) {
            //    terrain[(playerT[0] + i)*10000 + playerT[1] + j] = new Terrain(playerT[0] + i, playerT[1] + j);
            //}
            tTile = terrain[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
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
            tTile = terrain[(((int)playerT[0] + i)*10000 + (int)playerT[1] + j)];
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