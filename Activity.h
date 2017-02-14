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
#include <unordered_map>

class FileBuffer;
class Consist;
class Traffic;
class QTextStream;
class GLUU;

class Activity {
public:
    Activity();
    virtual ~Activity();
    Activity(QString p, QString n, bool isnew = false);
    Activity(QString src, QString p, QString n, bool nowe = false);
    
    struct ActivityObject {
        Consist* con = NULL;
        QString objectType;
        float direction = 0;
        int id = 0;
        float tile[4];
        ActivityObject();
        ActivityObject(int tid);
        void load(FileBuffer* data);
        void save(QTextStream* out);
    };
    
    struct PlayerTrafficDefinition {
        int id = 0;
        std::vector<int> arrivalTime;
        std::vector<int> departTime;
        std::vector<int> skipCount;
        std::vector<float> distanceDownPath;
        std::vector<int> platformStartID;
        void load(FileBuffer* data);
        void save(QTextStream* out);
    };
    
    struct ServiceDefinition {
        QString name;
        int path = -1;
        int uid = -1;
        bool player = false;
        bool empty = true;
        std::vector<float> efficiency;
        std::vector<int> skipCount;
        std::vector<float> distanceDownPath;
        std::vector<int> platformStartId;
        PlayerTrafficDefinition *trafficDefinition = NULL;
        void load(FileBuffer* data);
        void save(QTextStream* out);
    };
    
    struct TrafficDefinition {
        QString name;
        std::vector<ServiceDefinition> service;
    };
    
    struct RestrictedSpeedZone {
        float startPosition[4];
        float endPosition[4];
        void load(FileBuffer* data);
        void save(QTextStream* out);
    };
    
    struct Outcome {
        QString displayMessage;
        QString *activitysuccess = NULL;
        std::vector<int> incactlevel;
        std::vector<int> decactlevel;
        std::vector<int> activateevent;
        QString *activityfail = NULL;
        QString *startignoringspeedlimits = NULL;
    };
    
    struct Event {
        enum EventType {
            EventTypeNone = 0,
            EventTypeTime = 1,
            EventTypeLocation = 2,
            EventTypeAllstops = 3,
            EventTypePickupWagons = 4,
            EventTypeAssembleTrain = 5,
            EventTypeAssembleTrainAtLocation = 6,
            EventTypeDropoffWagonsAtLocation = 7,
            EventTypePickupPassengers = 8,
            EventTypeReachSpeed = 9
        };
        int category = -99999;
        EventType eventType = Event::EventTypeNone;
        std::vector<unsigned int> wagonListId;
        std::vector<unsigned int> wagonListSidingItem;
        std::vector<std::string> wagonListDescription;
        
        int id = -99999;
        bool reversableEvent = false;
        int activationLevel = -99999;
        QString name;
        int time = -99999;
        int stationStop = -99999;
        float* location = NULL;
        int sidingItem = -99999;
        float speed = -99999;
        int triggerOnStop = -99999;
        QString textToDisplayOnCompletionIfTriggered;
        QString textToDisplayOnCompletionIfNotTriggered;
        Outcome* outcome = NULL;
        void load(FileBuffer* data);
        void save(QTextStream* out);
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
    
    ServiceDefinition *playerServiceDefinition = NULL;
    ActivityHeader *header = NULL;
    TrafficDefinition *traffic = NULL;
    int nextServiceUID = -1;
    int nextActivityObjectUID = -1;
    std::vector<ActivityObject> activityObjects;
    std::vector<std::pair<int, int>> platformNumPassengersWaiting;
    std::vector<RestrictedSpeedZone> restrictedSpeedZone;
    std::vector<int> activityFailedSignal;
    std::vector<Event> event;
    int serial = -1;
    
    void load();
    void save();
    bool isInitActivityObjects = false;
    //void initActivityObjects();
    void render(GLUU* gluu, float * playerT);
    
    QString editorConListSelected;
    void init(QString route, QString name);
    bool isNew();
    bool isUnSaved();
    void setFileName(QString val);
    void setDisplayName(QString val);
    void setDifficulty(int val);
    void setDuration(int h, int m);
    void setStartTime(int h, int m, int s);
    void setSeason(int val);
    void setWeather(int val);
    void newLooseConsist(float *tdbPos);
    void createNewPlayerService(QString sName, int sTime );
    void createNewTrafficService(Traffic *t);
    void setFuelCoal(int val);
    void setFuelWater(int val);
    void setFuelDiesel(int val);
    void setHazardWorkers(int val);
    void setHazardAnimals(int val);
    void setDescription(QString val);
    void setBriefing(QString val);
    
private:
    bool modified = false;
    bool nowe = false;
};

#endif	/* ACTIVITY_H */

