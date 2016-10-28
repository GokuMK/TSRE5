/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
#include "TS.h"
#include "IghCoords.h"

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
    //qDebug() << "ok";
    loaded = 1;
    //save();
}

void Tile::load() {

    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/world/w" + getNameXY(x) + "" + getNameXY(-z) + ".w";
    path.replace("//", "/");
    
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << "W file: not exist " << path;
        return;
    }
    FileBuffer* data = ReadFile::read(file);
    //qDebug() << "Date:" << data->length;
    //data->off = 0;
    //for(int i = 0; i < 64; i++){
    //    data->off = i;
    //    qDebug() << (char)data->get()<<"-"<<data->get();
    //}
    data->setTokenOffset(261844);
    data->off = 32;
    if (data->getToken() != 375){
        qDebug() << "w file uncompressed " << path;
        data->off = 0;
        sh = "Tr_Worldfile";
        ParserX::szukajsekcji1(sh, data);

        for (int tt = 0;; tt++) {
            sh = ParserX::nazwasekcji(data).toLower();
            //qDebug() << "= " << sh;

            WorldObj* nowy;
            if (sh == "") {
                qDebug() << obiekty.size();
                loaded = 0;
                wczytajObiekty();
                loadWS();
                file->close();
                delete data;
                return;
            } else if (sh == "tr_watermark") {
                nowy = (WorldObj*)(new TrWatermarkObj((int)ParserX::GetNumber(data)));
                obiekty[jestObiektow++] = nowy;
                ParserX::SkipToken(data);
                continue;
            } else if (sh == "vdbidcount") {
                vDbIdCount = ParserX::GetNumber(data);
                viewDbSphere = new ViewDbSphere[vDbIdCount];
                ParserX::SkipToken(data);
                continue;
            } else if (sh == "viewdbsphere") {
                //qDebug() <<sh;
                /*int j = 0;
                do {
                    for(int i = 0; i< 3; i++){
                        sh = ParserX::NextTokenInside(data).toLower();
                        //qDebug() <<sh;
                        if(sh == ("vdbid")) {
                            viewDbSphere[j].vDbId = ParserX::GetNumber(data);
                        }
                        if(sh == ("position")) {
                            viewDbSphere[j].position[0] = ParserX::GetNumber(data);
                            viewDbSphere[j].position[1] = ParserX::GetNumber(data);
                            viewDbSphere[j].position[2] = ParserX::GetNumber(data);
                        }
                        if(sh == ("radius")) {
                            viewDbSphere[j].radius = ParserX::GetNumber(data);
                        }
                        ParserX::SkipToken(data);
                    }

                    if(j > 0) ParserX::SkipToken(data);
                    j++;
                } while (!((sh = ParserX::NextTokenInside(data).toLower()) == ""));
                */
                int start = data->off;
                ParserX::SkipToken(data);
                int end = data->off;
                viewDbSphereRaw = (data->getString(start, end));
                continue;
            } 
            if ((nowy = WorldObj::createObj(sh)) == NULL) {
                ParserX::SkipToken(data);
                continue;
            }
            //qDebug() << nowy->type;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                nowy->set(sh, data);
                ParserX::SkipToken(data);
            }
            obiekty[jestObiektow++] = nowy;

            ParserX::SkipToken(data);
            continue;
        }
    } else {
        qDebug() << "w file compressed   " << path;
        data->off+=5;
        int offset, offsetO;
        int idx, idxO;
        WorldObj* nowy;
        while(data->length > data->off){
            idx = data->getToken();
            offset = data->off + data->getInt() + 4;
            //qDebug() << idx;
            if(idx == TS::ViewDbSphere){
                data->off = offset;
                continue;
            }
            if(idx == TS::VDbIdCount){
                data->off = offset;
                continue;
            }
            if(idx == TS::Tr_Watermark){
                data->off++;
                nowy = (WorldObj*)(new TrWatermarkObj(data->getInt()));
                obiekty[jestObiektow++] = nowy;
                data->off = offset;
                continue;
            }
            if ((nowy = WorldObj::createObj(idx)) == NULL) {
                data->off = offset;
                continue;
            }
            data->off++;
            while (data->off < offset) {
                idxO = data->getToken();
                offsetO = data->off + data->getInt() + 4;
                //qDebug() << "- "<< idxO;
                nowy->set(idxO, data);
                data->off = offsetO;
            }
            obiekty[jestObiektow++] = nowy;
            data->off = offset;
       }
       qDebug() << obiekty.size();
       loaded = 0;
       wczytajObiekty();
       loadWS();
    }
    file->close();
    delete data;
}

