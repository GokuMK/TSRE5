/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PATH_H
#define	PATH_H

#include "TDB.h"

class Path {
public:
    QString displayName;
    QString name;
    QString nameId;
    QString path;
    QString pathid;  
    QString pathId;
    QString trPathName;
    QString trPathStart;
    QString trPathEnd;
    unsigned int trPathFlags;
    
    QVector<float*> trackPdp;
    QVector<unsigned int*> trPathNode;
    
    Path();
    Path(QString p, QString n, bool nowe = false);
    Path(const Path& orig);
    virtual ~Path();
    void load();
    bool isModified();
    static void CreatePaths(TDB * tdb);
private:
    int loaded;
    bool modified = false;
    int serial = -1;
};

#endif	/* PATH_H */

