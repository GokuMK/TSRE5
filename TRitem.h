/* 
 * Author: Goku
 */

#ifndef TRITEM_H
#define	TRITEM_H

#include <QString>;
#include "Pointer3d.h"

class FileBuffer;
class QTextStream;
class TDB;

class TRitem {
public:
    TRitem();
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
    QString station;
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
    
    bool init(QString sh);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    
    void render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot);
private:
    Pointer3d* pointer3d;
    float* drawPosition = NULL;
};

#endif	/* TRITEM_H */

