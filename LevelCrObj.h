#ifndef LEVELCROBJ_H
#define	LEVELCROBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class LevelCrObj : public WorldObj {
public:
    LevelCrObj();
    LevelCrObj(const LevelCrObj& orig);
    virtual ~LevelCrObj();
    void load(int x, int y);
    void set(int sh, FileBuffer* data);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool getBorder(float* border);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);

private:
    int levelCrParameters[2];
    int crashProbability = 0;
    unsigned int levelCrData[2];
    int levelCrTiming[3];
    int *trItemId = NULL;
    int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
};

#endif	/* LEVELCROBJ_H */

