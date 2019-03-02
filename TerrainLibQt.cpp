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

void TerrainLibQt::setDetailedAsCurrent(){
    currentQt = &terrainQt;
    currentQuadTree = quadTree;
}

void TerrainLibQt::setDistantAsCurrent(){
    currentQt = &terrainQtLo;
    currentQuadTree = quadTreeLo;
}

Terrain* TerrainLibQt::getTerrainByXY(int x, int y, bool load) {
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
        qDebug() << terrainNameId;
        (*currentQt)[terrainNameId]->t = new Terrain((*currentQt)[terrainNameId]);
        return (*currentQt)[terrainNameId]->t;
    }

    return NULL;
}

void TerrainLibQt::loadQuadTree() {
    quadTree = new QuadTree();
    quadTree->load();

    quadTreeLo = new QuadTree(true);
    quadTreeLo->load();

    //currentQt = &terrainQtLo;
    //currentQuadTree = quadTreeLo;
    
    //quadTree->listNames();
}

void TerrainLibQt::createNewRouteTerrain(int x, int z) {
    currentQuadTree = new QuadTree();
    currentQuadTree->createNew(x, z);
    QString name = currentQuadTree->getMyName(x, z);
    Terrain::SaveEmpty(name);
}

void TerrainLibQt::saveEmpty(int x, int z) {
    qDebug() << "#new tile add to QT ";
    currentQuadTree->addTile(x, z);
    qDebug() << "#new tile get name ";
    QString name = currentQuadTree->getMyName(x, z);
    qDebug() << "#new tile Gen "<<name;
    if(currentQuadTree->isLow())
        Terrain::SaveEmpty(name, 256, 128, 16, true);
    else
        Terrain::SaveEmpty(name);
}

bool TerrainLibQt::isLoaded(int x, int z) {
    unsigned int terrainNameId = quadTree->getMyNameId((int) x, -z);
    if (terrainNameId == 0)
        return false;
    if ((*currentQt)[terrainNameId] == NULL)
        return false;
    if ((*currentQt)[terrainNameId]->t == NULL)
        return false;
    if ((*currentQt)[terrainNameId]->t->loaded == false)
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

void TerrainLibQt::getUnsavedInfo(QVector<QString> &items) {
    if (!Game::writeEnabled) return;
    QHashIterator<unsigned int, TerrainInfo*> i(terrainQt);
    while (i.hasNext()) {
        i.next();
        if (i.value() == NULL) continue;
        Terrain* tTile = (Terrain*) i.value()->t;
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            items.push_back("[T] "+QString::number(tTile->mojex)+" "+QString::number(-tTile->mojez));
        }
    }
    QHashIterator<unsigned int, TerrainInfo*> i2(terrainQtLo);
    while (i2.hasNext()) {
        i2.next();
        if (i2.value() == NULL) continue;
        Terrain* tTile = (Terrain*) i2.value()->t;
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            items.push_back("[T] "+QString::number(tTile->mojex)+" "+QString::number(-tTile->mojez));
        }
    }
}

void TerrainLibQt::save() {
    if (!Game::writeEnabled) return;
    qDebug() << "save terrain";
    QHashIterator<unsigned int, TerrainInfo*> i(terrainQt);
    while (i.hasNext()) {
        i.next();
        if (i.value() == NULL) continue;
        Terrain* tTile = (Terrain*) i.value()->t;
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            tTile->save();
            tTile->setModified(false);
        }
    }
    qDebug() << "save lo terrain";
    QHashIterator<unsigned int, TerrainInfo*> i2(terrainQtLo);
    while (i2.hasNext()) {
        i2.next();
        if (i2.value() == NULL) continue;
        Terrain* tTile = (Terrain*) i2.value()->t;
        if (tTile == NULL) continue;
        if (tTile->loaded && tTile->isModified()) {
            tTile->save();
            tTile->setModified(false);
        }
    }
}

bool TerrainLibQt::reload(int x, int z) {
    unsigned int terrainNameId = currentQuadTree->getMyNameId((int) x, -z);
    if (terrainNameId == 0)
        return false;

    (*currentQt)[terrainNameId] = new TerrainInfo();
    currentQuadTree->fillTerrainInfo(x, -z, (*currentQt)[terrainNameId]);
    (*currentQt)[terrainNameId]->t = new Terrain((*currentQt)[terrainNameId]);
    if ((*currentQt)[terrainNameId]->t->loaded)
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
    
    terr->setHeight(x, z, posx, posz, h);
}

