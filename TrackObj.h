#ifndef TRACKOBJ_H
#define	TRACKOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackObj : public WorldObj  {
public:
    TrackObj();
    TrackObj(const TrackObj& orig);
    virtual ~TrackObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:

};

#endif	/* TRACKOBJ_H */

