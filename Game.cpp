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
int Game::start = 0;
bool Game::deleteTrWatermarks = false;
bool Game::deleteViewDbSpheres = false;
bool Game::createNewRoutes = false;
bool Game::writeEnabled = false;

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
        
        if(val == "startTileX"){
            Game::start++;
            startTileX = args[1].trimmed().toInt();
        }
        if(val == "startTileY"){
            Game::start++;
            startTileY = args[1].trimmed().toInt();
        }
        if(val == "deleteTrWatermarks")
            if(args[1].trimmed().toLower() == "true")
                deleteTrWatermarks = true;
        if(val == "deleteViewDbSpheres")
            if(args[1].trimmed().toLower() == "true")
                deleteViewDbSpheres = true;
        if(val == "createNewIfNotExist")
            if(args[1].trimmed().toLower() == "true")
                createNewRoutes = true;
        if(val == "writeEnabled")
            if(args[1].trimmed().toLower() == "true")
                writeEnabled = true;
    }

}

void Game::check_coords(int& x, int& z, float* p) {
    if (p[0] > 1024) {
        p[0] -= 2048;
        x++;
    }
    if (p[0] < -1024) {
        p[0] += 2048;
        x--;
    }
    if (p[2] > 1024) {
        p[2] -= 2048;
        z++;
    }
    if (p[2] < -1024) {
        p[2] += 2048;
        z--;
    }
}