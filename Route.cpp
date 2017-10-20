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
#include "CarSpawnerObj.h"
#include "Flex.h"
#include "ForestObj.h"
#include "Coords.h"
#include "CoordsMkr.h"
#include "CoordsKml.h"
#include "CoordsGpx.h"
#include "CoordsRoutePlaces.h"
#include "SoundList.h"
#include "ActLib.h"
#include "Trk.h"
#include "AboutWindow.h"
#include "TrkWindow.h"
#include "PlatformObj.h"
#include "GroupObj.h"
#include "Undo.h"
#include "Activity.h"
#include "Service.h"
#include "Traffic.h"
#include "Path.h"
#include "Environment.h"
#include "OrtsWeatherChange.h"

Route::Route() {
    Game::currentRoute = this;
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
    env = new Environment(Game::root + "/routes/" + Game::route + "/ENVFILES/editor.env");
    Game::routeName = trk->routeName.toLower();
    qDebug() << Game::routeName;

    this->tsection = new TSectionDAT();
    this->trackDB = new TDB(tsection, false);
    this->roadDB = new TDB(tsection, true);
    Game::trackDB = this->trackDB;
    Game::roadDB = this->roadDB;
    this->ref = new Ref((Game::root + "/routes/" + Game::route + "/" + Game::routeName + ".ref"));

    loadMkrList();
    createMkrPlaces();
    loadServices();
    loadTraffic();
    loadPaths();
    loadActivities();
    
    soundList = new SoundList();
    soundList->loadSoundSources(Game::root + "/routes/" + Game::route + "/ssource.dat");
    soundList->loadSoundRegions(Game::root + "/routes/" + Game::route + "/ttype.dat");
    Game::soundList = soundList;
    
    TerrainLib::LoadQuadTree();
    OrtsWeatherChange::LoadList();
    ForestObj::LoadForestList();
    ForestObj::ForestClearDistance = trk->forestClearDistance;
    CarSpawnerObj::LoadCarSpawnerList();
    
    loaded = true;
    
    Vec3::set(placementAutoTranslationOffset, 0, 0, 0);
    Vec3::set(placementAutoRotationOffset, 0, 0, 0);
}

Route::Route(const Route& orig) {
}

Route::~Route() {
}

void Route::activitySelected(Activity* selected){
    currentActivity = selected;
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

void Route::createMkrPlaces(){
    QString key;
    key = "| Route: Stations";
    mkrList[key] = new CoordsRoutePlaces(trackDB, "stations");
    
    key = "| Route: Sidings";
    mkrList[key] = new CoordsRoutePlaces(trackDB, "sidings");
}

void Route::loadMkrList(){
    //this->mkr = new CoordsMkr(Game::root + "/routes/" + Game::route + "/" + Game::routeName +".mkr");
    QDir dir(Game::root + "/routes/" + Game::route);
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()){
        if(dirFile.endsWith(".mkr", Qt::CaseInsensitive))
            mkrList[(dirFile).toLower()] = new CoordsMkr(Game::root + "/routes/" + Game::route + "/" + dirFile);
        if(dirFile.endsWith(".kml", Qt::CaseInsensitive))
            mkrList[(dirFile).toLower()] = new CoordsKml(Game::root + "/routes/" + Game::route + "/" + dirFile);
        if(dirFile.endsWith(".gpx", Qt::CaseInsensitive))
            mkrList[(dirFile).toLower()] = new CoordsGpx(Game::root + "/routes/" + Game::route + "/" + dirFile);
    }
    if(mkrList.size() > 0)
        if(mkrList[(Game::routeName+".mkr").toLower()] != NULL){
            if(mkrList[(Game::routeName+".mkr").toLower()]->loaded)
                mkr = mkrList[(Game::routeName+".mkr").toLower()];
            else
                mkr = mkrList.begin().value();
                //mkr = mkrList[(Game::routeName+".mkr").toLower().toStdString()];
        } else {
            mkr = mkrList.begin().value();
        }
}

void Route::setMkrFile(QString name){
    if(mkrList[name] != NULL)
        this->mkr = mkrList[name];
}

void Route::loadActivities(){
    QDir dir(Game::root + "/routes/" + Game::route + "/activities");
    if(!dir.exists()) 
        return;
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.act");
    foreach(QString actfile, dir.entryList()){
        activityId.push_back(ActLib::AddAct(dir.path(), actfile));
    }
    
    //for(int i = 0; i < ActLib::jestact; i++){
    //    ActLib::Act[i]->setRouteContent(&path, &service, &traffic);
    //}

    qDebug() << "activity loaded";
    return;
}

