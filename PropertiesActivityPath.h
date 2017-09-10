/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESACTIVITYPATH_H
#define	PROPERTIESACTIVITYPATH_H

#include "PropertiesAbstract.h"

class Path;

class PropertiesActivityPath : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesActivityPath();
    virtual ~PropertiesActivityPath();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    
private:
    Path *pathObj = NULL;
    QLineEdit ePathFName;
    QLineEdit eName;
    QLineEdit ePathStart;
    QLineEdit ePathEnd;
};

#endif	/* PROPERTIESACTIVITYPATH_H */

