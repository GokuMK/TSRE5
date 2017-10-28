/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ROUTE_H
#define	ROUTE_H
#include <QMap>
#include <QString>
#include "GLUU.h"
#include "TDB.h"
#include "WorldObj.h"
#include "Ref.h"

class Tile;
class Brush;
class Coords;
class Trk;
class SoundList;
class RouteEditorGLWidget;
class Activity;
class Consist;
class ActivityObject;
class Service;
class Traffic;
class Path;
class TRitem;
class Environment;
class GroupObj;

class Route : public QObject {
    Q_OBJECT
public:
    std::unordered_map<int, Tile*> tile;
    QVector<int> activityId;
    //QVector<Service*> service;
    //QVector<Traffic*> traffic;
    QVector<Path*> path;
    bool loaded = false;
    TSectionDAT *tsection; 
    SoundList *soundList;
    Ref *ref;
    Environment *env;
    bool placementStickToTarget = false;
    float placementAutoLength = 50;
    bool placementAutoTwoPointRot = true;
    bool snapableOnlyRotation = false;
    int placementAutoTargetType = 0;
    float placementAutoTranslationOffset[3];
    float placementAutoRotationOffset[3];
    Route();
    Route(const Route& orig);
    virtual ~Route();
    WorldObj* getObj(int x, int z, int id);
    Tile * requestTile(int x, int z);
    void activitySelected(Activity* selected);
    void save();
    void saveTrk();
    void createNewPaths();
    void createNew();
    void loadMkrList();
    void createMkrPlaces();
    void loadActivities();
    void loadServices();
    void loadTraffic();
    void loadPaths();
    void newTile(int x, int z);
    void reloadTile(int x, int z);
    void deleteObj(WorldObj* obj);
    void undoPlaceObj(int x, int y, int UiD);
    void removeTrackFromTDB(WorldObj* obj);
    void findSimilar(WorldObj* obj, GroupObj* group, float *playerT, int tileRadius = 0);
    void nextDefaultEnd();
    void flipObject(WorldObj *obj);
    void setTerrainTextureToObj(int x, int y, float *pos, Brush* brush, WorldObj* obj = NULL);
    void setTerrainToTrackObj(WorldObj* obj, Brush* brush);
    int getTileObjCount(int x, int z);
    int getTileHiddenObjCount(int x, int z);
    int getStartTileX();
    int getStartTileZ();
    float getStartpX();
    float getStartpZ();
    Trk *getTrk();
    TRitem *getTrackItem(int TID, int UID);
    QMap<QString, Coords*> getMkrList();
    void dragWorldObject(WorldObj* obj, int x, int z, float* pos);
    void setMkrFile(QString name);
    void getUnsavedInfo(std::vector<QString> &items);
    void showTrkEditr(Trk * val = NULL);
    void paintHeightMap(Brush* brush, int x, int z, float* p);
    WorldObj* makeFlexTrack(int x, int z, float* pos);
    WorldObj* placeObject(int x, int z, float* p);
    WorldObj* placeObject(int x, int z, float* p, float* q);
    WorldObj* placeObject(int x, int z, float* p, float* q, Ref::RefItem* r);
    WorldObj* autoPlaceObject(int x, int z, float* p, int mode);
    void replaceWorldObjPointer(WorldObj* o, WorldObj* n);
    void autoPlacementDeleteLast();
    void moveWorldObjToTile(int x, int z, WorldObj* obj);
    void deleteTDBTree(WorldObj* obj);
    void deleteTDBVector(WorldObj* obj);
    void toggleToTDB(WorldObj* obj);
    void addToTDBIfNotExist(WorldObj* obj);
    void addToTDB(WorldObj* obj);
    void newPositionTDB(WorldObj* obj);
    void linkSignal(int x, int z, float* p, WorldObj* obj);
    void actPickNewEventLocation(int x, int z, float* p);
    void actNewLooseConsist(int x, int z, float* p);
    void actNewFailedSignal(int x, int z, float* p);
    void actNewNewSpeedZone(int x, int z, float* p);
    void transalteObj(int x, int z, float px, float py, float pz, int uid);
    void setTDB(TDB* tdb, bool road);
    void updateSim(float *playerT, float deltaTime);
    ActivityObject* getActivityObject(int id);
    Activity* getCurrentActivity();
    void render(GLUU *gluu, float* playerT, float* playerW, float* target, float playerRot, float fov, int renderMode);
    void renderShadowMap(GLUU *gluu, float* playerT, float* playerW, float* target, float playerRot, float fov, bool selection);

signals:
    objSelected(GameObj* obj);

private:
    void loadTrk();
    TDB *trackDB;
    TDB *roadDB; 
    QMap<QString, Coords*> mkrList;
    Coords * mkr = NULL;
    Trk * trk = NULL;
    QVector<WorldObj*> autoPlacementLastPlaced;
    Activity* currentActivity = NULL;
};

#endif	/* ROUTE_H */

