/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESTERRAIN_H
#define	PROPERTIESTERRAIN_H

#include "PropertiesAbstract.h"

class Terrain;

class PropertiesTerrain : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesTerrain();
    virtual ~PropertiesTerrain();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
public slots:
    void bShaderEditorEnabled();
    void bWaterEditorEnabled();
    void bRotateEnabled();
    void bRemoveAllGapsEnabled();
    void bToggleWaterEnabled();
    void bToggleDrawEnabled();
    void bShowAdjacentEnabled();
    void eBiasEnabled(QString val);

private:
    Terrain* terrainObj = NULL;
    QLineEdit tP;
    QLineEdit tS;
    QLineEdit eBias;
};

#endif	/* PROPERTIESTERRAIN_H */

