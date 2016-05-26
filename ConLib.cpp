#include "ConLib.h"
#include "Consist.h"
#include <QDebug>
#include <QFile>
#include <QDir>

int ConLib::jestcon = 0;
std::unordered_map<int, Consist*> ConLib::con;
    
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
    //qDebug() << "Nowy " << jestcon << " con: " << pathid;

    con[jestcon] = new Consist(pathid, path, name);

    return jestcon++;
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