Terrain* TerrainLibQt::setHeight256(int x, int z, int posx, int posz, float h) {
    return setHeight256(x, z, posx, posz, h, 0, 0);
}

Terrain* TerrainLibQt::setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE) {
    Game::check_coords(x, z, posx, posz);
    Terrain *terr = getTerrainByXY(x, z);

    if (terr == NULL) return NULL;
    if (terr->loaded == false) return NULL;

    float lx = posx, lz = posz;
    terr->getLocalCoords(x, z, lx, lz);
    int sampleSize = terr->getSampleSize();
    posx = lx / sampleSize;
    posz = lz / sampleSize;
    
    if(diffC == 0 && diffE == 0){
        terr->terrainData[(posz)][(posx)] = h;
    } else {
        if(terr->terrainData[(posz)][(posx)] < h)
            if(terr->terrainData[(posz)][(posx)] < h - diffE) 
                terr->terrainData[(posz)][(posx)] = h - diffE;
        if(terr->terrainData[(posz)][(posx)] > h)
            if(terr->terrainData[(posz)][(posx)] > h + diffC) 
                terr->terrainData[(posz)][(posx)] = h + diffC;
    }
    terr->setErrorBias(x, z, posx, posz, 0);
    terr->setModified(true);
    
    return terr;
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
    if(heightWindow == NULL)
        heightWindow = new HeightWindow();
    
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;

    int X, Y;
    terr->getLowCornerTileXY(X, Y);
    heightWindow->tileX = X;
    heightWindow->tileZ = -Y;
    heightWindow->ok = false;
    int samples = terr->getSampleCount();
    heightWindow->terrainResolution = samples;
    heightWindow->terrainSize = terr->getSampleCount()*terr->getSampleSize();
    heightWindow->exec();
    if(heightWindow->ok){
        qDebug() << "ok";
        for (int i = 0; i < samples; i++) {
            for (int j = 0; j < samples; j++) {
                terr->terrainData[i][j] = heightWindow->terrainData[j][i];
            }
        }
        terr->setModified(true);
        int X, Y;
        for(int i = -1; i <= 1; i++)
            for(int j = -1; j<= 1; j++){
                terr->getCornerCoordsXY(X, Y, i, j);
                Terrain* tterr  = getTerrainByXY(X, Y);
                if (tterr != NULL) 
                    tterr->refresh();
            }
    }
}

void TerrainLibQt::setHeightFromGeo(int x, int z, float* p) {
    if(heightWindow == NULL)
        heightWindow = new HeightWindow();
    
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    int X, Y;
    terr->getLowCornerTileXY(X, Y);
    heightWindow->tileX = X;
    heightWindow->tileZ = -Y;
    heightWindow->ok = false;
    int samples = terr->getSampleCount();
    heightWindow->terrainResolution = samples;
    heightWindow->terrainSize = terr->getSampleCount()*terr->getSampleSize();
    heightWindow->load(false);
    if(heightWindow->ok){
        qDebug() << "ok";
        for (int i = 0; i < samples; i++) {
            for (int j = 0; j < samples; j++) {
                terr->terrainData[i][j] = heightWindow->terrainData[j][i];
            }
        }
        terr->setModified(true);
        //terr->refresh();
        int X, Y;
        terr->getCornerCoordsXY(X, Y, 0, 1);
        Terrain* tterr;
        tterr = getTerrainByXY(X, Y);
        if (tterr != NULL) 
            tterr->refresh();
        terr->getCornerCoordsXY(X, Y, 0, -1);
        tterr = getTerrainByXY(X, Y);
        if (tterr != NULL) 
            tterr->refresh();
        terr->getCornerCoordsXY(X, Y, 1, 0);
        tterr = getTerrainByXY(X, Y);
        if (tterr != NULL) 
            tterr->refresh();
        terr->getCornerCoordsXY(X, Y, -1, 0);
        tterr = getTerrainByXY(X, Y);
        if (tterr != NULL) 
            tterr->refresh();
    }
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
        if (terr == NULL)
            continue;
        if (terr->loaded == false) continue;
        terr->paintTexture(brush, ttx, ttz, posx, posz);
    }
}

