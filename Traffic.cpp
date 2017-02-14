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
            displayName = ParserX::GetString(data);
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("serial")) {
                    serial = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("service_definition")) {
                    service.push_back(ServiceDefinition());
                    service.back().name = ParserX::GetString(data);
                    service.back().time = ParserX::GetNumber(data);
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
}