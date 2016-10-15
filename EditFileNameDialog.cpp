/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "EditFileNameDialog.h"

EditFileNameDialog::EditFileNameDialog() : QDialog(){
    //this->setFixedSize(200, 100);
    this->setWindowTitle("Edit FileName");
    this->setFixedWidth(350);
    QPushButton* ok = new QPushButton("OK");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));

    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    int row = 0;
    vlist->addWidget(&name,row++,0,1,2);
    vlist->addWidget(ok,row, 0);
    vlist->addWidget(cancel,row++, 1);

    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
}

EditFileNameDialog::~EditFileNameDialog() {
}


void EditFileNameDialog::cancel(){
    this->isOk = false;
    this->close();
}
void EditFileNameDialog::ok(){
    this->isOk = true;
    this->close();
}