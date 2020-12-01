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
#include "OrtsWeatherChange.h"
#include "TRitem.h"
#include "Activity.h"
#include "PlatformObj.h"

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
    { ActivityEvent::Outcome::TypeActivitySuccess ,"Complete Activity successfully." },
    { ActivityEvent::Outcome::TypeActivityFail ,"End Activity without success." },
    { ActivityEvent::Outcome::TypeIncActLevel ,"Increase an event's activation level." },
    { ActivityEvent::Outcome::TypeDecActLevel ,"Decrease an event's activation level." },
    { ActivityEvent::Outcome::TypeRestorAactLevel ,"Restore an event's activation level." },
    { ActivityEvent::Outcome::TypeActivateEvent ,"Activate an event." },
    { ActivityEvent::Outcome::TypeStartIgnoringSpeedLimits ,"Start ignoring speed limits." },
    { ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits ,"Stop ignoring speed limits." },
    { ActivityEvent::Outcome::TypeORTSActSoundFile , "Play sound from file." },
    { ActivityEvent::Outcome::TypeORTSWeatherChange , "Change the Weather." }
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
    { ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits ,"StopIgnoringSpeedLimits" },
    { ActivityEvent::Outcome::TypeORTSActSoundFile , "ORTSActSoundFile" },
    { ActivityEvent::Outcome::TypeORTSWeatherChange , "ORTSWeatherChange" }
};

QMap<ActivityEvent::Outcome::OutcomeType, ActivityEvent::Outcome::OutcomeCategory> ActivityEvent::Outcome::OutcomeTypeCategory = {
    { ActivityEvent::Outcome::TypeNone , ActivityEvent::Outcome::CategoryNone },
    { ActivityEvent::Outcome::TypeDisplayMessage, ActivityEvent::Outcome::CategoryInfo },
    { ActivityEvent::Outcome::TypeActivitySuccess, ActivityEvent::Outcome::CategoryNone },
    { ActivityEvent::Outcome::TypeActivityFail, ActivityEvent::Outcome::CategoryInfo },
    { ActivityEvent::Outcome::TypeIncActLevel, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeDecActLevel, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeRestorAactLevel, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeActivateEvent, ActivityEvent::Outcome::CategoryEvent },
    { ActivityEvent::Outcome::TypeStartIgnoringSpeedLimits, ActivityEvent::Outcome::CategoryNone },
    { ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits, ActivityEvent::Outcome::CategoryNone },
    { ActivityEvent::Outcome::TypeORTSActSoundFile , ActivityEvent::Outcome::CategorySoundFile },
    { ActivityEvent::Outcome::TypeORTSWeatherChange , ActivityEvent::Outcome::CategoryWeatherChange }
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
    { "stopignoringspeedlimits", ActivityEvent::Outcome::TypeStopIgnoringSpeedLimits },
    { "ortsactsoundfile", ActivityEvent::Outcome::TypeORTSActSoundFile },
    { "ortsweatherchange", ActivityEvent::Outcome::TypeORTSWeatherChange }
};

ActivityEvent::ActivityEvent() {
}

ActivityEvent::ActivityEvent(int uid, EventCategory c){
    category = c;
    id = uid;
    activationLevel = 1;
    if(category == CategoryAction){
        eventType = EventTypeAllstops;
        reversableEvent = false;
    }
    if(category == CategoryLocation){
        eventType = EventTypeLocation;
        location = new float[5];
        location[0] = 0;
        location[1] = 0;
        location[2] = 0;
        location[3] = 0;
        location[4] = 10;
    }
    if(category == CategoryTime){
        eventType = EventTypeTime;
        time = 0;
    }
    
    modified = true;
}

ActivityEvent::~ActivityEvent() {
}

int ActivityEvent::newOutcome(){
    Outcome *o = new Outcome(Outcome::TypeNone);
    outcomes.push_back(o);
    modified = true;
    return outcomes.size() - 1;
}

void ActivityEvent::removeOutcome(int id){
    if(id < 0)
        return;
    if(outcomes.size() <= id)
        return;
    modified = true;
    
    // Support for OR features.
    if(outcomes[id]->category == Outcome::CategorySoundFile)
        ortsActSoundFileDeprecated.clear();
    if(outcomes[id]->category == Outcome::CategoryWeatherChange)
        ortsWeatherChangeDeprecated = NULL;
    
    outcomes.remove(id);
}

