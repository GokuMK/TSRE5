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

Route::Route() {

    QFile file(Game::root + "/routes");
    if(!file.exists()) return;
    file.setFileName(Game::root + "/global");
    if(!file.exists()) return;
    
    file.setFileName(Game::root + "/routes/" + Game::route);
    if (!file.exists()) {
        qDebug() << "route does not exist";
        if (Game::createNewRoutes) {
            qDebug() << "new Route";
            Route::createNew();
        }
    }
    
    this->loadTrk();
    this->trackDB = new TDB((Game::root + "/routes/" + Game::route + "/" + Game::route + ".tdb"));
    this->ref = new Ref((Game::root + "/routes/" + Game::route + "/" + Game::route + ".ref"));
}

Route::Route(const Route& orig) {
}

Route::~Route() {
}

void Route::loadTrk(){
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
        if (sh == ("routestart")) {
            startTileX = ParserX::parsujr(data);
            startTileY = ParserX::parsujr(data);
            startpX = ParserX::parsujr(data);
            startpZ = ParserX::parsujr(data);
            break;
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
    int mintile = -2;
    int maxtile = 2;

    if (!selection) {
        TerrainLib::render(gluu, playerT, playerW, target, fov);
        //trackDB->renderAll(gluu, playerT, playerRot);
        //trackDB->renderLines(gluu, playerT, playerRot);
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

        if (r->type == "trackobj") {
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

void Route::addToTDB(WorldObj* obj, float* post, float* pos) {
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

        this->trackDB->placeTrack(x, z, (float*) &p, (float*) &q, track->sectionIdx, obj->UiD);
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

        this->trackDB->findPosition(x, z, (float*) &p, (float*) &q, track->sectionIdx, obj->UiD);
        obj->setPosition(p);
        obj->setQdirection(q);
        obj->setMartix();
    }
}

void Route::save() {
    if(!Game::writeEnabled) return;
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
    this->trackDB->save();
}

void Route::createNewPaths() {
    if(!Game::writeEnabled) return;
    Path::CreatePaths(this->trackDB);
}

void Route::createNew() {
    if(!Game::writeEnabled) return;
    
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
    tile[x*10000 + z] = new Tile(x, z);
    return;
}

void Route::newTile(int x, int z){
    if(!Game::writeEnabled) return;
    Tile::saveEmpty(x, -z);
    Terrain::saveEmpty(x, -z);
    TerrainLib::reload(x, z);
    reloadTile(x, z);
}

void Route::saveTrk() {
    if(!Game::writeEnabled) return;
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
    out << "	RouteID ( " << Game::route << " )" << "\n";
    out << "	Name ( " << Game::route << " )" << "\n";
    out << "	Description ( "" )" << "\n";
    out << "	Graphic ( " << Game::route << ".ace )" << "\n";
    out << "	LoadingScreen ( Load.ace )" << "\n";
    out << "	FileName ( " << Game::route << " )" << "\n";
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