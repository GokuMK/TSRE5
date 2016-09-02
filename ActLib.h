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

#include <unordered_map>
#include <QString>

class Activity;

class ActLib {
public:
    static int jestact;
    static std::unordered_map<int, Activity*> act;
    static std::unordered_map<std::string, std::vector<int>> route;
    ActLib();
    virtual ~ActLib();
    static int addAct(QString path, QString name);
    static int loadAll(QString gameRoot);
private:

};

#endif	/* ACTLIB_H */

