/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITY_H
#define	ACTIVITY_H

#include <QString>
#include <QVector>
#include "ActivityEvent.h"
#include "ActivityObject.h"
#include "ActivityTimetable.h"

class FileBuffer;
class Consist;
class Path;
class Service;
class Traffic;
class QTextStream;
class GLUU;

class ActivityServiceDefinition {
public:
    QString name;
    Service* servicePointer = NULL;
    int time = -1;
    int uid = -1;
    bool player = false;
    bool empty = true;
    QVector<float> efficiency;
    QVector<int> skipCount;
    QVector<float> distanceDownPath;
    QVector<int> platformStartId;
    ActivityTimetable *trafficDefinition = NULL;
    void load(FileBuffer* data);
    void save(QTextStream* out);
    void reloadTimetable();
    void reloadDefinition();
    bool isModified();
    void calculateTimetable();
    void setTimetableEfficiency(int id, float val);
    void updateSim(float *playerT, float deltaTime);
    void render(GLUU *gluu, float* playerT, int renderMode);
    QMap<int, QString> getStationStopNameList();
private:
    bool modified = false;
};

class Activity {
public:
    Activity();
    virtual ~Activity();
    Activity(QString p, QString n, bool isnew = false);
    Activity(QString src, QString p, QString n, bool isnew = false);

    struct TrafficDefinition {
        QString name;
        QVector<ActivityServiceDefinition> service;
    };

    struct ActivityHeader {
        QString routeid;
        QString name;
        QString description;
        QString briefing;
        QString pathid;
        int completeActivity = 0;
        int type = -1;
        int mode = -1;
        int startTime[3];
        int season = -1;
        int weather = -1;
        int startingSpeed = -1;
        int duration[2];
        int difficulty = -1;
        int animals = -1;
        int workers = -1;
        int fuelWater = -1;
        int fuelCoal = -1;
        int fuelDiesel = -1;
        int* voltage = NULL;
        void load(FileBuffer* data);
        void save(QTextStream* out);
        ActivityHeader();
        ActivityHeader(QString route, QString hname);
    };

    QString name;
    QString nameid;
    QString path;
    QString pathid;
    int loaded = -1;
    int ref = 0;

    ActivityServiceDefinition *playerServiceDefinition = NULL;
    ActivityHeader *header = NULL;
    TrafficDefinition *traffic = NULL;
    int nextServiceUID = -1;
    int nextActivityObjectUID = -1;
    int nextEventUID = 0;
    int ortsAIHornAtCrossings = -9999;
    QVector<ActivityObject*> activityObjects;
    QVector<QPair<int, int>> platformNumPassengersWaiting;
    QVector<ActivityObject*> restrictedSpeedZone;
    QVector<ActivityObject*> activityFailedSignal;
    QVector<ActivityEvent> event;
    int serial = -1;

    void load();
    void save();
    bool isInitActivityObjects = false;
    //void initActivityObjects();
    void updateSim(float *playerT, float deltaTime);
    void render(GLUU* gluu, float * playerT, float playerRot, int renderMode);

    QString editorConListSelected;
    ActivityEvent *currentEventSelected = NULL;
    void init(QString route, QString name);
    //void setRouteContent(QVector<Path*>* p, QVector<Service*>* s, QVector<Traffic*>* t);
    //Traffic *getTrafficByName(QString name);
    bool isNew();
    void initToPlay();
    bool isUnSaved();
    void setFileName(QString val);
    void setDisplayName(QString val);
    void setOrtsHornAtCrossigns(bool val);
    bool isOrtsHornAtCrossigns();
    void setDifficulty(int val);
    void setDuration(int h, int m);
    void setStartTime(int h, int m, int s);
    void setSeason(int val);
    void setWeather(int val);
    void pickNewEventLocation(float *tdbPos);
    void newLooseConsist(float *tdbPos);
    void newSpeedZone(float *tdbPos);
    bool newFailedSignalFromSelected();
    void createNewPlayerService(QString sName, int sTime);
    void createNewTrafficService(Traffic *t);
    void setFuelCoal(int val);
    void setFuelWater(int val);
    void setFuelDiesel(int val);
    void setHazardWorkers(int val);
    void setHazardAnimals(int val);
    void setDescription(QString val);
    void setBriefing(QString val);
    void deleteObject(int id);
    void deleteObjectFailedSignal(int id);
    void deleteObjectSpeedZone(int id);
    void removeAllFailedSignals();
    void removeAllSpeedZones();
    void deleteCurrentEvent();
    void newEvent(ActivityEvent::EventCategory category);
    bool isServiceInUse(QString n);
    bool isTrafficInUse(QString n);
    bool isPlayerServiceInUse(QString n);
    QVector<ActivityServiceDefinition*> getServiceList();
    unsigned int getSelectedCarId();
    bool getCarPosition(int oid, int eid, float *posTW);
    QMap<int, QString> getEventIdNameList();
    QMap<int, QString> getServiceStationStopNameList();
    ActivityObject* getObjectById(int id);
    Consist* getServiceConsistById(int id);
    void updateService(QString serviceName);

private:
    bool modified = false;
    bool nowe = false;
    //QVector<Service*>* Services;
    //QVector<Traffic*>* Traffics;
    //QVector<Path*>* Paths;
};

#endif	/* ACTIVITY_H */

