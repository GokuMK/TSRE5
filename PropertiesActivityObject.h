/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESCONSIST_H
#define	PROPERTIESCONSIST_H

#include "PropertiesAbstract.h"

class ActivityObject;

class PropertiesActivityObject : public PropertiesAbstract {
    Q_OBJECT
public:
    PropertiesActivityObject();
    virtual ~PropertiesActivityObject();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
public slots:
    void bDeleteEnabled();
    
signals:
    
private:
    QLineEdit eObjectType;
    QLineEdit eId;
    QLineEdit eEid;
    QLineEdit eActivityName;
    ActivityObject* actObj = NULL;
};

#endif	/* PROPERTIESCONSIST_H */

