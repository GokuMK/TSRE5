#ifndef SOUNDREGION_H
#define	SOUNDREGION_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class SoundRegionObj : public WorldObj{
public:
    SoundRegionObj();
    virtual ~SoundRegionObj();
    bool allowNew();
    bool isTrackItem();
    bool isSoundItem();
    void initTrItems(float* tpos);
    void deleteTrItems();
    void load(int x, int y);
    void set(QString sh, int val);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    float soundregionRoty;
    int soundregionTrackType;
    std::vector<int> trItemId;
    //int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL; 
    float* drawPositionB = NULL;
    float* drawPositionE = NULL;
    OglObj* drawLine = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
    float* drawPositions = NULL;
    int selectionValue = 0;
    float angle;
};

#endif	/* SOUNDREGION_H */

