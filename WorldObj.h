#ifndef WORLDOBJ_H
#define	WORLDOBJ_H

#include <QString>
#include "GLUU.h"
#include "FileBuffer.h"
#include "OglObj.h"
#include "Ref.h"

class WorldObj {
public:
    unsigned int UiD = 0;
    QString fileName;
    float position[3];
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
    bool modified = false;;
    Vector3f selectionColor;
    OglObj box;
    float tRotation[2];
    
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
    void setQdirection(float* q);
    void setNewQdirection();
    void setMartix();
    virtual bool allowNew();
    virtual void deleteVBO();
    virtual void translate(float px, float py, float pz);
    virtual void rotate(float x, float y, float z);
    virtual void resize(float x, float y, float z);
    virtual Ref::RefItem* getRefInfo();
    void drawBox();
    virtual bool select();
    virtual bool unselect();
    virtual bool isSelected();
    virtual bool getBorder(float* border);
    virtual void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
protected:
    float* matrix3x3 = NULL;
    bool selected;
};

#endif	/* WORLDOBJ_H */