void Route::loadServices(){
    QDir dir(Game::root + "/routes/" + Game::route + "/services");
    if(!dir.exists()) 
        return;
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.srv");
    foreach(QString actfile, dir.entryList()){
        int id = ActLib::AddService(dir.path(), actfile);
        //service.push_back(ActLib::Services[id]);
    }

    qDebug() << "service loaded";
    return;
}

void Route::loadTraffic(){
    QDir dir(Game::root + "/routes/" + Game::route + "/traffic");
    if(!dir.exists()) 
        return;
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.trf");
    foreach(QString actfile, dir.entryList()){
        int id = ActLib::AddTraffic(dir.path(), actfile);
        //traffic.push_back(ActLib::Traffics[id]);
    }
    qDebug() << "traffic loaded";
    return;
}

void Route::loadPaths(){
    QDir dir(Game::root + "/routes/" + Game::route + "/paths");
    if(!dir.exists()) 
        return;
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.pat");
    foreach(QString actfile, dir.entryList()){
        int id = ActLib::AddPath(dir.path(), actfile);
        path.push_back(ActLib::Paths[id]);
    }

    qDebug() << "paths loaded";
    return;
}

WorldObj* Route::getObj(int x, int z, int id) {
    Tile *tTile;
    //try {
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        return NULL;
    return tTile->getObj(id);
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

void Route::updateSim(float *playerT, float deltaTime){
    if(!loaded) return;
    
    int mintile = -Game::tileLod;
    int maxtile = Game::tileLod;

    Tile *tTile;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            tTile = tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
            if (tTile == NULL)
                continue;
            if (tTile->loaded == 1) {
                tTile->updateSim(deltaTime);
            }
        }
    }
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
            if(Game::autoNewTiles)
                if (i == 0 && j == 0)
                    if (tTile->loaded == -2) {
                        Route::newTile((int)playerT[0] + i, (int)playerT[1] + j);
                        tTile = tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
                    }
            if (tTile->loaded == 1) {
                gluu->mvPushMatrix();
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
                tTile->render(playerT, playerW, target, fov, renderMode);
                gluu->mvPopMatrix();
            }
        }
    }
    if (renderMode == gluu->RENDER_DEFAULT) {
        if(Game::viewTrackDbLines)
            trackDB->renderAll(gluu, playerT, playerRot);
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
    if(Game::renderTrItems){
        trackDB->renderItems(gluu, playerT, playerRot, renderMode);
        roadDB->renderItems(gluu, playerT, playerRot, renderMode);
    }
    
    if(currentActivity != NULL){
        currentActivity->render(gluu, playerT, playerRot, renderMode);
    }
    
    for(int i = 0; i < path.size(); i++){
        if(path[i]->isSelected())
            path[i]->render(gluu, playerT, renderMode);
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
    if(currentActivity != NULL)
        currentActivity->render(gluu, playerT, playerRot, 0);
}

void Route::setTerrainTextureToObj(int x, int y, float *pos, Brush* brush, WorldObj* obj){
    bool useObj = false;
    if(obj != NULL){
        if(obj->typeObj == WorldObj::worldobj)
            useObj = obj->hasLinePoints();
    }
        
    float* punkty = new float[9000];
    
    if(useObj){
        float* ptr = punkty;
        obj->getLinePoints(ptr);
        int length = ptr - punkty;
        qDebug() << "lo "<<length;
        TerrainLib::setTextureToTrackObj(brush, punkty, length, obj->x, obj->y);
    } else {
        Game::check_coords(x, y, pos);
        float* ptr = punkty;
        float playerT[2];
        playerT[0] = x;
        playerT[1] = y;
        float tp[3];
        Vec3::copy(tp,pos);
        int ok1, ok2;

        if(placementAutoTargetType == 0) {
            this->trackDB->getVectorSectionPoints(x, y, pos, ptr);
        } else if(placementAutoTargetType == 1) {
            this->roadDB->getVectorSectionPoints(x, y, pos, ptr);
        } else if(placementAutoTargetType == 2) {
            bool road = false;
            ok1 = this->trackDB->findNearestPositionOnTDB(playerT, tp, NULL, NULL);
            ok2 = this->roadDB->findNearestPositionOnTDB(playerT, tp, NULL, NULL);
            if(ok2 >= 0)
                if(ok1 < 0 || ok2 < ok1){
                    road = true;
            }
            if(road)
                this->roadDB->getVectorSectionPoints(x, y, pos, ptr);
            else
                this->trackDB->getVectorSectionPoints(x, y, pos, ptr);
        }
        int length = ptr - punkty;
        qDebug() << "l "<<length;
        TerrainLib::setTextureToTrackObj(brush, punkty, length, x, y);
    }
    delete[] punkty;
}

void Route::setTerrainToTrackObj(WorldObj* obj, Brush* brush){
    if(obj == NULL) return;
    if(obj->typeObj != WorldObj::worldobj)
        return;
    
    if(obj->typeID == obj->groupobject) {
        GroupObj *gobj = (GroupObj*)obj;
        for(int i = 0; i < gobj->objects.size(); i++ ){
            setTerrainToTrackObj(gobj->objects[i], brush);
        }
        return;
    }
    
    if(obj->type == "trackobj" || obj->type == "dyntrack" ){
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
        if(length == 0){
            if(obj->sectionIdx >= 0){
                float matrix[16];
                for(int i = 0; i < tsection->shape[obj->sectionIdx]->path[0].n; i++){
                    memcpy(matrix, obj->matrix, sizeof(float)*16);
                    int sidx = tsection->shape[obj->sectionIdx]->path[0].sect[i];
                    tsection->sekcja[sidx]->getPoints(ptr, matrix);
                }
            } else {
                //obj->getLinePoints(ptr);
            }
            length = ptr - punkty;
            qDebug() << "l "<<length;
        }
        
        if(length > 0)
            TerrainLib::setTerrainToTrackObj(brush, punkty, length, obj->x, obj->y, obj->matrix);
        delete[] punkty;
    } else if(obj->hasLinePoints()) {
        float* punkty = new float[10000];
        float* ptr = punkty;
        obj->getLinePoints(ptr);
        int length = ptr - punkty;
        qDebug() << "l "<<length;
        TerrainLib::setTerrainToTrackObj(brush, punkty, length, obj->x, obj->y, obj->matrix);
        delete[] punkty;
    }
    
}

ActivityObject* Route::getActivityObject(int id){
    if(currentActivity == NULL)
        return NULL;
    return currentActivity->getObjectById(id);
    return NULL;
}

Activity* Route::getCurrentActivity(){
    if(currentActivity == NULL)
        return NULL;
    return currentActivity;
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
    
    if(itemTrackType != 0){
        Undo::PushTrackDB(trackDB, false);
        Undo::PushTrackDB(roadDB, true);
    }
    
    float* tpos = NULL;
    if(placementStickToTarget){
            tpos = new float[3];
            float* playerT = Vec2::fromValues(x, z);
            float* playerT2 = Vec2::fromValues(x, z);
            float tp[3], tp2[3];
            float tq[4], tq2[3];
            Vec3::copy(tp, p);
            Quat::copy(tq, q);
            Vec3::copy(tp2, p);
            Quat::copy(tq2, q);
            int ok = -1;
            if(placementAutoTargetType == 0) {
                ok = this->trackDB->findNearestPositionOnTDB(playerT, tp, tq, tpos);
            } else if(placementAutoTargetType == 1) {
                ok = this->roadDB->findNearestPositionOnTDB(playerT, tp, tq, tpos);
            } else if(placementAutoTargetType == 2) {
                ok = this->trackDB->findNearestPositionOnTDB(playerT, tp, tq, tpos);
                int ok2 = this->roadDB->findNearestPositionOnTDB(playerT2, tp2, tq2, tpos);
                if(ok2 >= 0)
                    if(ok < 0 || ok2 < ok){
                        ok = ok2;
                        Vec3::copy(tp, tp2);
                        Quat::copy(tq, tq2);
                        Vec2::copy(playerT, playerT2);
                    }
            }
            if(ok >= 0 && ok <= Game::snapableRadius) {
                Quat::copy(q, tq);
                if(!snapableOnlyRotation){
                    Vec3::copy(p, tp);
                    x = playerT[0];
                    z = playerT[1];
                }
            }
    }
    if(itemTrackType == 1){
        tpos = new float[3];
        float* playerT = Vec2::fromValues(x, z);
        int ok = this->trackDB->findNearestPositionOnTDB(playerT, p, q, tpos);
        if(ok < 0) return NULL;
        x = playerT[0];
        z = playerT[1];
    }
    if(itemTrackType == 2){
        tpos = new float[3];
        float* playerT = Vec2::fromValues(x, z);
        int ok = this->roadDB->findNearestPositionOnTDB(playerT, p, q, tpos);
        if(ok < 0) return NULL;
        x = playerT[0];
        z = playerT[1];
    } 
    if(itemTrackType == 3){
        tpos = new float[3];
        float* playerT = Vec2::fromValues(x, z);
        int ok = this->roadDB->findNearestPositionOnTDB(playerT, p, q, tpos);
        if(ok < 0) return NULL;
        float* buffer;
        int len;
        this->roadDB->getVectorSectionLine(buffer, len, playerT[0], playerT[1], tpos[0]);
        qDebug() << "len "<<len;
        bool ok1 = this->trackDB->getSegmentIntersectionPositionOnTDB(playerT, buffer, len, p, q, tpos);
        if(!ok1) return NULL;
        x = playerT[0];
        z = playerT[1];
        //return NULL;
    }

    Tile *tTile = requestTile(x, z);
    if(tTile == NULL) return NULL;
    if(tTile->loaded != 1) return NULL;
    
    int snapableSide = -1;
    if(placementStickToTarget && placementAutoTargetType == 3){
        snapableSide = tTile->getNearestSnapablePosition(p, q);
    }
        
    float endp[5];
    memset(endp, 0, sizeof(endp));
    endp[3] = 1;
    float firstPos[3];
    if ((r->type == "trackobj" || r->type == "dyntrack" )) {
        if(r->type == "dyntrack"){
            this->roadDB->setDefaultEnd(0);
            this->trackDB->setDefaultEnd(0);
        }
        qDebug() <<"1: "<< x <<" "<<z<<" "<<p[0]<<" "<<p[1]<<" "<<p[2]; 
        int oldx = x;
        int oldz = z;
        Vec3::copy(firstPos, p);
        if(this->tsection->isRoadShape(r->value))
            this->roadDB->findPosition(x, z, p, q, endp, r->value);
        else
            this->trackDB->findPosition(x, z, p, q, endp, r->value);
        Game::check_coords(x, z, p);
        firstPos[0] -= (x-oldx)*2048;
        firstPos[2] -= (z-oldz)*2048;
        qDebug() <<"2: "<< x <<" "<<z<<" "<<p[0]<<" "<<p[1]<<" "<<p[2]; 
        tTile = requestTile(x, z);
        if(tTile == NULL) return NULL;
        if(tTile->loaded != 1) return NULL;
    }
    
    WorldObj* nowy = tTile->placeObject(p, q, r, tpos);

    if ((r->type == "trackobj" || r->type == "dyntrack" )&& nowy != NULL) {
        if(nowy->endp == 0) nowy->endp = new float[5];
        memcpy(nowy->endp, endp, sizeof(float)*5);
        Vec3::copy(nowy->firstPosition,firstPos);
    }
    nowy->snapped(snapableSide);
    if(nowy->typeID == nowy->sstatic){
        moveWorldObjToTile(nowy->x, nowy->y, nowy);
    }
    Undo::PushWorldObjPlaced(nowy);
    return nowy;
}

void Route::dragWorldObject(WorldObj* obj, int x, int z, float* pos){
    if(obj->typeObj != WorldObj::worldobj)
        return;
    if(obj->isTrackItem() || obj->typeID == obj->groupobject || obj->typeID == obj->ruler ){
        obj->setPosition(x, z, pos);
        obj->setMartix();
        return;
    }
    if (obj->typeID == obj->trackobj || obj->typeID == obj->dyntrack)
        if(roadDB->ifTrackExist(obj->x, obj->y, obj->UiD) || trackDB->ifTrackExist(obj->x, obj->y, obj->UiD)){
            obj->setPosition(x, z, pos);
            obj->setMartix();
            return;
    }
    float tpos[3];
    Vec3::copy(tpos, pos);
    Game::check_coords(x, z, tpos);
    Tile *tTile = requestTile(x, z);
    if(tTile == NULL) return;
    if(tTile->loaded != 1) return;
    int snapableSide = -1;
    float q[4];
    Quat::copy(q, obj->qDirection);
    
    if(placementStickToTarget && placementAutoTargetType == 3){
        snapableSide = tTile->getNearestSnapablePosition(tpos, q, obj->UiD);
    }

    if ((obj->typeID == obj->trackobj || obj->typeID == obj->dyntrack )) {
        if(this->tsection->isRoadShape(obj->sectionIdx)){
            this->roadDB->setDefaultEnd(0);
            this->roadDB->findPosition(x, z, tpos, q, obj->endp, obj->sectionIdx);
        }else{
            this->trackDB->setDefaultEnd(0);
            this->trackDB->findPosition(x, z, tpos, q, obj->endp, obj->sectionIdx);
        }
        tTile = requestTile(x, z);
        if(tTile == NULL) return;
        if(tTile->loaded != 1) return;
    }

    obj->setPosition(tpos);
    Vec3::copy(obj->firstPosition, obj->position);
    obj->setQdirection(q);
    obj->snapped(snapableSide);
    moveWorldObjToTile(x, z, obj);
    obj->setMartix();
}

TRitem *Route::getTrackItem(int TID, int UID){
    if(TID == 0)
        return trackDB->trackItems[UID];
    if(TID == 1)
        return roadDB->trackItems[UID];
    return NULL;
    
}

void Route::actPickNewEventLocation(int x, int z, float* p){
    if(currentActivity == NULL)
        return;
    float tp[3];
    float tpos[3];
    float posT[2];
    
    Vec3::copy(tp, p);
    Game::check_coords(x, z, tp);
    posT[0] = x;
    posT[1] = z;            
    int ok = this->trackDB->findNearestPositionOnTDB(posT, tp, NULL, tpos);
    if(ok >= 0){
        currentActivity->pickNewEventLocation(tpos);
    }
}

void Route::actNewLooseConsist(int x, int z, float* p){
    if(currentActivity == NULL)
        return;
    float tp[3];
    float tpos[3];
    float posT[2];
    
    Vec3::copy(tp, p);
    Game::check_coords(x, z, tp);
    posT[0] = x;
    posT[1] = z;            
    int ok = this->trackDB->findNearestPositionOnTDB(posT, tp, NULL, tpos);
    if(ok >= 0){
        currentActivity->newLooseConsist(tpos);
    }
}

Tile * Route::requestTile(int x, int z){
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
    return tTile;
}

void Route::linkSignal(int x, int z, float* p, WorldObj* obj){
    if(obj == NULL)
        return;
    if(obj->typeObj != WorldObj::worldobj)
        return;
    if(obj->typeID != obj->signal)
        return;
    SignalObj* sobj = (SignalObj*)obj;
    float *tpos = new float[3];
    float* playerT = Vec2::fromValues(x, z);
    int ok = this->trackDB->findNearestPositionOnTDB(playerT, p, NULL, tpos);
    if(ok < 0) return;
    sobj->linkSignal(tpos[0], tpos[1]);
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

WorldObj* Route::autoPlaceObject(int x, int z, float* p, int mode) {
    if(ref->selected == NULL) return NULL;
    Game::check_coords(x, z, p);
    
    autoPlacementLastPlaced.clear();
    
    TDB * tdb = NULL;
    if(placementAutoTargetType == 0)
        tdb = this->trackDB;
    else if(placementAutoTargetType == 1)
        tdb = this->roadDB;
    else if(placementAutoTargetType == 2)
        tdb = this->trackDB;
    else
        return NULL;
    
    // pozycja wzgledem TDB:
    float* tpos = new float[3];
    float* playerT = Vec2::fromValues(x, z);
    int ok = tdb->findNearestPositionOnTDB(playerT, p, NULL, tpos);
    if(ok < 0) return NULL;
    
    x = playerT[0];
    z = playerT[1];
    int trackNodeIdx = tpos[0];
    int length = tdb->getVectorSectionLength(trackNodeIdx);
    int metry = 0;
    float drawPosition1[7];
    float drawPosition2[7];
    float xyz[3];
    float *quat = Quat::create();
    float *vec1 = Vec3::create();
    vec1[2] = -1.0;
    float *vec2 = Vec3::create();
    float step = placementAutoLength;
    float startPos = 0;
    float endPos = length;
    float rot = 0;
    if(mode == 1){
        startPos = tpos[1];
    }
    if(mode == 2){
        startPos = 0;
        endPos = tpos[1];
        rot = M_PI;
    }    
    float i1, i2;
    for(float i = startPos; i < endPos; i+=step ){
        if(mode == 2){
           i1 = endPos - i;
           i2 = i1-step;
           if(i2 < 0)
                i2 = 0 + 0.1;
        } else {
            i1 = i;
            i2 = i1+step;
            if(i2 > length)
                i2 = length - 0.1;
        }
        if(!tdb->getDrawPositionOnTrNode((float*)drawPosition1, trackNodeIdx, i1))
            return NULL;
        if(!tdb->getDrawPositionOnTrNode((float*)drawPosition2, trackNodeIdx, i2))
            return NULL;
        x = drawPosition1[5];
        z = -drawPosition1[6];
        
        /*x = currentPosition[5];
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
            Quat::rotateY(quat, quat, rot);
        }else {
            Quat::fill(quat);
            Quat::rotateY(quat, quat, currentPosition[3]);
            Quat::rotateX(quat, quat, -currentPosition[4]);
        }
        */
        drawPosition2[0] += 2048*(drawPosition2[5]-drawPosition1[5]);
        drawPosition2[2] += 2048*(drawPosition2[6]-drawPosition1[6]);
        float dlugosc = Vec3::distance(drawPosition1, drawPosition2);

        int someval = (((drawPosition2[2]-drawPosition1[2])+0.00001f)/fabs((drawPosition2[2]-drawPosition1[2])+0.00001f));
        float rotY = ((float)someval+1.0)*(M_PI/2)+(float)(atan((drawPosition1[0]-drawPosition2[0])/(drawPosition1[2]-drawPosition2[2]))); 
        float rotX = -(float)(asin((drawPosition1[1]-drawPosition2[1])/(dlugosc))); 

        if(placementAutoTwoPointRot){
            Quat::fill(quat);
            Quat::rotateY(quat, quat, -rotY+M_PI);
            Quat::rotateX(quat, quat, rotX);
        }else {
            Quat::fill(quat);
            Quat::rotateY(quat, quat, drawPosition1[3]+rot);
            Quat::rotateX(quat, quat, -drawPosition1[4]);
        }
        
        float offset[3];
        Vec3::copy(offset, placementAutoTranslationOffset);
        float offsetq[4];
        Quat::fill(offsetq);
        Quat::rotateY(offsetq,offsetq,(placementAutoRotationOffset[1]*M_PI)/180);
        Quat::rotateX(offsetq,offsetq,(placementAutoRotationOffset[0]*M_PI)/180);
        
        Vec3::transformQuat(offset, offset, quat);
        Quat::multiply(quat, quat, offsetq);
        
        xyz[0] = drawPosition1[0] + offset[0];
        xyz[1] = drawPosition1[1] + offset[1];
        xyz[2] = -drawPosition1[2] + offset[2];      
        
        autoPlacementLastPlaced.push_back(placeObject(x, z, (float*) xyz, quat, ref->selected));
    }

    return NULL;
    
}

void Route::findSimilar(WorldObj* obj, GroupObj* group, float *playerT, int tileRadius){
    if(obj->typeID == WorldObj::groupobject)
        return;
    int mintile = -tileRadius;
    int maxtile = tileRadius;
    
    Tile *tTile;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            tTile = tile[((int)playerT[0] + i)*10000 + (int)playerT[1] + j];
            if (tTile == NULL)
                continue;
            if (tTile->loaded == 1) {
                tTile->findSimilar(obj, group);
            }
        }
    }
}

