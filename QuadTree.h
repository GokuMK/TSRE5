/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef QUADTREE_H
#define	QUADTREE_H

#include <QString>
#include <QHash>
#include <QVector>

class FileBuffer;
class TerrainInfo;

class QuadTree {
public:

    /*struct TreePos {
        int level;
        int sum;
        int x;
        int y;
        QVector<std::pair<int, int>> tile;
        
        TreePos(int l){
            level = l;
            sum = 0;
        }
    };*/
    struct QuadTile {
        static QChar PrefixString[2];
        int level;
        int prefix;
        int sum;
        int x;
        int y;
        unsigned int nameId = 0;
        QuadTile* tile[2][2];
        bool populated[2][2];
        
        QuadTile(int l, int p, int xx, int yy);
        void save(QVector<unsigned char> &data);
        void load(FileBuffer* data);
        void addTile(int tileX, int tileY, int dLevel);
        QString getMyName(int tileX, int tileY);
        unsigned int getMyNameId(int tileX, int tileY);
        void fillTerrainInfo(int tileX, int tileY, TerrainInfo* info);
        void listNames();
    };
    struct TdFile {
        int x;
        int y;
        QuadTile* qt = NULL;
        bool modified = false;
        //unsigned char data[512][512];
    };
    QHash<int, TdFile*> td;
    QuadTree(bool l = false);
    virtual ~QuadTree();
    void load();
    void save();
    void createNew(int tileX, int tileY);
    void addTile(int tileX, int tileY);
    void fillTerrainInfo(int tileX, int tileY, TerrainInfo* info);
    QString getMyName(int tileX, int tileY);
    unsigned int getMyNameId(int tileX, int tileY);
    void listNames();
    bool isLow();
private:
    int terrainDescSize = 67108864;
    int depth = 6;
    bool low = false;
    void loadTD(int x, int y);
    void saveTD(int x, int y);
    QString getNameXY(int e);
};

#endif	/* QUADTREE_H */

