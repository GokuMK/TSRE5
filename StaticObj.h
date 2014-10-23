#ifndef STATICOBJ_H
#define	STATICOBJ_H

#include "WorldObj.h"
#include <QString>

class StaticObj : WorldObj  {
public:
    StaticObj();
    StaticObj(const StaticObj& orig);
    virtual ~StaticObj();
    void load(QString path, int x, int y);
    bool isAttribute(QString s);
    void render(GLUU* gluu);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov);
private:
    static QString attributes[5];
};

#endif	/* STATICOBJ_H */