void Route::autoPlacementDeleteLast(){
    for(int i = 0; i < autoPlacementLastPlaced.length(); i++){
        deleteObj(autoPlacementLastPlaced[i]);
    }
    autoPlacementLastPlaced.clear();
}


void Route::replaceWorldObjPointer(WorldObj* o, WorldObj* n){
    if(o->typeObj != WorldObj::worldobj)
        return;
    if(n->typeObj != WorldObj::worldobj)
        return;
    int x = o->x;
    int z = o->y;
    
    Tile *tTile;
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        return;
    
    for(int i = 0; i < tTile->jestObiektow; i++){
        if(tTile->obiekty[i] == NULL) continue;
        if(tTile->obiekty[i]->UiD == o->UiD){
            tTile->obiekty[i] = n;
            emit objSelected((GameObj*)n);
            return;
        }
    }
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
    if(obj->typeObj != WorldObj::worldobj)
        return;
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
        Undo::Clear();
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

void Route::setTDB(TDB* tdb, bool road){
    if(tdb == NULL)
        return;
    if(road){
        delete this->roadDB;
        this->roadDB = tdb;
        Game::roadDB = tdb;
    } else {
        delete this->trackDB;
        this->trackDB = tdb;
        Game::trackDB = tdb;
    }
}

void Route::toggleToTDB(WorldObj* obj) {
    if(obj == NULL) return;
    if(obj->typeObj != WorldObj::worldobj)
        return;
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
    if(obj->typeObj != WorldObj::worldobj)
        return;
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
    
    Undo::StateBegin();
    Undo::PushTrackDB(trackDB, false);
    Undo::PushTrackDB(roadDB, true);
    Undo::StateEnd();
    
    addToTDB(obj);
}

void Route::newPositionTDB(WorldObj* obj) {
    if(obj->typeObj != WorldObj::worldobj)
        return;
    int x = obj->x;//post[0];
    int z = obj->y;//post[1];
    float p[3]; 
    p[0] = obj->firstPosition[0];
    p[1] = obj->firstPosition[1];
    p[2] = obj->firstPosition[2];
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
            this->roadDB->findPosition(x, z, (float*) &p, (float*) &q, track->endp, track->sectionIdx);
        else
            this->trackDB->findPosition(x, z, (float*) &p, (float*) &q, track->endp, track->sectionIdx);

        //Vec3::copy(obj->position, p);
        obj->setPosition(p);
        obj->setQdirection(q);
        obj->setMartix();
        
        moveWorldObjToTile(x, z, obj);
    }
}

