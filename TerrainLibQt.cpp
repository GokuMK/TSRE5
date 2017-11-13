/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TerrainLibQt.h"
#include "Terrain.h"
#include "GLMatrix.h"
#include <QOpenGLShaderProgram>
#include <set>
#include <math.h>
#include "Game.h"
#include "Brush.h"
#include "HeightWindow.h"
#include "QuadTree.h"
#include "Undo.h"
#include "Route.h"
#include "Environment.h"
#include "TerrainInfo.h"

TerrainLibQt::TerrainLibQt() {
}

TerrainLibQt::TerrainLibQt(const TerrainLibQt& orig) {
}

TerrainLibQt::~TerrainLibQt() {
}

Terrain* TerrainLibQt::getTerrainByXY(int x, int y, bool load) {
    QString terrainName = quadTree->getMyName((int) x, -y);
    if (terrainName == "")
        return NULL;
    if (terrainQt[terrainName] != NULL) {
        return terrainQt[terrainName]->t;
    }
    if (load) {
        terrainQt[terrainName] = new TerrainInfo();
        quadTree->fillTerrainInfo(x, -y, terrainQt[terrainName]);
        qDebug() << terrainName;
        terrainQt[terrainName]->t = new Terrain(terrainQt[terrainName]);
        return terrainQt[terrainName]->t;
    }

    return NULL;
}

void TerrainLibQt::loadQuadTree() {
    quadTree = new QuadTree();
    quadTree->load();

    quadTreeLo = new QuadTree(true);
    quadTreeLo->load();

    //quadTree->listNames();
}

void TerrainLibQt::createNewRouteTerrain(int x, int z) {
    Terrain::saveEmpty(x, z);
    quadTree = new QuadTree();
    quadTree->createNew(x, z);
}

void TerrainLibQt::saveEmpty(int x, int z) {
    Terrain::saveEmpty(x, z);
    quadTree->addTile(x, z);
}

bool TerrainLibQt::isLoaded(int x, int z) {
    QString terrainName = quadTree->getMyName((int) x, -z);
    if (terrainName == "")
        return false;
    if (terrainQt[terrainName] == NULL)
        return false;
    if (terrainQt[terrainName]->t == NULL)
        return false;
    if (terrainQt[terrainName]->t->loaded == false)
        return false;
    return true;
}

bool TerrainLibQt::load(int x, int z) {
    Terrain *t = getTerrainByXY(x, z, true);
    if (t == NULL)
        return false;
    if (t->loaded == false)
        return false;
    return true;
}

void TerrainLibQt::getUnsavedInfo(std::vector<QString> &items) {
    if (!Game::writeEnabled) return;
    QHashIterator<QString, TerrainInfo*> i(terrainQt);
    while (i.hasNext()) {
        i.next();
        Terrain* tTile = (Terrain*) i.value();
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            items.push_back("[T] "+QString::number(tTile->mojex)+" "+QString::number(-tTile->mojez));
        }
    }
}

void TerrainLibQt::save() {
    if (!Game::writeEnabled) return;
    qDebug() << "save terrain";
    QHashIterator<QString, TerrainInfo*> i(terrainQt);
    while (i.hasNext()) {
        i.next();
        Terrain* tTile = (Terrain*) i.value();
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            tTile->save();
            tTile->setModified(false);
        }
    }
}

bool TerrainLibQt::reload(int x, int z) {
    QString terrainName = quadTree->getMyName((int) x, -z);
    if (terrainName == "")
        return false;

    terrainQt[terrainName] = new TerrainInfo();
    quadTree->fillTerrainInfo(x, -z, terrainQt[terrainName]);
    terrainQt[terrainName]->t = new Terrain(terrainQt[terrainName]);
    if (terrainQt[terrainName]->t->loaded)
        return true;
    return false;
}

float TerrainLibQt::getHeight(int x, int z, float posx, float posz) {
    return TerrainLibQt::getHeight(x, z, posx, posz, false);
}

void TerrainLibQt::refresh(int x, int z) {
    Terrain *terr = this->getTerrainByXY(x, z);

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->refresh();
}

void TerrainLibQt::setHeight(int x, int z, float posx, float posz, float h) {
    Game::check_coords(x, z, posx, posz);
    Terrain *terr = this->getTerrainByXY(x, z);

    if (terr == NULL) return;
    if (terr->loaded == false) return;
    
    terr->setHeight(x, z, posx, posz, float h);
}

int TerrainLibQt::setHeight256(int x, int z, int posx, int posz, float h) {
    return setHeight256(x, z, posx, posz, h, 0, 0);
}

