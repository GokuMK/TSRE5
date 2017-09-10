/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityEvent.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "Game.h"
#include <QDebug>
#include "GLUU.h"
#include "TDB.h"

ActivityEvent::ActivityEvent() {
}

ActivityEvent::~ActivityEvent() {
}

void ActivityEvent::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("eventtypetime")) {
            eventType = EventTypeTime;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypelocation")) {
            eventType = EventTypeLocation;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypeallstops")) {
            eventType = EventTypeAllstops;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypepickupwagons")) {
            eventType = EventTypePickupWagons;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypeassembletrain")) {
            eventType = EventTypeAssembleTrain;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypeassembletrainatlocation")) {
            eventType = EventTypeAssembleTrainAtLocation;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypedropoffwagonsatlocation")) {
            eventType = EventTypeDropoffWagonsAtLocation;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypepickuppassengers")) {
            eventType = EventTypePickupPassengers;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypereachspeed")) {
            eventType = EventTypeReachSpeed;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("eventtypepickup")) {
            eventType = EventTypePickUp;
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
        if (sh == ("texttodisplaydescriptionoftask")) {
            textToDisplayDescriptionOfTask = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("pickupidandamount")) {
            pickupIdAndAmount.push_back(ParserX::GetNumber(data));
            pickupIdAndAmount.push_back(ParserX::GetNumber(data));
            pickupIdAndAmount.push_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        
        qDebug() << "#event - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void ActivityEvent::save(QTextStream* out) {
    if(category == CategoryAction)
        *out << "			EventCategoryAction (\n";
    else if(category == CategoryLocation)
        *out << "			EventCategoryLocation (\n";
    else if(category == CategoryTime)
        *out << "			EventCategoryTime (\n";
    else 
        return;

    if(eventType == EventTypeTime)
        *out << "				EventTypeTime ( )\n";
    if(eventType == EventTypeLocation)
        *out << "				EventTypeLocation ( )\n";
    if(eventType == EventTypeAllstops)
        *out << "				EventTypeAllStops ( )\n";
    if(eventType == EventTypePickupWagons)
        *out << "				EventTypePickUpWagons ( )\n";
    if(eventType == EventTypeAssembleTrain)
        *out << "				EventTypeAssembleTrain ( )\n";
    if(eventType == EventTypeAssembleTrainAtLocation)
        *out << "				EventTypeAssembleTrainAtLocation ( )\n";
    if(eventType == EventTypeDropoffWagonsAtLocation)
        *out << "				EventTypeDropOffWagonsAtLocation ( )\n";
    if(eventType == EventTypePickupPassengers)
        *out << "				EventTypePickUpPassengers ( )\n";
    if(eventType == EventTypeReachSpeed)
        *out << "				EventTypeReachSpeed ( )\n";
    if(eventType == EventTypePickUp)
        *out << "				EventTypePickUp ( )\n";
    
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
    
    if(pickupIdAndAmount.size() == 3){
        *out << "				PickupIdAndAmount (\n";
        *out << "					TrItemId ( "<< pickupIdAndAmount[0] << " "<< pickupIdAndAmount[1] <<" ) "<< pickupIdAndAmount[2] << "\n";
        *out << "				)\n";
    }
    
    *out << "			)\n";
}