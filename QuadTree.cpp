#include "QuadTree.h"
#include "Game.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include "ReadFile.h"
#include <QString>
#include <QDebug>
#include <QFile>

QuadTree::QuadTree() {

}

void QuadTree::load() {
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/td/td_idx.dat";
    path.replace("//", "/");

    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "w file not exist    " << path;
        return;
    }
    FileBuffer* data = ReadFile::read(file);
    //qDebug() << "Date:" << data->length;
    //data->off = 0;
    //for(int i = 0; i < 64; i++){
    //    data->off = i;
    //    qDebug() << (char)data->get()<<"-"<<data->get();
    //}
    data->off = 16;
    sh = "terrain_desc";
    ParserX::szukajsekcji1(sh, data);
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == "terrain_desc_size") {
            terrainDescSize = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        } else if (sh == "depth") {
            depth = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        } else if (sh == "terrain_desc_tiles") {
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == "tdfile") {
                    int x = ParserX::parsujr(data);
                    int y = ParserX::parsujr(data);
                    qDebug() << sh << " "<<x<<" "<<y;
                    loadTD(x, y);
                    ParserX::pominsekcje(data);
                }
            }
            ParserX::pominsekcje(data);
            continue;
        }
        ParserX::pominsekcje(data);
    }
}

void QuadTree::loadTD(int x, int y){
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/td/"+ getNameXY(x) + "" + getNameXY(y) +".td";
    path.replace("//", "/");

    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "w file not exist    " << path;
        return;
    }
    FileBuffer* data = ReadFile::read(file);
    TdFile* ttd = new TdFile();
    ttd->x = x*512;
    ttd->y = y*512;
    td[ttd->x*10000+ttd->y] = ttd;
    
    for(int i = 0; i < 512; i++)
        memset(ttd->data[i], 0, 512);
    
    data->off = 38;
    
    QString a = "";
    unsigned char aa; 
    std::vector<TreePos*> tree;
    int level = 256;
    TreePos* tp;
    int tx = x*512;
    int ty = y*512;
    while(data->off < data->length){
        //qDebug() << (int)data->get();
        aa = data->get() >> 4;
        a = "";
        int sum = 0;
        tp = new TreePos(level);
        tp->x = tx;
        tp->y = ty;
        if(aa & 0b1000) {
            //tree.push_back(std::make_pair(level, 4));
            tp->sum++;
            tp->tile.push_back(std::make_pair(0, 1));
            a += "1";
        }
        else a += "-";
        if(aa & 0b0100){
            //tree.push_back(std::make_pair(level, 3));
            tp->sum++;
            tp->tile.push_back(std::make_pair(1, 1));
            a += "1";
        }
        else a += "-";
        if(aa & 0b0010){
            //tree.push_back(std::make_pair(level, 2));
            tp->sum++;
            tp->tile.push_back(std::make_pair(1, 0));
            a += "1";
        }
        else a += "-";
        if(aa & 0b0001){
            //tree.push_back(std::make_pair(level, 1));
            tp->sum++;
            tp->tile.push_back(std::make_pair(0, 0));
            a += "1";
        }
        else a += "-";
        
        if(tp->sum > 0)
            tree.push_back(tp);
        
        
        if(a == "----"){
            qDebug() << level <<" "<< a << tx << " " << ty;
            while(tree.size() > 0){
                tree.back()->sum--;
                if(tree.back()->sum == 0){
                    tree.pop_back();
                }else{
                    
                    break;
                }
            }
        }
        if(tree.size() == 0)
            break;
        level = tree.back()->level;
        tx = tree.back()->x + tree.back()->tile[tree.back()->tile.size() - tree.back()->sum].first*level;
        ty = tree.back()->y + tree.back()->tile[tree.back()->tile.size() - tree.back()->sum].second*level;
        level*=0.5;
    }
}

QString QuadTree::getNameXY(int e) {
    QString n = "+";
    if (e < 0) {
        n = "-";
        e = -e;
    }
    QString s = "000000" + QString::number(e);
    return n + s.right(5);
}

QuadTree::~QuadTree() {
}

