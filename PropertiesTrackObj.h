/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESTRACKOBJ_H
#define	PROPERTIESTRACKOBJ_H

#include "PropertiesAbstract.h"

class TrackObj;

class PropertiesTrackObj : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesTrackObj();
    virtual ~PropertiesTrackObj();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    void updateObj(WorldObj* obj);
    
public slots:
    void fixJNodePosnEnabled();
    void hacksButtonEnabled();
    void elevPromEnabled(QString val);
    void elev1inXmEnabled(QString val);
    void editFileNameEnabled();
    void cCollisionTypeEdited(int val);
    void haxRemoveTDBVectorEnabled();
    void haxRemoveTDBTreeEnabled();
        
signals:
    
private:
    TrackObj* trackObj;
    QLineEdit elevProm;
    QLineEdit elev1inXm;
    QComboBox cCollisionType;
    QLineEdit eCollisionFlags;
};

#endif	/* PROPERTIESTRACKOBJ_H */

