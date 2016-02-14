#ifndef PICKUPOBJ_H
#define	PICKUPOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class PickupObj : public WorldObj {
public:
    float speedRange[2];
    int pickupType[2];
    int pickupAnimData1;
    float pickupAnimData2;
    int pickupCapacity1;
    float pickupCapacity2;
    PickupObj();
    PickupObj(const PickupObj& orig);
    virtual ~PickupObj();
    bool allowNew();
    bool isTrackItem();
    void deleteTrItems();
    void initTrItems(float* tpos);
    void load(int x, int y);
    void set(int sh, FileBuffer* data);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool getBorder(float* border);
    float getPickupContent();
    void setPickupContent(float val);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);

private:
    int *trItemId = NULL;
    int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
};

#endif	/* PICKUPOBJ_H */

