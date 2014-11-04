#include "TSectionDAT.h"
#include "Game.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"

TSectionDAT::TSectionDAT() {
    loadGlobal();
    loadRoute();
}

TSectionDAT::TSectionDAT(const TSectionDAT& orig) {
}

TSectionDAT::~TSectionDAT() {
}

bool TSectionDAT::loadGlobal() {
    // Wczytaj główne tsection
    QString sh;
    QString path;
    path = Game::root + "/global/tsection.dat";
    path.replace("//","/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return false;
    FileBuffer* bufor = ReadFile::read(file);

    //szukanie TrackSections
    sh = "TrackSections";
    ParserX::szukajsekcji1(sh, bufor);

    int index = 0;
    bufor->off = 0;
    for (;;) {
        sh = ParserX::nazwasekcji(bufor);
        //qDebug() << sh;
        if (sh.toLower() == "trackshapes") break;
        if (sh.toLower() == "tracksection") {
            index = (int) ParserX::parsujr(bufor);
            //System.out.println("jest "+index);
            sekcja[index] = new TSection(index);
            continue;
        }
        if (sh.toLower() == "sectionsize") {
            sekcja[index]->val1 = (int) ParserX::parsujr(bufor);
            sekcja[index]->size = ParserX::parsujr(bufor);
            //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).size);
            //Parse.pominsekcje(bufor); 
        }
        if (sh.toLower() =="_info") {
            bufor->get();
            bufor->get();
        }
        if (sh.toLower() =="sectioncurve") {
            sekcja[index]->type = 1;
            sekcja[index]->radius = ParserX::parsujr(bufor);
            sekcja[index]->angle = (float) (ParserX::parsujr(bufor) * M_PI / 180);
            //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).radius+" "+sekcja.get(index).angle);
            ParserX::pominsekcje(bufor);
        }
        ParserX::pominsekcje(bufor);
    }
    return true;
}

bool TSectionDAT::loadRoute() {

    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/tsection.dat";
    path.replace("//","/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return false;
    FileBuffer* bufor = ReadFile::read(file);

    //szukanie TrackSections
    sh = "TrackSections";
    ParserX::szukajsekcji1(sh, bufor);

    int index = 0;

    for (;;) {
        sh = ParserX::nazwasekcji(bufor);
        //qDebug() << sh;
        if (sh.toLower() =="sectionidx") break;
        if (sh.toLower() =="tracksection") {
            bufor->get();
            bufor->get();
            continue;
        }
        if (sh.toLower() =="sectioncurve") {
            int typ = (int) ParserX::parsujr(bufor);
            index = (int) ParserX::parsujr(bufor);
            sekcja[index] = new TSection(index);
            sekcja[index]->type = typ;
            if (typ == 0) {
                sekcja[index]->size = ParserX::parsujr(bufor);
                sekcja[index]->val1 = ParserX::parsujr(bufor);
                //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).size);
            } else {
                sekcja[index]->angle = ParserX::parsujr(bufor);
                sekcja[index]->radius = ParserX::parsujr(bufor);
                //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).radius+""+sekcja.get(index).angle);
            }
        }
        ParserX::pominsekcje(bufor);
    }
    return true;
}