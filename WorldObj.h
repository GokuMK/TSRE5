#ifndef WORLDOBJ_H
#define	WORLDOBJ_H

#include <QString>
#include "GLUU.h"
#include "FileBuffer.h"
#include "OglObj.h"
#include "Ref.h"

class QString;

class WorldObj {
public:
    enum TypeID {
        none = 0,
        undefined = 1,
        sstatic = 2,
        signal = 3,
        speedpost = 4,
        trackobj = 5,
        gantry = 6,
        collideobject = 7,
        dyntrack = 8,
        forest = 9,
        transfer = 10,
        platform = 11,
        siding = 12,
        carspawner = 13,
        levelcr = 14,
        pickup = 15,
        hazard = 16
    };
    TypeID typeID = this->undefined;
    
    static WorldObj* createObj(QString sh);
    static int isTrackObj(QString sh);
    
    unsigned int UiD = 0;
    QString fileName;
    float position[3];
    float placedAtPosition[3];
    float qDirection[4];
    float matrix[16];
    int staticDetailLevel = -1;
    unsigned int staticFlags = 0;
    unsigned int vDbId = 4294967295;
    int collideFlags;
    QString type;
    QString resPath;
    int skipLevel;
    int x;
    int y;
    int shape = -1;
    bool loaded;
    float size;
    int jestPQ = 0;
    bool modified = false;
    Vector3f selectionColor;
    OglObj box;
    float tRotation[2];
    float *endp = 0;
    
    WorldObj();
    WorldObj(const WorldObj& orig);
    virtual ~WorldObj();
    virtual void load(int x, int y);
    virtual void set(QString sh, FileBuffer* data);
    virtual void set(QString sh, QString val);
    virtual void set(QString sh, float* val);
    virtual void set(QString sh, int val);
    virtual void save(QTextStream* out);
    void setPosition(float* p);
    void initPQ(float* p, float* q);
    virtual void initTrItems(float *tpos);
    void setQdirection(float* q);
    void setNewQdirection();
    void setMartix();
    virtual bool allowNew();
    virtual void deleteVBO();
    virtual void translate(float px, float py, float pz);
    virtual void rotate(float x, float y, float z);
    virtual void resize(float x, float y, float z);
    virtual bool isTrackItem();
    virtual Ref::RefItem* getRefInfo();
    void drawBox();
    virtual bool select();
    virtual bool select(int value);
    virtual bool unselect();
    virtual bool isSelected();
    virtual bool getBorder(float* border);
    virtual void deleteTrItems();
    virtual void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
protected:
    float* matrix3x3 = NULL;
    bool selected;
};

#endif	/* WORLDOBJ_H */

