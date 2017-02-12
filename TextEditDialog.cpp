/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TextEditDialog.h"

TextEditDialog::TextEditDialog() : QDialog(){
    textBox.setMinimumHeight(200);
    textBox.setMinimumWidth(300);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *ibuttons = new QHBoxLayout;
    ibuttons->setContentsMargins(1,1,1,1);
    QPushButton *bok = new QPushButton("OK");
    QObject::connect(bok, SIGNAL(released()), this, SLOT(ok()));
    QPushButton *bcancel = new QPushButton("Cancel");
    QObject::connect(bcancel, SIGNAL(released()), this, SLOT(cancel()));
    ibuttons->addWidget(bok);
    ibuttons->addWidget(bcancel);
    mainLayout->addWidget(&textBox);
    mainLayout->addItem(ibuttons);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);
}

TextEditDialog::~TextEditDialog() {
}

void TextEditDialog::ok(){
    this->changed = 1;
    this->close();
}

void TextEditDialog::cancel(){
    this->close();
}
    
                              