void TerrainLibQt::setTerrainToTrackObj(Brush* brush, float* punkty, int length, int tx, int tz, float* matrix, float offsetY) {
    QSet<Terrain*> uterr;
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
                if(terr != getTerrainByXY(ttx, ttz)){
                    terr = getTerrainByXY(ttx, ttz);
                    if (terr == NULL) continue;
                    if (!terr->loaded) continue;
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
                uterr.insert(setHeight256(tx, tz, xx*8, zz*8, h + offsetY, diffC, diffE));
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

    foreach (Terrain *value, uterr){
        if(value == NULL)
            continue;
        value->setModified(true);
        value->refresh();
    }
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
    float posx = p[0];
    float posz = p[2];
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr = this->getTerrainByXY(x, z);
    if (terr == NULL) return;
    if (terr->loaded == false) return;
    terr->makeTextureFromMap();
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

QSet<Terrain*> TerrainLibQt::paintHeightMap(Brush* brush, int x, int z, float* p) {

    QSet<Terrain*> uterr;
    
    float posx = round(p[0]/8.0)*8.0;
    float posz = round(p[2]/8.0)*8.0;
    
    Game::check_coords(x, z, posx, posz);
    qDebug() << x << " " << z << " " << posx << " " << posz;
    
    Terrain *terr;
    terr = getTerrainByXY(x, z);
    if (terr == NULL) return uterr;
    if (terr->loaded == false) return uterr;

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
            if(terr != getTerrainByXY(tx, tz)){
                terr = getTerrainByXY(tx, tz);
                if (terr == NULL) continue;
                if (!terr->loaded) continue;
                Undo::PushTerrainHeightMap(terr->mojex, terr->mojez, terr->terrainData, terr->getSampleCount());
            }
        }
    //
    
    terr = getTerrainByXY(x, z);
    h = brush->alpha*brush->direction*10.0;
    if(brush->hType == 1){
        rd = terr->setHeight(x, z, posx, posz, h, true);
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
                terr = getTerrainByXY(tx, tz);
                if (terr == NULL) continue;
                if (!terr->loaded) continue;
                float lx = tpx, lz = tpz;
                terr->getLocalCoords(tx, tz, lx, lz);
                int sampleSize = terr->getSampleSize();
                tpx = lx / sampleSize;
                tpz = lz / sampleSize;
                hAvg += terr->terrainData[tpz][tpx];
                count++;
            }
        hAvg /= count;
    }
    
    for(int i = -size; i < size; i++)
        for(int j = -size; j < size; j++){
            if(brush->hType == 1)
                if(i == 0 && j == 0) continue;
            tx = x;
            tz = z;
            tpx = px+i*8;
            tpz = pz+j*8;
            Game::check_coords(tx, tz, tpx, tpz);
            terr = getTerrainByXY(tx, tz);
            if (terr == NULL) continue;
            if (!terr->loaded) continue;
            uterr.insert(terr);
            
            if(sqrt(i*i + j*j) > size) continue;
            
            int sampleSize = terr->getSampleSize();
            float tileSizeMultipler = (8.0*8.0)/(sampleSize*sampleSize);
            h = (float)(size - (sqrt(i*i + j*j)))/size;
            h = h*brush->alpha*brush->direction*10.0*tileSizeMultipler;;
            
            terr->setErrorBias(tx, tz, tpx, tpz, 0);
            float lx = tpx, lz = tpz;
            terr->getLocalCoords(tx, tz, lx, lz);
            //qDebug() << tpx << lx << tpz << lz;
            
            tpx = lx / sampleSize;
            tpz = lz / sampleSize;
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
    
    foreach (Terrain *value, uterr){
        value->setModified(true);
        value->refresh();
    }
    return uterr;
}

void TerrainLibQt::fillWaterLevels(float *w, int mojex, int mojez) {
    Terrain *cTile = getTerrainByXY(mojex, mojez);
    Terrain *tTile;
    int X, Y;
    
    cTile->getCornerCoordsXY(X, Y, -1, -1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            w[0] = tTile->getWaterLevelSE();
        }
    cTile->getCornerCoordsXY(X, Y, 0, -1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            w[1] = tTile->getWaterLevelSW();
            w[2] = tTile->getWaterLevelSE();
        }
    cTile->getCornerCoordsXY(X, Y, 1, -1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            w[3] = tTile->getWaterLevelSW();
        }
    cTile->getCornerCoordsXY(X, Y, -1, 0);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL) 
        if(tTile->loaded){
            w[4] = tTile->getWaterLevelNE();
            w[6] = tTile->getWaterLevelSE();
        }
    cTile->getCornerCoordsXY(X, Y, 1, 0);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            w[5] = tTile->getWaterLevelNW();
            w[7] = tTile->getWaterLevelSW();
        }
    cTile->getCornerCoordsXY(X, Y, -1, 1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            w[8] = tTile->getWaterLevelNE();
        }
    cTile->getCornerCoordsXY(X, Y, 0, 1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL) 
        if(tTile->loaded){
            w[9] = tTile->getWaterLevelNW();
            w[10] = tTile->getWaterLevelNE();
        }
    cTile->getCornerCoordsXY(X, Y, 1, 1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            w[11] = tTile->getWaterLevelNW();
        }
}

