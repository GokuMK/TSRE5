/* 
 * File:   LevelCrObj.h
 * Author: Goku
 *
 * Created on 16 maja 2015, 08:18
 */
#ifndef LEVELCROBJ_H
#define	LEVELCROBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"


class LevelCrObj : public WorldObj {
public:
    LevelCrObj();
    LevelCrObj(const LevelCrObj& orig);
    virtual ~LevelCrObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);

private:
    int trItemId[4];
    int trItemIdCount = 0;
    int levelCrParameters[2];
    int crashProbability = 0;
    unsigned int levelCrData[2];
    int levelCrTiming[3];
};

#endif	/* LEVELCROBJ_H */

