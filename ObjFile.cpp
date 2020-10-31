/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ObjFile.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "Vector3f.h"
#include "Vector2f.h"

ObjFile::ObjFile(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    qDebug() << path;

    QTextStream in(&file);
    QString line;
    QStringList args;
    QString val;
    
    QVector<Vector3f> v;
    QVector<Vector2f> vt;
    QVector<Vector3f> vn;
    
    while (!in.atEnd()) {
        line = in.readLine();
        if(line.startsWith("#", Qt::CaseInsensitive)) 
            continue;
        args = line.split(" ");
        if(args.size() == 0)
            continue;
        //qDebug() << args[0] << args.size();
        if(args[0] == "v" && args.size() == 4){
            v.push_back(Vector3f(args[1].toFloat(), args[2].toFloat(), args[3].toFloat() ));
        }
        if(args[0] == "vt" && args.size() == 3){
            if(args[2].toFloat() < texYmin)
                texYmin = args[2].toFloat();
            if(args[2].toFloat() > texYmax)
                texYmax = args[2].toFloat();
            vt.push_back(Vector2f(args[1].toFloat(), args[2].toFloat()));
        }
        if(args[0] == "vn" && args.size() == 4){
            vn.push_back(Vector3f(args[1].toFloat(), args[2].toFloat(), args[3].toFloat() ));
        }
        if(args[0] == "f" && args.size() == 4){
            for(int i = 1; i < 4; i++ ){
                QStringList fargs = args[i].split("/");
                points.push_back(v[fargs[0].toInt()-1].x);
                points.push_back(v[fargs[0].toInt()-1].y);
                points.push_back(v[fargs[0].toInt()-1].z);
                
                points.push_back(vn[fargs[2].toInt()-1].x);
                points.push_back(vn[fargs[2].toInt()-1].y);
                points.push_back(vn[fargs[2].toInt()-1].z);
                
                points.push_back(vt[fargs[1].toInt()-1].x);
                points.push_back(vt[fargs[1].toInt()-1].y);
                
                count++;
            }
            
        }
    }
    qDebug() << "points.size"<<points.size();
}

