/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ConLib.h"
#include "Consist.h"
#include <QDebug>
#include <QFile>
#include <QDir>

int ConLib::jestcon = 0;
std::unordered_map<int, Consist*> ConLib::con;
QVector<QString> ConLib::conFileList;

ConLib::ConLib() {
}

ConLib::~ConLib() {
}

int ConLib::addCon(QString path, QString name) {
    QString pathid = (path + "/" + name).toLower();
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    //qDebug() << pathid;
    for ( auto it = con.begin(); it != con.end(); ++it ){
        if(it->second == NULL) continue;
        if (((Consist*) it->second)->pathid.length() == pathid.length())
            if (((Consist*) it->second)->pathid == pathid) {
                ((Consist*) it->second)->ref++;
                qDebug() <<"conid "<< pathid;
                return (int)it->first;
            }
    }
    qDebug() << "Nowy " << jestcon << " con: " << pathid;

    con[jestcon] = new Consist(pathid, path, name);

    return jestcon++;
}

int ConLib::refreshEngDataAll(){
    for ( auto it = con.begin(); it != con.end(); ++it ){
        if(it->second == NULL) continue;
        (it->second)->refreshEngData();
    }
}

int ConLib::loadAll(QString gameRoot){
    QString path;
    path = gameRoot + "/trains/consists/";
    QDir dir(path);
    QDir trainDir;
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.con");
    qDebug() << path;
    if(!dir.exists())
        qDebug() << "not exist";
    qDebug() << dir.count() <<" con files";
    foreach(QString engfile, dir.entryList())
        ConLib::addCon(path,engfile);
    qDebug() << "loaded";
    return 0;
}

int ConLib::loadSimpleList(QString gameRoot, bool reload){
    if(ConLib::conFileList.size() > 0 && reload == false)
        return 0;
    QString path;
    path = gameRoot + "/trains/consists/";
    QDir dir(path);
    QDir trainDir;
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.con");
    qDebug() << path;
    if(!dir.exists())
        qDebug() << "not exist";
    qDebug() << dir.count() <<" con files";
    foreach(QString engfile, dir.entryList())
        ConLib::conFileList.push_back(path.toLower()+engfile.toLower());
    qDebug() << "loaded";
    return 0;
}