void Route::moveWorldObjToTile(int x, int z, WorldObj* obj){
    if(obj == NULL)
        return;
    if(obj->typeObj != WorldObj::worldobj)
        return;
    //qDebug() << "new tile" << obj->x <<" "<< obj->y<<" "<< obj->position[0]<<" "<< -obj->position[2];
    float oldPos[3];
    int xx = x, zz = z;
    Vec3::copy(oldPos, obj->position);
    Game::check_coords(xx, zz, oldPos);
    if(xx == obj->x && zz == obj->y)
        return;
    Vec3::copy(obj->position, oldPos);
    x = xx;
    z = zz;
    
    qDebug() << "obj outside tile border !!!";
    //qDebug() << "new tile" << x <<" "<< z;
    qDebug() << "new tile" << xx <<" "<< zz <<" "<< obj->position[0]<<" "<< -obj->position[2];

    
    Undo::Clear();
    
    Tile *tTile = tile[((obj->x)*10000 + obj->y)];
    tTile->deleteObject(obj);
    
    tTile = requestTile(x, z);
    if(tTile == NULL) return;
    if(tTile->loaded != 1) return;
    
    if (tTile->loaded == 1) {
        obj->firstPosition[0] -= (x-obj->x)*2048;
        obj->firstPosition[2] -= (z-obj->y)*2048;
        obj->placedAtPosition[0] = obj->position[0];
        obj->placedAtPosition[2] = obj->position[2];
        tTile->placeObject(obj);
    }
    qDebug() << "--" << obj->x <<" "<< obj->y<<" "<< obj->position[0]<<" "<< -obj->position[2];
}

