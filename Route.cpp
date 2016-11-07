/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <QFile>
#include <QDir>
#include "Route.h"
#include "TSectionDAT.h"
#include "GLUU.h"
#include "Tile.h"
#include "GLMatrix.h"
#include "TerrainLib.h"
#include "Game.h"
#include "TrackObj.h"
#include "Path.h"
#include "Terrain.h"
#include "FileFunctions.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "DynTrackObj.h"
#include "PlatformObj.h"
#include "Flex.h"
#include "ForestObj.h"
#include "Coords.h"
#include "CoordsMkr.h"
#include "CoordsKml.h"
#include "CoordsGpx.h"
#include "SoundList.h"
#include "ActLib.h"
#include "Trk.h"
#include "AboutWindow.h"
#include "TrkWindow.h"
#include "PlatformObj.h"
#include "GroupObj.h"

Route::Route() {

    QFile file(Game::root + "/routes");
    if (!file.exists()) return;
    file.setFileName(Game::root + "/global");
    if (!file.exists()) return;

    file.setFileName(Game::root + "/routes/" + Game::route);
    if (!file.exists()) {
        qDebug() << "route does not exist";
        if (Game::createNewRoutes) {
            qDebug() << "new Route";
            Route::createNew();
        }
    }

    trk = new Trk();
    trk->load();
    Game::routeName = trk->routeName.toLower();
    qDebug() << Game::routeName;

    this->tsection = new TSectionDAT();
    this->trackDB = new TDB(tsection, false, (Game::root + "/routes/" + Game::route + "/" + Game::routeName + ".tdb"));
    this->roadDB = new TDB(tsection, true, (Game::root + "/routes/" + Game::route + "/" + Game::routeName + ".rdb"));
    this->ref = new Ref((Game::root + "/routes/" + Game::route + "/" + Game::routeName + ".ref"));

    loadMkrList();
    loadActivities();
    
    soundList = new SoundList();
    soundList->loadSoundSources(Game::root + "/routes/" + Game::route + "/ssource.dat");
    soundList->loadSoundRegions(Game::root + "/routes/" + Game::route + "/ttype.dat");
    Game::soundList = soundList;
    
    TerrainLib::LoadQuadTree();
    ForestObj::LoadForestList();
    PlatformObj::LoadCarSpawnerList();
    
    Game::trackDB = this->trackDB;
    Game::roadDB = this->roadDB;
    loaded = true;
    
    Vec3::set(placementAutoTranslationOffset, 0, 0, 0);
    Vec3::set(placementAutoRotationOffset, 0, 0, 0);
}

Route::Route(const Route& orig) {
}

Route::~Route() {
}

Trk *Route::getTrk(){
    return this->trk;
}

int Route::getStartTileX(){
    return this->trk->startTileX;
}

int Route::getStartTileZ(){
    return this->trk->startTileZ;
}

float Route::getStartpX(){
    return this->trk->startpX;
}

float Route::getStartpZ(){
    return this->trk->startpZ;
}

void Route::loadMkrList(){
    //this->mkr = new CoordsMkr(Game::root + "/routes/" + Game::route + "/" + Game::routeName +".mkr");
    QDir dir(Game::root + "/routes/" + Game::route);
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()){
        if(dirFile.endsWith(".mkr", Qt::CaseInsensitive))
            mkrList[(dirFile).toStdString()] = new CoordsMkr(Game::root + "/routes/" + Game::route + "/" + dirFile);
        if(dirFile.endsWith(".kml", Qt::CaseInsensitive))
            mkrList[(dirFile).toStdString()] = new CoordsKml(Game::root + "/routes/" + Game::route + "/" + dirFile);
        if(dirFile.endsWith(".gpx", Qt::CaseInsensitive))
            mkrList[(dirFile).toStdString()] = new CoordsGpx(Game::root + "/routes/" + Game::route + "/" + dirFile);
    }
    if(mkrList[(Game::routeName+".mkr").toStdString()] != NULL)
        this->mkr = mkrList[(Game::routeName+".mkr").toStdString()];
    else
        this->mkr = mkrList.begin()->second;
}

void Route::setMkrFile(QString name){
    if(mkrList[name.toStdString()] != NULL)
        this->mkr = mkrList[name.toStdString()];
}

