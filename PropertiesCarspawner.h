/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESCARSPAWNER_H
#define	PROPERTIESCARSPAWNER_H

#include "PropertiesAbstract.h"

class CarSpawnerObj;

class PropertiesCarspawner : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesCarspawner();
    virtual ~PropertiesCarspawner();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void carNumberEnabled(QString val);
    void carSpeedEnabled(QString val);
    void useCustomListEnabled(int val);
    void carspawnListSelected(QString val);
    void bExpandEnabled();
signals:
    
private:
    QLineEdit carNumber;
    QLineEdit carSpeed;
    CarSpawnerObj* cobj;
    QLineEdit lengthPlatform;
    QCheckBox useCustomList;
    QComboBox carspawnList;
};

#endif	/* PROPERTIESCARSPAWNER_H */

