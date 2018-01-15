/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "MstsSoundDefinition.h"
#include <QDebug>
#include <QFile>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"

int MstsSoundDefinition::jestsms = 0;
QMap<int, MstsSoundDefinition*> MstsSoundDefinition::Definitions;

MstsSoundDefinition::MstsSoundDefinition() {
}

MstsSoundDefinition::MstsSoundDefinition(QString p, QString n) {
    pathid = p;
    name = n;
    
    QString sh;
    pathid.replace("//", "/");
    qDebug() << "SMS" << pathid;
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
        if (sh == ("simisa@@@@@@@@@@jinx0x1t______")) {
            continue;
        }
        
        if (sh == ("tr_sms")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("scalabiltygroup")) {
                    group.push_back(new SoundDefinitionGroup(ParserX::GetNumber(data)));
                    group.back()->load(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#tr_sms - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
            continue;
            
            continue;
        }
        qDebug() << "#SMS - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
    delete data;
    loaded = 1;

    return;
}

QString SoundDefinitionGroup::Stream::Trigger::getFileName(){
    return Game::root+"/routes/"+Game::route+"/sound/"+playOneShotFiles.first();
}

void SoundDefinitionGroup::Stream::Trigger::load(FileBuffer* data){
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("delay_min_max")) {
            delayMin = ParserX::GetNumber(data);
            delayMax = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("volume_min_max")) {
            volumeMin = ParserX::GetNumber(data);
            volumeMax = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("selectionmethod")) {
            selectionMethod = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("playoneshot")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("file")) {
                    playOneShotFiles.push_back(ParserX::GetStringInside(data));
                    qDebug() << playOneShotFiles.back();
                    ParserX::SkipToken(data);
                    continue;
                }

                qDebug() << "#triggers - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }

            ParserX::SkipToken(data);
            continue;
        }

        qDebug() << "#Stream - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void SoundDefinitionGroup::Stream::load(FileBuffer* data){
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("priority")) {
            priority = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("triggers")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("random_trigger")) {
                    trigger.push_back(new Trigger());
                    trigger.back()->load(data);
                    //priority = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }

                qDebug() << "#triggers - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }

            ParserX::SkipToken(data);
            continue;
        }

        qDebug() << "#Stream - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

SoundDefinitionGroup::SoundDefinitionGroup(int l){
    level = l;
}

void SoundDefinitionGroup::load(FileBuffer* data){
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("activation")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("deactivation")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("volume")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("stereo")) {
            stereo = true;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ignore3d")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("streams")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("stream")) {
                    stream.push_back(new Stream());
                    stream.back()->load(data);
                    ParserX::SkipToken(data);
                    continue;
                }
            }
            ParserX::SkipToken(data);
            continue;
        }

        qDebug() << "#ScalabiltyGroup - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

MstsSoundDefinition::~MstsSoundDefinition() {
}

int MstsSoundDefinition::AddDefinition(QString pathid){
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    //console.log(pathid);
    QMapIterator<int, MstsSoundDefinition*> i(Definitions);
    while (i.hasNext()) {
    //for ( auto it = route->ref->refItems.begin(); it != route->ref->refItems.end(); ++it ){
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        i.next();
        if(i.value() == NULL) continue;
        if (i.value()->pathid.length() == pathid.length())
            if (i.value()->pathid == pathid) {
                i.value()->ref++;
                return (int)i.key();
            }
    }
    qDebug() << "Nowy " << jestsms << " sms: " << pathid;

    Definitions[jestsms] = new MstsSoundDefinition(pathid, pathid.split("/").last());
    Definitions[jestsms]->pathid = pathid;

    return jestsms++;
}