void Route::loadActivities(){
    QDir dir(Game::root + "/routes/" + Game::route + "/activities");
    if(!dir.exists()) 
        return;
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.act");
    foreach(QString actfile, dir.entryList()){
        activityId.push_back(ActLib::addAct(dir.path(), actfile));
    }

    qDebug() << "loaded";
    return;
}

WorldObj* Route::getObj(int x, int z, int uid) {
    Tile *tTile;
    //try {
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        return NULL;
    return tTile->getObj(uid);
    //} catch (const std::out_of_range& oor) {

    //}
}

void Route::transalteObj(int x, int z, float px, float py, float pz, int uid) {
    Tile *tTile;
    //try {
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        return;
    tTile->transalteObj(px, py, pz, uid);
    //} catch (const std::out_of_range& oor) {

    //}
}

void Route::render(GLUU *gluu, float * playerT, float* playerW, float* target, float playerRot, float fov, int renderMode) {
    if(!loaded) return;
    
    int mintile = -Game::tileLod;
    int maxtile = Game::tileLod;

    if(renderMode == gluu->RENDER_SELECTION){
        mintile = -1;
        maxtile = 1;
    }
    //if (!selection) {
        //TerrainLib::render(gluu, playerT, playerW, target, fov);
        //trackDB->renderAll(gluu, playerT, playerRot);
        //trackDB->renderLines(gluu, playerT, playerRot);
    //}
    //for (var key in this.tile){
    //    this.tile[key].inUse = false;
    // }
    
    Tile *tTile;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            //try {
            tTile = tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
            //    if (tTile->loaded == -2) continue;
            //} catch (const std::out_of_range& oor) {
            if (tTile == NULL){
                tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j] = new Tile((int)playerT[0] + i, (int)playerT[1] + j);
            }
            tTile = tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
            //tTile->inUse = true;
            if (tTile->loaded == 1) {
                gluu->mvPushMatrix();
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
                tTile->render(playerT, playerW, target, fov, renderMode);
                gluu->mvPopMatrix();
            }
        }
    }
    if (renderMode == gluu->RENDER_DEFAULT) {
        if(Game::viewTrackDbLines){
            if(Game::renderTrItems){
                trackDB->renderItems(gluu, playerT, playerRot);
                roadDB->renderItems(gluu, playerT, playerRot);
            }
            trackDB->renderAll(gluu, playerT, playerRot);
        }
        if(Game::viewTsectionLines)
            trackDB->renderLines(gluu, playerT, playerRot);
        if(Game::viewTrackDbLines)
            roadDB->renderAll(gluu, playerT, playerRot);
        if(Game::viewTsectionLines)
            roadDB->renderLines(gluu, playerT, playerRot);
        
        if(Game::viewMarkers)
            if(this->mkr != NULL)
                this->mkr->render(gluu, playerT, playerW, playerRot);
    }
    //trackDB->renderItems(gluu, playerT, playerRot);
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
    Game::ignoreLoadLimits = false;
}

void Route::renderShadowMap(GLUU *gluu, float * playerT, float* playerW, float* target, float playerRot, float fov, bool selection) {
    if(!loaded) return;
    
    int mintile = -1;
    int maxtile = 1;
    Tile *tTile;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            tTile = tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
            if (tTile == NULL){
                tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j] = new Tile((int)playerT[0] + i, (int)playerT[1] + j);
            }
            tTile = tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
            if (tTile->loaded == 1) {
                gluu->mvPushMatrix();
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
                tTile->render(playerT, playerW, target, fov, GLUU::RENDER_SHADOWMAP);
                gluu->mvPopMatrix();
            }
        }
    }
}

void Route::setTerrainToTrackObj(WorldObj* obj, Brush* brush){
    if(obj == NULL) return;
    
    if(obj->typeID == obj->groupobject) {
        GroupObj *gobj = (GroupObj*)obj;
        for(int i = 0; i < gobj->objects.size(); i++ ){
            setTerrainToTrackObj(gobj->objects[i], brush);
        }
        return;
    }
    
    if(obj->type != "trackobj" && obj->type != "dyntrack" ) return;
    //TrackObj* tobj = (TrackObj*)obj;
    //TrackObj* track = (TrackObj*)obj;
    float* punkty = new float[10000];
    float* ptr = punkty;
    if(this->tsection->isRoadShape(obj->sectionIdx))
        this->roadDB->getVectorSectionPoints(obj->x, obj->y, obj->UiD, ptr);
    else
        this->trackDB->getVectorSectionPoints(obj->x, obj->y, obj->UiD, ptr);
    int length = ptr - punkty;
    qDebug() << "l "<<length;
    TerrainLib::setTerrainToTrackObj(brush, punkty, length, obj->x, obj->y, obj->matrix);
}

