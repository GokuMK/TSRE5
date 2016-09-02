/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

/* 
 * File:   Ruch.h
 * Author: Goku
 *
 * Created on 13 maja 2015, 23:57
 */

#ifndef RUCH_H
#define	RUCH_H

#include <QString>
#include "TDB.h"
#include "Vector3f.h"
#include "Vector2f.h"

class Ruch {
public:
    Ruch(TDB *t);
    void toNext(float metry);
    void set(int at);
    Vector3f* getPosition();
    int getAktTx();
    int getAktTz();
private:
    TDB *trackDB;
    int aktt = -1;
    int idx;
    int kierunek;
    bool rozjazd = false;
    float metry = 0;
    float metrpp = 1;
    int u = 0, i = 0, akticz = 0;
    Vector3f pozW;
    Vector3f pozT;
    Vector3f pozO;
    bool next();
};

#endif	/* RUCH_H */

