#include "PropertiesSignal.h"
#include "SignalObj.h"
#include "Game.h"
#include "TDB.h"
#include "SigCfg.h"
#include "SignalShape.h"

PropertiesSignal::PropertiesSignal() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Signal:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QLabel* label = new QLabel("Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&name);
    label = new QLabel("Description:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&description);
    QPushButton *button = new QPushButton("Flip", this);
    vbox->addWidget(button);
    label = new QLabel("SubObjects:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    for(int i = 0; i < 10; i++){
        this->chSub[i].setText("");
        vSub[i].setSpacing(2);
        vSub[i].setContentsMargins(3,0,1,0);    
        dSub[i].setEnabled(false);
        vSub[i].addWidget(&this->chSub[i],0,0);
        vSub[i].addWidget(&this->bSub[i],0,1);
        vSub[i].addWidget(&this->dSub[i],1,0,1,2);
        wSub[i].setLayout(&vSub[i]);
        vbox->addWidget(&wSub[i]);
    }

    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesSignal::~PropertiesSignal() {
}

void PropertiesSignal::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    sobj = (SignalObj*)obj;
    this->infoLabel->setText("Object: "+obj->type);
    
    for (int i = 0; i < 10; i++) {
        this->wSub[i].hide();
        this->chSub[i].setChecked(false);
        this->bSub[i].hide();
        this->bSub[i].setEnabled(false);
    }
    
    TDB* tdb = Game::trackDB;
    SignalShape* signalShape = tdb->sigCfg->signalShape[sobj->fileName.toStdString()];
    if(signalShape == NULL) 
        return;
    
    this->name.setText(sobj->fileName);
    this->description.setText(signalShape->desc);

    for (int i = 0; i < signalShape->iSubObj; i++) {
        this->wSub[i].show();
        this->dSub[i].setText(signalShape->subObj[i].desc);
        if(sobj->isSubObjEnabled(i))
            this->chSub[i].setChecked(true);
    }
    int linkPtr;

    for (int i = 0; i < signalShape->iSubObj; i++) {
        if(signalShape->subObj[i].iLink > 0){
            this->bSub[i].show();
            this->bSub[i].setStyleSheet("color: gray"); 
            this->bSub[i].setText("Link");
            linkPtr = signalShape->subObj[i].sigSubJnLinkIf[0];
            if(this->chSub[linkPtr].isChecked()){
                int linkId = sobj->getLinkedJunctionValue(i);
                if(linkId == -1){
                    this->bSub[i].setEnabled(false);
                    this->bSub[i].setStyleSheet("color: red"); 
                    this->bSub[i].setText("NULL");
                } else if(linkId == 0){
                    this->bSub[i].setStyleSheet(""); 
                    if(sobj->isJunctionAvailable(i)){
                        this->bSub[i].setText("Link");
                        this->bSub[i].setEnabled(true);
                    } else {
                        this->bSub[i].setText("No Junction");
                        this->bSub[i].setEnabled(false);
                    }
                } else {
                    this->bSub[i].setEnabled(true);
                    this->bSub[i].setStyleSheet("color: green"); 
                    this->bSub[i].setText("Linked: "+QString::number(linkId, 10));
                }
            } else {
                this->bSub[i].setEnabled(false);
            }
        }
    }
    
    //this->carNumber.setText(QString::number(pobj->getCarNumber(),10));
    //this->carSpeed.setText(QString::number(pobj->getCarSpeed(),10));
}

bool PropertiesSignal::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "signal")
        return true;
    return false;
}