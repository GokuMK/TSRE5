#ifndef WORLDOBJ_H
#define	WORLDOBJ_H

#include <QString>
#include "GLUU.h"
#include <QString>

class WorldObj {
public:
    QString type;
    float UiD;
    QString fileName;
    float position[3];
    float qDirection[4];
    int skipLevel;
    int x;
    int y;
    int shape;
    bool loaded;
    float size;

    WorldObj();
    WorldObj(const WorldObj& orig);
    virtual ~WorldObj();
    virtual void load(QString path, int x, int y);
    virtual bool isAttribute(QString s);
    virtual void render(GLUU* gluu);
    virtual void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov);
private:
    static QString attributes[1];
};

#endif	/* WORLDOBJ_H */

