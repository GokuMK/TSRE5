/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SigCfg.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "SignalType.h"
#include "SignalShape.h"
#include "Game.h"

SigCfg::SigCfg() {
    QString sh;
    QString path = Game::root + "/routes/" + Game::route + "/sigcfg.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* bufor = ReadFile::read(&file);
    bufor->off += 46+16;
    //szukanie sigcfg

    qDebug() << "sigcfg!";
    int iSignalType = 0;
    int iSignalShape = 0;
    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
        if (sh == "") {
            break;
        }
        if (sh == "_info" || sh == "skip" || sh == "_skip") {
            ParserX::SkipToken(bufor);
            continue;
        }
        if (sh == "lighttextures") {
            while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "_info" || sh == "skip" || sh == "_skip") {
                    ParserX::SkipToken(bufor);
                    continue;
                }
                if (sh == "lighttex") {
                    /*nowySt = new SignalType();
                    nowySt->type = ParserX::GetString(bufor);

                    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                        nowySt->set(sh, bufor);
                        ParserX::SkipToken(bufor);
                    }
                    //this->signalType[iSignalType++] = nowySt;
                    iSignalType++;
                    this->signalType[nowySt->type.toStdString()] = nowySt;*/
                    ParserX::SkipToken(bufor);
                    continue;
                }
                qDebug() << "#lighttextures - undefined token: "<<sh;
                ParserX::SkipToken(bufor);
                continue;
            }
            ParserX::SkipToken(bufor);
            continue;
        }
        if (sh == "lightstab") {
            while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "_info" || sh == "skip" || sh == "_skip") {
                    ParserX::SkipToken(bufor);
                    continue;
                }
                if (sh == "lightstabentry") {
                    /*nowySt = new SignalType();
                    nowySt->type = ParserX::GetString(bufor);

                    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                        nowySt->set(sh, bufor);
                        ParserX::SkipToken(bufor);
                    }
                    //this->signalType[iSignalType++] = nowySt;
                    iSignalType++;
                    this->signalType[nowySt->type.toStdString()] = nowySt;*/
                    ParserX::SkipToken(bufor);
                    continue;
                }
                qDebug() << "#lightstab - undefined token: "<<sh;
                ParserX::SkipToken(bufor);
                continue;
            }
            ParserX::SkipToken(bufor);
            continue;
        }
        if (sh == "signaltypes") {
            SignalType* nowySt;
            while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "_info" || sh == "skip" || sh == "_skip") {
                    ParserX::SkipToken(bufor);
                    continue;
                }
                if (sh == "signaltype") {
                    nowySt = new SignalType();
                    nowySt->type = ParserX::GetString(bufor);
                    //    qDebug() << nowySt->type;
                    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                        nowySt->set(sh, bufor);
                        ParserX::SkipToken(bufor);
                    }
                    //this->signalType[iSignalType++] = nowySt;
                    iSignalType++;
                    this->signalType[nowySt->type] = nowySt;
                    ParserX::SkipToken(bufor);
                    continue;
                }
                qDebug() << "#signaltypes - undefined token: "<<sh;
                ParserX::SkipToken(bufor);
                continue;
            }
            ParserX::SkipToken(bufor);
            continue;
        }
        if (sh == "signalshapes") {
            SignalShape* nowySs;
            while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "_info" || sh == "skip" || sh == "_skip") {
                    ParserX::SkipToken(bufor);
                    continue;
                }
                if (sh == "signalshape") {
                    nowySs = new SignalShape();
                    nowySs->name = ParserX::GetString(bufor);
                    nowySs->desc = ParserX::GetString(bufor);
                    //qDebug() << "- " << nowySs->name;
                    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                        nowySs->set(sh, bufor);
                        ParserX::SkipToken(bufor);
                    }
                    //this->signalShape[iSignalShape++] = nowySs;
                    //for(int i = 0; i < nowySs->iSubObj; i++){
                    //    if(nowySs->subObj[i].optional){
                    //        qDebug() << nowySs->subObj[i].sigSubSType;
                    //    }
                    //}
                    nowySs->listId = iSignalShape;
                    this->signalShape[nowySs->name] = nowySs;
                    this->signalShapeById[iSignalShape] = nowySs;
                    iSignalShape++;
                    ParserX::SkipToken(bufor);
                    continue;
                }
                qDebug() << "#signalshapes - undefined token: "<<sh;
                ParserX::SkipToken(bufor);
                continue;
            }
            ParserX::SkipToken(bufor);
            continue;
        }
        qDebug() << "#sigcfg.dat - undefined token: "<<sh;
        ParserX::SkipToken(bufor);
        continue;
    }

    return;
}

SigCfg::~SigCfg() {
}

