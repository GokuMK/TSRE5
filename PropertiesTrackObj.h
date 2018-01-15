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
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
public slots:
    void enableCustomDetailLevelEnabled(int val);
    void customDetailLevelEdited(QString val);
    void fixJNodePosnEnabled();
    void hacksButtonEnabled();
    void elevPromEnabled(QString val);
    void elevProgEnabled(QString val);
    void elevPropEnabled(QString val);
    void elev1inXmEnabled(QString val);
    void editFileNameEnabled();
    void cCollisionTypeEdited(int val);
    void haxRemoveTDBVectorEnabled();
    void haxElevTDBVectorEnabled();
    void haxRemoveTDBTreeEnabled();
        
signals:
    
private:
    TrackObj* trackObj;
    QLineEdit elevProm;
    QLineEdit elevProg;
    QLineEdit elevProp;
    QLineEdit elev1inXm;
    QComboBox cCollisionType;
    QLineEdit eCollisionFlags;
    QLineEdit eSectionIdx;
};

#endif	/* PROPERTIESTRACKOBJ_H */