int TerrainLibQt::setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE) {
    /*Game::check_coords(x, z, posx, posz);
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
    terr->setErrorBias(x, z, posx, posz, 0);
    terr->setModified(true);
    
    return x * 10000 + z;*/
}

float TerrainLibQt::getHeight(int x, int z, float posx, float posz, bool addR) {
    Game::check_coords(x, z, posx, posz);

    Terrain *terr = getTerrainByXY(x, z, false);

    if (terr == NULL) return -1;
    if (terr->loaded == false) return -1;

    return terr->getHeight(x, z, posx, posz, addR);
}

void TerrainLibQt::fillHeightMap(int x, int z, float* data) {
    Terrain *terr = getTerrainByXY(x, z, false);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->fillHeightMap(data);
}

void TerrainLibQt::getRotation(float* rot, int x, int z, float posx, float posz) {
    Game::check_coords(x, z, posx, posz);
    rot[0] = 0;
    rot[1] = 0;
    
    Terrain *terr = getTerrainByXY(x, z, false);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->getRotation(rot, x, z, posx, posz);
    return;
}

void TerrainLibQt::setHeightFromGeoGui(int x, int z, float* p) {
    /*if(heightWindow == NULL)
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
    }*/
}

void TerrainLibQt::setHeightFromGeo(int x, int z, float* p) {
    /*if(heightWindow == NULL)
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
    heightWindow->load(false);
    if(heightWindow->ok){
        qDebug() << "ok";
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                terr->terrainData[i][j] = heightWindow->terrainData[j][i];
            }
        }
        terr->setModified(true);
        //terr->refresh();
        terr = terrain[(x * 10000 + z + 1)];
        if (terr != NULL) terr->refresh();
        terr = terrain[(x * 10000 + z - 1)];
        if (terr != NULL) terr->refresh();
        terr = terrain[((x+1) * 10000 + z)];
        if (terr != NULL) terr->refresh();
        terr = terrain[((x-1) * 10000 + z)];
        if (terr != NULL) terr->refresh();
    }*/
}

void TerrainLibQt::setTextureToTrackObj(Brush* brush, float* punkty, int length, int tx, int tz) {
    float posx, posz;
    int ttx, ttz;
    for(int i = 0; i < length; i+=3 ){
        posx = punkty[i];
        posz = punkty[i+2];
        ttx = tx;
        ttz = tz;
        Game::check_coords(ttx, ttz, posx, posz);
        
        Terrain *terr = this->getTerrainByXY(ttx, ttz);
        if (terr == NULL) continue;
        if (terr->loaded == false) continue;
        terr->paintTexture(brush, ttx, ttz, posx, posz);
    }
}

void TerrainLibQt::setTerrainToTrackObj(Brush* brush, float* punkty, int length, int tx, int tz, float* matrix) {
    /*std::set<int> uterr;
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
    
    //set to undo
    int ttx, ttz;
    Terrain *terr = NULL;
    for(int ii = -brush->eRadius; ii < brush->eRadius; ii++)
        for(int jj = -brush->eRadius; jj < brush->eRadius; jj++){
            if(sqrt(ii*ii + jj*jj) > brush->eRadius) continue;
            for(int i = 0; i< length; i+=3){
                xx = floor((float)punkty[i]/8.0) + ii;
                zz = floor((float)punkty[i+2]/8.0) + jj;
                xx *= 8;
                zz *= 8;
                ttx = tx;
                ttz = tz;
                Game::check_coords(ttx, ttz, xx, zz);
                if (terr != terrain[(ttx * 10000 + ttz)]){
                    terr = terrain[(ttx * 10000 + ttz)];
                    if (terr == NULL) continue;
                    if (terr->loaded == false) continue;
                    Undo::PushTerrainHeightMap(terr->mojex, terr->mojez, terr->terrainData, terr->getSampleCount());
                }
            }
        }
    //0
    
    for(int ii = -brush->eRadius; ii < brush->eRadius; ii++)
        for(int jj = -brush->eRadius; jj < brush->eRadius; jj++){
            if(sqrt(ii*ii + jj*jj) > brush->eRadius) continue;
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
                //
                diffC = sqrt(iis*iis + jjs*jjs)*brush->eCut;
                diffE = sqrt(iis*iis + jjs*jjs)*brush->eEmb;
                //qDebug() << diffC <<" "<<diffE;
                uterr.insert(setHeight256(tx, tz, xx*8, zz*8, h, diffC, diffE));
            }
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

    //Terrain *terr;
    /*for (std::set<int>::iterator it = uterr.begin(); it != uterr.end(); ++it) {
        if(*it == -1) continue;
        //console.log(obj.type);
        terr = terrain[(int)*it];
        terr->setModified(true);
        terr->refresh();
    }*/
}

