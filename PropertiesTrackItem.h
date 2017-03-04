/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */
#ifndef PROPERTIESTRACKITEM_H
#define	PROPERTIESTRACKITEM_H

#include "PropertiesAbstract.h"

class TRitem;

class PropertiesTrackItem : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesTrackItem();
    virtual ~PropertiesTrackItem();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
private:
    TRitem* itemObj = NULL;

};

#endif	/* PROPERTIESTRACKITEM_H */

