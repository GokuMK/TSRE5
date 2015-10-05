#ifndef HAZARDOBJ_H
#define	HAZARDOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class HazardObj : public WorldObj {
public:
    HazardObj();
    HazardObj(const HazardObj& orig);
    virtual ~HazardObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);

private:
    int trItemId[2];
};

#endif	/* HAZARDOBJ_H */

