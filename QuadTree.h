#ifndef QUADTREE_H
#define	QUADTREE_H

#include <QString>
#include <unordered_map>
#include <vector>

class FileBuffer;

class QuadTree {
public:

    /*struct TreePos {
        int level;
        int sum;
        int x;
        int y;
        std::vector<std::pair<int, int>> tile;
        
        TreePos(int l){
            level = l;
            sum = 0;
        }
    };*/
    struct QuadTile {
        int level;
        int sum;
        int x;
        int y;
        QuadTile* tile[2][2];
        bool populated[2][2];
        
        QuadTile(int l){
            level = l;
            sum = 0;
            tile[0][0] = NULL;
            tile[1][0] = NULL;
            tile[0][1] = NULL;
            tile[1][1] = NULL;
            populated[0][0] = false;
            populated[1][0] = false;
            populated[0][1] = false;
            populated[1][1] = false;
        }
        void save(std::vector<unsigned char> &data);
        void load(FileBuffer* data);
        void addTile(int tileX, int tileY);
    };
    struct TdFile {
        int x;
        int y;
        QuadTile* qt = NULL;
        bool modified = false;
        //unsigned char data[512][512];
    };
    std::unordered_map<int, TdFile*> td;
    QuadTree();
    virtual ~QuadTree();
    void load();
    void save();
    void createNew(int tileX, int tileY);
    void addTile(int tileX, int tileY);
private:
    int terrainDescSize = 67108864;
    int depth = 6;
    void loadTD(int x, int y);
    void saveTD(int x, int y);
    QString getNameXY(int e);
};

#endif	/* QUADTREE_H */

