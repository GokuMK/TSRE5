/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <unordered_map>
#include <QString>
#include "Texture.h"

#ifndef TEXLIB_H
#define	TEXLIB_H

class TexLib {
public:
    TexLib();
    TexLib(const TexLib& orig);
    virtual ~TexLib();
    static int jesttextur;
    static std::unordered_map<int, Texture*> mtex;
    static void reset();
    static void delRef(int texx);
    static void addRef(int texx);
    static int addTex(QString path, QString name);
    static int addTex(QString pathid);
    static int getTex(QString pathid);
    static int cloneTex(int id);
    static void save(QString type, QString path, int id);
private:

};

#endif	/* TEXLIB_H */

