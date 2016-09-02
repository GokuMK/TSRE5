/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESUNDEFINED_H
#define	PROPERTIESUNDEFINED_H

#include <QtWidgets>
#include "PropertiesAbstract.h"

class WorldObj;

class PropertiesUndefined : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesUndefined();
    virtual ~PropertiesUndefined();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    
signals:
    
    
private:

};

#endif	/* PROPERTIESUNDEFINED_H */

