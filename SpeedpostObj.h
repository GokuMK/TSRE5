#ifndef SPEEDPOSTOBJ_H
#define	SPEEDPOSTOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class SpeedpostObj : public WorldObj  {
public:
    SpeedpostObj();
    SpeedpostObj(const SpeedpostObj& orig);
    virtual ~SpeedpostObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    bool getBorder(float* border);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    QString speedDigitTex;
    float speedSignShape[9];
    float speedTextSize[3];
    unsigned int trItemId[2];
};

#endif	/* SPEEDPOSTOBJ_H */

