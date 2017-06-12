/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef OBJFILE_H
#define	OBJFILE_H

#include <QString>
#include <QVector>

class ObjFile {
public:

    ObjFile(QString path);
    QVector<float> points;
    int count = 0;
    float texYmin = 999;
    float texYmax = -1;
private:

};

#endif	/* OBJFILE_H */

