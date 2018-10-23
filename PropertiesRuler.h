/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESRULER_H
#define	PROPERTIESRULER_H

#include "PropertiesAbstract.h"

class PropertiesRuler: public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesRuler();
    virtual ~PropertiesRuler();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
        
public slots:
    void checkboxTwoPointEdited(int val);
    void checkboxDrawPointsEdited(int val);
    void createRoadPathsEdited();
    void removeRoadPathsEdited();
    void addShapeEdited();
    void elevTypeEdited(QString val);
    void eTemplateEdited(QString val);
    void hideElevBoxes();
    void showElevBox(QString val);
    
signals:
    
private:
    QLineEdit lengthM;
    QLineEdit lengthGM;
    QCheckBox checkboxTwoPoint;
    QCheckBox checkboxDrawPoints;
    
    QComboBox elevType;
    QLineEdit elevProm;
    QLineEdit elevProg;
    QLineEdit elevProp;
    QLineEdit elev1inXm;
    QLabel elevPromLabel;
    QLabel elevProgLabel;
    QLabel elevPropLabel;
    QLabel elev1inXmLabel;
};

#endif	/* PROPERTIESRULER_H */

