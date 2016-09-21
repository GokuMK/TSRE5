/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef WORLDOBJ_H
#define	WORLDOBJ_H

#include <QString>
#include "GLUU.h"
#include "FileBuffer.h"
#include "OglObj.h"
#include "Ref.h"

class SFile;

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
        hazard = 16,
        soundsource = 17,
        soundregion = 18
    };
    TypeID typeID = this->undefined;
    
    static WorldObj* createObj(int sh);
    static WorldObj* createObj(QString sh);
    static QString getResPath(Ref::RefItem* sh);
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
    SFile* shapePointer = NULL;
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
    virtual void set(int sh, FileBuffer* data);
    virtual void set(QString sh, FileBuffer* data);
    virtual void set(QString sh, QString val);
    virtual void set(QString sh, float* val);
    virtual void set(QString sh, int val);
    virtual void save(QTextStream* out);
    void setPosition(float* p);
    void setPosition(int x, int z, float* p);
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
    virtual bool isSoundItem();
    virtual Ref::RefItem* getRefInfo();
    void drawBox();
    virtual bool select();
    virtual bool select(int value);
    virtual bool unselect();
    virtual bool isSelected();
    virtual void deleteTrItems();
    virtual int getDefaultDetailLevel();
    virtual int getCustomDetailLevel();
    virtual int getCurrentDetailLevel();
    virtual void setCustomDetailLevel(int val);
    virtual bool customDetailLevelEnabled();
    virtual QString getShapePath();
    virtual void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
protected:
    virtual bool getSimpleBorder(float* border);
    virtual bool getBoxPoints(QVector<float> &points);
    float* matrix3x3 = NULL;
    bool selected;
};

#endif	/* WORLDOBJ_H */

