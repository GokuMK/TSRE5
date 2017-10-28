/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIGNALOBJ_H
#define	SIGNALOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;
class SignalShape;

class SignalObj : public WorldObj  {
public:
    struct SignalUnit {
        bool enabled = false;
        bool head = false;
        int tdbId = 0;
        int itemId = -1;
    };
    
    SignalObj();
    SignalObj(const SignalObj& o);
    WorldObj* clone();
    virtual ~SignalObj();
    void load(int x, int y);
    void set(QString sh, long long int val);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool select(int value);
    int getTrItemId();
    int getBaseSignalItemId();
    int getLinkedJunctionValue(int i);
    void getLinkInfo(int *ids);
    bool isSubObjEnabled(int i);
    bool isJunctionAvailable(int i);
    Ref::RefItem* getRefInfo();
    bool allowNew();
    bool isTrackItem();
    void initTrItems(float* tpos);
    void deleteTrItems();
    void enableSubObj(int i);
    void disableSubObj(int i);
    void flip(bool flipShape);
    //void linkSignal(int subsigId, int from, int to);
    void linkSignal(int trackId, int dist);
    QString getShapePath();
    int getDefaultDetailLevel();
    void checkFlags(QStringList &list);
    void fixFlags();
    bool isSimilar(WorldObj* obj);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);
    int subObjSelected = 0;
private:
    unsigned int signalSubObj;
    int signalUnits = 0;
    SignalUnit signalUnit[32];
    //int *trItemId;
    TrackItemObj* spointer3d = NULL;
    TrackItemObj* spointer3dSelected = NULL;
    float** drawPositions = NULL;
    int selectionValue = 0;
    void renderTritems(GLUU* gluu, int selectionColor);
    bool getSimpleBorder(float* border);
    bool getBoxPoints(QVector<float> &points);
    SignalShape* signalShape = NULL;
};

#endif	/* SIGNALOBJ_H */

