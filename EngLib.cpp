/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "EngLib.h"
#include "Eng.h"
#include <QDebug>
#include <QFile>
#include <QDir>

//int EngLib::jesteng = 0;
//std::unordered_map<int, Eng*> EngLib::eng;
    
EngLib::EngLib() {
}

EngLib::~EngLib() {
}

int EngLib::addEng(QString path, QString name) {
    QString pathid = (path + "/" + name).toLower();
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    //qDebug() << pathid;
    for ( auto it = eng.begin(); it != eng.end(); ++it ){
        if(it->second == NULL) continue;
        if (((Eng*) it->second)->pathid.length() == pathid.length())
            if (((Eng*) it->second)->pathid == pathid) {
                ((Eng*) it->second)->ref++;
                //qDebug() <<"engid "<< pathid;
                return (int)it->first;
            }
    }
    //qDebug() << "Nowy " << jesteng << " eng: " << pathid;

    eng[jesteng] = new Eng(pathid, path, name);

    return jesteng++;
}

int EngLib::removeBroken() {
    for (int i = 0; i < jesteng; i++){
        if(eng[i] == NULL) continue;
        if (eng[i]->loaded != 1)
            eng[i] = NULL;
    }
}

void EngLib::removeAll(){
    eng.clear();
    jesteng = 0;
}

int EngLib::getEngByPathid(QString pathid) {
    for ( auto it = eng.begin(); it != eng.end(); ++it ){
        if(it->second == NULL) continue;
        if (((Eng*) it->second)->pathid.length() == pathid.length())
            if (((Eng*) it->second)->pathid == pathid) {
                return (int)it->first;
            }
    }
    return -1;
}

int EngLib::loadAll(QString gameRoot){
    QString path;
    path = gameRoot + "/trains/trainset/";
    QDir dir(path);
    QDir trainDir;
    trainDir.setFilter(QDir::Files);
    trainDir.setNameFilters(QStringList()<<"*.eng"<<"*.wag");
    qDebug() << path;
    if(!dir.exists())
        qDebug() << "not exist";
    dir.setFilter(QDir::Dirs);
    qDebug() << dir.count() <<" dirs";
    foreach(QString dirFile, dir.entryList()){
        //qDebug() << dirFile;
        trainDir.setPath(path+dirFile);
        foreach(QString engfile, trainDir.entryList()){
            qDebug() << path << dirFile <<"/"<< engfile;
            addEng(path+dirFile,engfile);
        }
    }
    qDebug() << "loaded";
    return 0;
}