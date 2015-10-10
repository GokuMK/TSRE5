#ifndef STATICOBJ_H
#define	STATICOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class StaticObj : public WorldObj  {
public:
    StaticObj();
    StaticObj(const StaticObj& orig);
    virtual ~StaticObj();
    bool allowNew();
    void load(int x, int y);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool getBorder(float* border);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:

};

#endif	/* STATICOBJ_H */

