/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTLIB_H
#define	ACTLIB_H

#include <QHash>
#include <QString>

class Activity;
class Service;
class Traffic;
class Path;

class ActLib {
public:
    static int jestact;
    static int jestservice;
    static int jesttraffic;
    static int jestpath;
    static QHash<int, Activity*> Act;
    static QHash<int, Service*> Services;
    static QHash<int, Traffic*> Traffics;
    static QHash<int, Path*> Paths;
    static QHash<QString, QVector<int>> route;
    ActLib();
    virtual ~ActLib();
    static int GetAct(QString path, QString name);
    static int AddAct(QString path, QString name, bool nowe = false);
    static int AddService(QString path, QString name, bool nowe = false);
    static int AddTraffic(QString path, QString name, bool nowe = false);
    static Service* GetServiceByName(QString name);
    static Traffic* GetTrafficByName(QString name);
    static Path* GetPathByName(QString name);
    static bool IsServiceInUse(QString n);
    static bool IsTrafficInUse(QString n);
    static QVector<QString> GetServiceInUseList(QString n);
    static int AddPath(QString path, QString name);
    static int LoadAllAct(QString gameRoot, bool gui = false);
    static void GetUnsavedInfo(QVector<QString> &items);
    static void UpdateServiceChanges(QString serviceNameId);
    static void SaveAll();
private:

};

#endif	/* ACTLIB_H */

