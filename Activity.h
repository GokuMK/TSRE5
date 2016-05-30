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
    std::vector<ActivityObject> activityObjects;
    QString name;
    QString path;
    QString pathid;    
    int loaded = -1;
    int ref = 0;
    
    QString hrouteid;
    QString hname;
    QString hdescription;
    QString hbriefing;
    QString hpathid;
    
    void load();
private:
    void loadActivityobject(FileBuffer* data);
    
};

#endif	/* ACTIVITY_H */

