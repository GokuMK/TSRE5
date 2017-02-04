/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SoundList.h"
#include <QFile>
#include "ParserX.h"
#include <QDebug>
#include <cstdlib>
#include "Game.h"
#include "FileFunctions.h"
#include "ReadFile.h"

SoundListItem::SoundListItem() {
}

SoundListItem::~SoundListItem() {
}

void SoundList::loadSoundSources(QString path){
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    ParserX::NextLine(data);

    QString sh = "";
    SoundListItem* s;
    int id = 0;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("sound")) {
            s = new SoundListItem();
            ParserX::szukajsekcji1("name", data);
            s->name = ParserX::GetString(data);
            ParserX::szukajsekcji1("filename", data);
            s->file1 = ParserX::GetString(data);
            s->type = SoundListItem::Source;
            s->id = id++;
            sources[s->name] = s;
            ParserX::SkipToken(data);
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }

    if (sources.size() > 0)
        sourcesLoaded = true;
}
void SoundList::loadSoundRegions(QString path){
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    ParserX::NextLine(data);

    QString sh = "";
    SoundListItem* s;
    int id = 0;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("tracktype")) {
            s = new SoundListItem();
            s->name = ParserX::GetString(data);
            s->file1 = ParserX::GetString(data);
            s->file2 = ParserX::GetString(data);
            s->type = SoundListItem::Region;
            s->id = id++;
            regions[s->name] = s;
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }

    if (regions.size() > 0)
        regionsLoaded = true;
}