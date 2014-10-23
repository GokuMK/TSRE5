#include "Tile.h"
#include "Game.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "WorldObj.h"
#include "StaticObj.h"
#include "GLUU.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include "GLUU.h"

Tile::Tile() {
    loaded = -2;
    inUse = false;
    x = 0;
    z = 0;
    //obiekty = new Array();
    jestObiektow = 0;
}

Tile::Tile(int xx, int zz) {
    loaded = -2;
    inUse = false;
    x = xx;
    z = zz;
    //obiekty = new Array();
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
    return n + s.right(6);// .substr(s.length - 6);
}

void Tile::wczytajObiekty(){
    //this.obiekty.forEach(function(obj) {
    for ( auto it = obiekty.begin(); it != obiekty.end(); ++it ){
        //console.log(obj.type);
        WorldObj* obj = (WorldObj*)it->second;
        if(obj->type == "Static") {
            obj->skipLevel = 1;
            //obj.render = StaticObj.render;
            //obj.load = StaticObj.load;
            obj->load(Game::root+"/routes/"+Game::route+"/shapes",x,z); 
        }
        else if(obj->type == "Signal") {
            obj->skipLevel = 1;
            //obj.render = StaticObj.render;
            //obj.load = StaticObj.load;
            obj->load(Game::root+"/routes/"+Game::route+"/shapes",x,z); 
        }
        else if(obj->type == "Speedpost") {
            obj->skipLevel = 1;
            //obj.render = StaticObj.render;
            //obj.load = StaticObj.load;
            obj->load(Game::root+"/routes/"+Game::route+"/shapes",x,z); 
        }
        else if(obj->type == "TrackObj") {
            obj->skipLevel = 3;
            //obj.render = StaticObj.render;
            //obj.load = StaticObj.load;
            obj->load(Game::root+"/global/shapes",x,z); 
        }
        else if(obj->type == "Gantry") {
            obj->skipLevel = 1;
            //obj.render = StaticObj.render;
            //obj.load = StaticObj.load;
            obj->load(Game::root+"/routes/"+Game::route+"/shapes",x,z); 
        }        
        /*else if(obj.type.equalsIgnoreCase("DynTrack")) {
            obj.skipLevel = 3;
            obj.render = DyntrackObj.render;
            obj.load = DyntrackObj.load;
            obj.drawShape = DyntrackObj.drawShape;
            obj.genShape = DyntrackObj.genShape;
            obj.createVBO = DyntrackObj.createVBO;
            obj.load();
        }
        else if(obj.type.equalsIgnoreCase("Transfer")) {
            obj.skipLevel = 3;
            obj.render = TransferObj.render;
            obj.load = TransferObj.load;
            obj.load(this.x,this.z);
        }
        else if(obj.type.equalsIgnoreCase("Forest")) {
            obj.skipLevel = 3;
            obj.render = ForestObj.render;
            obj.load = ForestObj.load;
            obj.load(this.x,this.z);
        }*/
        //if(obj.type.equals("ForestObj")) 
        //else
        //obj.load = 
    }
    //);
    loaded = 1;
 }

