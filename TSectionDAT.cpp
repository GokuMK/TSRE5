/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TSectionDAT.h"
#include "Game.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include <QFile>
#include <QTextStream>

TSectionDAT::TSectionDAT(bool autoFix, bool loadRouteNow) {
    loadGlobal();
    if(loadRouteNow)
        loadRoute(autoFix);
}

TSectionDAT::TSectionDAT(const TSectionDAT& orig) {
}

TSectionDAT::~TSectionDAT() {
}

bool TSectionDAT::loadGlobal() {
    // Wczytaj główne tsection
    QString sh;
    QString path = Game::root + "/global/tsection.dat";
    QString orpath = Game::root + "/routes/" + Game::route + "/openrails/tsection.dat";
    QString incpath;
    path.replace("//", "/");
    orpath.replace("//", "/");

    QFile *file = new QFile(orpath);
    if (!file->open(QIODevice::ReadOnly)){
        incpath = path.toLower();
        file->close();
        file = new QFile(path);
        if (!file->open(QIODevice::ReadOnly)){
            qDebug() << path << "not exist";
            return false;
        } else {
            qDebug() << path;
            incpath = Game::root + "/global";
        }
    } else {
        qDebug() << orpath;
        incpath = Game::root + "/routes/" + Game::route + "/openrails";
    }

    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    data->skipBOM();

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("simisa@@@@@@@@@@jinx0f0t______")) {
            continue;
        }
        if (sh == ("include")) {
            QString incPath = ParserX::GetStringInside(data).toLower();
            ParserX::SkipToken(data);
            data->insertFile(incpath + "/" + incPath);
            continue;
        }
        if (sh == ("tracksections")) {
            //if (tsectionMaxIdx == 0)
            tsectionMaxIdx = ParserX::GetNumber(data);
            int index = 0;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == "tracksection") {
                    index = (int) ParserX::GetNumber(data);
                    //System.out.println("jest "+index);
                    sekcja[index] = new TSection(index);
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == "sectioncurve") {
                            sekcja[index]->type = 1;
                            sekcja[index]->radius = ParserX::GetNumber(data);
                            sekcja[index]->angle = (float) (ParserX::GetNumber(data) * M_PI / 180);
                            //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).radius+" "+sekcja.get(index).angle);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == "sectionsize") {
                            sekcja[index]->val1 = (int) ParserX::GetNumber(data);
                            sekcja[index]->size = ParserX::GetNumber(data);
                            //qDebug() << sekcja[index]->id<<" "<<sekcja[index]->size;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "_info") {
                    data->off += 2;
                }
                if (sh == "_skip") {
                    data->off += 2;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh.toLower() == "trackshapes") {
            //if (tsectionShapes == 0)
            tsectionShapes = ParserX::GetNumber(data);
            int t = 0;
            int sectionN;
            TrackShape *shp;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //nowy->set(sh, data);
                //qDebug() << sh;
                if (sh == "trackshape") {
                    t = ParserX::GetNumber(data);
                    //qDebug() << t;
                    shape[t] = new TrackShape(t);
                    shp = shape[t];
                    sectionN = 0;
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        //nowy->set(sh, data);
                        if (sh == ("filename")) {
                            shp->filename = ParserX::GetString(data);
                        } else if (sh == ("numpaths")) {
                            shp->numpaths = ParserX::GetNumber(data);
                            shp->path = new TrackShape::SectionIdx[shp->numpaths];
                        } else if (sh == ("sectionidx")) {
                            shp->path[sectionN].n = ParserX::GetNumber(data);
                            shp->path[sectionN].pos[0] = ParserX::GetNumber(data);
                            shp->path[sectionN].pos[1] = ParserX::GetNumber(data);
                            shp->path[sectionN].pos[2] = ParserX::GetNumber(data);
                            shp->path[sectionN].rotDeg = ParserX::GetNumber(data);
                            for (int i = 0; i < shp->path[sectionN].n; i++) {
                                shp->path[sectionN].sect[i] = ParserX::GetNumber(data);
                            }
                            sectionN++;
                        } else if (sh == ("mainroute")) {
                            shp->mainroute = ParserX::GetNumber(data);
                        } else if (sh == ("clearancedist")) {
                            shp->clearancedist = true;
                        } else if (sh == ("crossovershape")) {
                            shp->crossovershape = true;
                        } else if (sh == ("xoverpts")) {
                            shp->xoverpts = ParserX::GetNumber(data);
                            shp->xoverpt = new float[shp->xoverpts * 3];
                            for (int i = 0; i < shp->xoverpts; i++) {
                                shp->xoverpt[i * 3 + 0] = ParserX::GetNumber(data);
                                shp->xoverpt[i * 3 + 1] = ParserX::GetNumber(data);
                                shp->xoverpt[i * 3 + 2] = ParserX::GetNumber(data);
                            }
                            ParserX::SkipToken(data);
                        } else if (sh == ("tunnelshape")) {
                            shp->tunnelshape = true;
                        } else if (sh == ("roadshape")) {
                            shp->roadshape = true;
                        } else if (sh == ("manualjunctionshape")) {
                            shp->manualjunctionshape = true;
                        } else {
                            qDebug() << sh;
                        }
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "_info") {
                    data->off += 2;
                }
                if (sh == "_skip") {
                    data->off += 2;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "_info") {
            data->off += 2;
        }
        if (sh == "_skip") {
            data->off += 2;
        }
        ParserX::SkipToken(data);
    }

    qDebug() << "TsectionDAT: " << tsectionMaxIdx << " " << tsectionShapes;
    routeMaxIdx = tsectionMaxIdx;
    routeShapes = tsectionShapes;
    return true;
}

