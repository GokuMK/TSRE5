/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESRULER_H
#define	PROPERTIESRULER_H

#include "PropertiesAbstract.h"

class PropertiesRuler: public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesRuler();
    virtual ~PropertiesRuler();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
public slots:
    void checkboxTwoPointEdited(int val);
signals:
    
private:
    QLineEdit lengthM;
    QCheckBox checkboxTwoPoint;
};

#endif	/* PROPERTIESRULER_H */

