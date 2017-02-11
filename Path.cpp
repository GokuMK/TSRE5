/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Path.h"
#include "TRnode.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "Game.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "Ruch.h"
#include "Vector3f.h"
#include "ParserX.h"


Path::Path() {
}

Path::Path(QString p, QString n, bool nowe) {
    pathid = p + "/" + n;
    pathid.replace("//", "/");
    path = p;
    name = n;
    nameId = n.section(".", 0, -2);
    if(!nowe){
        loaded = -1;
        load();
    } else {
        loaded = 1;
        modified = true;
    }
}
bool Path::isModified(){
    return modified;
}
void Path::load(){
    QString sh;
    pathid.replace("//", "/");
    qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    data->skipBOM();

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("simisa@@@@@@@@@@jinx0p0t______")) {
            continue;
        }
        if (sh == ("serial")) {
            serial = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("trackpdps")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("trackpdp")) {
                    trackPdp.push_back(new float[7]);
                    for(int i = 0; i < 7; i++)
                        trackPdp.back()[i] = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#TrackPDPs - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("trackpath")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("trpathname")) {
                    trPathName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("name")) {
                    displayName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("trpathstart")) {
                    trPathStart = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("trpathend")) {
                    trPathEnd = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("trpathnodes")) {
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("trpathnode")) {
                            trPathNode.push_back(new unsigned int[4]);
                            trPathNode.back()[0] = ParserX::GetHex(data);
                            trPathNode.back()[1] = ParserX::GetUInt(data);
                            trPathNode.back()[2] = ParserX::GetUInt(data);
                            trPathNode.back()[3] = ParserX::GetUInt(data);
                            serial = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        qDebug() << "#TrPathNodes - undefined token: " << sh;
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#TrackPath - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#PAT - undefined token: " << sh;
        ParserX::SkipToken(data);
    }
}

Path::Path(const Path& orig) {
}

Path::~Path() {
}

void Path::CreatePaths(TDB * tdb){
    QString path;
    path = Game::root + "/routes/" + Game::route + "/paths";
    QDir dir(path);
    qDebug() << path;
    dir.setNameFilters(QStringList() << "*.pat");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        qDebug() << dirFile;
        if(!QDir(dirFile).exists())
            dir.remove(dirFile);
    }
    
    QFile file;
    QTextStream out;
    QString filepath;
    
    Ruch ruch;
    Vector3f* pos;
    for (int i = 1; i <= tdb->iTRnodes; i++) {
        if (tdb->trackNodes[i] == NULL) continue;
        if (tdb->trackNodes[i]->typ == 0) {
            //tdb->trackNodes[i]->TrPinS[0];
            ruch.set(i, 0, 0);

            filepath = path+"/"+QString::number(i,10)+".pat";
            file.setFileName(filepath);
            //qDebug() << filepath;
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            out.setDevice(&file);
            out.setRealNumberPrecision(8);
            out.setCodec("UTF-16");
            out.setGenerateByteOrderMark(true);

            out << "SIMISA@@@@@@@@@@JINX0P0t______" << "\n\n";
            out << "Serial ( 1 )" << "\n";
            out << "TrackPDPs (" << "\n";
            //ruch.toNext(1);
            //pos = ruch.getPosition();
            float *dPos = ruch.getCurrentPosition();
            Game::check_coords(dPos[5],dPos[6],dPos[0],dPos[2]);
            qDebug() << dPos[0]<<" "<<dPos[1]<<" "<<dPos[2];
            out << "	TrackPDP ( "<<dPos[5]<<" "<<dPos[6]<<" "<<dPos[0]<<" "<<dPos[1]<<" "<<dPos[2]<<" 1 1 )" << "\n";
            ruch.next(10);
            dPos = ruch.getCurrentPosition();
            Game::check_coords(dPos[5],dPos[6],dPos[0],dPos[2]);
            qDebug() << dPos[0]<<" "<<dPos[1]<<" "<<dPos[2];
            out << "	TrackPDP ( "<<dPos[5]<<" "<<dPos[6]<<" "<<dPos[0]<<" "<<dPos[1]<<" "<<dPos[2]<<" 1 1 )" << "\n";
            out << ")" << "\n";
            out << "TrackPath (" << "\n";
            out << "	TrPathName ( "<<i<<" )" << "\n";
            out << "	Name ( endNode_"<<i<<" )" << "\n";
            out << "	TrPathStart ( endNode_"<<i<<" )" << "\n";
            out << "	TrPathEnd ( endNode_"<<i<<" )" << "\n";
            out << "	TrPathNodes ( 2" << "\n";
            out << "		TrPathNode ( 00000000 1 4294967295 0 )" << "\n";
            out << "		TrPathNode ( 00000000 4294967295 4294967295 1 )" << "\n";
            out << "	)" << "\n";
            out << ")" << "\n";
            
            out.flush();
            file.close();
        }
        
    }
}