void TSectionDAT::mergeTSection(TSectionDAT* second, QHash<unsigned int,unsigned int>& fixedSectionIds, QHash<unsigned int,unsigned int>& fixedShapeIds){
    if (second->routeMaxIdx < 3) return;
    qDebug() <<"1";
    int routeCount = routeMaxIdx;
    for (int i = second->tsectionMaxIdx; i < second->routeMaxIdx; i++) {
        if (second->sekcja[i] == NULL) 
            continue;
        unsigned int foundIdx = -1;
        for(int j = tsectionMaxIdx; j < routeCount; j++ ){
            if (sekcja[j] == NULL) 
                continue;
            if(sekcja[j]->getHash() == second->sekcja[i]->getHash()){
                foundIdx = j;
                break;
            }
        }
        if(foundIdx > -1){
            fixedSectionIds[i] = foundIdx;
        } else {
            second->sekcja[i]->id = routeMaxIdx;
            sekcja[routeMaxIdx] = second->sekcja[i];
            fixedSectionIds[i] = routeMaxIdx++;
        }
    }
    qDebug() <<"2";
    for (int i = second->tsectionShapes; i < second->routeShapes; i++) {
        if (second->shape[i] == NULL)
            continue;
        if (second->shape[i]->numpaths != 1)
            continue;
        for(int j = 0; j < second->shape[i]->path[0].n; j++){
            second->shape[i]->path[0].sect[j] = fixedSectionIds[second->shape[i]->path[0].sect[j]];
        }
        shape[routeShapes] = second->shape[i];
        fixedShapeIds[i] = routeShapes;
        routeShapes++;
    }
}

bool TSectionDAT::saveRoute() {

    if (this->routeMaxIdx < 3) return true;

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
    
    saveRouteToStream(out);

    file.close();
    return true;
}

void TSectionDAT::saveRouteToStream(QTextStream &out){
    out << "TrackSections ( " << this->routeMaxIdx - tsectionMaxIdx << "\n";
    for (int i = tsectionMaxIdx; i < this->routeMaxIdx; i++) {
        if (this->sekcja[i] != NULL) {
            out << "	TrackSection ( \n";
            if (sekcja[i]->type == 0)
                out << "		SectionCurve ( " << sekcja[i]->type << " ) " << i << " " << sekcja[i]->size << " " << sekcja[i]->val1 << " \n";
            else
                out << "		SectionCurve ( " << sekcja[i]->type << " ) " << i << " " << sekcja[i]->angle << " " << sekcja[i]->radius << " \n";
            out << "	)\n";
        }
    }
    out << ")\n";
    //return true;
    out << "SectionIdx ( " << this->routeShapes - tsectionShapes << "\n";
    for (int i = tsectionShapes; i<this->routeShapes; i++) {
        if (this->shape[i] != NULL) {
            out << "	TrackPath ( " << i << " " << shape[i]->path[0].n;
            for (int j = 0; j < shape[i]->path[0].n; j++)
                out << " " << shape[i]->path[0].sect[j];
            out << " ) \n";
        }
    }
    out << ")";
}

