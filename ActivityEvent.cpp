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
#include "OglObj.h"
#include "TextObj.h"
#include "TerrainLib.h"
#include "GLMatrix.h"

OglObj *ActivityEvent::simpleMarkerObj = NULL;

QMap<ActivityEvent::EventType, QString> ActivityEvent::EventTypeDescription = {
    { ActivityEvent::EventTypeNone ,"None." },
    { ActivityEvent::EventTypeTime ,"Time Event." },
    { ActivityEvent::EventTypeLocation ,"Location Event." },
    { ActivityEvent::EventTypeAllstops ,"Stop at final station." },
    { ActivityEvent::EventTypePickupWagons ,"Pick up Wagons." },
    { ActivityEvent::EventTypeAssembleTrain ,"Assemble Train" },
    { ActivityEvent::EventTypeAssembleTrainAtLocation ,"Assemble Train at location." },
    { ActivityEvent::EventTypeDropoffWagonsAtLocation ,"Drop off cars at location." },
    { ActivityEvent::EventTypePickupPassengers ,"Pick up Passengers." },
    { ActivityEvent::EventTypeReachSpeed ,"Reach Speed." },
    { ActivityEvent::EventTypePickUp ,"Make a pickup." }
};

QMap<ActivityEvent::EventType, QString> ActivityEvent::EventTypeName = {
    { ActivityEvent::EventTypeNone ,"" },
    { ActivityEvent::EventTypeTime ,"EventTypeTime" },
    { ActivityEvent::EventTypeLocation ,"EventTypeLocation" },
    { ActivityEvent::EventTypeAllstops ,"EventTypeAllStops" },
    { ActivityEvent::EventTypePickupWagons ,"EventTypePickUpWagons" },
    { ActivityEvent::EventTypeAssembleTrain ,"EventTypeAssembleTrain" },
    { ActivityEvent::EventTypeAssembleTrainAtLocation ,"EventTypeAssembleTrainAtLocation" },
    { ActivityEvent::EventTypeDropoffWagonsAtLocation ,"EventTypeDropOffWagonsAtLocation" },
    { ActivityEvent::EventTypePickupPassengers ,"EventTypePickUpPassengers" },
    { ActivityEvent::EventTypeReachSpeed ,"EventTypeReachSpeed" },
    { ActivityEvent::EventTypePickUp ,"EventTypePickUp" }
};

QMap<QString, ActivityEvent::EventType> ActivityEvent::EventNameType = {
    { "", ActivityEvent::EventTypeNone },
    { "eventtypetime", ActivityEvent::EventTypeTime },
    { "eventtypelocation", ActivityEvent::EventTypeLocation },
    { "eventtypeallstops", ActivityEvent::EventTypeAllstops },
    { "eventtypepickupwagons", ActivityEvent::EventTypePickupWagons },
    { "eventtypeassembletrain", ActivityEvent::EventTypeAssembleTrain },
    { "eventtypeassembletrainatlocation", ActivityEvent::EventTypeAssembleTrainAtLocation },
    { "eventtypedropoffwagonsatlocation", ActivityEvent::EventTypeDropoffWagonsAtLocation },
    { "eventtypepickuppassengers", ActivityEvent::EventTypePickupPassengers },
    { "eventtypereachspeed", ActivityEvent::EventTypeReachSpeed },
    { "eventtypepickup", ActivityEvent::EventTypePickUp }
};

