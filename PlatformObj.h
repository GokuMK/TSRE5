#ifndef PLATFORMOBJ_H
#define	PLATFORMOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class PlatformObj : public WorldObj  {
public:
    PlatformObj();
    PlatformObj(const PlatformObj& orig);
    virtual ~PlatformObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    unsigned int platformData;
    int trItemId[4];
    int trItemIdCount = 0;
};

#endif	/* PLATFORMOBJ_H */

