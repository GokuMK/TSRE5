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
    void copyFEnabled();
    void copyREnabled();
    void pasteREnabled();
    void rotYEnabled();
    void resetRotEnabled();
    
signals:
    
private:
    TrackObj* trackObj;
    QLineEdit elevProm;
    QLineEdit elev1inXm;
    
};

#endif	/* PROPERTIESTRACKOBJ_H */

