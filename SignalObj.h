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
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    bool getBorder(float* border);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    unsigned int signalSubObj;
    int signalUnits = 0;
    int *trItemId;
    TrackItemObj* pointer3d = NULL;
    float** drawPositions = NULL;
    void renderTritems(GLUU* gluu);
};

#endif	/* SIGNALOBJ_H */

