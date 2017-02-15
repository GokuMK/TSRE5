/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "w file not exist    " << path;
        return;
    }
    FileBuffer* data = ReadFile::read(&file);
    //qDebug() << "Date:" << data->length;
    //data->off = 0;
    //for(int i = 0; i < 64; i++){
    //    data->off = i;
    //    qDebug() << (char)data->get()<<"-"<<data->get();
    //}
    data->off = 32;
    sh = "terrain_desc";
    ParserX::FindTokenDomIgnore(sh, data);
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == "terrain_desc_size") {
            terrainDescSize = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        } else if (sh == "depth") {
            depth = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        } else if (sh == "terrain_desc_tiles") {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == "tdfile") {
                    int x = ParserX::GetNumber(data);
                    int y = ParserX::GetNumber(data);
                    qDebug() << sh << " "<<x<<" "<<y;
                    loadTD(x, y);
                    ParserX::SkipToken(data);
                }
            }
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }
}

void QuadTree::createNew(int tileX, int tileY){
    int qx = floor((float)tileX/512.0);
    int qy = floor((float)tileY/512.0);
    qDebug() <<"td xy " << qx <<" "<< qy;
    int tx = qx*512;
    int ty = qy*512;
    TdFile* ttd = new TdFile();
    ttd->x = tx;
    ttd->y = ty;
    td[ttd->x*100000+ttd->y] = ttd;
    ttd->qt = new QuadTile(256);
    ttd->qt->x = tx;
    ttd->qt->y = ty;
    ttd->qt->addTile(tileX, tileY);
    ttd->modified = true;
    save();
}
void QuadTree::addTile(int tileX, int tileY){
    int qx = floor((float)tileX/512.0);
    int qy = floor((float)tileY/512.0);
    qDebug() <<"td xy " << qx <<" "<< qy;
    int tx = qx*512;
    int ty = qy*512;
    
    
    if(td[tx*100000+ty] == NULL) {
        td[tx*100000+ty] = new TdFile();
        td[tx*100000+ty]->x = tx;
        td[tx*100000+ty]->y = ty;
        td[tx*100000+ty]->qt = new QuadTile(256);
        td[tx*100000+ty]->qt->x = tx;
        td[tx*100000+ty]->qt->y = ty;
    }
    td[tx*100000+ty]->qt->addTile(tileX, tileY);
    td[tx*100000+ty]->modified = true;
    save();
}

void QuadTree::QuadTile::addTile(int tileX, int tileY){
    int px = 0;
    int py = 0;
    if(tileX >= x + level) px = 1;
    if(tileY >= y + level) py = 1;
    if(level == 1){
        populated[px][py] = true;
    } else {
        if(tile[px][py] == NULL){
            tile[px][py] = new QuadTile(level/2);
            tile[px][py]->x = x + px*level;
            tile[px][py]->y = y + py*level;
        }
        tile[px][py]->addTile(tileX, tileY);
    }
}

/*void QuadTree::loadTD(int x, int y){
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
}*/

void QuadTree::save(){
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/td/td_idx.dat";
    path.replace("//", "/");
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0D0t______\n";
    out << "\n";
    out << "terrain_desc (\n";
    out << "	terrain_desc_size ( "<< this->terrainDescSize<<" )\n";
    out << "	Depth ( "<<this->depth<<" )\n";
    out << "	terrain_desc_tiles ( \n";
    for(auto it = this->td.begin(); it != this->td.end(); ++it){
        out << "		TdFile ( "<<(it->second->x/512)<<" "<<(it->second->y/512)<<" )\n";
    }
    out << "	) \n";
    out << ")";
    file.close(); 
    
    for(auto it = this->td.begin(); it != this->td.end(); ++it){
        //qDebug() << it->second->modified;
        if(it->second->modified)
            saveTD((float)it->second->x/512.0, (float)it->second->y/512.0);
    }
}

void QuadTree::loadTD(int x, int y){
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/td/"+ getNameXY(x) + "" + getNameXY(y) +".td";
    path.replace("//", "/");

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "w file not exist    " << path;
        return;
    }
    FileBuffer* data = ReadFile::read(&file);
    TdFile* ttd = new TdFile();
    ttd->x = x*512;
    ttd->y = y*512;
    td[ttd->x*100000+ttd->y] = ttd;
    
    data->off = 38+16;
    
    int tx = x*512;
    int ty = y*512;
    ttd->qt = new QuadTile(256);
    ttd->qt->x = tx;
    ttd->qt->y = ty;
    ttd->qt->load(data);
    
    //saveTD(x, y);
}