void Route::deleteTDBTree(WorldObj* obj){
    Undo::StateBegin();
    Undo::PushTrackDB(this->trackDB, false);
    Undo::PushTrackDB(this->roadDB, true);
    if (obj->type == "trackobj" || obj->type == "dyntrack") {
        this->roadDB->deleteTree(obj->x, obj->y, obj->UiD);
        this->trackDB->deleteTree(obj->x, obj->y, obj->UiD);
    }
    Undo::StateEnd();
}

void Route::deleteTDBVector(WorldObj* obj){
    Undo::StateBegin();
    Undo::PushTrackDB(this->trackDB, false);
    Undo::PushTrackDB(this->roadDB, true);
    
    if (obj->type == "trackobj" || obj->type == "dyntrack") {
        this->roadDB->deleteVectorSection(obj->x, obj->y, obj->UiD);
        this->trackDB->deleteVectorSection(obj->x, obj->y, obj->UiD);
    }
    Undo::StateEnd();
}

void Route::undoPlaceObj(int x, int y, int UiD){
    Tile *tTile;
    tTile = tile[((x)*10000 + y)];
    if (tTile == NULL)
        return;
    
    for(int i = 0; i < tTile->jestObiektow; i++){
        if(tTile->obiekty[i] == NULL) continue;
        if(tTile->obiekty[i]->UiD == UiD){
            tTile->obiekty[i]->loaded = false;
            tTile->obiekty[i]->modified = false;
            tTile->obiekty[i]->UiD = -1;
            emit objSelected(NULL);
            return;
        }
    }
}

