/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "AboutWindow.h"
#include <QtWidgets>
#include <QDebug>

AboutWindow::AboutWindow(QWidget* parent) : QWidget(parent) {
    this->setWindowFlags( Qt::CustomizeWindowHint );
    this->setWindowFlags(Qt::WindowType::Tool);
    this->setFixedSize(600, 300);
    QImage* myImage = new QImage();
    myImage->load("resources/load.png");

    QLabel* myLabel = new QLabel("");
    myLabel->setContentsMargins(0,0,0,0);   
    
    QLabel* myLabel2 = new QLabel("<b>User guide</b> <a href=\"http://koniec.org/tsre5/\"><b>http://koniec.org/tsre5.</b></a>");
    myLabel2->setOpenExternalLinks(true);
    myLabel2->setContentsMargins(5,0,0,0);
    
    QLabel* myLabel3 = new QLabel("Copyright © GokuMK, mail pgadecki@gmail.com ");
    myLabel3->setContentsMargins(5,0,0,0);
  
    
    myLabel->setPixmap(QPixmap::fromImage(*myImage));

    QPushButton* browse = new QPushButton("Close");
    connect(browse, SIGNAL (released()), this, SLOT (close()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(myLabel);
    mainLayout->addWidget(myLabel2);
    mainLayout->addWidget(myLabel3);
    mainLayout->addWidget(browse);
    
    mainLayout->setAlignment(myLabel, Qt::AlignTop);
    mainLayout->setAlignment(myLabel2, Qt::AlignTop);
    mainLayout->setAlignment(myLabel3, Qt::AlignTop);
    mainLayout->setAlignment(browse, Qt::AlignBottom);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);
}

AboutWindow::AboutWindow(const AboutWindow& orig) {
}

AboutWindow::~AboutWindow() {
}

void AboutWindow::exitNow(){
    this->hide();
}

