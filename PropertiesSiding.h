/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESSIDING_H
#define	PROPERTIESSIDING_H

#include "PropertiesAbstract.h"

class PlatformObj;

class PropertiesSiding : public PropertiesAbstract{
    Q_OBJECT 
public:
    PropertiesSiding();
    virtual ~PropertiesSiding();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    
public slots:
    void disablePlatformEnabled(int state);
    void namePlatformEnabled(QString val);
        
signals:
    
private:
    PlatformObj* pobj = NULL;
    QCheckBox disablePlatform;
    QLineEdit namePlatform;
    QLineEdit lengthPlatform;
};

#endif	/* PROPERTIESSIDING_H */

