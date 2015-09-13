#include <QFile>
#include <QDir>
#include "Route.h"
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
#include "Flex.h"

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

    this->loadTrk();
    qDebug() << Game::routeName;
    
    this->tsection = new TSectionDAT();
    this->trackDB = new TDB(tsection, false, (Game::root + "/routes/" + Game::route + "/" + Game::routeName + ".tdb"));
    this->roadDB = new TDB(tsection, true, (Game::root + "/routes/" + Game::route + "/" + Game::routeName + ".rdb"));
    this->ref = new Ref((Game::root + "/routes/" + Game::route + "/" + Game::routeName + ".ref"));
    
    loaded = true;
}

Route::Route(const Route& orig) {
}

Route::~Route() {
}

void Route::loadTrk() {
    QString path = Game::root + "/routes/" + Game::route + "/" + Game::route + ".trk";
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    ParserX::nextLine(data);

    QString sh = "Tr_RouteFile";
    ParserX::szukajsekcji1(sh, data);
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("routestart")) {
            startTileX = ParserX::parsujr(data);
            startTileY = ParserX::parsujr(data);
            //qDebug() << startTileX << startTileY;
            //break;
            startpX = ParserX::parsujr(data);
            startpZ = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("filename")) {
            Game::routeName = ParserX::odczytajtc(data);
            ParserX::pominsekcje(data);
            continue;
        }
        ParserX::pominsekcje(data);
    }
}

