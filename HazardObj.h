#ifndef HAZARDOBJ_H
#define	HAZARDOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class HazardObj : public WorldObj {
public:
    HazardObj();
    HazardObj(const HazardObj& orig);
    virtual ~HazardObj();
    bool allowNew();
    bool isTrackItem();
    void deleteTrItems();
    void initTrItems(float* tpos);
    void load(int x, int y);
    void set(QString sh, QString val);
    void set(int sh, FileBuffer* data);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool getBorder(float* border);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);

private:
    int trItemIdCount = 0;
    int *trItemId = NULL;
    TrackItemObj* pointer3d = NULL;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
};

#endif	/* HAZARDOBJ_H */