QMap<ActivityEvent::Outcome::OutcomeType, QString> ActivityEvent::Outcome::OutcomeTypeDescription = {
    { ActivityEvent::Outcome::TypeNone ,"None." },
    { ActivityEvent::Outcome::TypeDisplayMessage ,"Display a message." },
    { ActivityEvent::Outcome::TypeActivitySuccess ,"Complete Activity succesfully." },
    { ActivityEvent::Outcome::TypeActivityFail ,"End Activity without success." },
    { ActivityEvent::Outcome::TypeIncActLevel ,"Increase an event's activation level." },
    { ActivityEvent::Outcome::TypeDecActLevel ,"Decrease an event's activation level." },
    { ActivityEvent::Outcome::TypeRestorAactLevel ,"Restore an event's activation level." },
    { ActivityEvent::Outcome::TypeActivateEvent ,"Activate an event." },
    { ActivityEvent::Outcome::TypeStartIgnoringSpeedLimits ,"Start ignoring speed limits." },
    { ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits ,"Stop ignoring speed limits." }
};

QMap<ActivityEvent::Outcome::OutcomeType, QString> ActivityEvent::Outcome::OutcomeTypeName = {
    { ActivityEvent::Outcome::TypeNone ,"" },
    { ActivityEvent::Outcome::TypeDisplayMessage ,"DisplayMessage" },
    { ActivityEvent::Outcome::TypeActivitySuccess ,"ActivitySuccess" },
    { ActivityEvent::Outcome::TypeActivityFail ,"ActivityFail" },
    { ActivityEvent::Outcome::TypeIncActLevel ,"IncActLevel" },
    { ActivityEvent::Outcome::TypeDecActLevel ,"DecActLevel" },
    { ActivityEvent::Outcome::TypeRestorAactLevel ,"RestoreActLevel" },
    { ActivityEvent::Outcome::TypeActivateEvent ,"ActivateEvent" },
    { ActivityEvent::Outcome::TypeStartIgnoringSpeedLimits ,"StartIgnoringSpeedLimits" },
    { ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits ,"StopIgnoringSpeedLimits" }
};

QMap<ActivityEvent::Outcome::OutcomeType, ActivityEvent::Outcome::OutcomeCategory> ActivityEvent::Outcome::OutcomeTypeCategory = {
    { ActivityEvent::Outcome::TypeNone , ActivityEvent::Outcome::CategoryNone },
    { ActivityEvent::Outcome::TypeDisplayMessage, ActivityEvent::Outcome::CategoryInfo },
    { ActivityEvent::Outcome::TypeActivitySuccess, ActivityEvent::Outcome::CategoryInfo },
    { ActivityEvent::Outcome::TypeActivityFail, ActivityEvent::Outcome::CategoryInfo },
    { ActivityEvent::Outcome::TypeIncActLevel, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeDecActLevel, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeRestorAactLevel, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeActivateEvent, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeStartIgnoringSpeedLimits, ActivityEvent::Outcome::CategoryInfo },
    { ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits, ActivityEvent::Outcome::CategoryInfo }
};

QMap<QString, ActivityEvent::Outcome::OutcomeType> ActivityEvent::Outcome::OutcomeNameType = {
    { "", ActivityEvent::Outcome::TypeNone },
    { "displaymessage", ActivityEvent::Outcome::TypeDisplayMessage },
    { "activitysuccess", ActivityEvent::Outcome::TypeActivitySuccess },
    { "activityfail", ActivityEvent::Outcome::TypeActivityFail },
    { "incactlevel", ActivityEvent::Outcome::TypeIncActLevel },
    { "decactlevel", ActivityEvent::Outcome::TypeDecActLevel },
    { "restoreactlevel", ActivityEvent::Outcome::TypeRestorAactLevel },
    { "activateevent", ActivityEvent::Outcome::TypeActivateEvent },
    { "startignoringspeedlimits", ActivityEvent::Outcome::TypeStartIgnoringSpeedLimits },
    { "stopignoringspeedlimits", ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits }
};

ActivityEvent::ActivityEvent() {
}

ActivityEvent::~ActivityEvent() {
}