void Tile::load() {
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/world/w" + getNameXY(x) + "" + getNameXY(-z) + ".w";
    path.replace("//","/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    //qDebug() << "Date:" << data->length;
    data->off = 0;
    sh = "Tr_Worldfile";
    ParserX::szukajsekcji1(sh, data);

    for (int tt = 0; ; tt++) {
        //wybranie zdarzenia
        sh = ParserX::nazwasekcji(data);
        //qDebug() << "= " << sh;
        
        WorldObj* nowy;
        if (sh == "") {
            //("wczytano "+obiekty.size());
            loaded = 0;
            wczytajObiekty();
            return;
        } else if(sh == "VDbIdCount") {
            ParserX::pominsekcje(data);
            continue;
        } else if(sh == "ViewDbSphere") {
            ParserX::pominsekcje(data);
            continue;
        } else if(sh == "Static") {
            nowy =  (WorldObj*)(new StaticObj());
        } else if(sh == "Signal") {
            nowy =  (WorldObj*)(new StaticObj());
        } else if(sh == "Speedpost") {
            nowy =  (WorldObj*)(new StaticObj());
        } else if(sh == "TrackObj") {
            nowy =  (WorldObj*)(new StaticObj());
        } else if(sh == "Gantry") {
            nowy =  (WorldObj*)(new StaticObj());
        }/*else if(obj.type.equalsIgnoreCase("DynTrack")) {

        }else if(obj.type.equalsIgnoreCase("Transfer")) {

        }else if(obj.type.equalsIgnoreCase("Forest")) {

        }*/ else {
            nowy = new WorldObj();
        }
        nowy->type = sh;
        
        while (!((sh = ParserX::nazwasekcji_inside(data)) == "")) {

            //qDebug() << "=== " << sh;
            
            if (sh == ("UiD") && nowy->isAttribute(sh)) {
                nowy->UiD = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("SectionIdx") && nowy->isAttribute(sh)) {
                //nowy->sectionIdx = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("Elevation") && nowy->isAttribute(sh)) {
                //nowy->elevation = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("CollideFlags") && nowy->isAttribute(sh)) {
                //nowy->collideFlags = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("FileName") && nowy->isAttribute(sh)) {
                nowy->fileName = ParserX::odczytajtc(data);
                //console.log(nowy.type+" ---- "+nowy.UiD+" "+nowy.fileName);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("StaticFlags") && nowy->isAttribute(sh)) {
                //nowy->staticFlags = ParserX::parsuj16(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("Position") && nowy->isAttribute(sh)) {
                nowy->position[0] = ParserX::parsujr(data);
                nowy->position[1] = ParserX::parsujr(data);
                nowy->position[2] = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("QDirection") && nowy->isAttribute(sh)) {
                nowy->qDirection[0] = ParserX::parsujr(data);
                nowy->qDirection[1] = ParserX::parsujr(data);
                nowy->qDirection[2] = ParserX::parsujr(data);
                nowy->qDirection[3] = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("VDbId") && nowy->isAttribute(sh)) {
                //nowy->vDbId = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("StaticDetailLevel") && nowy->isAttribute(sh)) {
                //nowy->staticDetailLevel = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("TrackSections") && nowy->isAttribute(sh)) {
                /*nowy.sections = new Array(5);
                for(var iii=0; iii<5; iii++){
                    nowy.sections[iii] = new Object();
                    nowy.sections[iii].type = ParserX::parsujr(data);
                    nowy.sections[iii].val1 = ParserX::parsujr(data);
                    nowy.sections[iii].a = ParserX::parsujr(data);
                    nowy.sections[iii].r = ParserX::parsujr(data);
                    ParserX::pominsekcje(data);
                }*/
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("TreeTexture") && nowy->isAttribute(sh)) {
                //nowy->treeTexture = ParserX::odczytajtc(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("ScaleRange") && nowy->isAttribute(sh)) {
                //nowy->scaleRangeX = ParserX::parsujr(data);
                //nowy->scaleRangeZ = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("Area") && nowy->isAttribute(sh)) {
                //nowy->areaX = ParserX::parsujr(data);
                //nowy->areaZ = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("TreeSize") && nowy->isAttribute(sh)) {
                //nowy->treeSizeX = ParserX::parsujr(data)*0.7;
                //nowy->treeSizeZ = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("Population") && nowy->isAttribute(sh)) {
                //nowy->population = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("JNodePosn") && nowy->isAttribute(sh)) {
                //nowy->jNodePosn = [ParserX::parsujr(data), ParserX::parsujr(data), ParserX::parsujr(data), 
                //                  ParserX::parsujr(data), ParserX::parsujr(data)
                //];
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("Width") && nowy->isAttribute(sh)) {
                //nowy->width = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("Height") && nowy->isAttribute(sh)) {
                //nowy->height = ParserX::parsujr(data);
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
            }/**/
            else {
                //qDebug() << "-----------------------------mam "<<sh;
            
                ParserX::pominsekcje(data);
                //qDebug() << "= aa";
            }
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
void Tile::render(float * playerT, float* playerW, float* target, float fov){
    if (loaded!= 1) return;
    GLUU* gluu = GLUU::get();
    //gl.activeTexture(gl.TEXTURE0);
    //gluu.setMatrixUniforms();
    //this.obiekty.forEach(function(obj) {
    for(int i=0; i<jestObiektow; i++){
        if(obiekty[i]->loaded){//
            float lodx = (x-playerT[0])*2048 + obiekty[i]->position[0] - playerW[0];
            float lodz = (z-playerT[1])*2048 + obiekty[i]->position[2] - playerW[2];
            //console.log(this.x);
            float lod = (float)sqrt( lodx*lodx + lodz*lodz);
            if( lod < Game::objectLod){
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