void Tile::loadWS() {

    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/world/w" + getNameXY(x) + "" + getNameXY(-z) + ".ws";
    path.replace("//", "/");
    
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)){
        //qDebug() << "ws file not exist    " << path;
        return;
    }
    FileBuffer* data = ReadFile::read(&file);
    //qDebug() << "Date:" << data->length;
    //data->off = 0;
    //for(int i = 0; i < 64; i++){
    //    data->off = i;
    //    qDebug() << (char)data->get()<<"-"<<data->get();
    //}
    data->setTokenOffset(261844);
    data->off = 32;
    if (data->getToken() != 375){
        qDebug() << "w file uncompressed " << path;
        data->off = 0;
        sh = "Tr_Worldsoundfile";
        ParserX::szukajsekcji1(sh, data);

        for (int tt = 0;; tt++) {
            sh = ParserX::nazwasekcji(data).toLower();
            //qDebug() << "= " << sh;

            WorldObj* nowy;
            if (sh == "") {
                qDebug() <<"WS size: "<< obiekty.size();
                //loaded = 0;
                return;
            }
            if ((nowy = WorldObj::createObj(sh)) == NULL) {
                ParserX::SkipToken(data);
                continue;
            }
            //qDebug() << nowy->type;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                nowy->set(sh, data);
                ParserX::SkipToken(data);
            }
            nowy->load(x, z);
            if(nowy->UiD < 1000000)
                if(nowy->UiD > maxUiDWS) maxUiDWS = nowy->UiD;
            obiekty[jestObiektow++] = nowy;

            ParserX::SkipToken(data);
            continue;
        }
    } else {
        qDebug() << "w file compressed   " << path;
        data->off+=5;
        int offset, offsetO;
        int idx, idxO;
        WorldObj* nowy;
        while(data->length > data->off){
            idx = data->getToken();
            offset = data->off + data->getInt() + 4;
            //qDebug() << idx;
            if ((nowy = WorldObj::createObj(idx)) == NULL) {
                data->off = offset;
                continue;
            }
            data->off++;
            while (data->off < offset) {
                idxO = data->getToken();
                offsetO = data->off + data->getInt() + 4;
                //qDebug() << "- "<< idxO;
                nowy->set(idxO, data);
                data->off = offsetO;
            }
            nowy->load(x, z);
            if(nowy->UiD < 1000000)
                if(nowy->UiD > maxUiDWS) maxUiDWS = nowy->UiD;
            obiekty[jestObiektow++] = nowy;
            data->off = offset;
       }
       qDebug() << obiekty.size();
       //loaded = 0;
    }
}

WorldObj* Tile::getObj(int uid) {
    for (int i = 0; i < jestObiektow; i++) {
        if (obiekty[i]->UiD == uid) {
            return obiekty[i];
        }
    }
    return NULL;
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
    placeObject(p, (float*)&q, itemData, NULL);
}

WorldObj* Tile::placeObject(float* p, float* q, Ref::RefItem* itemData, float* tpos) {
    if(loaded != 1) return NULL;
    if(itemData == NULL) return NULL;
    //qDebug() << pozW[0] << " " << pozW[1] << " " << pozW[2] << " " << itemData->type << " " << itemData->filename;

    WorldObj* nowy = WorldObj::createObj(itemData->type);
    if(nowy == NULL) return NULL;
    if(!nowy->allowNew()) {
        qDebug() << itemData->type << " <- object not supported yet ";
        return NULL;
    }

    nowy->set("ref_class", itemData->clas);
    nowy->set("ref_filename", itemData->filename);
    nowy->set("ref_value", itemData->value);
    if(itemData->staticFlags != 0)
        nowy->set("staticflags", itemData->staticFlags);

    //Quat::rotateY(q, q, M_PI/2);
    nowy->set("x", x);
    nowy->set("z", z);
    if(nowy->isTrackItem()){
        q[0] = 0;
        q[1] = 0;
        q[2] = 0;
        q[3] = 1;
        nowy->initPQ(p, q);
        nowy->initTrItems(tpos);
    } else {
        nowy->initPQ(p, q);
    }
       
    //qDebug() << maxUiD;
    if(nowy->isSoundItem())
        nowy->UiD = ++maxUiDWS;
    else
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
    if(!Game::sortTileObjects){
        for(int i = 0; i < this->jestObiektow; i++){
            if(this->obiekty[i]->isSoundItem()) continue;
                this->obiekty[i]->save(&out);
        }
    } else {
        int count = 0;
        for(int iLevel = -15; iLevel < 11; iLevel++){
            // get current level count;
            count = 0;
            if( iLevel > 0 ){
                for(int i = 0; i < this->jestObiektow; i++){
                    if(this->obiekty[i]->isSoundItem()) continue;
                    if(this->obiekty[i]->getCurrentDetailLevel() == iLevel)
                        count++;
                }
                if( count > 0 ){
                    out << "	Tr_Watermark ( "<<iLevel<<" )\n";
                }
            }
            // save current level objects;
            for(int i = 0; i < this->jestObiektow; i++){
                if(this->obiekty[i]->isSoundItem()) continue;
                if(this->obiekty[i]->getCurrentDetailLevel() == iLevel)
                    this->obiekty[i]->save(&out);
            }
        }
    }
    out << ")";
 
    // optional, as QFile destructor will already do it:
    file.close(); 
    saveWS();
}

