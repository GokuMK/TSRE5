#ifndef ACTIVITY_H
#define	ACTIVITY_H

#include <QString>
#include <unordered_map>

class FileBuffer;
class Consist;

class Activity {
public:
    Activity();
    virtual ~Activity();
    Activity(QString p, QString n);
    Activity(QString src, QString p, QString n);
    
    struct ActivityObject{
        Consist* con = NULL;
        QString objectType;
        float direction = 0;
        int id = 0;
        float tile[4];
    };
    
    struct TrafficDefinition {
        std::vector<int> arrivalTime;
        std::vector<int> departTime;
        std::vector<int> skipCount;
        std::vector<float> distanceDownPath;
        std::vector<int> platformStartID;
        void load(FileBuffer* data);
    };
    
    struct ServiceDefinition {
        QString name;
        int path = -1;
        int uid = -1;
        std::vector<int> efficiency;
        std::vector<int> skipCount;
        std::vector<float> distanceDownPath;
        std::vector<int> platformStartId;
        TrafficDefinition *trafficDefinition = NULL;
        void load(QString sh, FileBuffer* data);
    };
    
    struct Traffic {
        QString name;
        std::vector<ServiceDefinition> service;
    };
    
    struct RestrictedSpeedZone {
        float startPosition[4];
        float endPosition[4];
        void load(FileBuffer* data);
    };
    
    struct Outcome {
        QString displayMessage;
        bool activitysuccess = false;
        int incactlevel = -1;
        int decactlevel = -1;
        int activateevent = -1;
        bool activityfail = false;
        bool startignoringspeedlimits = false;
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
        int category = -1;
        EventType eventType = Event::EventTypeNone;
        std::vector<int> wagonListId;
        std::vector<int> wagonListSidingItem;
        std::vector<std::string> wagonListDescription;
        
        int id = -1;
        bool reversableEvent = false;
        int activationLevel = -1;
        QString name;
        int time = -1;
        int stationStop = -1;
        float* loaction = NULL;
        int sidingItem = -1;
        float speed = -1;
        int triggerOnStop = -1;
        QString textToDisplayOnCompletionIfTriggered;
        QString textToDisplayOnCompletionIfNotTriggered;
        Outcome* outcome = NULL;
        void load(FileBuffer* data);
    };
    
    ServiceDefinition *playerServiceDefinition = NULL;
    Traffic *traffic = NULL;
    int nextServiceUID = -1;
    int nextActivityObjectUID = -1;
    
    std::vector<ActivityObject> activityObjects;
    std::vector<std::pair<int, int>> platformNumPassengersWaiting;
    std::vector<RestrictedSpeedZone> restrictedSpeedZone;
    std::vector<int> activityFailedSignal;
    std::vector<Event> event;

    QString name;
    QString path;
    QString pathid;  
    int loaded = -1;
    int ref = 0;
    
    int serial = 0;
    // header;
    QString hrouteid;
    QString hname;
    QString hdescription;
    QString hbriefing;
    QString hpathid;
    int completeActivity = 0;
    int type;
    int mode;
    int startTime[3];
    int season;
    int weather;
    int startingSpeed;
    int duration[2];
    int difficulty;
    int animals;
    int workers;
    int fuelWater;
    int fuelCoal;
    int fuelDiesel;
    int voltage;


    
    void load();
private:
    void loadActivityObject(FileBuffer* data);
    void loadActivityHeader(FileBuffer* data);
};

#endif	/* ACTIVITY_H */

