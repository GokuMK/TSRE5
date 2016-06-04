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
    tsectionMaxIdx = ParserX::GetNumber(bufor);
    int index = 0;
    
    for (;;) {
        sh = ParserX::nazwasekcji(bufor);
        //qDebug() << sh;
        if (sh.toLower() == "trackshapes") break;
        if (sh.toLower() == "tracksection") {
            index = (int) ParserX::GetNumber(bufor);
            //System.out.println("jest "+index);
            sekcja[index] = new TSection(index);
            //qDebug() << index;
            continue;
        }
        if (sh.toLower() == "sectionsize") {
            sekcja[index]->val1 = (int) ParserX::GetNumber(bufor);
            sekcja[index]->size = ParserX::GetNumber(bufor);
            //qDebug() << sekcja[index]->id<<" "<<sekcja[index]->size;
            //Parse.SkipToken(bufor); 
        }
        if (sh.toLower() =="_info") {
            bufor->get();
            bufor->get();
        }
        if (sh.toLower() =="sectioncurve") {
            sekcja[index]->type = 1;
            sekcja[index]->radius = ParserX::GetNumber(bufor);
            sekcja[index]->angle = (float) (ParserX::GetNumber(bufor) * M_PI / 180);
            //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).radius+" "+sekcja.get(index).angle);
            ParserX::SkipToken(bufor);
        }
        ParserX::SkipToken(bufor);
    }
    int t=0;
    int sectionN;
    TrackShape *shp;
    if(sh.toLower() == "trackshapes" ){
        tsectionShapes = ParserX::GetNumber(bufor);
        while (!((sh = ParserX::nazwasekcji(bufor).toLower()) == "")) {
            //nowy->set(sh, data);
            //qDebug() << sh;
            if (sh =="trackshape") {
                t = ParserX::GetNumber(bufor);
                shape[t] = new TrackShape(t);
                shp = shape[t];
                sectionN = 0;
                while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                    //nowy->set(sh, data);
                    if (sh == ("filename")) {
                        shp->filename = ParserX::GetString(bufor);
                    } else if (sh == ("numpaths")) {
                        shp->numpaths = ParserX::GetNumber(bufor);
                        shp->path = new TrackShape::SectionIdx[shp->numpaths];
                    } else if (sh == ("sectionidx")) {
                        shp->path[sectionN].n = ParserX::GetNumber(bufor);
                        shp->path[sectionN].pos[0] = ParserX::GetNumber(bufor);
                        shp->path[sectionN].pos[1] = ParserX::GetNumber(bufor);
                        shp->path[sectionN].pos[2] = ParserX::GetNumber(bufor);
                        shp->path[sectionN].rotDeg = ParserX::GetNumber(bufor);
                        for(int i = 0; i < shp->path[sectionN].n; i++){
                            shp->path[sectionN].sect[i] = ParserX::GetNumber(bufor);
                        }
                        sectionN++;
                    } else if (sh == ("mainroute")) {
                        shp->mainroute = ParserX::GetNumber(bufor);
                    } else if (sh == ("clearancedist")) {
                        shp->clearancedist = true;
                    } else if (sh == ("crossovershape")) {
                        shp->crossovershape = true;
                    } else if (sh == ("xoverpts")) {
                        shp->xoverpts = ParserX::GetNumber(bufor);
                        shp->xoverpt = new float[shp->xoverpts*3];
                        for(int i = 0; i < shp->xoverpts; i++){
                            shp->xoverpt[i*3+0] = ParserX::GetNumber(bufor);
                            shp->xoverpt[i*3+1] = ParserX::GetNumber(bufor);
                            shp->xoverpt[i*3+2] = ParserX::GetNumber(bufor);
                        }
                        ParserX::SkipToken(bufor);
                    } else if (sh == ("tunnelshape")) {
                        shp->tunnelshape = true;
                    } else if (sh == ("roadshape")) {
                        shp->roadshape = true;
                    } else if (sh == ("manualjunctionshape")) {
                        shp->manualjunctionshape = true;
                    } else {
                        qDebug() << sh;
                    }
                    ParserX::SkipToken(bufor);
                }
            }
            if (sh.toLower() =="_info") {
                bufor->off+=2;
            }
            if (sh.toLower() =="_skip") {
                bufor->off+=2;
            }
            ParserX::SkipToken(bufor);
        }
    }
    qDebug() << "TsectionDAT: "<<tsectionMaxIdx<<" "<<tsectionShapes;
    routeMaxIdx = tsectionMaxIdx;
    routeShapes = tsectionShapes;
    return true;
}

