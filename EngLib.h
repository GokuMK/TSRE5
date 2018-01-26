/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ENGLIB_H
#define	ENGLIB_H

#include <unordered_map>
#include <QString>

class Eng;

class EngLib {
public:
    int jesteng = 0;
    std::unordered_map<int, Eng*> eng;
    EngLib();
    virtual ~EngLib();
    int addEng(QString path, QString name);
    int getEngByPathid(QString pathid);
    int getEngByPointer(Eng *pointer);
    int loadAll(QString gameRoot);
    int removeBroken();
    void removeAll();
private:

};

#endif	/* ENGLIB_H */

