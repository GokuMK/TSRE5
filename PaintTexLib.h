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
 * File:   PaintTexLib.h
 * Author: Goku
 *
 * Created on 14 maja 2015, 22:23
 */

#ifndef PAINTTEXLIB_H
#define	PAINTTEXLIB_H

#include <QThread>
#include "Texture.h"

class PaintTexLib// : public QThread
 {
 //    Q_OBJECT

public:
    Texture* texture;
    void run();
private:
    
protected:
     
};

#endif	/* PAINTTEXLIB_H */