WorldObj* Route::placeObject(int x, int z, float* p) {
    float* q = new float[4];
    Quat::fill((float*)q);
    placeObject(x, z, p, (float*) q, ref->selected);
}

WorldObj* Route::placeObject(int x, int z, float* p, float* q) {
    placeObject(x, z, p, q, ref->selected);
}

WorldObj* Route::placeObject(int x, int z, float* p, float* q, Ref::RefItem* r) {
    if(r == NULL) return NULL;
    Game::check_coords(x, z, p);

    // pozycja wzgledem TDB:
    int itemTrackType = WorldObj::isTrackObj(r->type);
    float* tpos = NULL;
    if(placementStickToTDB || itemTrackType == 1){
        tpos = new float[2];
        float* playerT = Vec2::fromValues(x, z);
        bool ok = this->trackDB->findNearestPositionOnTDB(playerT, p, q, tpos);
        if(!ok) return NULL;
        x = playerT[0];
        z = playerT[1];
    }
    if(itemTrackType == 2){
        tpos = new float[2];
        float* playerT = Vec2::fromValues(x, z);
        bool ok = this->roadDB->findNearestPositionOnTDB(playerT, p, q, tpos);
        if(!ok) return NULL;
        x = playerT[0];
        z = playerT[1];
    } 
    if(itemTrackType == 3){
        tpos = new float[2];
        float* playerT = Vec2::fromValues(x, z);
        bool ok = this->roadDB->findNearestPositionOnTDB(playerT, p, q, tpos);
        if(!ok) return NULL;
        x = playerT[0];
        z = playerT[1];
        float* buffer;
        int len;
        this->roadDB->getVectorSectionLine(buffer, len, playerT[0], playerT[1], tpos[0], 0, 0);
        qDebug() << "len "<<len;
        ok = this->trackDB->getSegmentIntersectionPositionOnTDB(playerT, buffer, len, p, q, tpos);
        if(!ok) return NULL;
        //return NULL;
    }

    Tile *tTile;
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        tile[(x)*10000 + z] = new Tile(x, z);
    tTile = tile[((x)*10000 + z)];
    if (tTile->loaded == -2) {
        if (TerrainLib::isLoaded(x, z)) {
            tTile->initNew();
        } else {
            return NULL;
        }
    }
    if (tTile->loaded == 1) {
        //float pozWW[3];
        //pozWW[2] = pozW[2];
        //for(int j = -1000; j < 1000; j+=10)
        //for(int i = -1000; i < 1000; i+=50){
        //pozWW[0] = pozW[0] + i;
        //pozWW[2] = pozW[2] + j;

        WorldObj* nowy = tTile->placeObject(p, q, r, tpos);
        

        if ((r->type == "trackobj" || r->type == "dyntrack" )&& nowy != NULL) {
            if(nowy->endp == 0) nowy->endp = new float[5];
            //this->trackDB->placeTrack(x, z, p, q, r->value, nowy->UiD);
            qDebug() <<"1: "<< x <<" "<<z<<" "<<p[0]<<" "<<p[1]<<" "<<p[2]; 
            int oldx = x;
            int oldz = z;
            if(this->tsection->isRoadShape(r->value))
                this->roadDB->findPosition(x, z, p, q, nowy->endp, r->value, nowy->UiD);
            else
                this->trackDB->findPosition(x, z, p, q, nowy->endp, r->value, nowy->UiD);
            //findPosition
            qDebug() <<"2: "<< x <<" "<<z<<" "<<p[0]<<" "<<p[1]<<" "<<p[2]; 
            p[0] -= (oldx-x)*2048;
            p[2] -= (oldz-z)*2048;
            nowy->setPosition(p);
            nowy->setQdirection(q);
            nowy->setMartix();
        }

        return nowy;
        //}
    }
    return NULL;
}

