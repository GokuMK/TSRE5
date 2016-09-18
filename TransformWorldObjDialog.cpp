/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TransformWorldObjDialog.h"

TransformWorldObjDialog::TransformWorldObjDialog() : QDialog(){
    //this->setFixedSize(200, 100);
    this->setWindowTitle("Transform");
    QPushButton* ok = new QPushButton("OK");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));

    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    int row = 0;
    useObjRot.setText("Use Object Rotation for translation");
    vlist->addWidget(&useObjRot,row++, 0, 1, 2);
    vlist->addWidget(new QLabel("Translate:"),row++, 0, 1, 2);
    vlist->addWidget(new QLabel("X:"),row, 0);
    vlist->addWidget(&this->posX,row++, 1);
    this->posX.setText("0");
    vlist->addWidget(new QLabel("Y:"),row, 0);
    vlist->addWidget(&this->posY,row++, 1);
    this->posY.setText("0");
    vlist->addWidget(new QLabel("Z:"),row, 0);
    vlist->addWidget(&this->posZ,row++, 1);
    this->posZ.setText("0");
    vlist->addWidget(new QLabel("Rotate:"),row++, 0, 1, 2);
    vlist->addWidget(new QLabel("X:"),row, 0);
    vlist->addWidget(&this->rotX,row++, 1);
    this->rotX.setText("0");
    vlist->addWidget(new QLabel("Y:"),row, 0);
    vlist->addWidget(&this->rotY,row++, 1);
    this->rotY.setText("0");
    vlist->addWidget(new QLabel("Z:"),row, 0);
    vlist->addWidget(&this->rotZ,row++, 1);
    this->rotZ.setText("0");
    vlist->addWidget(ok,row, 0);
    vlist->addWidget(cancel,row++, 1);

//    mainLayout->setAlignment(browse, Qt::AlignBottom);
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
}

TransformWorldObjDialog::~TransformWorldObjDialog() {
}


void TransformWorldObjDialog::cancel(){
    this->isOk = false;
    this->close();
}
void TransformWorldObjDialog::ok(){
    this->isOk = true;
    if(this->useObjRot.checkState() == Qt::Checked)
        this->isUseObjRot = true;
    else
        this->isUseObjRot = false;
    this->x = this->posX.text().toInt();
    this->y = this->posY.text().toInt();
    this->z = this->posZ.text().toInt();
    this->rx = this->rotX.text().toInt();
    this->ry = this->rotY.text().toInt();
    this->rz = this->rotZ.text().toInt();

    this->close();
}