bool TSectionDAT::loadRoute(bool autoFix) {

    QString path;
    path = Game::root + "/routes/" + Game::route + "/tsection.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    FileBuffer* data = ReadFile::read(&file);
    ParserX::NextLine(data);
    
    loadRouteUtf16Data(data, autoFix);
    
    this->routeMaxIdx += 2 - this->routeMaxIdx % 2;
    this->routeShapes++;
    if(autoFix)
        updateSectionDataRequired = true;
    //saveRoute();
    return true;
}


void TSectionDAT::loadRouteUtf16Data(FileBuffer* data, bool autoFix){
    int index = 0;
    QString sh;
    int newIdx = 0, newSdx = 0;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == "tracksections") {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //qDebug() << sh;
                //if (sh.toLower() =="sectionidx") break;
                if (sh.toLower() == "tracksection") {
                    //    bufor->get();
                    //    bufor->get();
                    //    continue;
                    //}
                    //if (sh.toLower() =="sectioncurve") {
                    int typ = (int) ParserX::GetNumber(data);
                    index = (int) ParserX::GetNumber(data);
                    if (index < this->tsectionMaxIdx){
                        if(autoFix){
                            autoFixedSectionIds[index] = this->tsectionMaxIdx + newIdx;
                            index = this->tsectionMaxIdx + newIdx;
                            newIdx++;
                        } else {
                            dataOutOfSync = true;
                        }
                    }
                    if (index > this->routeMaxIdx)
                        this->routeMaxIdx = index;                    
                    sekcja[index] = new TSection(index);
                    sekcja[index]->type = typ;
                    if (typ == 0) {
                        sekcja[index]->size = ParserX::GetNumber(data);
                        sekcja[index]->val1 = ParserX::GetNumber(data);
                        //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).size);
                    } else {
                        sekcja[index]->angle = ParserX::GetNumber(data);
                        sekcja[index]->radius = ParserX::GetNumber(data);
                        //System.out.println(sekcja.get(index).id+" "+sekcja.get(index).radius+""+sekcja.get(index).angle);
                    }
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "sectionidx") {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh.toLower() == "trackpath") {
                    //   qDebug() << (int) ParserX::GetNumber(bufor);
                    index = (int) ParserX::GetNumber(data);
                    if (index < this->tsectionShapes){
                        if(autoFix){
                            autoFixedShapeIds[index] = this->tsectionShapes + newSdx;
                            index = this->tsectionShapes + newSdx;
                            newSdx++;
                        } else {
                            dataOutOfSync = true;
                        }
                    }
                    if (index > this->routeShapes)
                        this->routeShapes = index;                    
                    shape[index] = new TrackShape(index);
                    shape[index]->dyntrack = true;
                    shape[index]->numpaths = 1;
                    shape[index]->path = new TrackShape::SectionIdx[1];
                    //this->routeShape[index] = new TrackShape::SectionIdx;
                    shape[index]->path[0].n = ParserX::GetNumber(data);
                    shape[index]->path[0].pos[0] = 0;
                    shape[index]->path[0].pos[1] = 0;
                    shape[index]->path[0].pos[2] = 0;
                    for (int i = 0; i < shape[index]->path[0].n; i++)
                        shape[index]->path[0].sect[i] = ParserX::GetNumber(data);
                    
                    if(autoFix){
                        for (int i = 0; i < shape[index]->path[0].n; i++){
                            int spid = shape[index]->path[0].sect[i];
                            if(autoFixedSectionIds[spid] != 0)
                                shape[index]->path[0].sect[i] = autoFixedSectionIds[spid];
                        }
                    }
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#RouteTsectionDat - undefined token " << sh;
        ParserX::SkipToken(data);
    }
}

bool TSectionDAT::isRoadShape(int id) {
    if (shape[id] == NULL) return false;
    return shape[id]->roadshape;
}

void TSectionDAT::getShapeData(int id) {
    if (shape[id] == NULL) return;



}