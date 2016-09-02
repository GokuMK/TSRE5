/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CONLIB_H
#define	CONLIB_H

#include <unordered_map>
#include <QString>

class Consist;

class ConLib {
public:
    static int jestcon;
    static std::unordered_map<int, Consist*> con;
    ConLib();
    virtual ~ConLib();
    static int addCon(QString path, QString name);
    static int loadAll(QString gameRoot);
private:

};
#endif	/* CONLIB_H */

