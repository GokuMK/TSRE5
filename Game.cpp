#include "Game.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString Game::root = "F:/Train Simulator";
QString Game::route = "bbb";
//QString Game::route = "traska";
//QString Game::route = "cmk";
int Game::allowObjLag = 1000;
int Game::maxObjLag = 10;
int Game::startTileX = 0;
int Game::startTileY = 0;
float Game::objectLod = 3000;

void Game::load() {
    QString sh;
    QString path;
    path = "settings.txt";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    qDebug() << path;

    QTextStream in(&file);
    QString line;
    QStringList args;
    QString val;
    while (!in.atEnd()) {
        line = in.readLine();
        //qDebug() << line;
        if(line.startsWith("#", Qt::CaseInsensitive)) continue;
        args = line.split("=");
        if(args.count() < 2) continue;
        val = args[0].trimmed();
        //qDebug() << args[0].trimmed() << " "<< args[1].trimmed();
        
        if(val == "gameRoot")
            root = args[1].trimmed();
        if(val == "routeName")
            route = args[1].trimmed();
        if(val == "startTileX")
            startTileX = args[1].trimmed().toInt();
        if(val == "startTileY")
            startTileY = args[1].trimmed().toInt();
    }

}