void Route::deleteObj(WorldObj* obj) {
    if(obj == NULL)
        return;
    if(obj->typeObj != WorldObj::worldobj)
        return;
    if(obj->typeID == obj->groupobject) {
        GroupObj *gobj = (GroupObj*)obj;
        for(int i = 0; i < gobj->objects.size(); i++ ){
            deleteObj(gobj->objects[i]);
        }
        return;
    }
    
    Undo::PushWorldObjRemoved(obj);
    
    if (obj->type == "trackobj" || obj->type == "dyntrack") {
        Undo::PushTrackDB(trackDB, false);
        Undo::PushTrackDB(roadDB, true);
        removeTrackFromTDB(obj);
        if(Game::leaveTrackShapeAfterDelete)
            return;
    }
    
    obj->loaded = false;
    obj->modified = true;
    if (obj->isTrackItem()) {
        Undo::PushTrackDB(trackDB, false);
        Undo::PushTrackDB(roadDB, true);
        obj->deleteTrItems();
    }
    Tile *tTile;
    tTile = tile[((obj->x)*10000 + obj->y)];
    if (tTile != NULL)
        tTile->jestHiddenObj++;
}

void Route::removeTrackFromTDB(WorldObj* obj) {
    if(obj->typeObj != WorldObj::worldobj)
        return;
    bool ok;
    ok = this->roadDB->removeTrackFromTDB(obj->x, obj->y, obj->UiD);
    ok |= this->trackDB->removeTrackFromTDB(obj->x, obj->y, obj->UiD);
    if(ok)
        obj->removedFromTDB();
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
    
    ActLib::GetUnsavedInfo(items);
    
    /*foreach(Service *s, service){
        if(s == NULL)
            continue;
        if(s->isModified())
            items.push_back("[S] "+s->name);
    }*/
    foreach(Path *p, path){
        if(p == NULL)
            continue;
        if(p->isModified())
            items.push_back("[P] "+p->name);
    }
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
    ActLib::SaveAll();
    /*foreach(Service *s, service){
        if(s == NULL)
            continue;
        if(s->isModified())
            s->save();
    }
    foreach(Path *p, path){
        if(p == NULL)
            continue;
        if(p->isModified())
            p->save();
    }*/
}

