/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesDyntrack.h"
#include "DynTrackObj.h"
#include "Flex.h"
#include "Game.h"

PropertiesDyntrack::PropertiesDyntrack() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("DynTrack:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);

    QLabel * label2 = new QLabel("Sections:");
    label2->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    
    
    this->chSect[0].setText("First Straight section");
    this->chSect[0].setChecked(true);
    //this->chSect[0].setEnabled(false);// .setCheckable(false);
    vbox->addWidget(&chSect[0]);
    vSect[0].setSpacing(2);
    vSect[0].setContentsMargins(3,0,3,0);
    vSect[0].addRow("Length:",&this->sSectA[0]);
    wSect[0].setLayout(&vSect[0]);
    vbox->addWidget(&wSect[0]);
    
    this->chSect[1].setText("First curve");
    vbox->addWidget(&chSect[1]);
    vSect[1].setSpacing(2);
    vSect[1].setContentsMargins(3,0,3,0);
    vSect[1].addRow("Angle:",&this->sSectA[1]);
    vSect[1].addRow("Radius:",&this->sSectR[1]);
    wSect[1].setLayout(&vSect[1]);
    vbox->addWidget(&wSect[1]);
    
    
    this->chSect[2].setText("Second Straight section");
    vbox->addWidget(&chSect[2]);
    vSect[2].setSpacing(2);
    vSect[2].setContentsMargins(3,0,3,0);
    vSect[2].addRow("Length:",&this->sSectA[2]);
    wSect[2].setLayout(&vSect[2]);
    vbox->addWidget(&wSect[2]);
    
    this->chSect[3].setText("Second curve");
    vbox->addWidget(&chSect[3]);
    vSect[3].setSpacing(2);
    vSect[3].setContentsMargins(3,0,3,0);
    vSect[3].addRow("Angle:",&this->sSectA[3]);
    vSect[3].addRow("Radius:",&this->sSectR[3]);
    wSect[3].setLayout(&vSect[3]);
    vbox->addWidget(&wSect[3]);
    
    
    this->chSect[4].setText("Third Straight section");
    vbox->addWidget(&chSect[4]);
    vSect[4].setSpacing(2);
    vSect[4].setContentsMargins(3,0,3,0);
    vSect[4].addRow("Length:",&this->sSectA[4]);
    wSect[4].setLayout(&vSect[4]);
    vbox->addWidget(&wSect[4]);
    
    QPushButton *flex = new QPushButton("Flex", this);
    vbox->addWidget(flex);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    for(int i=0; i<5; i++){
        if(i%2 == 0){
            sSectA[i].setDecimals(2);
            sSectA[i].setMinimum(0);
            sSectA[i].setMaximum(250);
            sSectA[i].setSingleStep(1.0);
        } else {
            sSectA[i].setDecimals(3);
            sSectA[i].setMinimum(-3.14);
            sSectA[i].setMaximum(3.14);
            sSectA[i].setSingleStep(0.01);
        }
        sSectR[i].setDecimals(2);
        sSectR[i].setMinimum(15);
        sSectR[i].setMaximum(5000);
        sSectR[i].setSingleStep(1.0);
    }
    
    for(int i = 0; i < 5; i++){
        dyntrackChSect.setMapping(&chSect[i], i);
        connect(&chSect[i], SIGNAL(clicked()), &dyntrackChSect, SLOT(map()));
    }
    
    QObject::connect(&dyntrackChSect, SIGNAL(mapped(int)),
            this, SLOT(chSectEnabled(int)));
    
    for(int i = 0; i < 5; i++){
        dyntrackSect.setMapping(&sSectR[i], i);
        connect(&sSectR[i], SIGNAL(valueChanged(double)), &dyntrackSect, SLOT(map()));
        dyntrackSect.setMapping(&sSectA[i], i);
        connect(&sSectA[i], SIGNAL(valueChanged(double)), &dyntrackSect, SLOT(map()));
    }
    
    QObject::connect(&dyntrackSect, SIGNAL(mapped(int)),
            this, SLOT(sSectEnabled(int)));
    
    QObject::connect(flex, SIGNAL(released()),
                      this, SLOT(flexEnabled()));
    
}

PropertiesDyntrack::~PropertiesDyntrack() {
}

void PropertiesDyntrack::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    dobj = (DynTrackObj*)obj;
    this->infoLabel->setText("Object: "+obj->type);
    
    for (int i = 0; i < 5; i++) {
        if(dobj->sections[i].sectIdx > 1000000){
            this->chSect[i].setChecked(false);
            this->wSect[i].hide();
            continue;
        }
        this->wSect[i].show();
        this->chSect[i].setChecked(true);
        this->sSectA[i].blockSignals(true);
        this->sSectR[i].blockSignals(true);
        this->sSectA[i].setValue(dobj->sections[i].a);
        this->sSectR[i].setValue(dobj->sections[i].r);
        this->sSectA[i].blockSignals(false);
        this->sSectR[i].blockSignals(false);
    }
    
    //this->carNumber.setText(QString::number(pobj->getCarNumber(),10));
    //this->carSpeed.setText(QString::number(pobj->getCarSpeed(),10));
}

void PropertiesDyntrack::chSectEnabled(int idx){
    qDebug() << "chSectEnabled";
    if(dobj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    bool state = this->chSect[idx].isChecked();
    
    if(state){
        this->wSect[idx].show();
        dobj->sections[idx].sectIdx = 0;
        if(idx%2 == 0){
            dobj->sections[idx].a = 2;
            dobj->sections[idx].r = 0;
        } else {
            dobj->sections[idx].a = 0.1;
            dobj->sections[idx].r = 100;
        }
        this->sSectA[idx].blockSignals(true);
        this->sSectR[idx].blockSignals(true);
        this->sSectA[idx].setValue(dobj->sections[idx].a);
        this->sSectR[idx].setValue(dobj->sections[idx].r);
        this->sSectA[idx].blockSignals(false);
        this->sSectR[idx].blockSignals(false);
    } else {
        this->wSect[idx].hide();
        dobj->sections[idx].sectIdx = 4294967295;
    }
    dobj->modified = true;
    dobj->deleteVBO();
}

void PropertiesDyntrack::sSectEnabled(int idx){
    qDebug() << "sSectEnabled";
    if(dobj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    if(idx%2 == 1 && fabs(this->sSectA[idx].value()) < 0.0001 )
        return;

    dobj->sections[idx].a = this->sSectA[idx].value();
    if(idx%2 == 1)
        dobj->sections[idx].r = this->sSectR[idx].value();
    dobj->modified = true;
    dobj->box.loaded = false;
    dobj->deleteVBO();
}

bool PropertiesDyntrack::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "dyntrack")
        return true;
    return false;
}

void PropertiesDyntrack::flexEnabled(){
    emit enableTool("FlexTool");
    
}

void PropertiesDyntrack::flexData(int x, int z, float* p){
    emit enableTool("");
    if(dobj == NULL) return;
    
    qDebug() << "flex1";
    
    float p1[3];
    float p2[3];
    p1[0] = dobj->position[0];
    p1[1] = dobj->position[1];
    p1[2] = dobj->position[2];
    p2[0] = p[0];
    p2[1] = p[1];
    p2[2] = p[2];
    
    float* dyntrackData[10];
    
    bool success = Flex::AutoFlex(dobj->x, dobj->y, (float*)p1, x, z, (float*)p2, (float*)dyntrackData);
    qDebug() << "flex2";
    dobj->set("dyntrackdata", (float*)dyntrackData);
    this->showObj(dobj);
    emit enableTool("selectTool");
}