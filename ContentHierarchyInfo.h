/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CONTENTHIERARCHYINFO_H
#define	CONTENTHIERARCHYINFO_H

#include <QString>
#include <QVector>

class Consist;
class Eng;
class SFile;

class ContentHierarchyInfo {
public:
    QString name;
    int parent = -1;
    int distanceLevelId = -1;
    QString type;
    Consist* con = NULL;
    Eng* eng = NULL;
    SFile* sfile = NULL;
    
    ContentHierarchyInfo();
    ContentHierarchyInfo(const ContentHierarchyInfo& orig);
    virtual ~ContentHierarchyInfo();
private:

};

#endif	/* CONTENTHIERARCHYINFO_H */

