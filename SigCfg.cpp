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
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* bufor = ReadFile::read(file);
    bufor->off += 46;
    //szukanie trackdb

    //int iiTRitems = (int) ParserX::parsujr(bufor); //odczytanie ilosci sciezek

    //TRitem* nowy; // = new TRitem();
    //nowy = new TRitem();
    //nowy->titLoading = true;
    qDebug() << "sigcfg!";
    int iSignalType = 0;
    int iSignalShape = 0;
    while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
        if (sh == "") {
            break;
        }
        if (sh == "signaltypes") {
            SignalType* nowySt;
            while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "signaltype") {
                    nowySt = new SignalType();
                    nowySt->type = ParserX::odczytajtc(bufor);

                    while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
                        nowySt->set(sh, bufor);
                        ParserX::pominsekcje(bufor);
                    }
                    //this->signalType[iSignalType++] = nowySt;
                    iSignalType++;
                    this->signalType[nowySt->type.toStdString()] = nowySt;
                }
                ParserX::pominsekcje(bufor);
                continue;
            }
            ParserX::pominsekcje(bufor);
            continue;
        }
        if (sh == "signalshapes") {
            SignalShape* nowySs;
            while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "signalshape") {
                    nowySs = new SignalShape();
                    nowySs->name = ParserX::odczytajtc(bufor);
                    nowySs->desc = ParserX::odczytajtc(bufor);

                    while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
                        nowySs->set(sh, bufor);
                        ParserX::pominsekcje(bufor);
                    }
                    //this->signalShape[iSignalShape++] = nowySs;
                    iSignalShape++;
                    this->signalShape[nowySs->name.toStdString()] = nowySs;
                }
                
                ParserX::pominsekcje(bufor);
                continue;
            }
            ParserX::pominsekcje(bufor);
            continue;
        }
        qDebug() << sh;
        ParserX::pominsekcje(bufor);
        continue;
    }

    return;
}

SigCfg::~SigCfg() {
}

