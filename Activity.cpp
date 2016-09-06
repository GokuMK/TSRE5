/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Activity.h"
#include "Consist.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "Game.h"
#include <QDebug>
#include <QFile>

Activity::Activity() {
}

Activity::~Activity() {
}

Activity::Activity(QString p, QString n) {
    pathid = p + "/" + n;
    pathid.replace("//", "/");
    path = p;
    name = n;
    loaded = -1;
    load();
}

Activity::Activity(QString src, QString p, QString n) {
    pathid = src;
    path = p;
    name = n;
    loaded = -1;
    load();
}

void Activity::load() {

    QString sh;
    pathid.replace("//", "/");
    qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->off = 48+16;

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("include")) {
            QString incPath = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            data->insertFile(path + "/" + incPath);
            continue;
        }
        if (sh == ("tr_activity")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

                if (sh == ("include")) {
                    QString incPath = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    data->insertFile(path + "/" + incPath);
                    continue;
                }
                if (sh == ("serial")) {
                    serial = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("tr_activity_header")) {
                    header = new ActivityHeader();
                    header->load(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("tr_activity_file")) {

                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

                        if (sh == ("player_service_definition")) {
                            playerServiceDefinition = new ServiceDefinition();
                            playerServiceDefinition->player = true;
                            playerServiceDefinition->load(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("nextserviceuid")) {
                            nextServiceUID = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("nextactivityobjectuid")) {
                            nextActivityObjectUID = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("traffic_definition")) {
                            traffic = new Traffic();
                            traffic->name = ParserX::GetStringInside(data);
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

                                if (sh == ("service_definition")) {
                                    traffic->service.emplace_back();
                                    traffic->service.back().load(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("events")) {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

                                if (sh == ("eventcategoryaction")) {
                                    event.emplace_back();
                                    event.back().category = 0;
                                    event.back().load(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if (sh == ("eventcategorylocation")) {
                                    event.emplace_back();
                                    event.back().category = 1;
                                    event.back().load(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if (sh == ("eventcategorytime")) {
                                    event.emplace_back();
                                    event.back().category = 2;
                                    event.back().load(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#events - undefined token: " << sh;
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("platformnumpassengerswaiting")) {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if (sh == ("platformdata")) {
                                    platformNumPassengersWaiting.emplace_back(std::make_pair(ParserX::GetNumber(data), ParserX::GetNumber(data)));
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#platformnumpassengerswaiting - undefined token: " << sh;
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("activityobjects")) {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

                                if (sh == ("activityobject")) {
                                    activityObjects.emplace_back();
                                    activityObjects.back().load(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#activityobjects - undefined token: " << sh;
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("activityrestrictedspeedzones")) {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if (sh == ("activityrestrictedspeedzone")) {
                                    restrictedSpeedZone.emplace_back();
                                    restrictedSpeedZone.back().load(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#activityrestrictedspeedzones - undefined token: " << sh;
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("activityfailedsignals")) {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if (sh == ("activityfailedsignal")) {
                                    activityFailedSignal.emplace_back(ParserX::GetNumber(data));
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#activityfailedsignals - undefined token: " << sh;
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        qDebug() << "#tr_activity_file - undefined token: " << sh;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#tr_activity - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#ACT - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
    delete data;
    loaded = 1;

    //save();
    return;
}

void Activity::save() {
    QString sh;
    QString path;
    path = pathid;
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0a0t______\n";
    out << "\n";
    out << "Tr_Activity (\n";
    if (serial >= 0) {
        out << "	Serial ( " << serial << " )\n";
    }

    if (header != NULL) {
        header->save(&out);
    }

    out << "	Tr_Activity_File (\n";
    if (playerServiceDefinition != NULL) {
        playerServiceDefinition->save(&out);
    }
    if (nextServiceUID != -1)
        out << "		NextServiceUID ( " << nextServiceUID << " )\n";
    if (nextActivityObjectUID != -1)
        out << "		NextActivityObjectUID ( " << nextActivityObjectUID << " )\n";
    if (traffic != NULL){
        out << "		Traffic_Definition ( " << ParserX::AddComIfReq(traffic->name) <<"\n";
        for(int i = 0; i<traffic->service.size(); i++)
            traffic->service[i].save(&out);
        out << "		)\n";
    }

    if (event.size() > 0 ){
        out << "		Events (\n";
        for(int i = 0; i<event.size(); i++)
            event[i].save(&out);
        out << "		)\n";
    }

    if (activityObjects.size() > 0 ){
        out << "		ActivityObjects (\n";
        for(int i = 0; i<activityObjects.size(); i++)
            activityObjects[i].save(&out);
        out << "		)\n";
    }

    if (restrictedSpeedZone.size() > 0 ){
        out << "		ActivityRestrictedSpeedZones (\n";
        for(int i = 0; i<restrictedSpeedZone.size(); i++)
            restrictedSpeedZone[i].save(&out);
        out << "		)\n";
    }
    if (activityFailedSignal.size() > 0 ){
        out << "		ActivityFailedSignals (\n";
        for(int i = 0; i<activityFailedSignal.size(); i++)
            out << "			ActivityFailedSignal ( " << activityFailedSignal[i] << " )\n";
        out << "		)\n";
    }
    if (platformNumPassengersWaiting.size() > 0 ){
        out << "		PlatformNumPassengersWaiting (\n";
        for(int i = 0; i<platformNumPassengersWaiting.size(); i++)
            out << "			PlatformData ( " << platformNumPassengersWaiting[i].second << " " << platformNumPassengersWaiting[i].first << " )\n";
        out << "		)\n";
    }
    out << "	)\n";
    

    out << ")";

    file.close();
    modified = false;
    for(int i = 0; i < activityObjects.size(); i++){
        if(activityObjects[i].con != NULL)
            activityObjects[i].con->setModified(false);
    }
    
    path = pathid.remove(pathid.length()-4, 4);
    path += ".asv";
    path.replace("//", "/");
    qDebug() << path;
    QFile::remove(path);
}

void Activity::TrafficDefinition::load(FileBuffer* data) {
    QString sh;
    id = ParserX::GetNumber(data);
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("arrivaltime")) {
            arrivalTime.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("departtime")) {
            departTime.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("skipcount")) {
            skipCount.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("distancedownpath")) {
            distanceDownPath.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("platformstartid")) {
            platformStartID.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#trafficDefinition - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void Activity::TrafficDefinition::save(QTextStream* out) {

    *out << "			Player_Traffic_Definition ( "<< id <<"\n";

    for(int i = 0; i<arrivalTime.size(); i++){
        *out << "				ArrivalTime ( "<<arrivalTime[i]<<" )\n";
        *out << "				DepartTime ( "<<departTime[i]<<" )\n";
        *out << "				SkipCount ( "<<skipCount[i]<<" )\n";
        *out << "				DistanceDownPath ( "<<distanceDownPath[i]<<" )\n";
        *out << "				PlatformStartID ( "<<platformStartID[i]<<" )\n";
    }
    *out << "			)\n";
}

void Activity::Event::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("eventtypetime")) {
            eventType = Event::EventTypeTime;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypelocation")) {
            eventType = Event::EventTypeLocation;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypeallstops")) {
            eventType = Event::EventTypeAllstops;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypepickupwagons")) {
            eventType = Event::EventTypePickupWagons;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypeassembletrain")) {
            eventType = Event::EventTypeAssembleTrain;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypeassembletrainatlocation")) {
            eventType = Event::EventTypeAssembleTrainAtLocation;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypedropoffwagonsatlocation")) {
            eventType = Event::EventTypeDropoffWagonsAtLocation;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypepickuppassengers")) {
            eventType = Event::EventTypePickupPassengers;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypereachspeed")) {
            eventType = Event::EventTypeReachSpeed;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("reversable_event")) {
            reversableEvent = true;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("id")) {
            id = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("activation_level")) {
            activationLevel = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("outcomes")) {
            outcome = new Outcome();
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("activitysuccess")) {
                    outcome->activitysuccess = new QString();
                    *outcome->activitysuccess = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("displaymessage")) {
                    outcome->displayMessage = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("incactlevel")) {
                    outcome->incactlevel.push_back(ParserX::GetNumber(data));
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("decactlevel")) {
                    outcome->decactlevel.push_back(ParserX::GetNumber(data));
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("activateevent")) {
                    outcome->activateevent.push_back(ParserX::GetNumber(data));
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("activityfail")) {
                    outcome->activityfail = new QString();
                    *outcome->activityfail = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("startignoringspeedlimits")) {
                    outcome->startignoringspeedlimits = new QString();
                    *outcome->startignoringspeedlimits = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#event outcomes - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("name")) {
            name = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("time")) {
            time = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("location")) {
            location = new float[5];
            location[0] = ParserX::GetNumber(data);
            location[1] = ParserX::GetNumber(data);
            location[2] = ParserX::GetNumber(data);
            location[3] = ParserX::GetNumber(data);
            location[4] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("triggeronstop")) {
            triggerOnStop = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("sidingitem")) {
            sidingItem = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("speed")) {
            speed = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("stationstop")) {
            stationStop = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("wagon_list")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("uid")) {
                    wagonListId.emplace_back(ParserX::GetUInt(data));
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("sidingitem")) {
                    wagonListSidingItem.emplace_back(ParserX::GetUInt(data));
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("description")) {
                    wagonListDescription.emplace_back(ParserX::GetStringInside(data).toStdString());
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#event wagonlist - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("texttodisplayoncompletioniftriggered")) {
            textToDisplayOnCompletionIfTriggered = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("texttodisplayoncompletionifnottriggered")) {
            textToDisplayOnCompletionIfNotTriggered = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#event - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void Activity::Event::save(QTextStream* out) {
    if(category == 0)
        *out << "			EventCategoryAction (\n";
    else if(category == 1)
        *out << "			EventCategoryLocation (\n";
    else if(category == 2)
        *out << "			EventCategoryTime (\n";
    else 
        return;

    if(eventType == Event::EventTypeTime)
        *out << "				EventTypeTime ( )\n";
    if(eventType == Event::EventTypeLocation)
        *out << "				EventTypeLocation ( )\n";
    if(eventType == Event::EventTypeAllstops)
        *out << "				EventTypeAllStops ( )\n";
    if(eventType == Event::EventTypePickupWagons)
        *out << "				EventTypePickUpWagons ( )\n";
    if(eventType == Event::EventTypeAssembleTrain)
        *out << "				EventTypeAssembleTrain ( )\n";
    if(eventType == Event::EventTypeAssembleTrainAtLocation)
        *out << "				EventTypeAssembleTrainAtLocation ( )\n";
    if(eventType == Event::EventTypeDropoffWagonsAtLocation)
        *out << "				EventTypeDropOffWagonsAtLocation ( )\n";
    if(eventType == Event::EventTypePickupPassengers)
        *out << "				EventTypePickUpPassengers ( )\n";
    if(eventType == Event::EventTypeReachSpeed)
        *out << "				EventTypeReachSpeed ( )\n";

    if(id != -99999)
        *out << "				ID ( "<<id<<" )\n";
    if(activationLevel != -99999)
        *out << "				Activation_Level ( "<<activationLevel<<" )\n";

    if(outcome != NULL){
        *out << "				Outcomes (\n";
            if(outcome->activitysuccess != NULL)
                if(outcome->activitysuccess->length() == 0)
                    *out << "					ActivitySuccess ( )\n";
                else
                    *out << "					ActivitySuccess ( "<<ParserX::SplitToMultiline(*outcome->activitysuccess)<<" )\n";
            if(outcome->activityfail != NULL)
                if(outcome->activityfail->length() == 0)
                    *out << "					ActivityFail ( )\n";
                else
                    *out << "					ActivityFail ( "<<ParserX::SplitToMultiline(*outcome->activityfail)<<" )\n";
            if(outcome->activateevent.size() > 0)
                for(int i = 0; i < outcome->activateevent.size(); i++)
                    *out << "					ActivateEvent ( "<<outcome->activateevent[i]<<" )\n";
            if (outcome->displayMessage.length() > 0){
                *out << "					DisplayMessage ( ";
                *out << ParserX::SplitToMultiline(outcome->displayMessage, "						 ");
                *out << " )\n";
            }
            if(outcome->startignoringspeedlimits != NULL)
                if(outcome->startignoringspeedlimits->length() == 0)
                    *out << "					StartIgnoringSpeedLimits ( )\n";
                else
                    *out << "					StartIgnoringSpeedLimits ( "<<ParserX::SplitToMultiline(*outcome->startignoringspeedlimits)<<" )\n";
            if(outcome->decactlevel.size() > 0)
                for(int i = 0; i < outcome->decactlevel.size(); i++)
                    *out << "					DecActLevel ( "<<outcome->decactlevel[i]<<" )\n";
            if(outcome->incactlevel.size() > 0)
                for(int i = 0; i < outcome->incactlevel.size(); i++)
                    *out << "					IncActLevel ( "<<outcome->incactlevel[i]<<" )\n";

        *out << "				)\n";
    }

    if(reversableEvent)
        *out << "				Reversable_Event ( )\n";
    
    if (textToDisplayOnCompletionIfTriggered.length() > 0){
        *out << "				TextToDisplayOnCompletionIfTriggered ( ";
        *out << ParserX::SplitToMultiline(textToDisplayOnCompletionIfTriggered, "					 ");
        *out << " )\n";
    }
    if (textToDisplayOnCompletionIfNotTriggered.length() > 0){
        *out << "				TextToDisplayOnCompletionIfNotTriggered ( ";
        *out << ParserX::SplitToMultiline(textToDisplayOnCompletionIfNotTriggered, "					 ");
        *out << " )\n";
    }
    
    if(name.length() > 0)
        *out << "				Name ( "<<ParserX::AddComIfReq(name)<<" )\n";
    if(location != NULL)
        *out << "				Location ( "<<location[0]<<" "<<location[1]<<" "<<location[2]<<" "<<location[3]<<" "<<location[4]<<" )\n";
    if(time != -99999)
        *out << "				Time ( "<<time<<" )\n";
    if(triggerOnStop != -99999)
        *out << "				TriggerOnStop ( "<<triggerOnStop<<" )\n";
    if(speed != -99999)
        *out << "				Speed ( "<<speed<<" )\n";
    if(stationStop != -99999)
        *out << "				StationStop ( "<<stationStop<<" )\n";

    if(wagonListId.size() > 0){
        *out << "				Wagon_List (\n";
        for(int i=0; i < wagonListId.size(); i++){
            *out << "					UiD ( "<<wagonListId[i]<<" )\n";
            if(i < wagonListSidingItem.size())
            *out << "					SidingItem ( "<<wagonListSidingItem[i]<<" )\n";
            if(i < wagonListDescription.size())
            *out << "					Description ( "<<ParserX::AddComIfReq(QString::fromStdString(wagonListDescription[i]))<<" )\n";
        }
        *out << "				)\n";
    }
    if(sidingItem != -99999)
        *out << "				SidingItem ( "<<sidingItem<<" )\n";

    *out << "			)\n";
}

void Activity::RestrictedSpeedZone::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("startposition")) {
            startPosition[0] = ParserX::GetNumber(data);
            startPosition[1] = ParserX::GetNumber(data);
            startPosition[2] = ParserX::GetNumber(data);
            startPosition[3] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("endposition")) {
            endPosition[0] = ParserX::GetNumber(data);
            endPosition[1] = ParserX::GetNumber(data);
            endPosition[2] = ParserX::GetNumber(data);
            endPosition[3] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#restrictedSpeedZone - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void Activity::RestrictedSpeedZone::save(QTextStream* out) {
    *out << "			ActivityRestrictedSpeedZone (\n";
    *out << "				StartPosition ( "<<startPosition[0]<<" "<<startPosition[1]<<" "<<startPosition[2]<<" "<<startPosition[3]<<" )\n";
    *out << "				EndPosition ( "<<endPosition[0]<<" "<<endPosition[1]<<" "<<endPosition[2]<<" "<<endPosition[3]<<" )\n";
    *out << "			)\n";
}

void Activity::ServiceDefinition::load(FileBuffer* data) {
    QString sh;
    name = ParserX::GetStringInside(data);
    if(!player)
        path = ParserX::GetNumber(data);
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("player_traffic_definition")) {
            trafficDefinition = new TrafficDefinition();
            trafficDefinition->load(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("uid")) {
            this->uid = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("efficiency")) {
            this->efficiency.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("skipcount")) {
            this->skipCount.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("distancedownpath")) {
            this->distanceDownPath.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("platformstartid")) {
            this->platformStartId.emplace_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#serviceDefinition - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void Activity::ServiceDefinition::save(QTextStream* out) {
    QString woff;
    
    if (player){
        woff = "";
        *out << "		Player_Service_Definition ( "<< ParserX::AddComIfReq(name) <<"\n";
    } else {
        woff = "	";
        *out << woff << "		Service_Definition ( "<< ParserX::AddComIfReq(name) <<" "<<path<<"\n";
    }
    if(trafficDefinition != NULL)
        trafficDefinition->save(out);
    
    if(uid != -1)
        *out << woff << "			UiD ( "<<uid<<" )\n";
    for(int i = 0; i<efficiency.size(); i++){
        *out << woff << "			Efficiency ( "<<efficiency[i]<<" )\n";
        *out << woff << "			SkipCount ( "<<skipCount[i]<<" )\n";
        *out << woff << "			DistanceDownPath ( "<<distanceDownPath[i]<<" )\n";
        *out << woff << "			PlatformStartID ( "<<platformStartId[i]<<" )\n";
    }
    *out << woff << "		)\n";
}

void Activity::ActivityHeader::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("routeid")) {
            routeid = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("name")) {
            name = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("description")) {
            description = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("briefing")) {
            briefing = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("completeactivity")) {
            completeActivity = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("type")) {
            type = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("mode")) {
            mode = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("starttime")) {
            startTime[0] = ParserX::GetNumber(data);
            startTime[1] = ParserX::GetNumber(data);
            startTime[2] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("season")) {
            season = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("weather")) {
            weather = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("pathid")) {
            this->pathid = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("startingspeed")) {
            startingSpeed = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("duration")) {
            duration[0] = ParserX::GetNumber(data);
            duration[1] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("difficulty")) {
            difficulty = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("animals")) {
            animals = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("workers")) {
            workers = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("fuelwater")) {
            fuelWater = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("fuelcoal")) {
            fuelCoal = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("fueldiesel")) {
            fuelDiesel = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("voltage")) {
            voltage = new int[2];
            voltage[0] = ParserX::GetNumber(data);
            voltage[1] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#activityHeader - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void Activity::ActivityHeader::save(QTextStream* out) {
    *out << "	Tr_Activity_Header (\n";
    if (routeid.length() > 0)
    *out << "		RouteID ( " << ParserX::AddComIfReq(routeid) << " )\n";
    if (name.length() > 0)
    *out << "		Name ( " << ParserX::AddComIfReq(name) << " )\n";
    if (description.length() > 0){
    *out << "		Description ( ";
    *out << ParserX::SplitToMultiline(description, "			 ");
    *out << " )\n";
    }
    if (briefing.length() > 0){
    *out << "		Briefing ( ";
    *out << ParserX::SplitToMultiline(briefing, "			 ");
    *out << " )\n";
    }
    *out << "		CompleteActivity ( " << completeActivity << " )\n";
    if (type != -1)
        *out << "		Type ( " << type << " )\n";
    if (mode != -1)
        *out << "		Mode ( " << mode << " )\n";
    *out << "		StartTime ( " << startTime[0] << " " << startTime[1] << " " << startTime[2] << " )\n";
    if (season != -1)
        *out << "		Season ( " << season << " )\n";
    if (weather != -1)
        *out << "		Weather ( " << weather << " )\n";
    if (pathid.length() > 0)
    *out << "		PathID ( " << ParserX::AddComIfReq(pathid) << " )\n";
    if (startingSpeed != -1)
        *out << "		StartingSpeed ( " << startingSpeed << " )\n";
    *out << "		Duration ( " << duration[0] << " " << duration[1] << " )\n";
    if (difficulty != -1)
        *out << "		Difficulty ( " << difficulty << " )\n";
    if (animals != -1)
        *out << "		Animals ( " << animals << " )\n";
    if (workers != -1)
        *out << "		Workers ( " << workers << " )\n";
    if (voltage != NULL)
        *out << "		Voltage ( " << voltage[0] << " "<< voltage[1] <<" )\n";
    if (fuelWater != -1)
        *out << "		FuelWater ( " << fuelWater << " )\n";
    if (fuelCoal != -1)
        *out << "		FuelCoal ( " << fuelCoal << " )\n";
    if (fuelDiesel != -1)
        *out << "		FuelDiesel ( " << fuelDiesel << " )\n";
    *out << "	)\n";
}

void Activity::ActivityObject::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("objecttype")) {
            objectType = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("train_config")) {
            con = new Consist();
            if (con->load(data)) {
                con->loaded = 1;
                con->setMaxVelocityFixed(true);
                con->initPos();
            }
            ParserX::SkipToken(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("direction")) {
            direction = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("id")) {
            id = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("tile")) {
            tile[0] = ParserX::GetNumber(data);
            tile[1] = ParserX::GetNumber(data);
            tile[2] = ParserX::GetNumber(data);
            tile[3] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#activityObject - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void Activity::ActivityObject::save(QTextStream* out) {
    *out << "			ActivityObject (\n";
    *out << "				ObjectType ( "<<ParserX::AddComIfReq(objectType)<<" )\n";
    if(con != NULL){
        *out << "				Train_Config (\n";
        con->save("				", out);
        *out << "				)\n";
    }
    *out << "				Direction ( "<<direction<<" )\n";
    *out << "				ID ( "<<id<<" )\n";
    *out << "				Tile ( "<<tile[0]<<" "<<tile[1]<<" "<<tile[2]<<" "<<tile[3]<<" )\n";
    *out << "			)\n";
}

bool Activity::isUnSaved(){
    for(int i = 0; i < activityObjects.size(); i++){
        if(activityObjects[i].con != NULL)
            if(activityObjects[i].con->isUnSaved())
                return true;
    }
    return modified;
}