float *fromtwovectors(float* out, float* u, float* v){
    //float m = sqrt(2.f + 2.f * Vec3::dot(u, v));
    float w[3];
    /*Vec3::cross((float*)w, u, v);
    Vec3::scale((float*)w, (float*)w, (1.f / m));
    out[0] = 0.5f * m;
    out[1] = w[0];
    out[2] = w[1];
    out[3] = w[2];*/
    float cos_theta = Vec3::dot(u, v);
    float angle = acos(cos_theta);
    Vec3::cross(w, u, v);
    Vec3::normalize(w, w);
    Quat::setAxisAngle(out, w, angle);
    return out;
}

WorldObj* Route::autoPlaceObject(int x, int z, float* p) {
    if(ref->selected == NULL) return NULL;
    Game::check_coords(x, z, p);
    
    autoPlacementLastPlaced.clear();

    TDB * tdb = NULL;
    if(placementAutoTargetType == 0)
        tdb = this->trackDB;
    else if(placementAutoTargetType == 1)
        tdb = this->roadDB;
    else
        return NULL;
    
    // pozycja wzgledem TDB:
    float* tpos = new float[2];
    float* playerT = Vec2::fromValues(x, z);
    bool ok = tdb->findNearestPositionOnTDB(playerT, p, NULL, tpos);
    if(!ok) return NULL;
    
    x = playerT[0];
    z = playerT[1];
    int trackNodeIdx = tpos[0];
    int length = tdb->getVectorSectionLength(trackNodeIdx);
    int metry = 0;
    float currentPosition[7];
    float currentPosition1[7];
    float xyz[3];
    float *quat = Quat::create();
    float *vec1 = Vec3::create();
    vec1[2] = -1.0;
    float *vec2 = Vec3::create();
    float step = placementAutoLength;
    for(float i = 0; i < length; i+=step ){
        if(!tdb->getDrawPositionOnTrNode((float*)currentPosition, trackNodeIdx, i))
            return NULL;
        int i2 = i+step;
        if(i2 > length)
            i2 = length - 0.1;
        if(!tdb->getDrawPositionOnTrNode((float*)currentPosition1, trackNodeIdx, i2))
            return NULL;
        x = currentPosition[5];
        z = -currentPosition[6];
        //vec1[0] = currentPosition[0];
        //vec1[1] = currentPosition[1];
        //vec1[2] = -currentPosition[2];
        vec2[0] = currentPosition1[0] - (currentPosition[0]-(currentPosition1[5]-currentPosition[5])*2048);
        vec2[1] = currentPosition1[1] - currentPosition[1];
        vec2[2] = -currentPosition1[2] + (currentPosition[2]-(currentPosition1[6]-currentPosition[6])*2048);
        Vec3::normalize(vec2, vec2);
        //Vec3::normalize(vec1, vec1);
        if(placementAutoTwoPointRot){
            fromtwovectors(quat, vec1, vec2);
        }else {
            Quat::fill(quat);
            Quat::rotateY(quat, quat, currentPosition[3]);
            Quat::rotateX(quat, quat, -currentPosition[4]);
        }
        float offset[3];
        Vec3::copy(offset, placementAutoTranslationOffset);
        float offsetq[4];
        Quat::fill(offsetq);
        Quat::rotateY(offsetq,offsetq,(placementAutoRotationOffset[1]*M_PI)/180);
        Quat::rotateX(offsetq,offsetq,(placementAutoRotationOffset[0]*M_PI)/180);
        
        Vec3::transformQuat(offset, offset, quat);
        Quat::multiply(quat, quat, offsetq);
        
        xyz[0] = currentPosition[0] + offset[0];
        xyz[1] = currentPosition[1] + offset[1];
        xyz[2] = -currentPosition[2] + offset[2];
        
        autoPlacementLastPlaced.push_back(placeObject(x, z, (float*) xyz, quat, ref->selected));
    }

    return NULL;
    
}

void Route::autoPlacementDeleteLast(){
    for(int i = 0; i < autoPlacementLastPlaced.length(); i++){
        deleteObj(autoPlacementLastPlaced[i]);
    }
    autoPlacementLastPlaced.clear();
}


