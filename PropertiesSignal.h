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

class PropertiesSignal : public PropertiesAbstract{
    Q_OBJECT
    
public:
    PropertiesSignal();
    virtual ~PropertiesSignal();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);

public slots:
    void flipSignal();
    void showSubObjList();
    
signals:
    
private:
    QLineEdit name;
    QLineEdit description;
    QCheckBox chFlipShape;
    SignalObj* sobj;
    SignalWindow* signalWindow;
};

#endif	/* PROPERTIESSIGNAL_H */

