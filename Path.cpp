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
#include "Ruch.h"
#include "Vector3f.h"


Path::Path() {
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
    
    Ruch ruch(tdb);
    Vector3f* pos;
    for (int i = 1; i <= tdb->iTRnodes; i++) {
        if (tdb->trackNodes[i] == NULL) continue;
        if (tdb->trackNodes[i]->typ == 0) {
            ruch.set(i);

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
            ruch.toNext(1);
            pos = ruch.getPosition();
            qDebug() << pos->x << " " << pos->y << " " << pos->z;
            out << "	TrackPDP ( "<<ruch.getAktTx()<<" "<<-ruch.getAktTz()<<" "<<pos->x<<" "<<pos->y<<" "<<-pos->z<<" 1 1 )" << "\n";
            ruch.toNext(10);
            pos = ruch.getPosition();
            qDebug() << pos->x << " " << pos->y << " " << pos->z;
            out << "	TrackPDP ( "<<ruch.getAktTx()<<" "<<-ruch.getAktTz()<<" "<<pos->x<<" "<<pos->y<<" "<<-pos->z<<" 1 1 )" << "\n";
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
