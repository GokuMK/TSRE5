/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TerrainWaterWindow.h"

TerrainWaterWindow::TerrainWaterWindow() : QDialog(){
    this->setFixedSize(200, 100);

    QPushButton* ok = new QPushButton("OK");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));

    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("WSW:",&this->eWSW);
    vlist->addRow("WSE:",&this->eWSE);
    vlist->addRow("WNE:",&this->eWNE);
    vlist->addRow("WNW:",&this->eWNW);
    vlist->addRow(ok,cancel);
//    mainLayout->setAlignment(browse, Qt::AlignBottom);
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
    
}

void TerrainWaterWindow::setWater(){
    this->eWNE.setText(QString::number(this->WNE));
    this->eWSE.setText(QString::number(this->WSE));
    this->eWNW.setText(QString::number(this->WNW));
    this->eWSW.setText(QString::number(this->WSW));    
}

void TerrainWaterWindow::cancel(){
    this->changed = false;
    this->close();
}
void TerrainWaterWindow::ok(){
    this->changed = true;
    this->WNE = this->eWNE.text().toFloat();
    this->WSE = this->eWSE.text().toFloat();
    this->WNW = this->eWNW.text().toFloat();
    this->WSW = this->eWSW.text().toFloat();
    this->close();
}

TerrainWaterWindow::~TerrainWaterWindow() {
}

