/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef REF_H
#define	REF_H

#include <QString>
#include <QMap>

class Ref {
public:
    struct RefItem{
        QString type;
        QString clas;
        QString filename;
        QString align;
        QString description;
        long long int value = 0;
        unsigned int staticFlags = 0;
        QString getShapePath();
    };

    QMap<QString, QVector<RefItem>> refItems;
    
    Ref();
    Ref(QString path);
    virtual ~Ref();
    bool loaded;
    RefItem *selected = NULL;
    
private:
    
};

#endif	/* REF_H */