void TerrainLibQt::setWaterLevels(float *w, int mojex, int mojez) {
    Terrain *cTile = getTerrainByXY(mojex, mojez);
    Terrain *tTile;
    int X, Y;
    
    cTile->getCornerCoordsXY(X, Y, -1, -1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelSE(w[0]);
            tTile->refreshWaterShapes();
        }
    cTile->getCornerCoordsXY(X, Y, 0, -1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelSW(w[1]);
            tTile->setWaterLevelSE(w[2]);
            tTile->refreshWaterShapes();
        }
    cTile->getCornerCoordsXY(X, Y, 1, -1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelSW(w[3]);
            tTile->refreshWaterShapes();
        }
    cTile->getCornerCoordsXY(X, Y, -1, 0);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL) 
        if(tTile->loaded){
            tTile->setWaterLevelNE(w[4]);
            tTile->setWaterLevelSE(w[6]);
            tTile->refreshWaterShapes();
        }
    cTile->getCornerCoordsXY(X, Y, 1, 0);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelNW(w[5]);
            tTile->setWaterLevelSW(w[7]);
            tTile->refreshWaterShapes();
        }
    cTile->getCornerCoordsXY(X, Y, -1, 1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelNE(w[8]);
            tTile->refreshWaterShapes();
        }
    cTile->getCornerCoordsXY(X, Y, 0, 1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL) 
        if(tTile->loaded){
            tTile->setWaterLevelNW(w[9]);
            tTile->setWaterLevelNE(w[10]);
            tTile->refreshWaterShapes();
        }
    cTile->getCornerCoordsXY(X, Y, 1, 1);
    tTile = getTerrainByXY(X, Y);
    if (tTile != NULL)
        if(tTile->loaded){
            tTile->setWaterLevelNW(w[11]);
            tTile->refreshWaterShapes();
        }
}


void TerrainLibQt::setDetailedTerrainAsCurrent(){
    currentQuadTree = quadTree;
    currentQt = &terrainQt;
}

void TerrainLibQt::setLowTerrainAsCurrent(){
    currentQuadTree = quadTreeLo;
    currentQt = &terrainQtLo;
}

