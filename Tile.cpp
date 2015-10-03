#include "Tile.h"
#include "Game.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "WorldObj.h"
#include "StaticObj.h"
#include "DynTrackObj.h"
#include "ForestObj.h"
#include "TransferObj.h"
#include "TrackObj.h"
#include "SpeedpostObj.h"
#include "SignalObj.h"
#include "PlatformObj.h"
#include "TrWatermarkObj.h"
#include "LevelCrObj.h"
#include "PickupObj.h"
#include "GLUU.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include "GLUU.h"
#include "GLMatrix.h"

Tile::Tile() {
    modified = false;
    loaded = -2;
    inUse = false;
    x = 0;
    z = 0;
    jestObiektow = 0;
    vDbIdCount = 0;
}

Tile::Tile(int xx, int zz) {
    modified = false;
    loaded = -2;
    inUse = false;
    x = xx;
    z = zz;
    jestObiektow = 0;
    vDbIdCount = 0;
    load();
}

void Tile::initNew(){
    loaded = 1;
    jestObiektow = 0;
    vDbIdCount = 0;
    inUse = false;
    obiekty.clear();
}

Tile::Tile(const Tile& orig) {
}

Tile::~Tile() {
}

QString Tile::getNameXY(int e) {
    QString n = "+";
    if (e < 0) {
        n = "-";
        e = -e;
    }
    QString s = "000000" + QString::number(e);
    return n + s.right(6);
}

void Tile::wczytajObiekty() {
    for (auto it = obiekty.begin(); it != obiekty.end(); ++it) {
        //console.log(obj.type);
        WorldObj* obj = (WorldObj*) it->second;

        obj->load(x, z);
        if(obj->UiD < 1000000)
            if(obj->UiD > maxUiD) maxUiD = obj->UiD;
    }
    loaded = 1;
    
    //save();
}

void Tile::load() {

    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/world/w" + getNameXY(x) + "" + getNameXY(-z) + ".w";
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    //qDebug() << "Date:" << data->length;

    data->off = 0;
    sh = "Tr_Worldfile";
    ParserX::szukajsekcji1(sh, data);

    for (int tt = 0;; tt++) {
        //wybranie zdarzenia
        sh = ParserX::nazwasekcji(data).toLower();
        //qDebug() << "= " << sh;
        
        WorldObj* nowy;
        if (sh == "") {
            //("wczytano "+obiekty.size());
            loaded = 0;
            wczytajObiekty();
            return;
        } else if (sh == "tr_watermark") {
            nowy = (WorldObj*)(new TrWatermarkObj((int)ParserX::parsujr(data)));
            obiekty[jestObiektow++] = nowy;
            ParserX::pominsekcje(data);
            continue;
        } else if (sh == "vdbidcount") {
            vDbIdCount = ParserX::parsujr(data);
            //qDebug() <<vDbIdCount;
            viewDbSphere = new ViewDbSphere[vDbIdCount];
            ParserX::pominsekcje(data);
            continue;
        } else if (sh == "viewdbsphere") {
            //qDebug() <<sh;
            /*int j = 0;
            do {
                for(int i = 0; i< 3; i++){
                    sh = ParserX::nazwasekcji_inside(data).toLower();
                    //qDebug() <<sh;
                    if(sh == ("vdbid")) {
                        viewDbSphere[j].vDbId = ParserX::parsujr(data);
                    }
                    if(sh == ("position")) {
                        viewDbSphere[j].position[0] = ParserX::parsujr(data);
                        viewDbSphere[j].position[1] = ParserX::parsujr(data);
                        viewDbSphere[j].position[2] = ParserX::parsujr(data);
                    }
                    if(sh == ("radius")) {
                        viewDbSphere[j].radius = ParserX::parsujr(data);
                    }
                    ParserX::pominsekcje(data);
                }
                
                if(j > 0) ParserX::pominsekcje(data);
                j++;
            } while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == ""));
            */
            int start = data->off;
            ParserX::pominsekcje(data);
            int end = data->off;
            viewDbSphereRaw = (data->getString(start, end));
            continue;
        } 
        //if(sh == "signal") sh = "static";
        if (!createObj(&nowy, sh)) {
            ParserX::pominsekcje(data);
            continue;
        }

        while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
            nowy->set(sh, data);
            ParserX::pominsekcje(data);
        }
        obiekty[jestObiektow++] = nowy;

        ParserX::pominsekcje(data);
        continue;
    }
}