void QuadTree::saveTD(int x, int y){
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/td/"+ getNameXY(x) + "" + getNameXY(y) +".td";
    path.replace("//", "/");
    QFile *file = new QFile(path);
    qDebug() << "zapis .td "<<path;
    if (!file->open(QIODevice::WriteOnly)){
        qDebug() << "td write fail";
        return;
    }
    QDataStream write(file);
    write.setByteOrder(QDataStream::LittleEndian);
    write.setFloatingPointPrecision(QDataStream::SinglePrecision);

    //write
    const char header[] = {
        0x53,0x49,0x4D,0x49,0x53,0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
        0x4A,0x49,0x4E,0x58,0x30,0x64,0x31,0x62,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x0D,0x0A
    };
    write.writeRawData(header, 32);
    std::vector<unsigned char> treeData;
    
    x = x*512;
    y = y*512;
    td[x*100000+y]->qt->save(treeData);
    
    write << (qint32)0x84;
    write << (qint32)treeData.size()+14;
    write << (qint8)0;
    write << (qint32)0x87;
    write << (qint32)treeData.size()+5;
    write << (qint8)0;
    write << (qint32)treeData.size();
    write.writeRawData((char*)&treeData[0], treeData.size());
    write.unsetDevice();
    file->close();
    td[x*100000+y]->modified = false;
}

void QuadTree::QuadTile::save(std::vector<unsigned char> &data){
    unsigned char isDivided = 0;
    unsigned char isPopulated = 0;
    
    if(tile[0][1] != 0)
        isDivided = isDivided | 0b1000;
    if(tile[1][1] != 0)
        isDivided = isDivided | 0b0100;
    if(tile[1][0] != 0)
        isDivided = isDivided | 0b0010;
    if(tile[0][0] != 0)
        isDivided = isDivided | 0b0001;
    if(populated[0][1])
        isPopulated = isPopulated | 0b1000;
    if(populated[1][1] != 0)
        isPopulated = isPopulated | 0b0100;
    if(populated[1][0] != 0)
        isPopulated = isPopulated | 0b0010;
    if(populated[0][0] != 0)
        isPopulated = isPopulated | 0b0001;
    isDivided = (isDivided << 4) | isPopulated;
    data.push_back(isDivided);
    
    if(tile[0][1] != 0)
        tile[0][1]->save(data);
    if(tile[1][1] != 0)
        tile[1][1]->save(data);
    if(tile[1][0] != 0)
        tile[1][0]->save(data);
    if(tile[0][0] != 0)
        tile[0][0]->save(data);
}

void QuadTree::QuadTile::load(FileBuffer* data){
        unsigned char aaa = data->get();
        unsigned char isDivided = aaa >> 4;
        unsigned char isPopulated = aaa & 0b1111;
        QString a = "";
        int sum = 0;
        int lev = level / 2;

        if(isDivided & 0b1000) {
            sum++;
            tile[0][1] = new QuadTile(lev);
            tile[0][1]->x = x;
            tile[0][1]->y = y + level;
            a += "1";
        }
        else a += "-";
        if(isDivided & 0b0100){
            sum++;
            tile[1][1] = new QuadTile(lev);
            tile[1][1]->x = x + level;
            tile[1][1]->y = y + level;
            a += "1";
        }
        else a += "-";
        if(isDivided & 0b0010){
            sum++;
            tile[1][0] = new QuadTile(lev);
            tile[1][0]->x = x + level;
            tile[1][0]->y = y;
            a += "1";
        }
        else a += "-";
        if(isDivided & 0b0001){
            sum++;
            tile[0][0] = new QuadTile(lev);
            tile[0][0]->x = x;
            tile[0][0]->y = y;
            a += "1";
        }
        else a += "-";
        
        if(isPopulated & 0b1000) {
            populated[0][1] = true;
            //qDebug() << "p" <<" "<< a << x << " " << y + level;
        }
        if(isPopulated & 0b0100){
            populated[1][1] = true;
            //qDebug() << "p" <<" "<< a << x + level << " " << y + level;
        }
        if(isPopulated & 0b0010){
            populated[1][0] = true;
            //qDebug() << "p" <<" "<< a << x + level << " " << y;
        }
        if(isPopulated & 0b0001){
            populated[0][0] = true;
            //qDebug() << "p" <<" "<< a << x << " " << y;
        }
        
        //qDebug() << level <<" "<< a << qt->x << " " << qt->y;
        
        if(tile[0][1] != 0)
            tile[0][1]->load(data);
        if(tile[1][1] != 0)
            tile[1][1]->load(data);
        if(tile[1][0] != 0)
            tile[1][0]->load(data);
        if(tile[0][0] != 0)
            tile[0][0]->load(data);
        
        if(a == "----"){
            //qDebug() << lev <<" "<< a << x << " " << y;
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

