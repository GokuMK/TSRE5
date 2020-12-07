/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TrackShape.h"
#include "ParserX.h"
#include <QTextStream>
#include "FileBuffer.h"

TrackShape::TrackShape() {
    
}

TrackShape::TrackShape(int t) {
    this->id = t;
}

QString TrackShape::getHashString(){
    if(filename.length() > 0)
        return filename;
    
    QString hash;
    for(int i = 0; i < numpaths; i++){
        for(int j = 0; j < path[i].n; j++)
        hash += QString::number(path[i].sect[j], 16);
    }
    return hash;
}

TrackShape::~TrackShape() {
}

void TrackShape::saveToStream(QTextStream &out){
    out << "TrackShape (\n";
            out << "	TrackPath ( " << id << " " << path[0].n;
            for (int j = 0; j < path[0].n; j++)
                out << " " << path[0].sect[j];
            out << " ) \n";
}

void TrackShape::loadUtf16Data(FileBuffer *data){
    QString sh;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh.toLower() == "trackpath") {
                    id = (int) ParserX::GetNumber(data);
                    dyntrack = true;
                    numpaths = 1;
                    path = new TrackShape::SectionIdx[1];
                    path[0].n = ParserX::GetNumber(data);
                    path[0].pos[0] = 0;
                    path[0].pos[1] = 0;
                    path[0].pos[2] = 0;
                    for (int i = 0; i < path[0].n; i++)
                        path[0].sect[i] = ParserX::GetNumber(data);
                }
                ParserX::SkipToken(data);
            }
}