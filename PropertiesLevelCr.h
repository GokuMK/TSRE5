/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESLEVELCR_H
#define	PROPERTIESLEVELCR_H

#include "PropertiesAbstract.h"

class LevelCrObj;

class PropertiesLevelCr : public PropertiesAbstract {
    Q_OBJECT
public:
    PropertiesLevelCr();
    virtual ~PropertiesLevelCr();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
public slots:
    void eActivateLevelCrossingEnabled(QString val);
    void eMinActDistEnabled(QString val);
    void eInitialWarningEnabled(QString val);
    void eMoreWarningEnabled(QString val);
    void eGateAnimLengthEnabled(QString val);
    void eCrashProbabilityEnabled(QString val);
    void eMaxPlacingDistanceEnabled(QString val);
    void chInvisibleEnabled(int val);
    void chSilentHaxEnabled(int val);
    void bDeleteSelectedEnabled();
    
signals:
    
private:
    LevelCrObj* lobj;

    QLineEdit eActivateLevelCrossing;
    QLineEdit eMinActDist;
    QLineEdit eInitialWarning;
    QLineEdit eGateAnimLength;
    QLineEdit eMoreWarning;
    QLineEdit eCrashProbability;
    QLineEdit eMaxPlacingDistance;
    
    QCheckBox chInvisible;
    QCheckBox chSilentHax;
};

#endif	/* PROPERTIESLEVELCR_H */

