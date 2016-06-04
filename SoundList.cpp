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
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
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
            sources[s->name.toStdString()] = s;
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
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
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
            regions[s->name.toStdString()] = s;
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }

    if (regions.size() > 0)
        regionsLoaded = true;
}