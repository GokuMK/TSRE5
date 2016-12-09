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
    void updateObj(WorldObj* obj);
    
public slots:
    void enableCustomDetailLevelEnabled(int val);
    void customDetailLevelEdited(QString val);
    void editPositionEnabled(QString val);
    void checkboxAnimEdited(int val);
    void checkboxTerrainEdited(int val);
    void cShadowTypeEdited(int val);
    void editFileNameEnabled();
    void cCollisionTypeEdited(int val);
    void RemoveCollisionsEnabled();
    
signals:
    
private:
    QCheckBox checkboxAnim;
    QCheckBox checkboxTerrain;
    QComboBox cShadowType;
    QComboBox cCollisionType;
    QLineEdit eCollisionFlags;
};

#endif	/* PROPERTIESSTATIC_H */

