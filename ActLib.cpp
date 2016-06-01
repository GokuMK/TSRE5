#include "ActLib.h"
#include "Activity.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include "Game.h"

int ActLib::jestact = 0;
std::unordered_map<int, Activity*> ActLib::act;
std::unordered_map<std::string, std::vector<int>> ActLib::route;
 
ActLib::ActLib() {
}

ActLib::~ActLib() {
}

int ActLib::addAct(QString path, QString name) {
    QString pathid = (path + "/" + name).toLower();
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    //qDebug() << pathid;
    for ( auto it = act.begin(); it != act.end(); ++it ){
        if(it->second == NULL) continue;
        if (((Activity*) it->second)->pathid.length() == pathid.length())
            if (((Activity*) it->second)->pathid == pathid) {
                ((Activity*) it->second)->ref++;
                qDebug() <<"actid "<< pathid;
                return (int)it->first;
            }
    }
    //qDebug() << "Nowy " << jestcon << " con: " << pathid;
    act[jestact] = new Activity(pathid, path, name);
    route[act[jestact]->header->routeid.toStdString()].push_back(jestact);
    return jestact++;
}

int ActLib::loadAll(QString gameRoot){
    QString path;
    path = gameRoot + "/routes";
    QDir dir(path);
    qDebug() << path;
    dir.setFilter(QDir::Dirs);
    
    foreach(QString dirFile, dir.entryList()){
        if(dirFile == "." || dirFile == "..")   
            continue;
        
        dirFile += "/activities";
        //qDebug() <<dirFile;
        QDir aDir(path+"/"+dirFile);
        if(!aDir.exists()) continue;
        
        aDir.setFilter(QDir::Files);
        aDir.setNameFilters(QStringList()<<"*.act");
        foreach(QString actfile, aDir.entryList()){
            ActLib::addAct(path+"/"+dirFile, actfile);
        }
    }
    qDebug() << "loaded";
    return 0;
}