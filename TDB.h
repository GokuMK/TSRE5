#ifndef TDB_H
#define	TDB_H
#include <QString>
#include <unordered_map>
#include "OglObj.h"
#include "TextObj.h"

class TRnode;
class TRitem;
class TSectionDAT;
class DynTrackObj;
class SigCfg;
class Ref;
class GLUU;
class FileBuffer;
class SpeedPostDAT;

class TDB {
public:
    bool loaded;
    TSectionDAT *tsection; 
    SigCfg* sigCfg;
    SpeedPostDAT* speedPostDAT;
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
    bool deleteAllTrItemsFromVectorSection(int id);
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
    void getVectorSectionLine(float * &buffer, int &len, int x, int y, int uid, float begin = 0, float end = 0);
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
    void findNearestPositionOnTDB(float* posT, float* pos, float* q, float* tpos = NULL);
    void deleteTrItem(int trid);
    void deleteTree(int x, int y, int UiD);
    void deleteTree(int d);
    int getEndpointType(int trid, int endp);
    bool isRoad();
    void newPlatformObject(int* itemId, int trNodeId, float metry, int type);
    void newPickupObject(int* &itemId, int trNodeId, float metry, int type);
    void newSignalObject(QString filename, int* &itemId, int &signalUnits, int trNodeId, float metry, int type);
    void newLevelCrObject(int* &itemId, int trNodeId, float metry, int type);
    void newHazardObject(int* &itemId, int trNodeId, float metry, int type);
    void getSegmentIntersectionPositionOnTDB(float* posT, float* segment, float len, float* pos, float * q, float* tpos);
    void newSpeedPostObject(int speedPostId, int speedPostType, std::vector<int> & itemId, int trNodeId, float metry, int type);
    void newSoundRegionObject(int soundregionTrackType, std::vector<int> & itemId, int trNodeId, float metry, int type);
private:
    bool deleteNulls();
    int findBiggest();
    void addToDeletedTree(int* drzewo, int d);
    int getLineBufferSize(int idx, int pointSize, int offset, int step = 0);
    void checkSignals();
    void drawLine(GLUU *gluu, float* &ptr, Vector3f p, Vector3f o, int idx);
    void getLine(float* &ptr, Vector3f p, Vector3f o, int idx, int id, int vid, float offset = 0, int step = 0);
    void addItemToTrNode(int tid, int iid);
    void deleteItemFromTrNode(int tid, int iid);
    OglObj linieSieci;
    OglObj konceSieci;
    OglObj punktySieci;
    OglObj sectionLines;
    int defaultEnd = 0;
    int wysokoscSieci;
    int lineHash;
    int sectionHash;
    int iobjHash;
    bool isInitSectLines = false;
    bool isInitLines = false;
    bool road = false;
    
    float *collisionLineBuffer = NULL;
    int collisionLineLength = 0;
    int collisionLineHash = 0;
};

#endif	/* TDB_H */

