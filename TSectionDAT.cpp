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
    bufor->off = 0;

    //szukanie TrackSections
    sh = "TrackSections";
    ParserX::szukajsekcji1(sh, bufor);

    int index = 0;
    
    for (;;) {
        sh = ParserX::nazwasekcji(bufor);
        //qDebug() << sh;
        if (sh.toLower() == "trackshapes") break;
        if (sh.toLower() == "tracksection") {
            index = (int) ParserX::parsujr(bufor);
            //System.out.println("jest "+index);
            sekcja[index] = new TSection(index);
            //qDebug() << index;
            continue;
        }
        if (sh.toLower() == "sectionsize") {
            sekcja[index]->val1 = (int) ParserX::parsujr(bufor);
            sekcja[index]->size = ParserX::parsujr(bufor);
            //qDebug() << sekcja[index]->id<<" "<<sekcja[index]->size;
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
    int t=0;
    int sectionN;
    TrackShape *shp;
    if(sh.toLower() == "trackshapes" ){
        while (!((sh = ParserX::nazwasekcji(bufor).toLower()) == "")) {
            //nowy->set(sh, data);
            //qDebug() << sh;
            if (sh =="trackshape") {
                t = ParserX::parsujr(bufor);
                shape[t] = new TrackShape(t);
                shp = shape[t];
                sectionN = 0;
                while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
                    //nowy->set(sh, data);
                    if (sh == ("filename")) {
                        shp->filename = ParserX::odczytajtc(bufor);
                    } else if (sh == ("numpaths")) {
                        shp->numpaths = ParserX::parsujr(bufor);
                        shp->path = new TrackShape::SectionIdx[shp->numpaths];
                    } else if (sh == ("sectionidx")) {
                        shp->path[sectionN].n = ParserX::parsujr(bufor);
                        shp->path[sectionN].pos[0] = ParserX::parsujr(bufor);
                        shp->path[sectionN].pos[1] = ParserX::parsujr(bufor);
                        shp->path[sectionN].pos[2] = ParserX::parsujr(bufor);
                        shp->path[sectionN].rotDeg = ParserX::parsujr(bufor);
                        for(int i = 0; i < shp->path[sectionN].n; i++){
                            shp->path[sectionN].sect[i] = ParserX::parsujr(bufor);
                        }
                        sectionN++;
                    } else if (sh == ("mainroute")) {
                        shp->mainroute = ParserX::parsujr(bufor);
                    } else if (sh == ("clearancedist")) {
                        shp->clearancedist = true;
                    } else if (sh == ("crossovershape")) {
                        shp->crossovershape = true;
                    } else if (sh == ("xoverpts")) {
                        shp->xoverpts = ParserX::parsujr(bufor);
                        shp->xoverpt = new float[shp->xoverpts*3];
                        for(int i = 0; i < shp->xoverpts; i++){
                            shp->xoverpt[i*3+0] = ParserX::parsujr(bufor);
                            shp->xoverpt[i*3+1] = ParserX::parsujr(bufor);
                            shp->xoverpt[i*3+2] = ParserX::parsujr(bufor);
                        }
                        ParserX::pominsekcje(bufor);
                    } else if (sh == ("tunnelshape")) {
                        shp->tunnelshape = true;
                    } else if (sh == ("roadshape")) {
                        shp->roadshape = true;
                    } else if (sh == ("manualjunctionshape")) {
                        shp->manualjunctionshape = true;
                    } else {
                        qDebug() << sh;
                    }
                    ParserX::pominsekcje(bufor);
                }
            }
            if (sh.toLower() =="_info") {
                bufor->off+=2;
            }
            if (sh.toLower() =="_skip") {
                bufor->off+=2;
            }
            ParserX::pominsekcje(bufor);
        }
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