bool Tile::createObj(WorldObj** nowy, QString sh) {
    if (sh == "static") {
        *nowy = (WorldObj*) (new StaticObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
    } else if (sh == "signal") {
        *nowy = (WorldObj*) (new SignalObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
    } else if (sh == "speedpost") {
        *nowy = (WorldObj*) (new SpeedpostObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
    } else if (sh == "trackobj") {
        *nowy = (WorldObj*) (new TrackObj());
        (*nowy)->resPath = Game::root + "/global/shapes";
    } else if (sh == "gantry") {
        *nowy = (WorldObj*) (new StaticObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
    } else if (sh == "collideobject") {
        *nowy = (WorldObj*) (new StaticObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
    } else if (sh == "dyntrack") {
        *nowy = (WorldObj*) (new DynTrackObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
    } else if (sh == "forest") {
        *nowy = (WorldObj*) (new ForestObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
    } else if (sh == "transfer") {
        *nowy = (WorldObj*) (new TransferObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
    } else if (sh == "platform") {
        *nowy = (WorldObj*) (new PlatformObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (*nowy)->typeID = (*nowy)->platform;
    } else if (sh == "siding") {
        *nowy = (WorldObj*) (new PlatformObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (*nowy)->typeID = (*nowy)->siding;
    } else if (sh == "carspawner") {
        *nowy = (WorldObj*) (new PlatformObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";     
        (*nowy)->typeID = (*nowy)->carspawner;
    } else if (sh == "levelcr") {
        *nowy = (WorldObj*) (new LevelCrObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";   
    } else if (sh == "pickup") {
        *nowy = (WorldObj*) (new PickupObj());
        (*nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
    } else {
        qDebug() << sh;
        //(*nowy) = new WorldObj();
        return false;
        //
    }
    (*nowy)->type = sh;
    return true;
}

WorldObj* Tile::getObj(int uid) {
    for (int i = 0; i < jestObiektow; i++) {
        if (obiekty[i]->UiD == uid) {
            return obiekty[i];
        }
    }
    return new WorldObj();
}

void Tile::transalteObj(float px, float py, float pz, int uid) {
    for (int i = 0; i < jestObiektow; i++) {
        if (obiekty[i]->UiD == uid) {
            obiekty[i]->translate(px, py, pz);
        }
    }
    modified = true;
}

WorldObj* Tile::placeObject(float* p, Ref::RefItem* itemData) {
    float q[4];
    q[0] = 0; 
    q[1] = 0;
    q[2] = 0;
    q[3] = 1;
    placeObject(p, (float*)&q, itemData);
}

WorldObj* Tile::placeObject(float* p, float* q, Ref::RefItem* itemData) {
    if(loaded != 1) return NULL;
    if(itemData == NULL) return NULL;
    //qDebug() << pozW[0] << " " << pozW[1] << " " << pozW[2] << " " << itemData->type << " " << itemData->filename;
    
    WorldObj* nowy;
    if(!createObj(&nowy, itemData->type)) return NULL;
    if(!nowy->allowNew()) {
        qDebug() << itemData->type << " <- object not supported yet ";
        return NULL;
    }
    
    if(nowy->type == "trackobj"){
        nowy->set("sectionidx", itemData->value);
    }
    
    //if(nowy->type == "transfer"){
    nowy->set("filename", itemData->filename);
    // }
    
    //Quat::rotateY(q, q, M_PI/2);
    nowy->initPQ(p, q);
    //qDebug() << maxUiD;
    nowy->UiD = ++maxUiD;
    qDebug() << itemData->type << " " << itemData->filename << nowy->UiD;
    //nowy->fileName = itemData->filename;
    nowy->load(x, z);
    obiekty[jestObiektow++] = nowy;
    //qDebug() << obiekty[jestObiektow-1]->qDirection[3];
    
    modified = true;
    return nowy;
}

void Tile::saveEmpty(int nx, int nz) {
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/world/w" + getNameXY(nx) + "" + getNameXY(nz) + ".w";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if(file.exists()) return;
    
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0w0t______\n";
    out << "\n";
    out << "Tr_Worldfile (\n";
    out << ")";

    file.close(); 
}

void Tile::save() {
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/world/w" + getNameXY(x) + "" + getNameXY(-z) + ".w";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0w0t______\n";
    out << "\n";
    out << "Tr_Worldfile (\n";
    if(!Game::deleteViewDbSpheres && viewDbSphereRaw != NULL && this->vDbIdCount > 0){
        out << "	VDbIdCount ( "<<this->vDbIdCount<<" )\n";
        out << "	ViewDbSphere (";
        out << *viewDbSphereRaw;
        out << "\n";
    }
    /*if(this->vDbIdCount > 0){
        out << "	VDbIdCount ( "<<this->vDbIdCount<<" )\n";
        out << "	ViewDbSphere (\n";
        out << "		VDbId ( "<<viewDbSphere[0].vDbId<<" )\n";
        out << "		Position ( "<<viewDbSphere[0].position[0]<<" "<<viewDbSphere[0].position[1]<<" "<<viewDbSphere[0].position[2]<<" )\n";
        out << "		Radius ( "<<viewDbSphere[0].radius<<" )\n";
        for(int i = 1; i < this->vDbIdCount; i++){
            out << "		ViewDbSphere (\n";
            out << "			VDbId ( "<<viewDbSphere[i].vDbId<<" )\n";
            out << "			Position ( "<<viewDbSphere[i].position[0]<<" "<<viewDbSphere[i].position[1]<<" "<<viewDbSphere[i].position[2]<<" )\n";
            out << "			Radius ( "<<viewDbSphere[i].radius<<" )\n";
            out << "		)\n";
        }
        out << "	)\n";
    }*/
    for(int i = 0; i < this->jestObiektow; i++){
        this->obiekty[i]->save(&out);
    }
    out << ")";
 
    // optional, as QFile destructor will already do it:
    file.close(); 
    
}


bool Tile::isModified(){
    bool value = this->modified;
    
    if(value == false)
        for (int i = 0; i < jestObiektow; i++) {
            if(obiekty[i]->modified)
                return true;
            }
    
    return value;
}


void Tile::setModified(bool value){
    this->modified = value;
    
    if(value == false){
        for (int i = 0; i < jestObiektow; i++) {
            obiekty[i]->modified = false;
        }
    }
}

void Tile::render() {
    //render(0, 0);
}

void Tile::render(float * playerT, float* playerW, float* target, float fov, bool selection) {
    if (loaded != 1) return;
    GLUU* gluu = GLUU::get();
    //gl.activeTexture(gl.TEXTURE0);
    //gluu->setMatrixUniforms();
    //this.obiekty.forEach(function(obj) {
    int selectionColor = 0;
    for (int i = 0; i < jestObiektow; i++) {
        if (obiekty[i]->loaded) {//
            float lodx = (x - playerT[0])*2048 + obiekty[i]->position[0] - playerW[0];
            float lodz = (z - playerT[1])*2048 + obiekty[i]->position[2] - playerW[2];
            //console.log(this.x);
            float lod = (float) sqrt(lodx * lodx + lodz * lodz);
            if (lod < Game::objectLod) {
                gluu->mvPushMatrix();
                //obiekty[i]->render(gluu, lod, x-playerT[0]*2048, z-playerT[1]*2048);
                if (selection) {
                    int sxx = (x - playerT[0] + 1)*10 + (-z + playerT[1] + 1);
                    selectionColor = obiekty[i]->UiD + sxx * 65536;
                }
                obiekty[i]->render(gluu, lod, lodx, lodz, playerW, target, fov, selectionColor);
                //obiekty[i]->render(gluu);
                gluu->mvPopMatrix();
            }
        }
    }
    //});

}
/*
Tile.prototype.getObjHash = function(UiD) {
    for(int objK : obiekty.keySet()){
     Obj o = obiekty.get(objK);
     if( o==null || !o.loaded) continue;
     if(o.UiD == UiD) return objK;
     }
    return 0;
};
 */