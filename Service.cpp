/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#include "Service.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "Game.h"
#include <QDebug>
#include <QFile>
#include "GLUU.h"
#include "TDB.h"
#include "AboutWindow.h"
#include "ActLib.h"
#include "ConLib.h"
#include "Consist.h"
#include "Path.h"

Service::Service(QString p, QString n, bool nowe) {
    pathid = p + "/" + n;
    pathid.replace("//", "/");
    path = p;
    name = n;
    nameId = n.section(".", 0, -2);
    if(!nowe){
        loaded = -1;
        load();
    } else {
        maxWheelAcceleration = 0;
        efficiency = 0.75;
        startingSpeed = 0;
        endingSpeed = 0;
        startInWorld = 0;
        endInWorld = 0;
        loaded = 1;
        serial = 1;
        displayName = nameId;
        modified = true;
    }
}

Service::Service(const Service& orig) {
}

Service::~Service() {
}

void Service::load(){
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
    data->toUtf16();
    data->skipBOM();

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("simisa@@@@@@@@@@jinx0v0t______")) {
            continue;
        }
        if (sh == ("service_definition")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("serial")) {
                    serial = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("name")) {
                    displayName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("train_config")) {
                    trainConfig = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("pathid")) {
                    pathId = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("maxwheelacceleration")) {
                    maxWheelAcceleration = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("efficiency")) {
                    efficiency = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("timetable")) {
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("startingspeed")) {
                            startingSpeed = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("endingspeed")) {
                            endingSpeed = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("startinworld")) {
                            startInWorld = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("endinworld")) {
                            endInWorld = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("stationstop")) {
                            stationStop.push_back(StationStop());
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if (sh == ("platformstartid")) {
                                    stationStop.back().platformStartID = ParserX::GetNumber(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if (sh == ("distancedownpath")) {
                                    stationStop.back().distanceDownPath = ParserX::GetNumber(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if (sh == ("skipcount")) {
                                    stationStop.back().skipCount = ParserX::GetNumber(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#stationstop - undefined token: " << sh;
                                ParserX::SkipToken(data);
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        qDebug() << "#timetable - undefined token: " << sh;
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#service_definition - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#SRV - undefined token: " << sh;
        ParserX::SkipToken(data);
    }
    loaded = 1;
}

void Service::save(){
    QString tpath;
    
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    tpath = path+"/"+name;
    tpath.replace("//", "/");
    qDebug() << tpath;
    QFile file(tpath);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    
    out << "SIMISA@@@@@@@@@@JINX0v0t______\n";
    out << "\n";
    out << "Service_Definition (\n";
    if (serial >= 0) {
        out << "	Serial ( " << serial << " )\n";
    }
    out << "	Name ( "<<ParserX::AddComIfReq(displayName)<<" )\n";
    out << "	Train_Config ( "<<ParserX::AddComIfReq(this->trainConfig)<<" )\n";
    out << "	PathID ( "<<ParserX::AddComIfReq(this->pathId)<<" )\n";
    out << "	MaxWheelAcceleration ( "<<this->maxWheelAcceleration<<" )\n";
    out << "	Efficiency ( "<<this->efficiency<<" )\n";
    out << "	TimeTable (\n";
    out << "		StartingSpeed ( "<<this->startingSpeed<<" )\n";
    out << "		EndingSpeed ( "<<this->endingSpeed<<" )\n";
    out << "		StartInWorld ( "<<this->startInWorld<<" )\n";
    out << "		EndInWorld ( "<<this->endInWorld<<" )\n";
    for(int i = 0; i < stationStop.size(); i++){
        out << "		StationStop (\n";
        out << "			PlatformStartID ( "<<stationStop[i].platformStartID<<" )\n";
        out << "			DistanceDownPath ( "<<stationStop[i].distanceDownPath<<" )\n";
        out << "			SkipCount ( "<<stationStop[i].skipCount<<" )\n";
        out << "		)\n";
    }
    out << "	)\n";
    out << ")\n";
    
    file.close();
    modified = false;
}

void Service::render(GLUU* gluu, float* playerT, int selectionColor){
    initToPlay();
    
    pathPointer->render(gluu, playerT, selectionColor);
    
    conPointer->renderOnTrack(gluu, playerT, selectionColor);
}

void Service::initToPlay(){
    if(pathPointer == NULL){
        QDir dir(Game::root + "/routes/" + Game::route + "/paths");
        int pathPointerId;
        qDebug() << "pathid" << (pathPointerId = ActLib::AddPath(dir.path(), pathId+".pat"));
        pathPointer = ActLib::Paths[pathPointerId];
    }
    if(conPointer == NULL){
        QDir dir(Game::root + "/trains/consists/");
        int conPointerId;
        qDebug() << "conid" << (conPointerId =  ConLib::addCon(dir.path(), trainConfig+".con"));
        conPointer = new Consist(ConLib::con[conPointerId], true);
        conPointer->initOnTrack(pathPointer->getStartPositionTXZ(), pathPointer->getStartDirection(), pathPointer->getJunctionDirections());
    }
}

Consist *Service::getConsistPointer(){
    return conPointer;
}
void Service::updateSim(float* playerT, float deltaTime){
    if(pathPointer == NULL || conPointer == NULL){
        return;
    }
    conPointer->updateSim(deltaTime);
}

bool Service::isModified(){
    return modified;
}

void Service::setNameId(QString val){
    nameId = val;
    name = val+".srv";
    modified = true;
}

void Service::setNewPath(QString pathName){
    stationStop.clear();
    pathId = pathName;
    if(pathId.length() <= 0)
        return;
    qDebug() << "new path:" << pathId;
    Path *p = ActLib::GetPathByName(pathId);
    if(p == NULL){
        qDebug() << "null path";
        return;
    }
    p->init3dShapes(false);

    int ii = 0;
    foreach(Path::PathObject* i, p->pathObjects){
        if(i == NULL)
            continue;
        stationStop.push_back(StationStop());
        stationStop.back().platformStartID = i->trItemId;
        stationStop.back().distanceDownPath = i->distanceDownPath;
        stationStop.back().skipCount = ii++;
    }
    modified = true;
    ActLib::UpdateServiceChanges(nameId);
}

void Service::disableStationStop(int count){
    for(int ii = 0; ii < stationStop.size(); ii++ ){
        if(stationStop[ii].skipCount == count){
            stationStop.remove(ii);
            ActLib::UpdateServiceChanges(nameId);
            return;
        }
    }
    modified = true;
}

void Service::setDisplayName(QString val){
    displayName = val;
    modified = true;
}
void Service::setEfficiency(float val){
    efficiency = val;
    modified = true;
}

void Service::setStartSpeed(float val){
    startingSpeed = val;
    modified = true;
}
    
void Service::setEndSpeed(float val){
    endingSpeed = val;
    modified = true;
}
    
void Service::enableStationStop(int count){
    for(int i = 0; i < stationStop.size(); i++ ){
        if(stationStop[i].skipCount == count){
            return;            
        }
    }
    Path *p = ActLib::GetPathByName(pathId);
    if(p == NULL){
        return;
    }
    p->init3dShapes(false);
    if(p->pathObjects.size() < count)
        return;
    
    float dist = p->pathObjects[count]->distanceDownPath;
    int i;
    for(i = 0; i < stationStop.size(); i++ ){
        if(stationStop[i].distanceDownPath > dist){
            break;            
        }
    }
    stationStop.insert(i, StationStop());
    stationStop[i].distanceDownPath = p->pathObjects[count]->distanceDownPath;
    stationStop[i].platformStartID = p->pathObjects[count]->trItemId;
    stationStop[i].skipCount = count;
    
    ActLib::UpdateServiceChanges(nameId);
    modified = true;
}