void TerrainLibQt::setTerrainTexture(Brush* brush, int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setTexture(brush, x, z, posx, posz);
}

void TerrainLibQt::toggleWaterDraw(int x, int z, float* p, float direction) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->toggleWaterDraw(x, z, posx, posz, direction);
}

void TerrainLibQt::makeTextureFromMap(int x, int z, float* p) {
    /*float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->makeTextureFromMap();*/
}

void TerrainLibQt::removeTileTextureFromMap(int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->removeTextureFromMap();
}

void TerrainLibQt::setTileBlob(int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setTileBlob();
}

void TerrainLibQt::setWaterLevelGui(int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->setWaterLevelGui();
}

void TerrainLibQt::toggleDraw(int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->toggleDraw(x, z, posx, posz);
}

int TerrainLibQt::getTexture(int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return -1;
    if (terr->loaded == false) return -1;
    return terr->getTexture(x, z, posx, posz);
}

void TerrainLibQt::paintTexture(Brush* brush, int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->paintTexture(brush, x, z, posx, posz);
}

void TerrainLibQt::lockTexture(Brush* brush, int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->lockTexture(brush, x, z, posx, posz);
}

void TerrainLibQt::toggleGaps(int x, int z, float* p, float direction) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->toggleGaps(x, z, posx, posz, direction);
}

void TerrainLibQt::setFixedTileHeight(Brush* brush, int x, int z, float* p) {
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    Undo::PushTerrainHeightMap(terr->mojex, terr->mojez, terr->terrainData, terr->getSampleCount());
    terr->setFixedHeight(brush->hFixed);
}

QSet<int> TerrainLibQt::paintHeightMap(Brush* brush, int x, int z, float* p) {

    QSet<int> uterr;
    /*
    float posx = round(p[0]/8.0)*8.0;
    float posz = round(p[2]/8.0)*8.0;
    
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = terrain[(x * 10000 + z)];
    if (terr == NULL) return uterr;
    if (terr->loaded == false) return uterr;
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
    
    // add tiles that can be modified to undo;
    Undo::PushTerrainHeightMap(terr->mojex, terr->mojez, terr->terrainData, terr->getSampleCount());
    for(int i = -size; i < size; i++)
        for(int j = -size; j < size; j++){
            tpx = px+i*8;
            tpz = pz+j*8;
            tx = x;
            tz = z;
            Game::check_coords(tx, tz, tpx, tpz);
            if(terr != terrain[(tx * 10000 + tz)]){
                terr = terrain[(tx * 10000 + tz)];
                if (terr == NULL) continue;
                if (!terr->loaded) continue;
                Undo::PushTerrainHeightMap(terr->mojex, terr->mojez, terr->terrainData, terr->getSampleCount());
            }
        }
    //
    terr = terrain[(x * 10000 + z)];
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
            
            terr->setErrorBias(tx, tz, tpx, tpz, 0);
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
    
    foreach (int value, uterr){
    //for (std::set<int>::iterator it = uterr.begin(); it != uterr.end(); ++it) {
        //console.log(obj.type);
        terr = terrain[value];
        terr->setModified(true);
        terr->refresh();
    }
    //terr->setModified(true);
    //terr->refresh();*/
    return uterr;
}

void TerrainLibQt::fillWaterLevels(float *w, int mojex, int mojez) {
    /*Terrain *tTile;
    
    tTile = terrain[((mojex - 1)*10000 + mojez - 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            w[0] = tTile->getWaterLevelSE();
        }
    tTile = terrain[((mojex)*10000 + mojez - 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            w[1] = tTile->getWaterLevelSW();
            w[2] = tTile->getWaterLevelSE();
        }
    tTile = terrain[((mojex + 1)*10000 + mojez - 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            w[3] = tTile->getWaterLevelSW();
        }
    tTile = terrain[((mojex - 1)*10000 + mojez)];
    if (tTile != NULL) 
        if(tTile->loaded){
            w[4] = tTile->getWaterLevelNE();
            w[6] = tTile->getWaterLevelSE();
        }
    tTile = terrain[((mojex + 1)*10000 + mojez)];
    if (tTile != NULL)
        if(tTile->loaded){
            w[5] = tTile->getWaterLevelNW();
            w[7] = tTile->getWaterLevelSW();
        }
    tTile = terrain[((mojex - 1)*10000 + mojez + 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            w[8] = tTile->getWaterLevelNE();
        }
    tTile = terrain[((mojex)*10000 + mojez + 1)];
    if (tTile != NULL) 
        if(tTile->loaded){
            w[9] = tTile->getWaterLevelNW();
            w[10] = tTile->getWaterLevelNE();
        }
    tTile = terrain[((mojex + 1)*10000 + mojez + 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            w[11] = tTile->getWaterLevelNW();
        }*/
}

