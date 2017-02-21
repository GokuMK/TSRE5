/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESSOUNDREGION_H
#define	PROPERTIESSOUNDREGION_H
#include "PropertiesAbstract.h"

class SoundRegionObj;

class PropertiesSoundRegion : public PropertiesAbstract {
    Q_OBJECT
public:
    PropertiesSoundRegion();
    virtual ~PropertiesSoundRegion();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    
public slots:
    void flip();
    void sourcesListSelected(QString val);
    void eMaxPlacingDistanceEnabled(QString val);
    void bDeleteSelectedEnabled();
    void bExpandEnabled();
    
signals:
    
private:
    SoundRegionObj* sobj;
    QComboBox sources;
    QLineEdit sName;
    QLineEdit eMaxPlacingDistance;
};

#endif	/* PROPERTIESSOUNDREGION_H */