bool ActivityEvent::isModified(){
    for(int i = 0; i < outcomes.size(); i++)
        if(outcomes[i]->isModified())
            return true;
    return modified;
}

bool ActivityEvent::isSelected(){
    return selected;
}

bool ActivityEvent::setSelected(bool val){
    selected = val;
    return selected;
}

bool ActivityEvent::select(){
    selected = true;
    return true;
}

bool ActivityEvent::unselect(){
    selected = false;
    return false;
}

void ActivityEvent::setModified(bool val){
    for(int i = 0; i < outcomes.size(); i++)
        outcomes[i]->setModified(val);
    modified = val;
}

void ActivityEvent::setActivationLevel(int val){
    activationLevel = val;
    modified = true;
}

void ActivityEvent::setName(QString val){
    name = val;
    modified = true;
}

void ActivityEvent::setTriggeredText(QString val){
    textToDisplayOnCompletionIfTriggered = val;
    modified = true;
}

void ActivityEvent::setUntriggeredText(QString val){
    textToDisplayOnCompletionIfNotTriggered = val;
    modified = true;
}

void ActivityEvent::setLocation(int X, int Z, float x, float z){
    if(category != CategoryLocation)
        return;
    location[0] = X;
    location[1] = Z;
    location[2] = x;
    location[3] = z;
    modified = true;
}

void ActivityEvent::setNotes(QString val){
    textToDisplayDescriptionOfTask = val;
    modified = true;
}

void ActivityEvent::setSpeed(int val){
    speed = val;
    modified = true;
}

void ActivityEvent::setLocationRadius(int val){
    if(category != CategoryLocation)
        return;
    location[4] = val;
    modified = true;
}

void ActivityEvent::setLocationStop(bool val){
    if(category != CategoryLocation)
        return;
    if(val)
        triggerOnStop = 1;
    else 
        triggerOnStop = 0;
    modified = true;
}

void ActivityEvent::setReversable(bool val){
    if(category != CategoryAction)
        return;
    if(val)
        reversableEvent = true;
    else 
        reversableEvent = false;
    modified = true;
}

void ActivityEvent::setAutoContinue(int val){
    //if(val >= 0)
    ortsContinue = val;
    modified = true;
}

void ActivityEvent::setTime(int val){
    if(category != CategoryTime)
        return;
    time = val;
    modified = true;
}

int ActivityEvent::getWagonListSize(){
    return wagonList.size();
}

QString ActivityEvent::getWagonListDescription(int i){
    QString desc;
    if(wagonList[i].sidingItem > 1000000){
        desc += wagonList[i].description;
    } else if(Game::trackDB != NULL){
        if(Game::trackDB->trackItems[wagonList[i].sidingItem] != NULL)
            desc += "Siding: " + Game::trackDB->trackItems[wagonList[i].sidingItem]->platformName;
    }
        
    return desc;
}

QString ActivityEvent::getWagonListIdDescription(int i){
    int consistId = wagonList[i].uid / 65536;
    int wagonId = wagonList[i].uid - consistId*65536;
    QString desc = QString::number(consistId) + "-" + QString::number(wagonId);
    return desc;
}

bool ActivityEvent::Outcome::isModified(){
    return modified;
}

