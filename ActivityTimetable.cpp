/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityTimetable.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include <qDebug>
#include <QTextStream>
#include "ActLib.h"
#include "Service.h"
#include "Path.h"

ActivityTimetable::ActivityTimetable() {
}

ActivityTimetable::ActivityTimetable(QString n, int t){
    name = n;
    time = t;
    nameTime = name+QString::number(time);
    empty = false;
}

ActivityTimetable::ActivityTimetable(const ActivityTimetable& orig) {
}

ActivityTimetable::~ActivityTimetable() {
}

void ActivityTimetable::reloadTimetable(){
    clear();

    Service *s = ActLib::GetServiceByName(name);
    if(s == NULL){
        qDebug() << "s == NILL " << name;
        return;
    }
    
    for(int i = 0; i < s->stationStop.size(); i++ ){
        arrivalTime.push_back(time);
        departTime.push_back(time);
        skipCount.push_back(s->stationStop[i].skipCount);
        distanceDownPath.push_back(s->stationStop[i].distanceDownPath);
        platformStartID.push_back(s->stationStop[i].platformStartID);
    }
}

void ActivityTimetable::clear(){
    arrivalTime.clear();
    departTime.clear();
    skipCount.clear();
    distanceDownPath.clear();
    platformStartID.clear();
}

void ActivityTimetable::load(FileBuffer* data) {
    QString sh;
    if(actTimetable){
        name = "";
        time = ParserX::GetNumber(data);
    } else {
        name = ParserX::GetStringInside(data);
        time = ParserX::GetNumber(data);
    } 
    nameTime = name+QString::number(time);
    empty = false;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("arrivaltime")) {
            arrivalTime.push_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("departtime")) {
            departTime.push_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("skipcount")) {
            skipCount.push_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("distancedownpath")) {
            distanceDownPath.push_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("platformstartid")) {
            platformStartID.push_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#trafficDefinition-Timetable - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void ActivityTimetable::save(QTextStream* out, QString off) {
    if(empty)
        return;
    if(actTimetable)
        *out << off << "Player_Traffic_Definition ( "<< time <<"\n";
    else
        *out << off << "Service_Definition ( "<< ParserX::AddComIfReq(name) << " "<< time <<"\n";

    for(int i = 0; i<arrivalTime.size(); i++){
        *out << off << "	ArrivalTime ( "<<arrivalTime[i]<<" )\n";
        *out << off << "	DepartTime ( "<<departTime[i]<<" )\n";
        *out << off << "	SkipCount ( "<<skipCount[i]<<" )\n";
        *out << off << "	DistanceDownPath ( "<<distanceDownPath[i]<<" )\n";
        *out << off << "	PlatformStartID ( "<<platformStartID[i]<<" )\n";
    }
    *out << off << ")\n";
}