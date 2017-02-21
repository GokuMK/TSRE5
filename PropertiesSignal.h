/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESSIGNAL_H
#define	PROPERTIESSIGNAL_H

#include "PropertiesAbstract.h"

class SignalWindow;
class SignalObj;

class PropertiesSignal : public PropertiesAbstract {
    Q_OBJECT
    
public:
    PropertiesSignal();
    virtual ~PropertiesSignal();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);

public slots:
    void flipSignal();
    void showSubObjList();
    void checkboxAnimEdited(int val);
    void checkboxTerrainEdited(int val);
    void cShadowTypeEdited(int val);
    void hacksButtonEnabled();
    void haxFixFlagsEnabled();
    void msg(QString name, QString val);
    
signals:
    void enableTool(QString val);
    
private:
    QLineEdit name;
    QLineEdit description;
    QCheckBox chFlipShape;
    SignalObj* sobj;
    SignalWindow* signalWindow;
    
    QCheckBox checkboxAnim;
    QCheckBox checkboxTerrain;
    QComboBox cShadowType;
};

#endif	/* PROPERTIESSIGNAL_H */

