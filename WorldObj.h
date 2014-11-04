#ifndef WORLDOBJ_H
#define	WORLDOBJ_H

#include <QString>
#include "GLUU.h"
#include <QString>
#include "FileBuffer.h"

class WorldObj {
public:
    float UiD;
    QString fileName;
    float position[3];
    float qDirection[4];
    float staticDetailLevel;
    float staticFlags;
    float vDbId;
    
    QString type;
    QString resPath;
    int skipLevel;
    int x;
    int y;
    int shape;
    bool loaded;
    float size;
    int jestPQ = 0;
    
    WorldObj();
    WorldObj(const WorldObj& orig);
    virtual ~WorldObj();
    virtual void load(int x, int y);
    virtual void set(QString sh, FileBuffer* data);
    virtual void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov);
private:

};

#endif	/* WORLDOBJ_H */