void ActivityEvent::render(GLUU* gluu, float * playerT, float playerRot, int renderMode){
    if(category == EventTypeLocation){
        if(!Game::viewInteractives)
            return;
        gluu->setMatrixUniforms();

        if (simpleMarkerObj == NULL) {
            simpleMarkerObj = new OglObj();
            float *punkty = new float[3 * 2];
            int ptr = 0;
            int i = 0;

            punkty[ptr++] = 0;
            punkty[ptr++] = 0;
            punkty[ptr++] = 0;
            punkty[ptr++] = 0;
            punkty[ptr++] = 30;
            punkty[ptr++] = 0;

            simpleMarkerObj->setMaterial(1.0, 0.0, 0.0);
            simpleMarkerObj->setLineWidth(4);
            simpleMarkerObj->init(punkty, ptr, simpleMarkerObj->V, GL_LINES);
            delete[] punkty;
        }
        
        if (txtMarkerObj == NULL) {
            txtMarkerObj = new TextObj(name, 16, 2.0);
            txtMarkerObj->setColor(0,0,0);
        }

        //if (fabs(markerList[i].tileX[j] - playerT[0]) + fabs(-markerList[i].tileZ[j] - playerT[1]) > 2) {
        //    continue;
        //}
        gluu->mvPushMatrix();
        float h = TerrainLib::getHeight(location[0], -location[1], location[2], -location[3]);
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, location[2] + 2048 * (location[0] - playerT[0]), h, -location[3] + 2048 * (-location[1] - playerT[1]));
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        simpleMarkerObj->render();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 30, 0);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        txtMarkerObj->render(playerRot);
        gluu->mvPopMatrix();
    }
}

void ActivityEvent::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if(EventNameType[sh] != EventTypeNone){
            eventType = EventNameType[sh];
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
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if(Outcome::OutcomeNameType[sh] != Outcome::TypeNone){
                    outcomes.push_back(new Outcome(Outcome::OutcomeNameType[sh]));
                    outcomes.back()->load(data);
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
        if (sh == ("ortscontinue")) {
            ortsContinue = ParserX::GetNumber(data);
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
    if(EventTypeName[eventType] == "")
        return;
    
    if(category == CategoryAction)
        *out << "			EventCategoryAction (\n";
    else if(category == CategoryLocation)
        *out << "			EventCategoryLocation (\n";
    else if(category == CategoryTime)
        *out << "			EventCategoryTime (\n";
    else 
        return;

    *out << "				" << EventTypeName[eventType] << " ( )\n";
    
    if(id != -99999)
        *out << "				ID ( "<<id<<" )\n";
    if(activationLevel != -99999)
        *out << "				Activation_Level ( "<<activationLevel<<" )\n";

    if(outcomes.size() > 0){
        *out << "				Outcomes (\n";
        for(int i = 0; i < outcomes.size(); i++){
            outcomes[i]->save(out);
        }
        *out << "				)\n";
    } else {
        *out << "				Outcomes ( )\n";
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
    if (textToDisplayDescriptionOfTask.length() > 0){
        *out << "				TextToDisplayDescriptionOfTask ( ";
        *out << ParserX::SplitToMultiline(textToDisplayDescriptionOfTask, "					 ");
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
    if(ortsContinue  != -99999)
        *out << "				ORTSContinue ( "<<ortsContinue<<" )\n";
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

void ActivityEvent::Outcome::load(FileBuffer* data){
    category = OutcomeTypeCategory[type];
    if(category == CategoryInfo)
        value.setValue(ParserX::GetStringInside(data));
    if(category == CategoryEvent)
        value.setValue(ParserX::GetNumber(data));
}

void ActivityEvent::Outcome::save(QTextStream* out) {
    if(type == TypeNone)
        return;
    
    QString tabOffset = "						 ";
    
    if(category == CategoryInfo){
        QString val = value.toString();
        if(val.length() < 1)
            *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( )\n";
        else
            *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( "<<ParserX::SplitToMultiline(val, tabOffset)<<" )\n";
    }
    if(category == CategoryEvent){
        *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( "<<value.toInt()<<" )\n";
        //else
        //    *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( )\n";
    }
}