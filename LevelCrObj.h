/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    void set(QString sh, QString val);
    void set(int sh, FileBuffer* data);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void setSensitivityActivateLevel(float val);
    void setSensitivityMinimunDistance(float val);
    void setTimingInitialWarning(float val);
    void setTimingSeriousWarning(float val);
    void setTimingAnimationLength(float val);
    void setCrashProbability(float val);
    void setInvisible(bool val);
    void setSilentMstsHax(bool val);
    float getSensitivityActivateLevel();
    float getSensitivityMinimunDistance();
    float getTimingInitialWarning();
    float getTimingSeriousWarning();
    float getTimingAnimationLength();
    float getCrashProbability();
    bool isInvisibleEnabled();
    bool isSilentMstsHaxEnabled();
    bool allowNew();
    bool isTrackItem();
    void deleteTrItems();
    void initTrItems(float* tpos);
    int getDefaultDetailLevel();
    bool select(int value);
    void deleteSelectedTrItem();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);
private:
    int selectionValue = 0;
    int levelCrParameters[2];
    float crashProbability = 0;
    unsigned int levelCrData[2];
    int levelCrTiming[3];
    QVector<int> trItemId;
    //int *trItemId = NULL;
    int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL;
    TrackItemObj* pointer3dSelected = NULL;
    std::vector<float*> drawPositions;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
    bool getSimpleBorder(float* border);
    bool getBoxPoints(QVector<float> &points);
};

#endif	/* LEVELCROBJ_H */

