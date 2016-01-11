#include "PropertiesSignal.h"
#include "SignalWindow.h"
#include "SignalObj.h"
#include "Game.h"
#include "TDB.h"
#include "SigCfg.h"
#include "SignalShape.h"

PropertiesSignal::PropertiesSignal() {
    signalWindow = new SignalWindow();
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Signal:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->uid.setDisabled(true);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlist->addRow("UiD:",&this->uid);
    vlist->addRow("Tile X:",&this->tX);
    vlist->addRow("Tile Z:",&this->tY);
    vbox->addItem(vlist);
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
    connect(button, SIGNAL(released()), this, SLOT(flipSignal()));
    chFlipShape.setText("Flip Shape");
    chFlipShape.setChecked(true);
    vbox->addWidget(&chFlipShape);
    label = new QLabel("SubObjects:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    button = new QPushButton("Show list", this);
    vbox->addWidget(button);
    connect(button, SIGNAL(released()), this, SLOT(showSubObjList()));
    /*for(int i = 0; i < maxSubObj; i++){
        this->chSub[i].setText("");
        vSub[i].setSpacing(2);
        vSub[i].setContentsMargins(3,0,1,0);    
        dSub[i].setEnabled(false);
        vSub[i].addWidget(&this->chSub[i],0,0);
        vSub[i].addWidget(&this->bSub[i],0,1);
        vSub[i].addWidget(&this->dSub[i],1,0,1,2);
        wSub[i].setLayout(&vSub[i]);
        vbox->addWidget(&wSub[i]);
        wSub[i].hide();

        signalsChSect.setMapping(&chSub[i], i);
        connect(&chSub[i], SIGNAL(clicked()), &signalsChSect, SLOT(map()));
    }*/

    //QObject::connect(&signalsChSect, SIGNAL(mapped(int)),
    //    this, SLOT(chSubEnabled(int)));
    
    
    
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
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    
    /*for (int i = 0; i < maxSubObj; i++) {
        this->wSub[i].hide();
        this->chSub[i].setChecked(false);
        this->bSub[i].hide();
        this->bSub[i].setEnabled(false);
    }*/
    
    TDB* tdb = Game::trackDB;
    SignalShape* signalShape = tdb->sigCfg->signalShape[sobj->fileName.toStdString()];
    if(signalShape == NULL){ 
        infoLabel->setText("NULL");
        return;
    }
    
    this->name.setText(sobj->fileName);
    this->description.setText(signalShape->desc);

    signalWindow->showObj(sobj);
    
    QRect rec = QApplication::desktop()->screenGeometry();
    signalWindow->move(rec.width()/2-signalWindow->width()/2 ,rec.height()/2-signalWindow->height()/2);
}

void PropertiesSignal::flipSignal(){
    if(sobj == NULL)
        return;
    sobj->flip(chFlipShape.isChecked());
}

void PropertiesSignal::showSubObjList(){
    this->signalWindow->show();
}

bool PropertiesSignal::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "signal")
        return true;
    return false;
}