void Tile::saveWS() {
    QString path;
    
    path = Game::root + "/routes/" + Game::route + "/world/w" + getNameXY(x) + "" + getNameXY(-z) + ".ws";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    
    int countWS = 0;
    for(int i = 0; i < this->jestObiektow; i++){
        if(this->obiekty[i]->isSoundItem() && this->obiekty[i]->loaded) countWS++;
    }
    qDebug() << countWS;
    if(countWS == 0){
        qDebug() << "delete ws file if exist";
        file.remove();
        return;
    }
    
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0w0t______\n";
    out << "\n";
    out << "Tr_Worldsoundfile (\n";
    for(int i = 0; i < this->jestObiektow; i++){
        if(!this->obiekty[i]->isSoundItem()) continue;
            this->obiekty[i]->save(&out);
    }
    out << ")";
 
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

void Tile::updateTerrainObjects(){
    for (int i = 0; i < jestObiektow; i++) {
        if (obiekty[i]->loaded) 
            if(obiekty[i]->typeID == WorldObj::forest || obiekty[i]->typeID == WorldObj::transfer)
               obiekty[i]->deleteVBO();
    }
}

void Tile::render(float * playerT, float* playerW, float* target, float fov, int renderMode) {
    if (loaded != 1) return;
    GLUU* gluu = GLUU::get();
    //gl.activeTexture(gl.TEXTURE0);
    //gluu->setMatrixUniforms();
    //this.obiekty.forEach(function(obj) {
    int selectionColor = 0;
    float lodx, lodz, lod;
    for (int i = 0; i < jestObiektow; i++) {
        if (obiekty[i]->loaded) {
            lodx = (x - playerT[0])*2048 + obiekty[i]->position[0] - playerW[0];
            lodz = (z - playerT[1])*2048 + obiekty[i]->position[2] - playerW[2];
            //console.log(this.x);
            lod = (float) sqrt(lodx * lodx + lodz * lodz);
            if (lod < Game::objectLod) {
                gluu->mvPushMatrix();
                //obiekty[i]->render(gluu, lod, x-playerT[0]*2048, z-playerT[1]*2048);
                if (renderMode == gluu->RENDER_SELECTION) {
                    int sxx = 0;
                         if(x < playerT[0] && z < playerT[1]) sxx = 0;
                    else if(x < playerT[0] && z == playerT[1]) sxx = 1;
                    else if(x < playerT[0] && z > playerT[1]) sxx = 2;
                    else if(x == playerT[0] && z < playerT[1]) sxx = 3;
                    else if(x == playerT[0] && z == playerT[1]) sxx = 4;
                    else if(x == playerT[0] && z > playerT[1]) sxx = 5;
                    else if(x > playerT[0] && z < playerT[1]) sxx = 6;
                    else if(x > playerT[0] && z == playerT[1]) sxx = 7;
                    // only 3 bits for tile number but this requires 4 .. 
                    //else if(x > playerT[0] && z > playerT[1]) sxx = 8;
                    //int sxx = (x - playerT[0] + 1)*4 + (z - playerT[1] + 1);
                    //qDebug() << sxx;
                    selectionColor = obiekty[i]->UiD + sxx * 131072;
                }
                obiekty[i]->render(gluu, lod, lodx, lodz, playerW, target, fov, selectionColor, renderMode);
                //obiekty[i]->render(gluu);
                gluu->mvPopMatrix();
            }
        }
    }
    
    /*for (int i = 0; i < jestObiektowWS; i++) {
        if (obiektyWS[i]->loaded) {//
            gluu->mvPushMatrix();
            if (selection) {
                int sxx = (x - playerT[0] + 1)*4 + (z - playerT[1] + 1);
                //qDebug() << sxx;
                selectionColor = (obiektyWS[i]->UiD) + sxx * 131072;
            }
            obiektyWS[i]->render(gluu, lod, lodx, lodz, playerW, target, fov, selectionColor);
            //obiekty[i]->render(gluu);
            gluu->mvPopMatrix();
        }
    }*/
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