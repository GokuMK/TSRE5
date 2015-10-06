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

class TRitem {
public:
    static TRitem* newPlatformItem(int trItemId, int metry);
    static TRitem* newSidingItem(int trItemId, int metry);
    static TRitem* newCarspawnerItem(int trItemId, int metry);
    TRitem();
    TRitem(int id);
    TRitem(const TRitem& orig);
    virtual ~TRitem();
    
    QString type;
    
    unsigned int trItemId;
    float trItemSData1;
    int trItemSData2;
    float *trItemPData;
    float *trItemRData;
    
    int *crossoverTrItemData;
    
    unsigned int *platformTrItemData;
    QString platformName;
    QString stationName;
    int platformMinWaitingTime;
    int platformNumPassengersWaiting;
    
    float *trItemSRData;
    
    float *speedpostTrItemData;
    
    int trSignalType1;
    int trSignalType2;
    float trSignalType3;
    QString trSignalType4;
    int trSignalDirs;
    int *trSignalDir;
    float *trSignalRDir;
    bool titLoading = false;
    unsigned int *pickupTrItemData;
    
    bool init(QString sh);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void save(QTextStream* out, bool tit);
    void addToTrackPos(float d);
    void flipTrackPos(float d);
    void setTrItemRData(float* posT, float*pos);
    void render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot);
private:
    float* drawPosition = NULL;
};

#endif	/* TRITEM_H */

