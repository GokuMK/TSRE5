#include "Route.h"
#include "GLUU.h"
#include "Tile.h"
#include "GLMatrix.h"
#include "TerrainLib.h"
#include "Game.h"
#include "TrackObj.h"

Route::Route() {
    this->trackDB = new TDB((Game::root + "/routes/" + Game::route + "/" + Game::route + ".tdb"));
    this->ref = new Ref((Game::root + "/routes/" + Game::route + "/" + Game::route + ".ref"));
}

Route::Route(const Route& orig) {
}

Route::~Route() {
}

WorldObj* Route::getObj(int x, int z, int uid) {
    Tile *tTile;
    //try {
        tTile = tile[((x)*10000 + z)];
        if(tTile == NULL)
            return new WorldObj();
        return tTile->getObj(uid);
    //} catch (const std::out_of_range& oor) {
        
    //}
}

void Route::transalteObj(int x, int z, float px, float py, float pz, int uid) {
    Tile *tTile;
    //try {
        tTile = tile[((x)*10000 + z)];
        if(tTile == NULL)
            return;
        tTile->transalteObj(px,py,pz,uid);
    //} catch (const std::out_of_range& oor) {

    //}
}

void Route::render(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, bool selection) {
    int mintile = -2;
    int maxtile = 2;

    if (!selection) {
        TerrainLib::render(gluu, playerT, playerW, target, fov);
        trackDB->renderAll(gluu, playerT);
        trackDB->renderLines(gluu, playerT);
    }
    //for (var key in this.tile){
    //    this.tile[key].inUse = false;
    // }
    Tile *tTile;
    for (int i = mintile; i <= maxtile; i++) {
        for (int j = maxtile; j >= mintile; j--) {
            //try {
                tTile = tile[(playerT[0] + i)*10000 + playerT[1] + j];
            //    if (tTile->loaded == -2) continue;
            //} catch (const std::out_of_range& oor) {
                if(tTile == NULL)
                    tile[(playerT[0] + i)*10000 + playerT[1] + j] = new Tile(playerT[0] + i, playerT[1] + j);
            //}
            tTile = tile[(playerT[0] + i)*10000 + playerT[1] + j];
            //tTile->inUse = true;
            if (tTile->loaded == 1) {
                gluu->mvPushMatrix();
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
                tTile->render(playerT, playerW, target, fov, selection);
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

WorldObj* Route::placeObject(int x, int z, float* p){
    float q[4];
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    q[3] = 1;
    placeObject(x, z, p, (float*)&q, ref->selected);
}

WorldObj* Route::placeObject(int x, int z, float* p, float* q){
    placeObject(x, z, p, (float*)&q, ref->selected);
}

WorldObj* Route::placeObject(int x, int z, float* p, float* q, Ref::RefItem* r){
    Game::check_coords(x, z, p);
    
    Tile *tTile;
    //try {
        tTile = tile[((x)*10000 + z)];
    //} catch (const std::out_of_range& oor) {
    if(tTile == NULL)
        tile[(x)*10000 + z] = new Tile(x, z);
    //}
    if (tTile->loaded == -2){
        if(TerrainLib::isLoaded(x, z)){
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
        
        WorldObj* nowy = tTile->placeObject(p, q, r);
        
        if(r->type == "trackobj"){
            //this->trackDB->placeTrack(x, z, p, q, r->value, nowy->UiD);
            this->trackDB->findPosition(x, z, p, q, r->value, nowy->UiD);
            //findPosition
            nowy->setPosition(p);
            nowy->setQdirection(q);
            nowy->setMartix();
        }
        
        return nowy;
        //}
    }
    return NULL;
}

void Route::addToTDB(WorldObj* obj, float* post, float* pos){
    int x = post[0];
    int z = post[1];
    float p[3];
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    Game::check_coords(x, z, (float*)&p);
    
    if(obj->type == "trackobj"){
        float q[4];
        q[0] = 0;
        q[1] = 0;
        q[2] = 0;
        q[3] = 1;
        TrackObj* track = (TrackObj*)obj;
            //this->trackDB->placeTrack(x, z, p, q, r, nowy->UiD);
            
            this->trackDB->placeTrack(x, z, (float*)&p, (float*)&q, track->sectionIdx, obj->UiD);
            obj->setPosition(p);
            obj->setQdirection(q);
            obj->setMartix();
        }
}

void Route::newPositionTDB(WorldObj* obj, float* post, float* pos){
    int x = post[0];
    int z = post[1];
    float p[3];
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    Game::check_coords(x, z, (float*)&p);
    
    if(obj->type == "trackobj"){
        float q[4];
        q[0] = 0;
        q[1] = 0;
        q[2] = 0;
        q[3] = 1;
        TrackObj* track = (TrackObj*)obj;
            //this->trackDB->placeTrack(x, z, p, q, r, nowy->UiD);
            
            this->trackDB->findPosition(x, z, (float*)&p, (float*)&q, track->sectionIdx, obj->UiD);
            obj->setPosition(p);
            obj->setQdirection(q);
            obj->setMartix();
        }
}
void Route::save(){
    qDebug() << "save";
    for (auto it = tile.begin(); it != tile.end(); ++it) {
        //console.log(obj.type);
        Tile* tTile = (Tile*) it->second;
        if (tTile == NULL) continue;
        if (tTile->loaded == 1 && tTile->modified){
            tTile->save();
            tTile->modified = false;
        }
    }
    this->trackDB->save();
}