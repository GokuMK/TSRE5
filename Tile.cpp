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
#include "GLUU.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include "GLUU.h"
#include "GLMatrix.h"

Tile::Tile() {
    loaded = -2;
    inUse = false;
    x = 0;
    z = 0;
    jestObiektow = 0;
}

Tile::Tile(int xx, int zz) {
    loaded = -2;
    inUse = false;
    x = xx;
    z = zz;
    jestObiektow = 0;
    load();
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

        if (obj->type == "static") {
            obj->skipLevel = 1;
            obj->resPath = Game::root + "/routes/" + Game::route + "/shapes";
            obj->load(x, z);
        } else if (obj->type == "signal") {
            obj->skipLevel = 1;
            obj->resPath = Game::root + "/routes/" + Game::route + "/shapes";
            obj->load(x, z);
        } else if (obj->type == "speedpost") {
            obj->skipLevel = 1;
            obj->resPath = Game::root + "/routes/" + Game::route + "/shapes";
            obj->load(x, z);
        } else if (obj->type == "trackobj") {
            obj->skipLevel = 3;
            obj->resPath = Game::root + "/global/shapes";
            obj->load(x, z);
        } else if (obj->type == "gantry") {
            obj->skipLevel = 1;
            obj->resPath = Game::root + "/routes/" + Game::route + "/shapes";
            obj->load(x, z);
        }
        else if (obj->type == "dyntrack") {
            obj->skipLevel = 3;
            obj->resPath = Game::root + "/routes/" + Game::route + "/textures";
            obj->load(x, z);
        } else if (obj->type == "forest") {
            obj->skipLevel = 3;
            obj->resPath = Game::root + "/routes/" + Game::route + "/textures";
            obj->load(x, z);
        } else if (obj->type == "transfer") {
            obj->skipLevel = 3;
            obj->resPath = Game::root + "/routes/" + Game::route + "/textures";
            obj->load(x, z);
        }
    }
    loaded = 1;
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
        } else if (sh == "vdbidcount") {
            ParserX::pominsekcje(data);
            continue;
        } else if (sh == "viewobsphere") {
            ParserX::pominsekcje(data);
            continue;
        } else if (sh == "static") {
            nowy = (WorldObj*) (new StaticObj());
        } else if (sh == "signal") {
            nowy = (WorldObj*) (new StaticObj());
        } else if (sh == "speedpost") {
            nowy = (WorldObj*) (new StaticObj());
        } else if (sh == "trackobj") {
            nowy = (WorldObj*) (new StaticObj());
        } else if (sh == "gantry") {
            nowy = (WorldObj*) (new StaticObj());
        } else if (sh == "dyntrack") {
            nowy = (WorldObj*) (new DynTrackObj());
        } else if (sh == "forest") {
            nowy = (WorldObj*) (new ForestObj());
        } else if (sh == "transfer") {
            nowy = (WorldObj*) (new TransferObj());
        } else {
            nowy = new WorldObj();
        }
        nowy->type = sh;

        while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
            nowy->set(sh, data);
            //qDebug() << "=== " << sh;
            /*
            if(sh == ("JNodePosn") && nowy->isAttribute(sh)) {
                //nowy->jNodePosn = [ParserX::parsujr(data), ParserX::parsujr(data), ParserX::parsujr(data), 
                //                  ParserX::parsujr(data), ParserX::parsujr(data)
                //];
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("Speed_Digit_Tex") && nowy->isAttribute(sh)) {
                //nowy->speed_Digit_Tex = ParserX::odczytajtc(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("Speed_Sign_Shape") && nowy->isAttribute(sh)) {
                //nowy->speed_Sign_Shape = [ParserX::parsujr(data), ParserX::parsujr(data), ParserX::parsujr(data), 
                //               ParserX::parsujr(data), ParserX::parsujr(data), ParserX::parsujr(data),
                //               ParserX::parsujr(data), ParserX::parsujr(data), ParserX::parsujr(data)
                //];
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("Speed_Sign_Shape") && nowy->isAttribute(sh)) {
                //nowy->speed_Sign_Shape = [ParserX::parsujr(data), ParserX::parsujr(data), ParserX::parsujr(data)];
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("TrItemId") && nowy->isAttribute(sh)) {
                //nowy->trItemId = [ParserX::parsujr(data), ParserX::parsujr(data)];
                ParserX::pominsekcje(data);
                continue;
            }/*
            else {
                //qDebug() << "-----------------------------mam "<<sh;
                //qDebug() << "= aa";
            }*/
            ParserX::pominsekcje(data);
        }
        obiekty[jestObiektow++] = nowy;
        //obiekty.put(nowy.UiD, nowy);
        //if(wlasciwosci!=0) System.out.println("wlasciwosci fail");

        ParserX::pominsekcje(data);
        continue;
    }
}

void Tile::render() {
    //render(0, 0);
}

void Tile::render(float * playerT, float* playerW, float* target, float fov) {
    if (loaded != 1) return;
    GLUU* gluu = GLUU::get();
    //gl.activeTexture(gl.TEXTURE0);
    //gluu->setMatrixUniforms();
    //this.obiekty.forEach(function(obj) {

    for (int i = 0; i < jestObiektow; i++) {
        if (obiekty[i]->loaded) {//
            float lodx = (x - playerT[0])*2048 + obiekty[i]->position[0] - playerW[0];
            float lodz = (z - playerT[1])*2048 + obiekty[i]->position[2] - playerW[2];
            //console.log(this.x);
            float lod = (float) sqrt(lodx * lodx + lodz * lodz);
            if (lod < Game::objectLod) {
                gluu->mvPushMatrix();
                //obiekty[i]->render(gluu, lod, x-playerT[0]*2048, z-playerT[1]*2048);
                obiekty[i]->render(gluu, lod, lodx, lodz, playerW, target, fov);
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