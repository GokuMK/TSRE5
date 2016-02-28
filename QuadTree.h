#ifndef QUADTREE_H
#define	QUADTREE_H

#include <QString>
#include <unordered_map>
#include <vector>

class QuadTree {
public:
    struct TdFile {
        int x;
        int y;
        unsigned char data[512][512];
    };
    struct TreePos {
        int level;
        int sum;
        int x;
        int y;
        std::vector<std::pair<int, int>> tile;
        
        TreePos(int l){
            level = l;
            sum = 0;
        }
    };
    std::unordered_map<int, TdFile*> td;
    QuadTree();
    virtual ~QuadTree();
    void load();
private:
    int terrainDescSize = 0;
    int depth = 0;
    void loadTD(int x, int y);
    QString getNameXY(int e);
};

#endif	/* QUADTREE_H */

