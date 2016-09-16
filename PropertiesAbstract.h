/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESABSTRACT_H
#define	PROPERTIESABSTRACT_H

#include <QtWidgets>

class WorldObj;

class PropertiesAbstract : public QWidget{
    Q_OBJECT
public:
    PropertiesAbstract();
    virtual ~PropertiesAbstract();
    virtual bool support(WorldObj* obj);
    virtual void showObj(WorldObj* obj);
    virtual void updateObj(WorldObj* obj);

protected:
    QLabel *infoLabel;    
    QLineEdit flags;
    QLineEdit fileName;
    QLineEdit posX;
    QLineEdit posY;
    QLineEdit posZ;
    QLineEdit quat;
    QLineEdit tX;
    QLineEdit tY;
    QLineEdit uid;
    QLineEdit defaultDetailLevel;
    QLineEdit customDetailLevel;
private:

};

#endif	/* PROPERTIESABSTRACT_H */

