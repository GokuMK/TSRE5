/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    enum SType {
        MILEPOST = 0,
        WARNING = 1,
        SIGN = 2,
        RESUME = 3
    };
    static TRitem* newPlatformItem(int trItemId, float metry);
    static TRitem* newSidingItem(int trItemId, float metry);
    static TRitem* newCarspawnerItem(int trItemId, float metry);
    static TRitem* newPickupItem(int trItemId, float metry);
    static TRitem* newSignalItem(int trItemId, float metry, int direction, unsigned int flags, QString type);
    static TRitem* newSpeedPostItem(int trItemId, float metry, int speedPostType);
    static TRitem* newLevelCrItem(int trItemId, float metry);
    static TRitem* newSoundRegionItem(int trItemId, float metry);
    static TRitem* newHazardItem(int trItemId, float metry);
    static TRitem* newCrossOverItem(int trItemId, float metry, int trItemId2, int shapeIdx);
    TRitem();
    TRitem(int id);
    TRitem(const TRitem& o);
    virtual ~TRitem();
    
    QString type;
    
    unsigned int trItemId;
    int trItemSData2;
    float *trItemPData = NULL;
    float *trItemRData = NULL;
    
    int *crossoverTrItemData = NULL;
    
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
    float getSpeedpostRot();
    void setSoundRegionRot(float rot);
    void setSoundRegionData(float rot, float ttype, float val = 0);
    void setSignalDirection(int dir);
    void flipSignal();
    void flipSpeedpost();
    void flipSoundRegion();
    void setSpeedpostSpeed(float val);
    void setSpeedpostNumber(float val);
    float getSpeedpostSpeed();
    float getSpeedpostNumber();
    void setSpeedpostSpeedInsteadNumber(bool val);
    void setSpeedpostNumberInsteadSpeed(bool val);
    bool getSpeedpostSpeedInsteadNumber();
    bool getSpeedpostNumberInsteadSpeed();
    void setSpeedPostSpeedUnitId(int val);
    int getSpeedPostSpeedUnitId();
    int getSpeedPostTrainType();
    void setSpeedPostTrainType(int val);
    bool getSpeedPostNumberDot();
    void setSpeedPostNumberDot(bool val);
    SType getSpeedpostType();
    //void linkSignal(int from, int to);
    void linkSignal(int trackId, int dist);
    void enableSignalSubObjFlag(int i);
    void disableSignalSubObjFlag(int i);
    void refresh();
    float getTrackPosition();
    void setTrackPosition(float val);
    void trackPositionAdd(float val);
    void render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot);
private:
    float trItemSData1;
    
    float* drawPosition = NULL;
    int speedpostTrItemDataLength = 0;
    static TrackItemObj* pointer3d; 
};

#endif	/* TRITEM_H */

