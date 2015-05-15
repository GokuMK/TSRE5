#include "TexLib.h"
#include "AceLib.h"
#include "PaintTexLib.h"
#include "Texture.h"
#include <QDebug>

int TexLib::jesttextur = 0;
std::unordered_map<int, Texture*> TexLib::mtex;

void TexLib::reset() {
    jesttextur = 0;
    mtex.clear();
}

void TexLib::delRef(int texx) {
    try {
        Texture* t = mtex.at(texx);
        t->ref--;
        if (t->ref <= 0) {
            //System.out.println("--refs: "+mtex.get(texx).ref);
            if (t->glLoaded) {
                t->delVBO();
                mtex.erase(texx);
            }
        }
    } catch (const std::out_of_range& oor) {
            
    }
}

void TexLib::addRef(int texx) {
    try {
        Texture* t = mtex.at(texx);
        t->ref++;
    } catch (const std::out_of_range& oor) {
            
    }    
}

int TexLib::addTex(QString path, QString name) {
    QString pathid = (path+"/"+name).toLower();
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    return addTex(pathid);
}

int TexLib::addTex(QString pathid) {

    for ( auto it = mtex.begin(); it != mtex.end(); ++it ){
        if (((Texture*) it->second)->pathid == pathid) {
            ((Texture*) it->second)->ref++;
            return (int)it->first;
        }
    }
    //qDebug() << "Nowa " << jesttextur << " textura: " << pathid;

    Texture* newFile = new Texture(pathid);
    newFile->ref++;
    mtex[jesttextur] = newFile;
    //qDebug() << pathid.toLower();
    QString tType = pathid.toLower().split(".").last();
    //qDebug() << tType;
    if(tType == "ace"){
        AceLib* t = new AceLib();
        t->texture = newFile;
        t->start();
    } else if(tType == ":painttex"){
        PaintTexLib* t = new PaintTexLib();
        t->texture = newFile;
        //t->start();
        t->run();
    }
    //AceLib::LoadACE(newFile);
    //tConcurrent::run();
    return jesttextur++;
}

