/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TexLib.h"
#include "AceLib.h"
#include "ImageLib.h"
#include "PaintTexLib.h"
#include "MapLib.h"
#include "Texture.h"
#include <QDebug>
#include <QFile>

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
        if(it->second == NULL) continue;
        if (((Texture*) it->second)->pathid.length() == pathid.length()) 
            if (((Texture*) it->second)->pathid == pathid) {
                ((Texture*) it->second)->ref++;
                return (int)it->first;
            }
    }
    //qDebug() << "Nowa " << jesttextur << " textura: " << pathid;

    QString tType = pathid.toLower().split(".").last();
    
    // Openrails uses .dds textures instead of .ace
    if(tType == "ace"){
        QFile file(pathid);
        if (!file.exists()){
            tType = "dds";
            pathid = pathid.left(pathid.length() - 3)+"dds";
        }
    }
    
    Texture* newFile = new Texture(pathid);
    newFile->ref++;
    mtex[jesttextur] = newFile;
    //qDebug() << pathid.toLower();
    //qDebug() << tType;
        
    if(tType == "ace"){
        AceLib* t = new AceLib();
        t->texture = newFile;
        if(AceLib::IsThread)
            t->start();
        else
            t->run();
    } else if(tType == "png"||tType == "bmp"||tType == "jpg"||tType == "dds"){
        ImageLib* t = new ImageLib();
        t->texture = newFile;
        t->start();
    } else if(tType == ":painttex"){
        PaintTexLib* t = new PaintTexLib();
        t->texture = newFile;
        //t->start();
        t->run();
    } else if(tType == ":maptex"){
        MapLib* t = new MapLib();
        t->texture = newFile;
        t->start();
    }
    //AceLib::LoadACE(newFile);
    //tConcurrent::run();
    return jesttextur++;
}

int TexLib::cloneTex(int id) {
    Texture* t = mtex[id];
    if(t == NULL) {
        qDebug() << "null texture " << id;
        return -2;
    }
    Texture* newFile = new Texture(t);
    newFile->ref++;
    mtex[jesttextur] = newFile;
 
    return jesttextur++;
}

void TexLib::save(QString type, QString path, int id){
    Texture* t = mtex.at(id);
    if(t == NULL) return;
    AceLib::save(path, t);
}