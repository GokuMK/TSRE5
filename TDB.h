#ifndef TDB_H
#define	TDB_H
#include <QString>
#include "TSectionDAT.h"
#include "TRnode.h"
#include "TRitem.h"
#include "FileBuffer.h"
#include "GLUU.h"
#include "OglObj.h"
#include <unordered_map>
#include "Ref.h"
#include "TextObj.h"

class DynTrackObj;

class TDB {
public:
    bool loaded;
    TSectionDAT *tsection; 
    //TRnode *trackNodes;
    std::unordered_map<int, TRitem*> trackItems;
    std::unordered_map<int, TRnode*> trackNodes;
    std::unordered_map<int, TextObj*> endIdObj;
    //std::vector<TRnode> trackNodes;
    int iTRnodes;
    int iTRitems;
    int serial;
    TDB(TSectionDAT* tsection, bool road, QString path);
    TDB(const TDB& orig);
    virtual ~TDB();
    void refresh();
    void trpin(TRnode* tr, FileBuffer* bufor );
    void save();
    void saveTit();
    void loadTit();
    static void saveEmpty(bool road);
    void removeTrackFromTDB(int x, int y, int UiD);
    int findNearestNode(int &x, int &z, float* p, float* q);
    int joinTracks(int iendp);
    int joinVectorSections(int id1, int id2);
    void deleteJunction(int id);
    void deleteVectorSection(int id);
    bool deleteFromVectorSection(int id, int j);
    int splitVectorSection(int id, int j);
    int rotate(int id);
    int appendTrack(int id, int* ends, int r, int sect, int uid);
    int newJunction(int x, int z, float* p, float* qe, int r, int uid, int end);
    void fillDynTrack(DynTrackObj* track);
    int fillJunction(int id);
    void setDefaultEnd(int val);
    void nextDefaultEnd();
    float getVectorSectionLength(int id);
    float getVectorSectionLengthToIdx(int id, int idx);
    void getVectorSectionPoints(int x, int y, int uid, float * &ptr);
    void moveItemsFrom2to1(int id2, int id1);
    int appendToJunction(int junctionId, int eId, int idx);
    int newTrack(int x, int z, float* p, float* q, int* ends, int r, int sect, int uid);
    int newTrack(int x, int z, float* p, float* q, int* ends, int r, int sect, int uid, int* start);
    bool placeTrack(int x, int z, float* p, float* q, int sectionIdx, int uid, float elevation);
    bool placeTrack(int x, int z, float* p, float* q, int sectionIdx, int uid);
    bool findPosition(int x, int z, float* p, float* q, float* endp, int sectionIdx, int uid);
    void getLines(float * &lineBuffer, int &length, float* playerT);
    void renderAll(GLUU *gluu, float * playerT, float playerRot);
    void renderLines(GLUU *gluu, float* playerT, float playerRot);
    void renderItems(GLUU *gluu, float* playerT, float playerRot);
    bool getDrawPositionOnTrNode(float* out, int id, float metry);
    int findTrItemNodeId(int id);
    void findNearestPositionOnTDB(float* posT, float* &pos);
private:
    bool deleteNulls();
    int findBiggest();
    int getLineBufferSize(int idx, int pointSize, int offset);
    void checkSignals();
    void drawLine(GLUU *gluu, float* &ptr, Vector3f p, Vector3f o, int idx);
    void getLine(float* &ptr, Vector3f p, Vector3f o, int idx, int id);
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
    bool road = false;
    
    float *collisionLineBuffer = NULL;
    int collisionLineLength = 0;
    int collisionLineHash = 0;
};

#endif	/* TDB_H */

