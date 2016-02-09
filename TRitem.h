/* 
 * Author: Goku
 */

#ifndef TRITEM_H
#define	TRITEM_H

#include <QString>
#include "Pointer3d.h"

class FileBuffer;
class QTextStream;
class TDB;
class TrackItemObj;

class TRitem {
public:
    static TRitem* newPlatformItem(int trItemId, float metry);
    static TRitem* newSidingItem(int trItemId, float metry);
    static TRitem* newCarspawnerItem(int trItemId, float metry);
    static TRitem* newPickupItem(int trItemId, float metry);
    static TRitem* newSignalItem(int trItemId, float metry, unsigned int flags, QString type);
    static TRitem* newSpeedPostItem(int trItemId, float metry, int speedPostType);
    static TRitem* newLevelCrItem(int trItemId, float metry);
    TRitem();
    TRitem(int id);
    TRitem(const TRitem& orig);
    virtual ~TRitem();
    
    QString type;
    
    unsigned int trItemId;
    float trItemSData1;
    int trItemSData2;
    float *trItemPData = NULL;
    float *trItemRData = NULL;
    
    int *crossoverTrItemData;
    
    unsigned int *platformTrItemData = NULL;
    QString platformName;
    QString stationName;
    int platformMinWaitingTime;
    int platformNumPassengersWaiting;
    
    float *trItemSRData = NULL;
    
    float *speedpostTrItemData = NULL;
    
    unsigned int trSignalType1;
    int trSignalType2;
    float trSignalType3;
    QString trSignalType4;
    int trSignalDirs = 0;
    int *trSignalDir = NULL;
    float *trSignalRDir = NULL;
    bool titLoading = false;
    float pickupTrItemData1;
    unsigned int pickupTrItemData2;
    
    bool init(QString sh);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void save(QTextStream* out, bool tit);
    void addToTrackPos(float d);
    void flipTrackPos(float d);
    void setTrItemRData(float* posT, float*pos);
    void setTrItemPData(float* posT, float* pos);
    void setPickupContent(float val);
    void setSignalRot(float rot);
    void setSpeedpostRot(float rot);
    void setSignalDirection(int dir);
    void flipSignal();
    void enableSignalSubObj(int i);
    void disableSignalSubObj(int i);
    void render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot);
private:
    float* drawPosition = NULL;
    int speedpostTrItemDataLength = 0;
    static TrackItemObj* pointer3d; 
};

#endif	/* TRITEM_H */

