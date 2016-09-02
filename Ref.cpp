/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Ref.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"

Ref::Ref(QString path) {
    loaded = false;

    qDebug() << "Wczytywanie pliku ref: " << path;

    int x, i, j, ii, jj, uu;
    float xx;
    int t;
    QString sh;
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);

    ParserX::NextLine(data);

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("include")) {
            QString incPath = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            data->insertFile(path + "/" + incPath);
            continue;
        }
        if (sh == ("skip")) {
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("comment")) {
            ParserX::SkipToken(data);
            continue;
        }
        RefItem item;
        item.type = sh;
        while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
            //qDebug() << sh;
            if (sh == ("class")) {
                item.clas = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("filename")) {
                item.filename = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("align")) {
                item.align = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("description")) {
                item.description = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
        }
        if (item.clas != "") {
            refItems[item.clas.trimmed().toStdString()].push_back(item);
        }
        ParserX::SkipToken(data);
    }

    //for ( auto it = refItems.begin(); it != refItems.end(); ++it ){
    //    qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
    //  //std::cout << " " << it->first << ":" << it->second;
    //}
    selected = NULL;
}

Ref::Ref() {
    loaded = false;
}

Ref::~Ref() {
}

