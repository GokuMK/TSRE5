/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESFOREST_H
#define	PROPERTIESFOREST_H

#include "PropertiesAbstract.h"

class ForestObj;

class PropertiesForest : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesForest();
    virtual ~PropertiesForest();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void sizeEnabled(QString val);
    void populationEnabled(QString val);
    void densitykmEnabled(QString val);
    
signals:
    
private:
    ForestObj *forestObj;
    QLineEdit sizeX;
    QLineEdit sizeY;
    QLineEdit population;
    QLineEdit densitykm;
};

#endif	/* PROPERTIESFOREST_H */

