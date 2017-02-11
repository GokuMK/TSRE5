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
    
}

void Service::save(){
    
}

bool Service::isModified(){
    return modified;
}

void Service::setNameId(QString val){
    nameId = val;
    name = val+".srv";
}