bool TSectionDAT::saveRoute() {
    
    if(this->routeMaxIdx < 3) return true;
    
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/tsection.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setRealNumberPrecision(6);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0T0t______\n\n";
    out << "TrackSections ( "<<this->routeMaxIdx-tsectionMaxIdx<<"\n";
    for(int i = tsectionMaxIdx; i< this->routeMaxIdx; i++){
        if(this->sekcja[i] != NULL){
            out << "	TrackSection ( \n";
            if (sekcja[i]->type == 0)
                out << "		SectionCurve ( "<<sekcja[i]->type<<" ) "<<i<<" "<<sekcja[i]->size<<" "<<sekcja[i]->val1<<" \n";
            else
                out << "		SectionCurve ( "<<sekcja[i]->type<<" ) "<<i<<" "<<sekcja[i]->angle<<" "<<sekcja[i]->radius<<" \n";
            out << "	)\n";
        }
    }
    out << ")\n";
    //return true;
    out << "SectionIdx ( "<<this->routeShapes - tsectionShapes<<"\n";
    for(int i = tsectionShapes; i<this->routeShapes; i++){
        if(this->shape[i] != NULL){
            out << "	TrackPath ( "<<i<<" "<<shape[i]->path[0].n;
            for(int j = 0; j<shape[i]->path[0].n; j++)
                out <<" "<<shape[i]->path[0].sect[j];
            out <<" ) \n";
        }
    }
    out << ")";
    
    file.close();
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

    //for (;;) {
    //this->routeMaxIdx = 0;
    //this->routeShapes = 0;
    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
    //    sh = ParserX::nazwasekcji(bufor);
        //qDebug() << sh;
        //if (sh.toLower() =="sectionidx") break;
        if (sh.toLower() =="tracksection") {
        //    bufor->get();
        //    bufor->get();
        //    continue;
        //}
        //if (sh.toLower() =="sectioncurve") {
            int typ = (int) ParserX::GetNumber(bufor);
            index = (int) ParserX::GetNumber(bufor);
            if(index > this->routeMaxIdx)
                this->routeMaxIdx = index;
            sekcja[index] = new TSection(index);
            sekcja[index]->type = typ;
            if (typ == 0) {
                sekcja[index]->size = ParserX::GetNumber(bufor);
                sekcja[index]->val1 = ParserX::GetNumber(bufor);
                //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).size);
            } else {
                sekcja[index]->angle = ParserX::GetNumber(bufor);
                sekcja[index]->radius = ParserX::GetNumber(bufor);
                //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).radius+""+sekcja.get(index).angle);
            }
        }
        ParserX::SkipToken(bufor);
    }
    this->routeMaxIdx += 2 - this->routeMaxIdx%2;
    
    sh = "SectionIdx";
    ParserX::szukajsekcji1(sh, bufor);
    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
        if (sh.toLower() =="trackpath") {
            //   qDebug() << (int) ParserX::GetNumber(bufor);
            index = (int) ParserX::GetNumber(bufor);
            if(index > this->routeShapes)
                this->routeShapes = index;
            shape[index] = new TrackShape(index);
            shape[index]->dyntrack = true;
            shape[index]->numpaths = 1;
            shape[index]->path = new TrackShape::SectionIdx[1];
            //this->routeShape[index] = new TrackShape::SectionIdx;
            shape[index]->path[0].n = ParserX::GetNumber(bufor);
            shape[index]->path[0].pos[0] = 0;
            shape[index]->path[0].pos[1] = 0;
            shape[index]->path[0].pos[2] = 0;
            for(int i = 0; i < shape[index]->path[0].n; i++)
                shape[index]->path[0].sect[i] = ParserX::GetNumber(bufor);
        }
        ParserX::SkipToken(bufor);
    }
    this->routeShapes++;
    //saveRoute();
    return true;
}

bool TSectionDAT::isRoadShape(int id){
    if(shape[id] == NULL) return false;
    return shape[id]->roadshape;
}

void TSectionDAT::getShapeData(int id){
    if(shape[id] == NULL) return;
    
    

}