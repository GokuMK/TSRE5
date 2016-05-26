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