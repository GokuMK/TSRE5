/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESSTATIC_H
#define	PROPERTIESSTATIC_H

#include "PropertiesAbstract.h"

class StaticObj;

class PropertiesStatic : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesStatic();
    virtual ~PropertiesStatic();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void copyFEnabled();
    void copyREnabled();
    void pasteREnabled();
    void rotYEnabled();
    void resetRotEnabled();
        
signals:
    
private:
    StaticObj *staticObj;
    QCheckBox checkboxAnim;
    QCheckBox checkboxTerrain;
};

#endif	/* PROPERTIESSTATIC_H */