void ActivityEvent::Outcome::setModified(bool val){
    modified = val;
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
            simpleMarkerObj->init(punkty, ptr, RenderItem::V, GL_LINES);
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
        float h = Game::terrainLib->getHeight(location[0], -location[1], location[2], -location[3]);
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
            isWagonList = true;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("uid")) {
                    wagonList.push_back(WagonList());
                    wagonList.back().uid = ParserX::GetUInt(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("sidingitem")) {
                    wagonList.back().sidingItem = ParserX::GetUInt(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("description")) {
                    wagonList.back().description = ParserX::GetStringInside(data);
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
        if (sh == "ortsactsoundfile"){
            ortsActSoundFileDeprecated.push_back(ParserX::GetStringInside(data));
            ortsActSoundFileDeprecated.push_back(ParserX::GetStringInside(data));
        }
        if (sh == "ortsweatherchange"){
            ortsWeatherChangeDeprecated = new OrtsWeatherChange();
            ortsWeatherChangeDeprecated->load(data);
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
    
    // Support for Orts features.
    if(outcomes.size() > 0){
        // Sound File.
        for(int i = 0; i < outcomes.size(); i++){
            if(outcomes[i]->category == Outcome::CategorySoundFile){
                ortsActSoundFileDeprecated.clear();
                ortsActSoundFileDeprecated.push_back(outcomes[i]->value.toStringList()[0]);
                ortsActSoundFileDeprecated.push_back(outcomes[i]->value.toStringList()[1]);
                break;
            }
        }
        // Weather.
        for(int i = 0; i < outcomes.size(); i++){
            if(outcomes[i]->category == Outcome::CategoryWeatherChange){
                ortsWeatherChangeDeprecated = OrtsWeatherChange::OrtsWeatherChanges[outcomes[i]->value.toString()];
                break;
            }
        }
    }
    
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

    if(isWagonList || wagonList.size() > 0){
        *out << "				Wagon_List (\n";
        for(int i=0; i < wagonList.size(); i++){
            *out << "					UiD ( "<<wagonList[i].uid<<" )\n";
            *out << "					SidingItem ( "<<wagonList[i].sidingItem<<" )\n";
            if(wagonList[i].sidingItem > 1000000)
            *out << "					Description ( "<<ParserX::AddComIfReq(wagonList[i].description)<<" )\n";
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
    if (ortsActSoundFileDeprecated.size() > 0){
        *out << "				ORTSActSoundFile ( "<< ParserX::AddComIfReq(ortsActSoundFileDeprecated[0]) <<" "<< ParserX::AddComIfReq(ortsActSoundFileDeprecated[1]) <<" )\n";
    }
    if (ortsWeatherChangeDeprecated != NULL){
        QString off = "				";
        ortsWeatherChangeDeprecated->save(out, off);
    }
    
    *out << "			)\n";
}

void ActivityEvent::Outcome::load(FileBuffer* data){
    if(category == CategoryInfo){
        value.setValue(ParserX::GetStringInside(data));
    }
    if(category == CategoryEvent){
        value.setValue(ParserX::GetNumber(data));
    }
    if(category == CategorySoundFile){
        QStringList values;
        values.push_back(ParserX::GetStringInside(data));
        values.push_back(ParserX::GetStringInside(data));
        value.setValue(values);
    }
    if(category == CategoryWeatherChange){
        value.setValue(ParserX::GetStringInside(data));
    }
}

void ActivityEvent::Outcome::save(QTextStream* out) {
    if(type == TypeNone)
        return;
    
    QString tabOffset = "						 ";
    
    if(category == CategoryNone){
        *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( )\n";
    }
    if(category == CategoryInfo){
        QString val = value.toString();
        if(val.length() < 1)
            *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( )\n";
        else
            *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( "<<ParserX::SplitToMultiline(val, tabOffset)<<" )\n";
    }
    if(category == CategoryEvent){
        *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( "<<value.toInt()<<" )\n";
    }
    if(category == CategorySoundFile){
        *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( "<<ParserX::AddComIfReq((value.toStringList())[0])<< " "<<(value.toStringList())[1]<< " )\n";
    }
    if(category == CategoryWeatherChange){
        *out << "					"<< Outcome::OutcomeTypeName[type] <<" ( "<<ParserX::AddComIfReq(value.toString())<<" )\n";
    }
}

void ActivityEvent::Outcome::setMessage(QString val){
    if(category != CategoryInfo)
        return;
    value.setValue(val);
    modified = true;
}

void ActivityEvent::Outcome::setEventLinkId(int id){
    value.setValue(id);
    modified = true;
}

void ActivityEvent::Outcome::setSoundFileName(QString val){
    if(category != CategorySoundFile)
        return;
    QStringList l = value.toStringList();
    l[0] = val;
    value.setValue(l);
    modified = true;
}

void ActivityEvent::Outcome::setSoundType(QString val){
    if(category != CategorySoundFile)
        return;
    QStringList l = value.toStringList();
    l[1] = val;
    value.setValue(l);
    modified = true;
}

void ActivityEvent::Outcome::setWeatherName(QString val){
    if(category != CategoryWeatherChange)
        return;
    value.setValue(val);
    modified = true;
}
       
void ActivityEvent::setStationStop(int tid){
    stationStop = tid;
    modified = true;
}

void ActivityEvent::setActionToNewType(EventType newType){
    eventType = newType;
    speed = -99999;
    stationStop = -99999;
    wagonList.clear();
    sidingItem = -99999;
    pickupIdAndAmount.clear();
    isWagonList = false;
    
    if(eventType == EventTypeReachSpeed)
        speed = 0;
    if(eventType == EventTypePickupPassengers)
        stationStop = 0;
    if(eventType == EventTypeAssembleTrainAtLocation
    || eventType == EventTypeDropoffWagonsAtLocation)
        sidingItem = 0;
    if(eventType == EventTypeAssembleTrain
    || eventType == EventTypeAssembleTrainAtLocation
    || eventType == EventTypeDropoffWagonsAtLocation
    || eventType == EventTypePickupWagons)
        isWagonList = true;
}

void ActivityEvent::Outcome::setToNewType(OutcomeType newType){
    type = newType;
    category = OutcomeTypeCategory[type];
    
    if(category == CategoryInfo){
        value.clear();
        value.setValue(QString(""));
    }
    if(category == CategoryEvent){
        value.clear();
        value.setValue(0);
    }
    if(category == CategorySoundFile){
        value.clear();
        QStringList v;
        v.push_back("");
        v.push_back("Everywhere");
        value.setValue(v);
    }
    if(category == CategoryWeatherChange){
        value.clear();
        value.setValue(QString(""));
    }
    modified = true;
}

void ActivityEvent::setParentActivity(Activity* a){
    parentActivity = a;
}

void ActivityEvent::removeWagonListItem(int id){
    if(id >= wagonList.size())
        return;
    if(id < 0)
        return;
    wagonList.remove(id);
}

QString ActivityEvent::getWagonListItemDescription(int id){
    if(id >= wagonList.size())
        return "";
    if(id < 0)
        return ""; 
    return wagonList[id].description;
}

void ActivityEvent::setWagonListItemDescription(int id, QString val){
    if(id >= wagonList.size())
        return;
    if(id < 0)
        return;
    wagonList[id].description = val;
}

void ActivityEvent::addSelectedWagonToList(){
    if(parentActivity == NULL)
        return;
    
    unsigned int uid = parentActivity->getSelectedCarId();
    if(uid == -1)
        return;
    
    for(int i = 0; i < wagonList.size(); i++)
        if(wagonList[i].uid == uid)
            return;
    
    wagonList.push_back(WagonList());
    wagonList.back().uid = uid;
    wagonList.back().sidingItem = 4294967295;
}

bool ActivityEvent::getWagonListItemPosition(int id, float *posTW){
    if(id >= wagonList.size())
        return false;
    if(id < 0)
        return false;
    if(parentActivity == NULL)
        return false;
    
    int oid = wagonList[id].uid / 65536;
    int eid = wagonList[id].uid - oid*65536;
    
    return parentActivity->getCarPosition(oid, eid, posTW);
}

QString ActivityEvent::getSidingDescription(){
    if(sidingItem < 0)
        return "";
    if(Game::trackDB == NULL)
        return "";
    if(Game::trackDB->trackItems[sidingItem] == NULL)
        return "";
    return Game::trackDB->trackItems[sidingItem]->platformName;
}

bool ActivityEvent::setSidingFromSelected(){
    if(Game::currentSelectedGameObj == NULL)
        return false;
    if(Game::currentSelectedGameObj->typeObj != GameObj::worldobj)
        return false;
    WorldObj *w = (WorldObj*)Game::currentSelectedGameObj;
    if(w->typeID != WorldObj::siding)
        return false;
    int tid = ((PlatformObj*)w)->getTrackBegItemId();
    if(tid < 0)
        return false;
    sidingItem = tid;
    
    return true;
}