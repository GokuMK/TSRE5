/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESPLATFORM_H
#define	PROPERTIESPLATFORM_H

#include "PropertiesAbstract.h"

class PlatformObj;

class PropertiesPlatform : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesPlatform();
    virtual ~PropertiesPlatform();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void leftSideEnabled(int state);
    void rightSideEnabled(int state);
    void disablePlatformEnabled(int state);
    void nameStationEnabled(QString val);
    void namePlatformEnabled(QString val);
    void waitMinEnabled(QString val);
    void waitSecEnabled(QString val);
    void waitPasEnabled(QString val);
        
signals:

private:
    QCheckBox leftSide;
    QCheckBox rightSide;
    QCheckBox disablePlatform;
    QLineEdit lengthPlatform;
    QLineEdit nameStation;
    QLineEdit namePlatform;
    QLineEdit waitMin;
    QLineEdit waitSec;
    QLineEdit waitPas;
    PlatformObj* pobj;
};

#endif	/* PROPERTIESPLATFORM_H */

