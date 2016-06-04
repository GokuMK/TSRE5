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

    //int iiTRitems = (int) ParserX::GetNumber(bufor); //odczytanie ilosci sciezek

    //TRitem* nowy; // = new TRitem();
    //nowy = new TRitem();
    //nowy->titLoading = true;
    qDebug() << "sigcfg!";
    int iSignalType = 0;
    int iSignalShape = 0;
    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
        if (sh == "") {
            break;
        }
        if (sh == "signaltypes") {
            SignalType* nowySt;
            while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "signaltype") {
                    nowySt = new SignalType();
                    nowySt->type = ParserX::GetString(bufor);

                    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                        nowySt->set(sh, bufor);
                        ParserX::SkipToken(bufor);
                    }
                    //this->signalType[iSignalType++] = nowySt;
                    iSignalType++;
                    this->signalType[nowySt->type.toStdString()] = nowySt;
                }
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
                if (sh == "signalshape") {
                    nowySs = new SignalShape();
                    nowySs->name = ParserX::GetString(bufor);
                    nowySs->desc = ParserX::GetString(bufor);

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
                    this->signalShape[nowySs->name.toStdString()] = nowySs;
                    this->signalShapeById[iSignalShape] = nowySs;
                    iSignalShape++;
                }
                
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

SigCfg::~SigCfg() {
}

