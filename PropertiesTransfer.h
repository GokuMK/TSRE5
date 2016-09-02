/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESTRANSFER_H
#define	PROPERTIESTRANSFER_H

#include "PropertiesAbstract.h"

class TransferObj;

class PropertiesTransfer : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesTransfer();
    virtual ~PropertiesTransfer();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void sizeEnabled(QString val);
    
private:
    TransferObj *transferObj;
    QPixmap* texPreview;
    QLabel* texPreviewLabel;
    QLineEdit sizeX;
    QLineEdit sizeY;
};

#endif	/* PROPERTIESTRANSFER_H */

