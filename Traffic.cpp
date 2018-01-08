/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Traffic.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "Game.h"
#include <QDebug>
#include <QFile>
#include "GLUU.h"
#include "TDB.h"
#include "ActivityTimetable.h"

Traffic::Traffic(QString p, QString n, bool nowe) {
    pathid = p + "/" + n;
    pathid.replace("//", "/");
    path = p;
    name = n;
    nameId = n.section(".", 0, -2);
    if(!nowe){
        loaded = -1;
        load();
    } else {
        loaded = 1;
        serial = 1;
        modified = true;
    }
}

Traffic::Traffic(const Traffic& orig) {
}

Traffic::~Traffic() {
}

void Traffic::load(){
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
        //qDebug() << sh;
        if (sh == ("simisa@@@@@@@@@@jinx0f0t______")) {
            continue;
        }
        if (sh == ("traffic_definition")) {
            nameId = ParserX::GetString(data);
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("serial")) {
                    serial = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("service_definition")) {
                    service.push_back(new ActivityTimetable());
                    service.back()->load(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#traffic_definition - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#TRF - undefined token: " << sh;
        ParserX::SkipToken(data);
    }
    loaded = 1;
}

ActivityTimetable* Traffic::getTimetableByServiceName(QString nameTime){
    for(int i = 0; i < service.size(); i++){
        //qDebug() << nameTime << service[i]->nameTime;
        if(service[i]->nameTime == nameTime)
            return service[i];
    }
    return NULL;
}

bool Traffic::isModified(){
    for(int i = 0; i < service.size(); i++){
        if(service[i]->isModified())
            return true;
    }
    return modified;
}

void Traffic::save(){
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
    
    out << "SIMISA@@@@@@@@@@JINX0f0t______\n";
    out << "\n";
    out << "Traffic_Definition ( " << nameId << "\n";
    if (serial >= 0) {
        out << "	Serial ( " << serial << " )\n";
    }
    QString off = "	";
    for(int i = 0; i < service.size(); i++){
        if(!service[i]->empty)
            service[i]->save(&out, off);
    }
    out << ")\n";
    
    file.close();
    modified = false;
}