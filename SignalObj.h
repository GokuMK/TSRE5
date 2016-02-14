#ifndef SIGNALOBJ_H
#define	SIGNALOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class SignalObj : public WorldObj  {
public:
    SignalObj();
    SignalObj(const SignalObj& orig);
    virtual ~SignalObj();
    void load(int x, int y);
    void set(QString sh, int val);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    bool getBorder(float* border);
    void save(QTextStream* out);
    bool select(int value);
    int getLinkedJunctionValue(int i);
    bool isSubObjEnabled(int i);
    bool isJunctionAvailable(int i);
    bool allowNew();
    bool isTrackItem();
    void initTrItems(float* tpos);
    void deleteTrItems();
    void enableSubObj(int i);
    void disableSubObj(int i);
    void flip(bool flipShape);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    unsigned int signalSubObj;
    int signalUnits = 0;
    int *trItemId;
    TrackItemObj* pointer3d = NULL;
    TrackItemObj* pointer3dSelected = NULL;
    float** drawPositions = NULL;
    int selectionValue = 0;
    void renderTritems(GLUU* gluu, int selectionColor);
};

#endif	/* SIGNALOBJ_H */

