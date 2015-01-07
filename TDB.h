#ifndef TDB_H
#define	TDB_H
#include <QString>
#include "TSectionDAT.h"
#include "TRnode.h"
#include "FileBuffer.h"
#include "GLUU.h"
#include "OglObj.h"
#include <unordered_map>
#include "Ref.h"

class TDB {
public:
    bool loaded;
    TSectionDAT *tsection; 
    //TRnode *trackNodes;
    std::unordered_map<int, TRnode*> trackNodes;
    //std::vector<TRnode> trackNodes;
    int iTRnodes;
    TDB(QString path);
    TDB(const TDB& orig);
    virtual ~TDB();
    void refresh();
    void trpin(TRnode* tr, FileBuffer* bufor );
    void save();
    int findNearestNode(int &x, int &z, float* p, float* q);
    int joinTracks(int iendp);
    int joinVectorSections(int id1, int id2);
    int rotate(int id);
    int appendTrack(int id, int* ends, int r, int sect, int uid);
    int newJunction(int x, int z, float* p, float* qe, int r, int uid, int end);
    int fillJunction(int id);
    void setDefaultEnd(int val);
    void nextDefaultEnd();
    int appendToJunction(int junctionId, int eId, int idx);
    int newTrack(int x, int z, float* p, float* q, int* ends, int r, int sect, int uid);
    int newTrack(int x, int z, float* p, float* q, int* ends, int r, int sect, int uid, int* start);
    bool placeTrack(int x, int z, float* p, float* q, Ref::RefItem* r, int uid);
    void renderAll(GLUU *gluu, float * playerT);
    void renderLines(GLUU *gluu, float* playerT);
private:
    bool deleteNulls();
    int findBiggest();
    int getLineBufferSize(int idx);
    void drawLine(GLUU *gluu, float* &ptr, Vector3f p, Vector3f o, int idx);
    OglObj linieSieci;
    OglObj konceSieci;
    OglObj punktySieci;
    OglObj sectionLines;
    int defaultEnd = 0;
    int wysokoscSieci;
    int lineHash;
    int sectionHash;
    bool isInitSectLines = false;
    bool isInitLines = false;
};

#endif	/* TDB_H */

