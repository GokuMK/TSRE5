/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESSPEEDPOST_H
#define	PROPERTIESSPEEDPOST_H

#include "PropertiesAbstract.h"

class SpeedpostObj;

class PropertiesSpeedpost : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesSpeedpost();
    virtual ~PropertiesSpeedpost();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    void updateObj(WorldObj* obj);
    
public slots:
    void flipSignal();
    void speedEnabled(QString val);
    void numberEnabled(QString val);
    void kmmListSelected(int);
    void ptbListSelected(int);
    void eMaxPlacingDistanceEnabled(QString val);
    void bDeleteSelectedEnabled();
    void bExpandEnabled();
    
signals:
    
private:
    SpeedpostObj* sobj;
    QLineEdit speedpostType;
    QLineEdit speed;
    QLineEdit number;
    QComboBox ptb;
    QComboBox kmm;
    QCheckBox chFlipShape;
    QLineEdit eMaxPlacingDistance;
};

#endif	/* PROPERTIESSPEEDPOST_H */

