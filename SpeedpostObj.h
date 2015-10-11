#ifndef SPEEDPOSTOBJ_H
#define	SPEEDPOSTOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class SpeedpostObj : public WorldObj  {
public:
    SpeedpostObj();
    SpeedpostObj(const SpeedpostObj& orig);
    virtual ~SpeedpostObj();
    void load(int x, int y);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    bool getBorder(float* border);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    QString speedDigitTex;
    float* speedSignShape;
    float speedTextSize[3];
    int *trItemId = NULL;
    int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
};

#endif	/* SPEEDPOSTOBJ_H */

