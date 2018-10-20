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
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
public slots:
    void flexEnabled();
    void flexData(int x, int z, float* p);
    void chSectEnabled(int idx);
    void sSectEnabled(int idx);
    void msg(QString name, QString val);
    
    void elevPromEnabled(QString val);
    void elevProgEnabled(QString val);
    void elevPropEnabled(QString val);
    void elev1inXmEnabled(QString val);
    void elevStepEnabled(QString val);
    void elevTypeEdited(QString val);
    
signals:
    void enableTool(QString val);
    void setMoveStep(float val);
    
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
    
    QComboBox elevType;
    QLineEdit elevStep;
    QLineEdit elevProm;
    QLineEdit elevProg;
    QLineEdit elevProp;
    QLineEdit elev1inXm;
    QLabel elevPromLabel;
    QLabel elevProgLabel;
    QLabel elevPropLabel;
    QLabel elev1inXmLabel;
    
    void hideElevBoxes();
    void showElevBox(QString val);
    void setStepValue(float step);
    float getStepValue(float step);
};

#endif	/* PROPERTIESDYNTRACK_H */