WorldObj* Route::makeFlexTrack(int x, int z, float* p) {
    float qe[4];
    qe[0] = 0;
    qe[1] = 0;
    qe[2] = 0;
    qe[3] = 1;
    this->trackDB->findNearestNode(x, z, p,(float*) &qe);
    float* dyntrackData[10];
    bool success = Flex::NewFlex(x, z, p, (float*)qe, (float*)dyntrackData);
    if(!success) return NULL;
    
    Ref::RefItem r;
    r.type = "dyntrack";
    r.value = -1;
    qDebug() << "1";
    qe[0] = 0;
    qe[1] = 0;
    qe[2] = 0;
    qe[3] = 1;
    DynTrackObj* track = (DynTrackObj*)placeObject(x, z, p, (float*)&qe, &r);
    if(track != NULL){
        qDebug() << "2";
        QString sh = "dyntrackdata";
        track->set(sh, (float*)dyntrackData);
    }
    return track;
}

void Route::addToTDB(WorldObj* obj) {
    if(obj == NULL) return;
    
    int x = obj->x;//post[0];
    int z = obj->y;//post[1];
    float p[3];
    //p[0] = pos[0];
    //p[1] = pos[1];
    //p[2] = pos[2];
    p[0] = obj->position[0];
    p[1] = obj->position[1];
    p[2] = obj->position[2];
    //Game::check_coords(x, z, (float*) &p);
    float q[4];
    //q[0] = obj->tRotation[0]; //track->qDirection[0];
    //q[1] = obj->tRotation[1]; //qDirection[1];
    //q[2] = 0; //track->qDirection[2];
    //q[3] = 1; //track->qDirection[3];
    q[0] = obj->qDirection[0];
    q[1] = obj->qDirection[1];
    q[2] = obj->qDirection[2];
    q[3] = obj->qDirection[3];
    
    if (obj->type == "trackobj") {
        TrackObj* track = (TrackObj*) obj;
        //this->trackDB->placeTrack(x, z, p, q, r, nowy->UiD);
        //float scale = (float) sqrt(track->qDirection[0] * track->qDirection[0] + track->qDirection[1] * track->qDirection[1] + track->qDirection[2] * track->qDirection[2]);
        //float elevation = ((track->qDirection[0] + 0.0000001f) / fabs(scale + 0.0000001f))*(float) -acos(track->qDirection[3])*2;
        //float elevation = -3.14/16.0;
        //q[0] = elevation;
        if(this->tsection->isRoadShape(track->sectionIdx))
            this->roadDB->placeTrack(x, z, (float*) &p, (float*) &q, track->sectionIdx, obj->UiD);
        else
            this->trackDB->placeTrack(x, z, (float*) &p, (float*) &q, track->sectionIdx, obj->UiD, &track->jNodePosn);
        //obj->setPosition(p);
        //obj->setQdirection(q);
        //obj->setMartix();
        //track->setJNodePosN();
    } else if(obj->type == "dyntrack"){
        DynTrackObj* dynTrack = (DynTrackObj*) obj;
        if(dynTrack->sectionIdx == -1){
            this->trackDB->fillDynTrack(dynTrack);
        }
        this->trackDB->placeTrack(x, z, (float*) &p, (float*) &q, dynTrack->sectionIdx, obj->UiD);
        obj->setPosition(p);
        obj->setQdirection(q);
        obj->setMartix();
    } 
}

void Route::toggleToTDB(WorldObj* obj) {
    if(obj == NULL) return;
    
    if(obj->typeID == obj->groupobject) {
        GroupObj *gobj = (GroupObj*)obj;
        for(int i = 0; i < gobj->objects.size(); i++ ){
            toggleToTDB(gobj->objects[i]);
        }
        return;
    }
    
    if (obj->type != "trackobj" && obj->type != "dyntrack") {
            return;
    }
    if(roadDB->ifTrackExist(obj->x, obj->y, obj->UiD) || trackDB->ifTrackExist(obj->x, obj->y, obj->UiD)){
        removeTrackFromTDB(obj);
        return;
    }
    addToTDB(obj);
}

