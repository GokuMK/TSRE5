#ifndef TRACKOBJ_H
#define	TRACKOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"
#include "Ref.h"

class TrackObj : public WorldObj  {
public:
    TrackObj();
    TrackObj(const TrackObj& orig);
    virtual ~TrackObj();
    bool allowNew();
    void load(int x, int y);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void set(QString sh, int val);
    bool getBorder(float* border);
    void rotate(float x, float y, float z);
    Ref::RefItem* getRefInfo();
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
    int sectionIdx;
private:
    float elevation;
    float* jNodePosn = NULL;
};

#endif	/* TRACKOBJ_H */

