/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SPEEDPOSTDAT_H
#define	SPEEDPOSTDAT_H

#include <QString>
#include <QVector>
//#include "SpeedPost.h"
class SpeedPost;

class SpeedPostDAT {
public:
    QString speed_Warning_Sign_Shape;
    QString restricted_Shape;
    QString end_Restricted_Shape;
    
    QVector<SpeedPost*> speedPost;
    
    SpeedPostDAT();
    virtual ~SpeedPostDAT();
private:

};

#endif	/* SPEEDPOSTDAT_H */