void Route::addToTDBIfNotExist(WorldObj* obj) {
    if(obj == NULL) return;
    
    if(obj->typeID == obj->groupobject) {
        GroupObj *gobj = (GroupObj*)obj;
        for(int i = 0; i < gobj->objects.size(); i++ ){
            addToTDBIfNotExist(gobj->objects[i]);
        }
        return;
    }
    
    if (obj->type != "trackobj" && obj->type != "dyntrack") {
            return;
    }
    if(roadDB->ifTrackExist(obj->x, obj->y, obj->UiD) || trackDB->ifTrackExist(obj->x, obj->y, obj->UiD)){
        return;
    }
    addToTDB(obj);
}

void Route::newPositionTDB(WorldObj* obj, float* post, float* pos) {
    int x = post[0];
    int z = post[1];
    float p[3]; 
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    Game::check_coords(x, z, (float*) &p);

    if (obj->type == "trackobj") {
        float q[4];
        q[0] = 0;
        q[1] = 0;
        q[2] = 0;
        q[3] = 1;
        TrackObj* track = (TrackObj*) obj;
        //this->trackDB->placeTrack(x, z, p, q, r, nowy->UiD);
        if(this->tsection->isRoadShape(track->sectionIdx))
            this->roadDB->findPosition(x, z, (float*) &p, (float*) &q, track->endp, track->sectionIdx, obj->UiD);
        else
            this->trackDB->findPosition(x, z, (float*) &p, (float*) &q, track->endp, track->sectionIdx, obj->UiD);
        obj->setPosition(p);
        obj->setQdirection(q);
        obj->setMartix();
    }
}

void Route::deleteTDBTree(WorldObj* obj){
    if (obj->type == "trackobj" || obj->type == "dyntrack") {
        this->roadDB->deleteTree(obj->x, obj->y, obj->UiD);
        this->trackDB->deleteTree(obj->x, obj->y, obj->UiD);
    }
}

void Route::deleteObj(WorldObj* obj) {
    if(obj == NULL)
        return;
    if(obj->typeID == obj->groupobject) {
        GroupObj *gobj = (GroupObj*)obj;
        for(int i = 0; i < gobj->objects.size(); i++ ){
            deleteObj(gobj->objects[i]);
        }
        return;
    }
    
    if (obj->type == "trackobj" || obj->type == "dyntrack") {
        removeTrackFromTDB(obj);
        if(Game::leaveTrackShapeAfterDelete)
            return;
    }
    obj->loaded = false;
    obj->modified = true;
    if (obj->isTrackItem()) {
        obj->deleteTrItems();
    }
    Tile *tTile;
    tTile = tile[((obj->x)*10000 + obj->y)];
    if (tTile != NULL)
        tTile->jestHiddenObj++;
}

void Route::removeTrackFromTDB(WorldObj* obj) {
    this->roadDB->removeTrackFromTDB(obj->x, obj->y, obj->UiD);
    this->trackDB->removeTrackFromTDB(obj->x, obj->y, obj->UiD);
}

int Route::getTileObjCount(int x, int z) {
    Tile *tTile;
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        return 0;
    return tTile->jestObiektow;
}

int Route::getTileHiddenObjCount(int x, int z) {
    Tile *tTile;
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        return 0;
    return tTile->jestHiddenObj;
}

void Route::getUnsavedInfo(std::vector<QString> &items){
    if (!Game::writeEnabled) return;
    for (auto it = tile.begin(); it != tile.end(); ++it) {
        Tile* tTile = (Tile*) it->second;
        if (tTile == NULL) continue;
        if (tTile->loaded == 1 && tTile->isModified()) {
            items.push_back("[W] "+QString::number(tTile->x)+" "+QString::number(-tTile->z));
        }
    }
    TerrainLib::getUnsavedInfo(items);
    if(this->trk->isModified())
        items.push_back("[S] Route Settings - TRK File");
    //this->trackDB->save();
    //this->roadDB->save();
}


void Route::save() {
    if (!Game::writeEnabled) return;
    qDebug() << "save";
    for (auto it = tile.begin(); it != tile.end(); ++it) {
        //console.log(obj.type);
        Tile* tTile = (Tile*) it->second;
        if (tTile == NULL) continue;
        if (tTile->loaded == 1 && tTile->isModified()) {
            tTile->save();
            tTile->setModified(false);
        }
    }
    TerrainLib::save();
    this->trackDB->save();
    this->roadDB->save();
    this->trk->save();
}