void TerrainLibQt::setWaterLevels(float *w, int mojex, int mojez) {
    /*Terrain *tTile;
    
    tTile = terrain[((mojex - 1)*10000 + mojez - 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelSE(w[0]);
            tTile->refreshWaterShapes();
        }
    tTile = terrain[((mojex)*10000 + mojez - 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelSW(w[1]);
            tTile->setWaterLevelSE(w[2]);
            tTile->refreshWaterShapes();
        }
    tTile = terrain[((mojex + 1)*10000 + mojez - 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelSW(w[3]);
            tTile->refreshWaterShapes();
        }
    tTile = terrain[((mojex - 1)*10000 + mojez)];
    if (tTile != NULL) 
        if(tTile->loaded){
            tTile->setWaterLevelNE(w[4]);
            tTile->setWaterLevelSE(w[6]);
            tTile->refreshWaterShapes();
        }
    tTile = terrain[((mojex + 1)*10000 + mojez)];
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelNW(w[5]);
            tTile->setWaterLevelSW(w[7]);
            tTile->refreshWaterShapes();
        }
    tTile = terrain[((mojex - 1)*10000 + mojez + 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelNE(w[8]);
            tTile->refreshWaterShapes();
        }
    tTile = terrain[((mojex)*10000 + mojez + 1)];
    if (tTile != NULL) 
        if(tTile->loaded){
            tTile->setWaterLevelNW(w[9]);
            tTile->setWaterLevelNE(w[10]);
            tTile->refreshWaterShapes();
        }
    tTile = terrain[((mojex + 1)*10000 + mojez + 1)];
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelNW(w[11]);
            tTile->refreshWaterShapes();
        }*/
}