void TerrainLibQt::fillRaw(Terrain *cTerr, int mojex, int mojez) {
    QuadTree* tQuadTree = currentQuadTree;
    QHash<unsigned int, TerrainInfo*> *tterrainQt = currentQt;
    
    if(cTerr->lowTile){
        currentQuadTree = quadTreeLo;
        currentQt = &terrainQtLo;
    } else {
        currentQuadTree = quadTree;
        currentQt = &terrainQt;
    }
    
    Terrain *tTile;
    int X, Y;
    cTerr->getCornerCoordsXY(X, Y, 1, 0);
    tTile = getTerrainByXY(X, Y, true);
    if(tTile != NULL)
        if (tTile->loaded) {
            cTerr->fillTerrainDataX(tTile);
        }

    cTerr->getCornerCoordsXY(X, Y, 0, 1);
    tTile = getTerrainByXY(X, Y, true);
    if(tTile != NULL)
        if (tTile->loaded) {
            cTerr->fillTerrainDataY(tTile);
        }

    cTerr->getCornerCoordsXY(X, Y, 1, 1);
    tTile = getTerrainByXY(X, Y, true);
    if(tTile != NULL)
        if (tTile->loaded) {
            cTerr->fillTerrainDataXY(tTile);
        }
    
    currentQuadTree = tQuadTree;
    currentQt = tterrainQt;
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

void TerrainLibQt::renderWaterLo(GLUU* gluu, float* playerT, float* playerW, float* target, float fov, int renderMode, int layer) {
    int renderCount = 90*90 ;
    if (renderMode == gluu->RENDER_SELECTION) {
        renderCount = 9;
    }

    gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, 0.0f);
    gluu->enableNormals();

    Terrain *tTile;
    int selectionColor = 0;
    unsigned int terrainNameId;
    for (int n = -1, i = 0, j = 0; n < renderCount; n+=16) {
        if (n != -1)
            spiralLoop(n, i, j);

            terrainNameId = quadTreeLo->getMyNameId((int) playerT[0] + i, -(int) playerT[1] - j);
            if (terrainNameId == 0)
                continue;
            if (terrainQtLo[terrainNameId] == NULL) {
                terrainQtLo[terrainNameId] = new TerrainInfo();
                quadTreeLo->fillTerrainInfo((int) playerT[0] + i, -(int) playerT[1] - j, terrainQtLo[terrainNameId]);
                qDebug() << terrainNameId;
                terrainQtLo[terrainNameId]->t = new Terrain(terrainQtLo[terrainNameId]);
            }
            if (terrainQtLo[terrainNameId]->rendered)
                continue;
            terrainQtLo[terrainNameId]->rendered = true;
            tTile = terrainQtLo[terrainNameId]->t;

            if (tTile->loaded == false)
                continue;

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

    QHashIterator<unsigned int, TerrainInfo*> i(terrainQtLo);
    while (i.hasNext()) {
        i.next();
        if (i.value() == NULL) continue;
        i.value()->rendered = false;
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
    QHash<QString, bool> rendered;
    
    for (int n = -1, i = 0, j = 0; n < renderCount; n++) {
        if (n != -1)
            spiralLoop(n, i, j);

        tTile = getTerrainByXY((int) playerT[0] + i, (int) playerT[1] + j, true);
        if(tTile == NULL)
            continue;
        
        tTile->inUse = true;
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
    
    if(renderMode == gluu->RENDER_SELECTION)
        return;
    
    QHashIterator<unsigned int, TerrainInfo*> i(terrainQt);
    while (i.hasNext()) {
        i.next();
        if (i.value() == NULL) continue;
        Terrain* obj = (Terrain*) i.value()->t;
        if(obj == NULL) continue;
        if(!obj->inUse && obj->loaded && !obj->isModified() && !obj->isSelected()){
           delete obj;
           i.value()->t = NULL;
       } else {
           obj->inUse = false;
       }
    }

}

void TerrainLibQt::renderLo(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, int renderMode) {
    int distantCount = Game::distantLod/1000 - 10;
    int renderCount = distantCount*distantCount ;
    if (renderMode == gluu->RENDER_SELECTION) {
        renderCount = 9;
    }

    gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, 0.0f);
    gluu->enableNormals();

    Terrain *tTile;
    int selectionColor = 0;
    unsigned int terrainNameId;
    for (int n = -1, i = 0, j = 0; n < renderCount; n+=16) {
        if (n != -1)
            spiralLoop(n, i, j);

            terrainNameId = quadTreeLo->getMyNameId((int) playerT[0] + i, -(int) playerT[1] - j);
            if (terrainNameId == 0)
                continue;
            if (terrainQtLo[terrainNameId] == NULL) {
                terrainQtLo[terrainNameId] = new TerrainInfo();
                quadTreeLo->fillTerrainInfo((int) playerT[0] + i, -(int) playerT[1] - j, terrainQtLo[terrainNameId]);
                qDebug() << terrainNameId;
                terrainQtLo[terrainNameId]->t = new Terrain(terrainQtLo[terrainNameId]);
            }
            if (terrainQtLo[terrainNameId]->rendered)
                continue;
            terrainQtLo[terrainNameId]->rendered = true;
            tTile = terrainQtLo[terrainNameId]->t;

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

    QHashIterator<unsigned int, TerrainInfo*> i(terrainQtLo);
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