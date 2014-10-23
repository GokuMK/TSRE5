#ifndef DYNTRACKOBJ_H
#define	DYNTRACKOBJ_H

#include "WorldObj.h"
#include <QString>

class DynTrackObj : WorldObj{
public:
    DynTrackObj();
    DynTrackObj(const DynTrackObj& orig);
    virtual ~DynTrackObj();
    void load(QString path, int x, int y);
    bool isAttribute(QString s);
    void render(GLUU* gluu);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov);
private:
    static QString attributes[5];

};

#endif	/* DYNTRACKOBJ_H */

