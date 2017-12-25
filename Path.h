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


#include <QString>
#include "GLUU.h"
#include "GameObj.h"
#include "TDB.h"

class OglObj;

class Path : public GameObj {
    Q_OBJECT
public:
    static void CreatePaths(TDB * tdb);
    
    struct PathNode {
        unsigned int flags;
        unsigned int next;
        unsigned int next2;
        int tilex;
        int tilez;
        float pos[3];
        unsigned int flag1;
        unsigned int flag2;
    };
    
    struct PathObject {
        QString name;
        int trItemId = -1;
        float distanceDownPath;
    };

    QVector<PathObject*> pathObjects;
    
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
    int ref = 0;
    
    QVector<float*> trackPdp;
    QVector<unsigned int*> trPathNode;
    QVector<PathNode> node;
    
    Path();
    Path(QString p, QString n, bool nowe = false);
    Path(const Path& orig);
    virtual ~Path();
    void load();
    float* getStartPositionTXZ(float *out = NULL);
    void initRoute();
    void init3dShapes(bool initShapes = true);
    bool isModified();
    void render(GLUU* gluu, float * playerT, int selectionColor);
private:
    int loaded;
    bool modified = false;
    bool isinit1 = false;
    bool isinit2 = false;
    int serial = -1;
    TrackItemObj* pointer3d = NULL;
    QVector<OglObj*> lines;
    QVector<float> linesX;
    QVector<float> linesZ;
    QMap<float, PathObject*> pathObjectsMap;
};

#endif	/* PATH_H */

