/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CARSPAWNEROBJ_H
#define	CARSPAWNEROBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;
class OglObj;
class SFile;

class CarSpawnerObj : public WorldObj  {
public:
    struct CarSpawnerList {
        QString name;
        bool ignoreXRot = false;
        QVector<QString> carName;
        QVector<int> val;
    };
   
    struct SimpleCar {
        bool loaded;
        int x;
        int y;
        QString carName;
        int shapeId = -1;
        SFile* shapePointer;
        unsigned int shapeState;
        bool ignoreXRot = false;
        int trNodeId;
        float trPosMb;
        float trPosMe;
        float direction;
        float speed;
        
        float drawPosition[7];
        SimpleCar();
        SimpleCar(QString name);
        ~SimpleCar();
        void updateSim(float deltaTime);
        void render(GLUU *gluu, int selectionColor);
    };
    
    static void LoadCarSpawnerList();
    static QVector<CarSpawnerList> carSpawnerList;
    
    CarSpawnerObj();
    CarSpawnerObj(const CarSpawnerObj& o);
    WorldObj* clone();
    virtual ~CarSpawnerObj();
    void load(int x, int y);
    bool allowNew();
    void set(int sh, FileBuffer* val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool select(int value);
    bool isTrackItem();
    void initTrItems(float* tpos);
    void setPosition(int x, int z, float* p);
    void translate(float px, float py, float pz);
    float getCarNumber();
    float getCarSpeed();
    QString getCarListName();
    void setCarListName(QString val);
    float getLength();
    void setCarNumber(float val);
    void setCarSpeed(float val);
    void deleteTrItems();
    void expand();
    int getDefaultDetailLevel();
    void updateSim(float deltaTime);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);
private:
    int trItemId[4];
    int trItemIdCount = 0;
    float carFrequency = 5;
    float carAvSpeed = 20;
    TrackItemObj* spointer3d = NULL;
    TrackItemObj* spointer3dSelected = NULL;
    OglObj* line = NULL;
    float* drawPositionB = NULL;
    float* drawPositionE = NULL;
    int rotB = 0;
    int rotE = 0;
    int selectionValue = 0;
    QString carspawnerListName;
    int carListId = 0;
    float carsNewTime = 0;
    float carFreq = 1;
    QVector<SimpleCar> cars;
    void renderTritems(GLUU* gluu, int selectionColor);
    void makelineShape();
    static void parseCarList(FileBuffer* data);
};

#endif	/* CARSPAWNEROBJ_H */

