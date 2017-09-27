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

ActivityTimetable::ActivityTimetable() {
}

ActivityTimetable::ActivityTimetable(const ActivityTimetable& orig) {
}

ActivityTimetable::~ActivityTimetable() {
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

    if(name.length() < 2)
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