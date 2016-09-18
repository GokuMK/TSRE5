/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PLATFORMOBJ_H
#define	PLATFORMOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;
class OglObj;

class PlatformObj : public WorldObj  {
public:
    static void LoadCarSpawnerList();
    static QVector<QString> CarSpawnerList;
    
    PlatformObj();
    PlatformObj(const PlatformObj& orig);
    virtual ~PlatformObj();
    void load(int x, int y);
    bool allowNew();
    void set(int sh, FileBuffer* val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool select(int value);
    bool isTrackItem();
    void initTrItems(float* tpos);
    void translate(float px, float py, float pz);
    QString getStationName();
    QString getPlatformName();
    int getPlatformMinWaitingTime();
    int getPlatformNumPassengersWaiting();
    void setStationName(QString name);
    void setPlatformName(QString name);
    void setPlatformMinWaitingTime(int val);
    void setPlatformNumPassengersWaiting(int val);
    bool getSideLeft();
    bool getSideRight();
    bool getDisabled();
    void setSideLeft(bool val);
    void setSideRight(bool val);
    void setDisabled(bool val);
    int getCarNumber();
    int getCarSpeed();
    QString getCarListName();
    void setCarListName(QString val);
    float getLength();
    void setCarNumber(int val);
    void setCarSpeed(int val);
    void deleteTrItems();
    int getDefaultDetailLevel();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);

private:
    unsigned int platformData;
    int trItemId[4];
    int trItemIdCount = 0;
    int carFrequency = 0;
    int	carAvSpeed = 0;
    TrackItemObj* pointer3d = NULL;
    TrackItemObj* pointer3dSelected = NULL;
    OglObj* line = NULL;
    float* drawPositionB = NULL;
    float* drawPositionE = NULL;
    int rotB = 0;
    int rotE = 0;
    int selectionValue = 0;
    QString carspawnerListName;
    void renderTritems(GLUU* gluu, int selectionColor);
    void makelineShape();
};


#endif	/* PLATFORMOBJ_H */

