/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef RANDOMTRANSFORMWORLDOBJDIALOG_H
#define RANDOMTRANSFORMWORLDOBJDIALOG_H

#include <QtWidgets>

#include "Ref.h"


class RandomTransformWorldObjDialog : public QDialog {
    Q_OBJECT
public:
    bool isOk = false;
    RandomTransformWorldObjDialog();
    virtual ~RandomTransformWorldObjDialog();
    Ref::RandomTransformation* getTransform();
    
public slots:
    void cancel();
    void ok();
private:
    Ref::RandomTransformation* transformation = NULL;
    QLineEdit rbX;
    QLineEdit reX;
    QLineEdit rbY;
    QLineEdit reY;
    QLineEdit rbZ;
    QLineEdit reZ;
    
    QLineEdit tbX;
    QLineEdit teX;
    QLineEdit tbY;
    QLineEdit teY;
    QLineEdit tbZ;
    QLineEdit teZ;
};

#endif /* RANDOMTRANSFORMWORLDOBJDIALOG_H */