void Route::createNewPaths() {
    if (!Game::writeEnabled) return;
    Path::CreatePaths(this->trackDB);
}

QMap<QString, Coords*> Route::getMkrList(){
    return this->mkrList;
}

void Route::nextDefaultEnd(){
    this->trackDB->nextDefaultEnd();
    this->roadDB->nextDefaultEnd();
}

void Route::flipObject(WorldObj *obj){
    if(obj == NULL)
        return;
    if(obj->typeObj != WorldObj::worldobj)
        return;
    if(obj->typeID == obj->trackobj || obj->typeID == obj->dyntrack ){
        nextDefaultEnd();
        newPositionTDB(obj);                
    } else {
        obj->flip();
    }
                
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

    QString templateDir = "templateroute_0.6/";
    QString res = QString("tsre_assets/templateroute_0.6/");//+templateDir;
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
    
    if (tile[x*10000 + z] == NULL)
        tile[x*10000 + z] = new Tile(x, z);
    if (tile[x*10000 + z]->loaded == 1)
        return;
            
    Tile::saveEmpty(x, -z);
    //Terrain::saveEmpty(x, -z);
    TerrainLib::saveEmpty(x, -z);
    TerrainLib::reload(x, z);
    reloadTile(x, z);

    if(Game::autoGeoTerrain){
        float pos[3];
        Vec3::set(pos, 0, 0, 0);
        TerrainLib::setHeightFromGeo(x, z, (float*)&pos);
    }
}

void Route::showTrkEditr(Trk * val){
    TrkWindow trkWindow;
    if(val == NULL)
        val = this->trk;
    trkWindow.trk = val;
    trkWindow.exec();
}