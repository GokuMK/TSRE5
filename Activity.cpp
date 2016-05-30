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
    int i;
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

                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("tr_activity_header")) {

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

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("type")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("mode")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("starttime")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("season")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("weather")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("pathid")) {
                            this->hpathid = ParserX::odczytajtcInside(data);
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("startingspeed")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("duration")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("difficulty")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("animals")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("workers")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("fuelwater")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("fuelcoal")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("fueldiesel")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("voltage")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        qDebug() << "--" << sh;
                        ParserX::pominsekcje(data);
                        continue;
                    }
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("tr_activity_file")) {

                    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                        if (sh == ("player_service_definition")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("nextserviceuid")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("nextactivityobjectuid")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("traffic_definition")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("events")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("platformnumpassengerswaiting")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("activityobjects")) {
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {

                                if (sh == ("activityobject")) {
                                    loadActivityobject(data);
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                qDebug() << "---" << sh;
                                ParserX::pominsekcje(data);
                                continue;
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("activityrestrictedspeedzones")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("activityfailedsignals")) {

                            ParserX::pominsekcje(data);
                            continue;
                        }
                        qDebug() << "--" << sh;
                        ParserX::pominsekcje(data);
                        continue;
                    }
                    ParserX::pominsekcje(data);
                    continue;
                }
                qDebug() << "-" << sh;
                ParserX::pominsekcje(data);
                continue;
            }
            ParserX::pominsekcje(data);
            continue;
        }
        qDebug() << "" << sh;
        ParserX::pominsekcje(data);
        continue;
    }
    delete data;
    loaded = 1;

    return;
}

void Activity::loadActivityobject(FileBuffer* data) {
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
            if(activityObjects.back().con->load(data)){
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
        qDebug() << "----" << sh;
        ParserX::pominsekcje(data);
        continue;
    }
}