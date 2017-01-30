/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESDYNTRACK_H
#define	PROPERTIESDYNTRACK_H

#include "PropertiesAbstract.h"

class DynTrackObj;

class PropertiesDyntrack : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesDyntrack();
    virtual ~PropertiesDyntrack();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void flexEnabled();
    void flexData(int x, int z, float* p);
    void chSectEnabled(int idx);
    void sSectEnabled(int idx);
    
signals:
    void enableTool(QString val);
    
private:
    QCheckBox chSect[5];
    QDoubleSpinBox sSectA[5];
    QDoubleSpinBox sSectR[5];
    QFormLayout vSect[5];
    QWidget wSect[5];
    DynTrackObj* dobj;
    QSignalMapper dyntrackChSect;
    QSignalMapper dyntrackSect;
    QLineEdit eSectionIdx;
};

#endif	/* PROPERTIESDYNTRACK_H */

