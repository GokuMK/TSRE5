/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRAFFIC_H
#define	TRAFFIC_H

#include <QString>
#include <QVector>

class ActivityTimetable;

class Traffic {
public:
    
    QVector<ActivityTimetable*> service;
    QString displayName;
    QString name;
    QString nameId;
    QString path;
    QString pathid;  
    int loaded = -1;
    int ref = 0;
    
    Traffic(QString p, QString n, bool nowe = false);
    Traffic(const Traffic& orig);
    virtual ~Traffic();
    void load();
    bool isModified();
    ActivityTimetable* getTimetableByServiceName(QString nameTime);
private:
    bool modified = false;
    int serial = -1;
};

#endif	/* TRAFFIC_H */

