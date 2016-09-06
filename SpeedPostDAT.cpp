/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SpeedPostDAT.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "SpeedPost.h"
#include "Game.h"

SpeedPostDAT::SpeedPostDAT() {
    QString sh;
    QString path = Game::root + "/routes/" + Game::route + "/speedpost.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* bufor = ReadFile::read(file);
    bufor->off += 46+16;

    qDebug() << "speedpost!";
    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
        if (sh == "speed_warning_sign_shape") {
            this->speed_Warning_Sign_Shape = ParserX::GetString(bufor);
            ParserX::SkipToken(bufor);
            continue;
        }
        if (sh == "restricted_shape") {
            this->restricted_Shape = ParserX::GetString(bufor);
            ParserX::SkipToken(bufor);
            continue;
        }
        if (sh == "end_restricted_shape") {
            this->end_Restricted_Shape = ParserX::GetString(bufor);
            ParserX::SkipToken(bufor);
            continue;
        }
        if (sh == "speedpost_set") {
            SpeedPost* spost = new SpeedPost();
            this->speedPost.push_back(spost);
            while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                spost->set(sh, bufor);
                ParserX::SkipToken(bufor);
                continue;
            }
            ParserX::SkipToken(bufor);
            continue;
        }
        qDebug() << sh;
        ParserX::SkipToken(bufor);
        continue;
    }
    return;
}

SpeedPostDAT::~SpeedPostDAT() {
}