void TerrainLibQt::fillRaw(float** terrainData, int mojex, int mojez) {
    /*Terrain *tTile;
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

void TerrainLibQt::renderWater(GLUU* gluu, float* playerT, float* playerW, float* target, float fov, int renderMode, int layer) {
    int mintile = -Game::tileLod;
    int maxtile = Game::tileLod;

    if (renderMode == gluu->RENDER_SELECTION) {
        mintile = -1;
        maxtile = 1;
    }

    gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, 0.0f);
    gluu->enableNormals();

    Terrain *tTile;
    int selectionColor = 0;
    int i = 0, j = 0;
    QHash<QString, bool> rendered;
    for (int n = -1; n < (Game::tileLod * 2 + 1)*(Game::tileLod * 2 + 1); n++) {
        if (n != -1)
            spiralLoop(n, i, j);

        tTile = getTerrainByXY((int) playerT[0] + i, (int) playerT[1] + j, true);
        if(tTile == NULL)
            continue;
        if (tTile->loaded == false)
            continue;
        if (rendered[tTile->name])
            continue;
        rendered[tTile->name] = true;

        if (tTile->loaded) {
            float lodx = 2048 * i - playerW[0];
            float lodz = 2048 * j - playerW[2];
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, Game::currentRoute->env->water[layer].height, 2048 * j);
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            if (renderMode == gluu->RENDER_SELECTION) {
                selectionColor = 10 << 20;
                selectionColor |= ((i + 1) << 10);
                selectionColor |= ((j + 1) << 8);
            }
            tTile->renderWater(lodx, lodz, playerT[0] + i, playerT[1] + j, playerW, target, fov, layer, selectionColor);
            gluu->mvPopMatrix();
        }
    }
}

void TerrainLibQt::renderShadowMap(GLUU *gluu, float * playerT, float* playerW, float* target, float fov) {
    gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, 0.0f);
    gluu->enableNormals();

    Terrain *tTile;
    int i = 0, j = 0;
    QHash<QString, bool> rendered;
    for (int n = -1; n < 9; n++) {
        if (n != -1)
            spiralLoop(n, i, j);

        tTile = getTerrainByXY((int) playerT[0] + i, (int) playerT[1] + j, true);
        if(tTile == NULL)
            continue;
        if (tTile->loaded == false)
            continue;
        if (rendered[tTile->name])
            continue;
        rendered[tTile->name] = true;
        
        if (tTile->loaded) {
            float lodx = 2048 * i - playerW[0];
            float lodz = 2048 * j - playerW[2];
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            tTile->render(lodx, lodz, playerT[0]+i, playerT[1]+j, playerW, target, fov, 1);
            gluu->mvPopMatrix();
        }
    }
}

void TerrainLibQt::renderEmpty(GLUU *gluu, float * playerT, float* playerW, float* target, float fov) {
    int i = 0, j = 0;
    for (int n = -1; n < 9; n++) {
        if (n != -1)
            spiralLoop(n, i, j);
        getTerrainByXY((int) playerT[0] + i, (int) playerT[1] + j, true);
    }
}

void TerrainLibQt::render(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, int renderMode) {
    int renderCount = (Game::tileLod * 2 + 1)*(Game::tileLod * 2 + 1);
    if (renderMode == gluu->RENDER_SELECTION)
        renderCount = 9;

    gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, 0.0f);
    gluu->enableNormals();

    Terrain *tTile;
    int selectionColor = 0;
    int i = 0, j = 0;
    QHash<QString, bool> rendered;
    for (int n = -1; n < renderCount; n++) {
        if (n != -1)
            spiralLoop(n, i, j);

        tTile = getTerrainByXY((int) playerT[0] + i, (int) playerT[1] + j, true);
        if(tTile == NULL)
            continue;
        if (tTile->loaded == false)
            continue;
        if (rendered[tTile->name])
            continue;
        rendered[tTile->name] = true;

        if (tTile->loaded) {
            float lodx = 2048 * i - playerW[0];
            float lodz = 2048 * j - playerW[2];
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
            if (renderMode == gluu->RENDER_SELECTION) {
                selectionColor = 10 << 20;
                selectionColor |= ((i + 1) << 10);
                selectionColor |= ((j + 1) << 8);
            }
            tTile->render(lodx, lodz, playerT[0] + i, playerT[1] + j, playerW, target, fov, selectionColor);
            gluu->mvPopMatrix();
        }
    }
}

void TerrainLibQt::renderLo(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, int renderMode) {
    int mintile = -20;
    int maxtile = 20;

    if (renderMode == gluu->RENDER_SELECTION) {
        mintile = -1;
        maxtile = 1;
    }

    gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, 0.0f);
    gluu->enableNormals();

    Terrain *tTile;
    int selectionColor = 0;
    QString terrainName;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {

            terrainName = quadTreeLo->getMyName((int) playerT[0] + i, -(int) playerT[1] - j);
            if (terrainName == "")
                continue;
            if (terrainQtLo[terrainName] == NULL) {
                terrainQtLo[terrainName] = new TerrainInfo();
                quadTreeLo->fillTerrainInfo((int) playerT[0] + i, -(int) playerT[1] - j, terrainQtLo[terrainName]);
                qDebug() << terrainName;
                terrainQtLo[terrainName]->t = new Terrain(terrainQtLo[terrainName]);
            }
            if (terrainQtLo[terrainName]->rendered)
                continue;
            terrainQtLo[terrainName]->rendered = true;
            tTile = terrainQtLo[terrainName]->t;

            if (tTile->loaded == false)
                continue;

            if (tTile->loaded) {
                float lodx = 2048 * i - playerW[0];
                float lodz = 2048 * j - playerW[2];
                gluu->mvPushMatrix();
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
                if (renderMode == gluu->RENDER_SELECTION) {
                    selectionColor = 10 << 20;
                    selectionColor |= ((i + 1) << 10);
                    selectionColor |= ((j + 1) << 8);
                }
                tTile->render(lodx, lodz, playerT[0] + i, playerT[1] + j, playerW, target, fov, selectionColor);
                gluu->mvPopMatrix();
            }
        }
    }

    QHashIterator<QString, TerrainInfo*> i(terrainQtLo);
    while (i.hasNext()) {
        i.next();
        if (i.value() == NULL) continue;
        i.value()->rendered = false;
    }
}

void TerrainLibQt::spiralLoop(int n, int &x, int &y) {
    int r = floor((sqrt(n + 1) - 1) / 2) + 1;
    int p = (8 * r * (r - 1)) / 2;
    int en = r * 2;
    int a = (1 + n - p) % (r * 8);

    switch ((int) (floor((float) a / (r * 2)))) {
        case 0:
            x = a - r;
            y = -r;
            break;
        case 1:
            x = r;
            y = (a % en) - r;
            break;
        case 2:
            x = r - (a % en);
            y = r;
            break;
        case 3:
            x = -r;
            y = r - (a % en);
            break;
    }
}