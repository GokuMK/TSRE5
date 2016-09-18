/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRANSFORMWORLDOBJDIALOG_H
#define	TRANSFORMWORLDOBJDIALOG_H

#include <QtWidgets>

class TransformWorldObjDialog : public QDialog {
    Q_OBJECT
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float rx = 0;
    float ry = 0;
    float rz = 0;
    bool isUseObjRot = false;
    bool isOk = false;
    TransformWorldObjDialog();
    virtual ~TransformWorldObjDialog();
    
public slots:
    void cancel();
    void ok();
private:
    QCheckBox useObjRot;
    QLineEdit posX;
    QLineEdit posY;
    QLineEdit posZ;
    QLineEdit rotY;
    QLineEdit rotX;
    QLineEdit rotZ;
};

#endif	/* TRANSFORMWORLDOBJDIALOG_H */

