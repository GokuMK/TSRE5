/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESPICKUP_H
#define	PROPERTIESPICKUP_H

#include "PropertiesAbstract.h"

class PickupObj;

class PropertiesPickup : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesPickup();
    virtual ~PropertiesPickup();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    
public slots:
    void cPickupTypeEdited(int val);
    void cAnimTypeEdited(int val);
    void eCapacityEnabled(QString val);
    void eContentEnabled(QString val);
    void eFillEnabled(QString val);
    void eSpeedMinEnabled(QString val);
    void eSpeedMaxEnabled(QString val);
    void eAnimLengthEnabled(QString val);
    void chInfiniteEnabled(int val);
    void chBrokenEnabled(int val);
signals:
    
private:
    PickupObj* pobj;
    QLineEdit eBrokenFileName;
    QComboBox cPickupType;
    QComboBox cAnimType;
    QLineEdit eCapacity;
    QLineEdit eContent;
    QLineEdit eFill;
    QLineEdit eSpeedMin;
    QLineEdit eSpeedMax;
    QLineEdit eAnimLength;
    QCheckBox chInfinite;
    QCheckBox chBroken;
};

#endif	/* PROPERTIESPICKUP_H */

