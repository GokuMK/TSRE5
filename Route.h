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
#include <QHash>
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
class Skydome;
class FileBuffer;

class Route : public QObject {
    Q_OBJECT
public:
    QHash<int, Tile*> tile;
    QVector<int> activityId;
    //QVector<Service*> service;
    //QVector<Traffic*> traffic;
    QVector<Path*> path;
    bool loaded = false;
    TSectionDAT *tsection = NULL; 
    SoundList *soundList = NULL;
    Ref *ref = NULL;
    Environment *env = NULL;
    Skydome *skydome = NULL;
    bool placementStickToTarget = false;
    float placementAutoLength = 50;
    bool placementAutoTwoPointRot = true;
    bool snapableOnlyRotation = false;
    int placementAutoTargetType = 0;
    float placementAutoTranslationOffset[3];
    float placementAutoRotationOffset[3];
    TerrainLib* terrainLib = NULL;
    Trk * trk = NULL;
    Route();
    virtual void load();
    virtual void load(QString name);
    Route(const Route& orig);
    virtual ~Route();
    void loadAddons();
    void loadTdbData(FileBuffer *data, QString type);
    void loadTSectionData(FileBuffer *data);
    void loadQuadTreeDetailed(FileBuffer *data);
    void loadQuadTreeDistant(FileBuffer *data);
    void loadTrkData(FileBuffer *data);
    void updateTileData(FileBuffer *data);
    WorldObj* updateWorldObjData(FileBuffer *data);
    void mergeRoute(QString route2Name, float offsetX, float offsetY, float offsetZ);
    WorldObj* getObj(int x, int z, int id);
    WorldObj* findNearestObj(int x, int z, float *pos);
    virtual Tile * requestTile(int x, int z, bool allowNew = true);
    void activitySelected(Activity* selected);
    virtual void save();
    //void saveTrk();
    void createNewPaths();
    void createNew();
    bool checkTrackSectionDatabase();
    void checkRouteDatabase();
    void loadMkrList();
    void createMkrPlaces();
    void loadActivities();
    void loadServices();
    void loadTraffic();
    void loadPaths();
    void preloadWFiles(bool gui = false);
    void preloadWFilesInit();
    int newTile(int x, int z, bool forced = false);
    void reloadTile(int x, int z);
    void deleteObj(WorldObj* obj);
    void undoPlaceObj(int x, int y, int UiD);
    void removeTrackFromTDB(WorldObj* obj);
    void fillWorldObjectsByTrackItemId(QVector<WorldObj*> &objects, int tdbId, int id);
    void fillWorldObjectsByTrackItemIds(QHash<int, QVector<WorldObj*>> &objects, int tdbId);
    void findSimilar(WorldObj* obj, GroupObj* group, float *playerT, int tileRadius = 0);
    void nextDefaultEnd();
    void flipObject(WorldObj *obj);
    void setTerrainTextureToObj(int x, int y, float *pos, Brush* brush, WorldObj* obj = NULL);
    void setTerrainTextureToTrack(int x, int y, float *pos, Brush* brush, int mode = 0);
    void setTerrainToTrackObj(WorldObj* obj, Brush* brush);
    int getTileObjCount(int x, int z);
    int getTileHiddenObjCount(int x, int z);
    int getStartTileX();
    int getStartTileZ();
    float getStartpX();
    float getStartpZ();
    Trk *getTrk();
    TRitem *getTrackItem(int TID, int UID);
    void deleteTrackItem(TRitem *item);
    QMap<QString, Coords*> getMkrList();
    void dragWorldObject(WorldObj* obj, int x, int z, float* pos);
    float* getPointerPosition(float *out, int &x, int &z, float *pos);
    void setMkrFile(QString name);
    virtual void getUnsavedInfo(QVector<QString> &items);
    void showTrkEditr(Trk * val = NULL);
    void paintHeightMap(Brush* brush, int x, int z, float* p);
    WorldObj* makeFlexTrack(int x, int z, float* pos);
    WorldObj* placeObject(int x, int z, float* p);
    WorldObj* placeObject(int x, int z, float* p, float* q, float elev = 0);
    WorldObj* placeObject(int x, int z, float* p, float* q, float elev, Ref::RefItem* r);
    WorldObj* autoPlaceObject(int x, int z, float* p, int mode);
    void replaceWorldObjPointer(WorldObj* o, WorldObj* n);
    void autoPlacementDeleteLast();
    void moveWorldObjToTile(int x, int z, WorldObj* obj);
    void deleteTDBTree(WorldObj* obj);
    void fixTDBVectorElevation(WorldObj* obj);
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
    Consist* getActivityConsist(int id);
    Activity* getCurrentActivity();
    float getDistantTerrainYOffset();
    void setAsCurrentGameRoute();
    void selectObjectsByXYRange(int mojex, int mojez, int minx, int maxx, int minz, int maxz);
    void pushRenderItems(float* playerT, float* playerW, float* target, float playerRot, float fov, int renderMode);
    void render(GLUU *gluu, float* playerT, float* playerW, float* target, float playerRot, float fov, int renderMode);
    void renderShadowMap(GLUU *gluu, float* playerT, float* playerW, float* target, float playerRot, float fov, bool selection);

signals:
    void objectSelected(GameObj* obj);
    void objectSelected(QVector<GameObj*> obj);
    void sendMsg(QString val);

protected:
    QString trkName;
    QString routeDir;
    QString routeName;
    
    void loadTrk();
    TDB *trackDB = NULL;
    TDB *roadDB = NULL; 
    QMap<QString, Coords*> mkrList;
    Coords * mkr = NULL;
    QVector<WorldObj*> autoPlacementLastPlaced;
    Activity* currentActivity = NULL;
    int loadingProgress = 0;
};

#endif	/* ROUTE_H */