void Route::createNewPaths() {
    if (!Game::writeEnabled) return;
    Path::CreatePaths(this->trackDB);
}

std::unordered_map<std::string, Coords*> Route::getMkrList(){
    return this->mkrList;
}

void Route::nextDefaultEnd(){
    this->trackDB->nextDefaultEnd();
    this->roadDB->nextDefaultEnd();
}

void Route::paintHeightMap(Brush* brush, int x, int z, float* p){
    Game::ignoreLoadLimits = true;
    QSet<int> modifiedTiles = TerrainLib::paintHeightMap(brush, x, z, p);
    Tile *ttile;
    foreach (int value, modifiedTiles){
        ttile = tile[value];
        if(ttile != NULL)
            ttile->updateTerrainObjects();
    }
}

void Route::createNew() {
    if (!Game::writeEnabled) return;

    QString path;

    path = Game::root + "/routes/" + Game::route;
    if (QDir(path).exists()) {
        qDebug() << "route folder exist - aborting";
        return;
    }
    QDir().mkdir(path);
    QDir().mkdir(path + "/envfiles");
    QDir().mkdir(path + "/envfiles/textures");
    QDir().mkdir(path + "/paths");
    QDir().mkdir(path + "/shapes");
    QDir().mkdir(path + "/sound");
    QDir().mkdir(path + "/textures");
    QDir().mkdir(path + "/terrtex");
    QDir().mkdir(path + "/tiles");
    QDir().mkdir(path + "/td");
    QDir().mkdir(path + "/world");

    int x = Game::newRouteX;
    int z = Game::newRouteZ;
    
    Trk * newTrk = new Trk();
    newTrk->idName = Game::route;
    newTrk->routeName = Game::route;
    newTrk->displayName = Game::route;
    newTrk->startTileX = Game::newRouteX;
    newTrk->startTileZ = Game::newRouteZ;
    showTrkEditr(newTrk);
    newTrk->save();
    
    TDB::saveEmpty(false);
    TDB::saveEmpty(true);
    TerrainLib::createNewRouteTerrain(x, z);
    Tile::saveEmpty(x, z);
    //Terrain::saveEmpty(x, z);

    QString res = "resources/templateRoute/";
    path += "/";

    QFile::copy(res + "sigcfg.dat", path + "sigcfg.dat");
    QFile::copy(res + "sigscr.dat", path + "sigscr.dat");
    QFile::copy(res + "ttype.dat", path + "ttype.dat");
    QFile::copy(res + "template.ref", path + Game::route + ".ref");
    QFile::copy(res + "carspawn.dat", path + "carspawn.dat");
    QFile::copy(res + "deer.haz", path + "deer.haz");
    QFile::copy(res + "forests.dat", path + "forests.dat");
    QFile::copy(res + "speedpost.dat", path + "speedpost.dat");
    QFile::copy(res + "spotter.haz", path + "spotter.haz");
    QFile::copy(res + "ssource.dat", path + "ssource.dat");
    QFile::copy(res + "telepole.dat", path + "telepole.dat");

    FileFunctions::copyFiles(res + "envfiles", path + "envfiles");
    FileFunctions::copyFiles(res + "envfiles/textures", path + "envfiles/textures");
    FileFunctions::copyFiles(res + "shapes", path + "shapes");
    FileFunctions::copyFiles(res + "sound", path + "sound");
    FileFunctions::copyFiles(res + "terrtex", path + "terrtex");
    FileFunctions::copyFiles(res + "textures", path + "textures");
}

void Route::reloadTile(int x, int z) {
    tile[x * 10000 + z] = new Tile(x, z);
    return;
}

void Route::newTile(int x, int z) {
    if (!Game::writeEnabled) return;
    Tile::saveEmpty(x, -z);
    //Terrain::saveEmpty(x, -z);
    TerrainLib::saveEmpty(x, -z);
    TerrainLib::reload(x, z);
    reloadTile(x, z);
}

void Route::showTrkEditr(Trk * val){
    TrkWindow trkWindow;
    if(val == NULL)
        val = this->trk;
    trkWindow.trk = val;
    trkWindow.exec();
}