#ifndef TRWATERMARKOBJ_H
#define	TRWATERMARKOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrWatermarkObj : public WorldObj  {
public:
    TrWatermarkObj();
    TrWatermarkObj(int level);
    TrWatermarkObj(const TrWatermarkObj& orig);
    virtual ~TrWatermarkObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    int dstLevel = 0;
};
#endif	/* TRWATERMARKOBJ_H */

