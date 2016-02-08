#ifndef SOUNDSOURCE_H
#define	SOUNDSOURCE_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class PoleObj;

class SoundSourceObj : WorldObj {
public:
    SoundSourceObj();
    virtual ~SoundSourceObj();
    bool allowNew();
    void load(int x, int y);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    //void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    PoleObj* pointer3d = NULL;
};

#endif	/* SOUNDSOURCE_H */

