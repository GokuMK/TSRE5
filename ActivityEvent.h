/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <QString>
#include <QVector>

#ifndef ACTIVITYEVENT_H
#define	ACTIVITYEVENT_H

class FileBuffer;
class QTextStream;

class ActivityEvent {

public:
    struct Outcome {
        QString displayMessage;
        QString *activitysuccess = NULL;
        std::vector<int> incactlevel;
        std::vector<int> decactlevel;
        std::vector<int> activateevent;
        QString *activityfail = NULL;
        QString *startignoringspeedlimits = NULL;
    };

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
        EventTypeReachSpeed = 9,
        EventTypePickUp = 10
    };

    enum EventCategory {
        CategoryUndefined = 0,
        CategoryAction = 1,
        CategoryLocation = 2,
        CategoryTime = 3
    };
    
    EventCategory category = CategoryUndefined;
    EventType eventType = EventTypeNone;
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
    QVector<int> pickupIdAndAmount;
    QString textToDisplayOnCompletionIfTriggered;
    QString textToDisplayOnCompletionIfNotTriggered;
    QString textToDisplayDescriptionOfTask;
    Outcome* outcome = NULL;
    
    ActivityEvent();
    ~ActivityEvent();
    void load(FileBuffer* data);
    void save(QTextStream* out);
};

#endif	/* ACTIVITYEVENT_H */