WorldObj* Route::getObj(int x, int z, int uid) {
    Tile *tTile;
    //try {
    tTile = tile[((x)*10000 + z)];
    if (tTile == NULL)
        return new WorldObj();
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

void Route::render(GLUU *gluu, float * playerT, float* playerW, float* target, float playerRot, float fov, bool selection) {
    if(!loaded) return;
    
    int mintile = -2;
    int maxtile = 2;

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
            tTile = tile[(playerT[0] + i)*10000 + playerT[1] + j];
            //    if (tTile->loaded == -2) continue;
            //} catch (const std::out_of_range& oor) {
            if (tTile == NULL)
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
    if (!selection) {
        trackDB->renderAll(gluu, playerT, playerRot);
        trackDB->renderLines(gluu, playerT, playerRot);
        roadDB->renderAll(gluu, playerT, playerRot);
        roadDB->renderLines(gluu, playerT, playerRot);
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

void Route::setTerrainToTrackObj(WorldObj* obj){
    if(obj == NULL) return;
    //TrackObj* track = (TrackObj*)obj;
    float* punkty = new float[10000];
    float* ptr = punkty;
    this->trackDB->setTerrainToTrackObj(obj->x, obj->y, obj->UiD, obj->matrix, ptr);
    
    int length = ptr - punkty;
    qDebug() << "l "<<length;
    //for(int i = 0; i < length; ){
    //    qDebug() << punkty[i++] << " " << punkty[i++] <<" "<<punkty[i++] <<"";
    //}
    
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
    Vec3::transformMat4(p3, p3, obj->matrix);
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
    
    for(int i = 0; i < length;i+=3 ){
        float h = vec3d - vec3.x*punkty[i] - vec3.z*punkty[i+2];
        qDebug() << punkty[i] << " " << punkty[i+1] <<" " << punkty[i+2] <<" "<<h <<"";
    }
    //qDebug() << p1[0] << " " << p1[1] <<" "<<p1[2] <<"";
    //qDebug() << p2[0] << " " << p2[1] <<" "<<p2[2] <<"";
    //qDebug() << p3[0] << " " << p3[1] <<" "<<p3[2] <<"";
    int xx, zz;
    float h; 
    
    for(int ii = -1; ii < 3; ii++)
        for(int jj = -1; jj < 3; jj++)
            for(int i = 0; i< length; i+=3){
                xx = punkty[i]/8;
                zz = punkty[i+2]/8;
                xx+=ii;
                zz+=jj;
                h = vec3d - vec3.x*xx*8 - vec3.z*zz*8;
                TerrainLib::setHeight256(obj->x, obj->y, xx, zz, h);
                //qDebug() << xx << " " << zz << " " << h;
            }
    
    TerrainLib::refresh(obj->x, obj->y);
}

WorldObj* Route::placeObject(int x, int z, float* p) {
    float q[4];
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    q[3] = 1;
    placeObject(x, z, p, (float*) &q, ref->selected);
}

WorldObj* Route::placeObject(int x, int z, float* p, float* q) {
    placeObject(x, z, p, (float*) &q, ref->selected);
}

WorldObj* Route::placeObject(int x, int z, float* p, float* q, Ref::RefItem* r) {
    if(r == NULL) return NULL;
    Game::check_coords(x, z, p);
    
    Tile *tTile;
    //try {
    tTile = tile[((x)*10000 + z)];
    //} catch (const std::out_of_range& oor) {
    if (tTile == NULL)
        tile[(x)*10000 + z] = new Tile(x, z);
    //}
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

        WorldObj* nowy = tTile->placeObject(p, q, r);

        if ((r->type == "trackobj" || r->type == "dyntrack" )&& nowy != NULL) {
            //this->trackDB->placeTrack(x, z, p, q, r->value, nowy->UiD);
            if(this->tsection->isRoadShape(r->value))
                this->roadDB->findPosition(x, z, p, q, r->value, nowy->UiD);
            else
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

void Route::addToTDB(WorldObj* obj, float* post, float* pos) {
    if(obj == NULL) return;
    
    int x = post[0];
    int z = post[1];
    float p[3];
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    Game::check_coords(x, z, (float*) &p);
    float q[4];
    q[0] = obj->tRotation[0]; //track->qDirection[0];
    q[1] = obj->tRotation[1]; //qDirection[1];
    q[2] = 0; //track->qDirection[2];
    q[3] = 1; //track->qDirection[3];

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
            this->trackDB->placeTrack(x, z, (float*) &p, (float*) &q, track->sectionIdx, obj->UiD);
        obj->setPosition(p);
        obj->setQdirection(q);
        obj->setMartix();
    } else if(obj->type == "dyntrack"){
        DynTrackObj* dynTrack = (DynTrackObj*) obj;
        if(dynTrack->sectionIdx == -1)
            this->trackDB->fillDynTrack(dynTrack);
        this->trackDB->placeTrack(x, z, (float*) &p, (float*) &q, dynTrack->sectionIdx, obj->UiD);
        obj->setPosition(p);
        obj->setQdirection(q);
        obj->setMartix();
    }
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
            this->roadDB->findPosition(x, z, (float*) &p, (float*) &q, track->sectionIdx, obj->UiD);
        else
            this->trackDB->findPosition(x, z, (float*) &p, (float*) &q, track->sectionIdx, obj->UiD);
        obj->setPosition(p);
        obj->setQdirection(q);
        obj->setMartix();
    }
}

void Route::deleteObj(WorldObj* obj) {
    obj->loaded = false;
    obj->modified = true;
    if (obj->type == "trackobj" || obj->type == "dyntrack") {
        removeTrackFromTDB(obj);
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
}

void Route::createNewPaths() {
    if (!Game::writeEnabled) return;
    Path::CreatePaths(this->trackDB);
}

void Route::nextDefaultEnd(){
    this->trackDB->nextDefaultEnd();
    this->roadDB->nextDefaultEnd();
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
    QDir().mkdir(path + "/world");

    int x = -5000;
    int z = 15000;
    saveTrk();
    TDB::saveEmpty();
    Tile::saveEmpty(x, z);
    Terrain::saveEmpty(x, z);

    QString res = "resources/templateRoute/";
    path += "/";

    QFile::copy(res + "sigcfg.dat", path + "sigcfg.dat");
    QFile::copy(res + "ttype.dat", path + "ttype.dat");
    QFile::copy(res + "template.ref", path + Game::route + ".ref");
    QFile::copy(res + "ttype.dat", path + "ttype.dat");

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
    Terrain::saveEmpty(x, -z);
    TerrainLib::reload(x, z);
    reloadTile(x, z);
}

void Route::saveTrk() {
    if (!Game::writeEnabled) return;
    QFile file;
    QTextStream out;
    QString filepath;

    filepath = Game::root + "/routes/" + Game::route + "/" + Game::route + ".trk";
    file.setFileName(filepath);
    //qDebug() << filepath;
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    out.setDevice(&file);
    out.setRealNumberPrecision(8);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);

    out << "SIMISA@@@@@@@@@@JINX0r1t______" << "\n\n";
    out << "Tr_RouteFile (" << "\n";
    out << "	RouteID ( \"" << Game::route << "\" )" << "\n";
    out << "	Name ( \"" << Game::route << "\" )" << "\n";
    out << "	Description ( \"\" )" << "\n";
    out << "	Graphic ( \"" << Game::route << ".ace\" )" << "\n";
    out << "	LoadingScreen ( Load.ace )" << "\n";
    out << "	FileName ( \"" << Game::route << "\" )" << "\n";
    out << "	Electrified ( 00000000 )" << "\n";
    out << "	Mountains ( 00000000 )" << "\n";
    out << "	OverheadWireHeight ( 7.23 )" << "\n";
    out << "	PassengerRuleSet ( 0 )" << "\n";
    out << "	FreightRuleSet ( 0 )" << "\n";
    out << "	SignalSet ( 0 )" << "\n";
    out << "	GantrySet ( 0 )" << "\n";
    out << "	TrackGauge ( 0 )" << "\n";
    out << "	Era ( 0 )" << "\n";
    out << "	SpeedLimit ( 35.7632 )" << "\n";
    out << "	Environment (" << "\n";
    out << "		SpringClear ( sun.env )" << "\n";
    out << "		SpringRain ( rain.env )" << "\n";
    out << "		SpringSnow ( snow.env )" << "\n";
    out << "		SummerClear ( sun.env )" << "\n";
    out << "		SummerRain ( rain.env )" << "\n";
    out << "		SummerSnow ( snow.env )" << "\n";
    out << "		AutumnClear ( sun.env )" << "\n";
    out << "		AutumnRain ( rain.env )" << "\n";
    out << "		AutumnSnow ( snow.env )" << "\n";
    out << "		WinterClear ( sun.env )" << "\n";
    out << "		WinterRain ( rain.env )" << "\n";
    out << "		WinterSnow ( snow.env )" << "\n";
    out << "	)" << "\n";
    out << "	TerrainErrorScale ( 1 )" << "\n";
    out << "	RouteStart ( -5000 15000 0 0 )" << "\n";
    out << "	DefaultCrossingSMS ( crossing.sms )" << "\n";
    out << "	DefaultSignalSMS ( signal.sms )" << "\n";
    out << "	DefaultWaterTowerSMS ( wtower.sms )" << "\n";
    out << "	DefaultCoalTowerSMS ( ctower.sms )" << "\n";
    out << "	DefaultDieselTowerSMS ( dtower.sms )" << "\n";
    out << "	TempRestrictedSpeed ( 6.7056 )" << "\n";

    out << ")" << "\n";

    out.flush();
    file.close();
}