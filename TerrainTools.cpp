/* 
 * File:   TerrainTools.cpp
 * Author: Goku
 * 
 * Created on 1 czerwca 2015, 20:39
 */

#include "TerrainTools.h"

TerrainTools::TerrainTools(QString name)
    : QWidget(){
    
    QPushButton *heightTool = new QPushButton("Paint HeightMap", this);
    QPushButton *paintTool = new QPushButton("Paint Texture", this);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(heightTool);
    vbox->addWidget(paintTool);
    
    vbox->setContentsMargins(0,1,5,1);
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(heightTool, SIGNAL(released()),
                      this, SLOT(heightToolEnabled()));
    
    QObject::connect(paintTool, SIGNAL(released()),
                      this, SLOT(paintToolEnabled()));
}


TerrainTools::~TerrainTools() {
}

void TerrainTools::heightToolEnabled(){
    emit enableTool("heightTool");
}

void TerrainTools::paintToolEnabled(){
    emit enableTool("paintTool");
}