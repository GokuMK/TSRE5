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
    data->off = 48;

    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

        if (sh == ("include")) {
            QString incPath = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            data->insertFile(path + "/" + incPath);
            continue;
        }
        if (sh == ("tr_activity")) {
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                if (sh == ("include")) {
                    QString incPath = ParserX::odczytajtcInside(data);
                    ParserX::pominsekcje(data);
                    data->insertFile(path + "/" + incPath);
                    continue;
                }
                if (sh == ("serial")) {
                    serial = ParserX::parsujr(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("tr_activity_header")) {
                    loadActivityHeader(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("tr_activity_file")) {

                    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                        if (sh == ("player_service_definition")) {
                            playerServiceDefinition = new ServiceDefinition();
                            playerServiceDefinition->name = ParserX::odczytajtcInside(data);
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                                if (sh == ("player_traffic_definition")) {
                                    playerServiceDefinition->trafficDefinition = new TrafficDefinition();
                                    playerServiceDefinition->trafficDefinition->load(data);
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                playerServiceDefinition->load(sh, data);
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("nextserviceuid")) {
                            nextServiceUID = ParserX::parsujr(data);
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("nextactivityobjectuid")) {
                            nextActivityObjectUID = ParserX::parsujr(data);
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("traffic_definition")) {
                            traffic = new Traffic();
                            traffic->name = ParserX::odczytajtcInside(data);
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                                if (sh == ("service_definition")) {
                                    traffic->service.emplace_back();
                                    traffic->service.back().name = ParserX::odczytajtcInside(data);
                                    traffic->service.back().path = ParserX::parsujr(data);
                                    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                                        traffic->service.back().load(sh, data);
                                        ParserX::pominsekcje(data);
                                        continue;
                                    }
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                playerServiceDefinition->load(sh, data);
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("events")) {
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                                if (sh == ("eventcategoryaction")) {
                                    event.emplace_back();
                                    event.back().category = 0;
                                    event.back().load(data);
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                if (sh == ("eventcategorylocation")) {
                                    event.emplace_back();
                                    event.back().category = 1;
                                    event.back().load(data);
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                if (sh == ("eventcategorytime")) {
                                    event.emplace_back();
                                    event.back().category = 2;
                                    event.back().load(data);
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                qDebug() << "#events - undefined token: " << sh;
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("platformnumpassengerswaiting")) {
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                                if (sh == ("platformdata")) {
                                    platformNumPassengersWaiting.emplace_back(std::make_pair(ParserX::parsujr(data), ParserX::parsujr(data)));
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                qDebug() << "#platformnumpassengerswaiting - undefined token: " << sh;
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("activityobjects")) {
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                                if (sh == ("activityobject")) {
                                    loadActivityObject(data);
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                qDebug() << "#activityobjects - undefined token: " << sh;
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("activityrestrictedspeedzones")) {
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                                if (sh == ("activityrestrictedspeedzone")) {
                                    restrictedSpeedZone.emplace_back();
                                    restrictedSpeedZone.back().load(data);
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                qDebug() << "#activityrestrictedspeedzones - undefined token: " << sh;
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("activityfailedsignals")) {
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                                if (sh == ("activityfailedsignal")) {
                                    activityFailedSignal.emplace_back(ParserX::parsujr(data));
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                qDebug() << "#activityfailedsignals - undefined token: " << sh;
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        qDebug() << "#tr_activity_file - undefined token: " << sh;
                        ParserX::pominsekcje(data);
                        continue;
                    }
                    ParserX::pominsekcje(data);
                    continue;
                }
                qDebug() << "#tr_activity - undefined token: " << sh;
                ParserX::pominsekcje(data);
                continue;
            }
            ParserX::pominsekcje(data);
            continue;
        }
        qDebug() << "#ACT - undefined token: " << sh;
        ParserX::pominsekcje(data);
        continue;
    }
    delete data;
    loaded = 1;

    return;
}

void Activity::TrafficDefinition::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

        if (sh == ("arrivaltime")) {
            arrivalTime.emplace_back(ParserX::parsujr(data));
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("departtime")) {
            departTime.emplace_back(ParserX::parsujr(data));
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("skipcount")) {
            skipCount.emplace_back(ParserX::parsujr(data));
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("distancedownpath")) {
            distanceDownPath.emplace_back(ParserX::parsujr(data));
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("platformstartid")) {
            platformStartID.emplace_back(ParserX::parsujr(data));
            ParserX::pominsekcje(data);
            continue;
        }
        qDebug() << "#trafficDefinition - undefined token: " << sh;
        ParserX::pominsekcje(data);
        continue;
    }
}

void Activity::Event::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        if (sh == ("eventtypetime")) {
            eventType = Event::EventTypeTime;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypelocation")) {
            eventType = Event::EventTypeLocation;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypeallstops")) {
            eventType = Event::EventTypeAllstops;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypepickupwagons")) {
            eventType = Event::EventTypePickupWagons;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypeassembletrain")) {
            eventType = Event::EventTypeAssembleTrain;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypeassembletrainatlocation")) {
            eventType = Event::EventTypeAssembleTrainAtLocation;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypedropoffwagonsatlocation")) {
            eventType = Event::EventTypeDropoffWagonsAtLocation;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypepickuppassengers")) {
            eventType = Event::EventTypePickupPassengers;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("eventtypereachspeed")) {
            eventType = Event::EventTypeReachSpeed;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("reversable_event")) {
            reversableEvent = true;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("id")) {
            id = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("activation_level")) {
            activationLevel = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("outcomes")) {
            outcome = new Outcome();
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == ("activitysuccess")) {
                    outcome->activitysuccess = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("displaymessage")) {
                    outcome->displayMessage = ParserX::odczytajtcInside(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("incactlevel")) {
                    outcome->incactlevel = ParserX::parsujr(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("decactlevel")) {
                    outcome->decactlevel = ParserX::parsujr(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("activateevent")) {
                    outcome->activateevent = ParserX::parsujr(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("activityfail")) {
                    outcome->activityfail = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("startignoringspeedlimits")) {
                    outcome->startignoringspeedlimits = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                qDebug() << "#event outcomes - undefined token: " << sh;
                ParserX::pominsekcje(data);
                continue;
            }
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("name")) {
            name = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("time")) {
            time = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("location")) {
            loaction = new float[5];
            loaction[0] = ParserX::parsujr(data);
            loaction[1] = ParserX::parsujr(data);
            loaction[2] = ParserX::parsujr(data);
            loaction[3] = ParserX::parsujr(data);
            loaction[4] = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("triggeronstop")) {
            triggerOnStop = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("sidingitem")) {
            sidingItem = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("speed")) {
            speed = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("stationstop")) {
            stationStop = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("wagon_list")) {
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == ("uid")) {
                    wagonListId.emplace_back(ParserX::parsujr(data));
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("sidingitem")) {
                    wagonListSidingItem.emplace_back(ParserX::parsujr(data));
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("description")) {
                    wagonListDescription.emplace_back(ParserX::odczytajtcInside(data).toStdString());
                    ParserX::pominsekcje(data);
                    continue;
                }
                qDebug() << "#event wagonlist - undefined token: " << sh;
                ParserX::pominsekcje(data);
                continue;
            }
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("texttodisplayoncompletioniftriggered")) {
            textToDisplayOnCompletionIfTriggered = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("texttodisplayoncompletionifnottriggered")) {
            textToDisplayOnCompletionIfNotTriggered = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        qDebug() << "#event - undefined token: " << sh;
        ParserX::pominsekcje(data);
        continue;
    }
}

void Activity::RestrictedSpeedZone::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        if (sh == ("startposition")) {
            startPosition[0] = ParserX::parsujr(data);
            startPosition[1] = ParserX::parsujr(data);
            startPosition[2] = ParserX::parsujr(data);
            startPosition[3] = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("endposition")) {
            endPosition[0] = ParserX::parsujr(data);
            endPosition[1] = ParserX::parsujr(data);
            endPosition[2] = ParserX::parsujr(data);
            endPosition[3] = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        qDebug() << "#restrictedSpeedZone - undefined token: " << sh;
        ParserX::pominsekcje(data);
        continue;
    }
}

void Activity::ServiceDefinition::load(QString sh, FileBuffer* data) {
    if (sh == ("uid")) {
        this->uid = ParserX::parsujr(data);
        return;
    }
    if (sh == ("efficiency")) {
        this->efficiency.emplace_back(ParserX::parsujr(data));
        return;
    }
    if (sh == ("skipcount")) {
        this->skipCount.emplace_back(ParserX::parsujr(data));
        return;
    }
    if (sh == ("distancedownpath")) {
        this->distanceDownPath.emplace_back(ParserX::parsujr(data));
        return;
    }
    if (sh == ("platformstartid")) {
        this->platformStartId.emplace_back(ParserX::parsujr(data));
        return;
    }
    qDebug() << "#serviceDefinition - undefined token: " << sh;
    return;
}

void Activity::loadActivityHeader(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << "--"<< sh;
        if (sh == ("routeid")) {
            this->hrouteid = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("name")) {
            this->hname = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("description")) {
            this->hdescription = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("briefing")) {
            this->hbriefing = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("completeactivity")) {
            completeActivity = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("type")) {
            type = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("mode")) {
            mode = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("starttime")) {
            startTime[0] = ParserX::parsujr(data);
            startTime[1] = ParserX::parsujr(data);
            startTime[2] = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("season")) {
            season = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("weather")) {
            weather = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("pathid")) {
            this->hpathid = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("startingspeed")) {
            startingSpeed = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("duration")) {
            duration[0] = ParserX::parsujr(data);
            duration[1] = ParserX::parsujr(data);
            duration[2] = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("difficulty")) {
            difficulty = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("animals")) {
            animals = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("workers")) {
            workers = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("fuelwater")) {
            fuelWater = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("fuelcoal")) {
            fuelCoal = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("fueldiesel")) {
            fuelDiesel = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("voltage")) {
            voltage = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        qDebug() << "#activityHeader - undefined token: " << sh;
        ParserX::pominsekcje(data);
        continue;
    }
}

void Activity::loadActivityObject(FileBuffer* data) {
    QString sh;
    activityObjects.emplace_back();
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

        if (sh == ("objecttype")) {
            activityObjects.back().objectType = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("train_config")) {
            activityObjects.back().con = new Consist();
            if (activityObjects.back().con->load(data)) {
                activityObjects.back().con->loaded = 1;
                activityObjects.back().con->initPos();
            }
            ParserX::pominsekcje(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("direction")) {
            activityObjects.back().direction = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("id")) {
            activityObjects.back().id = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("tile")) {
            activityObjects.back().tile[0] = ParserX::parsujr(data);
            activityObjects.back().tile[1] = ParserX::parsujr(data);
            activityObjects.back().tile[2] = ParserX::parsujr(data);
            activityObjects.back().tile[3] = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        qDebug() << "#activityObject - undefined token: " << sh;
        ParserX::pominsekcje(data);
        continue;
    }
}