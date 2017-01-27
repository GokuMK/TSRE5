/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SignalWindow.h"
#include <QDebug>
#include "SignalObj.h"
#include "Game.h"
#include "TDB.h"
#include "SigCfg.h"
#include "SignalShape.h"
#include "SignalWindowLink.h"
#include "Undo.h"

SignalWindow::SignalWindow()
: QWidget() {
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setFixedWidth(400);


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0, 1, 1, 1);

    for (int i = 0; i < maxSubObj; i++) {
        this->chSub[i].setText("");
        vSub[i].setSpacing(2);
        vSub[i].setContentsMargins(3, 0, 1, 0);
        dSub[i].setEnabled(false);
        vSub[i].addWidget(&this->chSub[i], 0, 0);
        vSub[i].addWidget(&this->bSub[i], 0, 2);
        vSub[i].addWidget(&this->dSub[i], 0, 1);
        wSub[i].setLayout(&vSub[i]);
        vbox->addWidget(&wSub[i]);
        //wSub[i].hide();

        signalsChSect.setMapping(&chSub[i], i);
        connect(&chSub[i], SIGNAL(clicked()), &signalsChSect, SLOT(map()));

        signalsLinkButton.setMapping(&bSub[i], i);
        connect(&bSub[i], SIGNAL(clicked()), &signalsLinkButton, SLOT(map()));

    }

    connect(&signalsChSect, SIGNAL(mapped(int)), this, SLOT(chSubEnabled(int)));
    connect(&signalsLinkButton, SIGNAL(mapped(int)), this, SLOT(bLinkEnabled(int)));
    QPushButton* closeButton = new QPushButton("Close");
    connect(closeButton, SIGNAL(released()), this, SLOT(close()));
    //vbox->setAlignment(setLinkButton, Qt::AlignBottom);
    vbox->addWidget(closeButton);
    QLabel *label = new QLabel("SubObj Link Info (press Link button above to show):");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,1,0);    
    setLinkButton = new QPushButton("Set Link");
    connect(setLinkButton, SIGNAL(released()), this, SLOT(setLink()));
    label = new QLabel("From - To: ");
    vlist->addWidget(label,0,0);
    vlist->addWidget(&eLink1,0,1);
    eLink1.setDisabled(true);
    vlist->addWidget(&eLink2,0,2);
    eLink2.setDisabled(true);
    vlist->addWidget(&eLink3,0,3);
    eLink3.setDisabled(true);
    vlist->addWidget(setLinkButton,0,4);
    vbox->addItem(vlist);

    vbox->setContentsMargins(1, 1, 1, 1);
    vbox->addStretch(1);
    this->setLayout(vbox);
}

void SignalWindow::chSubEnabled(int i) {
    if (sobj == NULL)
        return;

    Undo::StateBegin();
    Undo::PushWorldObjData(sobj);
    Undo::PushTrackDB(Game::trackDB);
    if (chSub[i].isChecked())
        sobj->enableSubObj(i);
    else
        sobj->disableSubObj(i);
    Undo::StateEnd();
    showObj(sobj);
}

void SignalWindow::bLinkEnabled(int i) {
    if (sobj == NULL)
        return;
    
    currentSubObjLinkInfo = i;
    if(currentSubObjLinkInfo < 0){
        setLinkButton->setDisabled(true);
        setLinkButton->setText("Set Link");
        eLink1.setText("");
        eLink2.setText("");
        eLink3.setText("");
        return;
    }
    this->sobj->subObjSelected = i;
    setLinkButton->setDisabled(false);
    setLinkButton->setText(QString("Set Link [")+QString::number(i)+"]");
    int ids[3];
    int linkId = sobj->getLinkedJunctionValue(i);
    if(linkId < 1){
        eLink1.setText("");
        eLink2.setText("");
        eLink3.setText("");
        return;
    }
    sobj->getLinkInfo((int*)&ids);
    eLink1.setText(QString::number(ids[0]));
    eLink2.setText(QString::number(ids[1]));
    eLink3.setText(QString::number(ids[2]));
    
    /*SignalWindowLink window;
    window.setWindowTitle("Link Signal");
    window.exec();
    if (window.changed) {
        int from = window.from.text().toInt();
        int to = window.to.text().toInt();
        qDebug() << "link val: " << from << " " << to;
        sobj->linkSignal(i, from, to);
        showObj(sobj);
    }*/

}

void SignalWindow::setLink(){
    if(currentSubObjLinkInfo >= 0)
        emit sendMsg("enableTool","signalLinkTool");
}

void SignalWindow::showObj(SignalObj* obj) {
    this->sobj = obj;
    for (int i = 0; i < maxSubObj; i++) {
        this->wSub[i].hide();
        this->chSub[i].setChecked(false);
        this->bSub[i].hide();
        this->bSub[i].setEnabled(false);
    }

    TDB* tdb = Game::trackDB;
    SignalShape* signalShape = tdb->sigCfg->signalShape[sobj->fileName.toStdString()];

    int iSubObj = signalShape->iSubObj;
    if (iSubObj > maxSubObj) iSubObj = maxSubObj;
    QString backFace = "[B] ";
    for (int i = 0; i < iSubObj; i++) {
        this->wSub[i].show();
        if(signalShape->subObj[i].backFacing)
            this->dSub[i].setText(backFace+signalShape->subObj[i].desc);
        else
            this->dSub[i].setText(signalShape->subObj[i].desc);
        if (sobj->isSubObjEnabled(i))
            this->chSub[i].setChecked(true);
        if (!signalShape->subObj[i].optional)
            this->chSub[i].setEnabled(false);
        else
            this->chSub[i].setEnabled(true);
    }
    int linkPtr;

    for (int i = 0; i < iSubObj; i++) {
        if (signalShape->subObj[i].isJnLink) {
            this->bSub[i].show();
            this->bSub[i].setStyleSheet("color: gray");
            this->bSub[i].setText("Link");
            this->bSub[i].setEnabled(false);
            if (this->chSub[i].isChecked()) {
                setLinkInfo(i);
            }
        } else if (signalShape->subObj[i].iLink > 0) {
            this->bSub[i].show();
            this->bSub[i].setStyleSheet("color: gray");
            this->bSub[i].setText("Link");
            this->bSub[i].setEnabled(false);
            for (int j = 0; j < signalShape->subObj[i].iLink; j++) {
                linkPtr = signalShape->subObj[i].sigSubJnLinkIf[j];
                if (this->chSub[linkPtr].isChecked()) {
                    setLinkInfo(i);
                    break;
                }
            }
        }
    }
    this->resize(this->width(), this->minimumHeight());
    bLinkEnabled(-1);
}

void SignalWindow::updateObj(SignalObj* obj) {
    this->sobj = obj;
    if(sobj == NULL)
        return;
    
    TDB* tdb = Game::trackDB;
    SignalShape* signalShape = tdb->sigCfg->signalShape[sobj->fileName.toStdString()];
    int iSubObj = signalShape->iSubObj;
    if (iSubObj > maxSubObj) iSubObj = maxSubObj;
    int linkPtr;
    
    for (int i = 0; i < iSubObj; i++) {
        if(this->bSub[i].hasFocus())
            continue;
        if (signalShape->subObj[i].isJnLink) {
            this->bSub[i].show();
            this->bSub[i].setStyleSheet("color: gray");
            this->bSub[i].setText("Link");
            this->bSub[i].setEnabled(false);
            if (this->chSub[i].isChecked()) {
                setLinkInfo(i);
            }
        } else if (signalShape->subObj[i].iLink > 0) {
            this->bSub[i].show();
            this->bSub[i].setStyleSheet("color: gray");
            this->bSub[i].setText("Link");
            this->bSub[i].setEnabled(false);
            for (int j = 0; j < signalShape->subObj[i].iLink; j++) {
                linkPtr = signalShape->subObj[i].sigSubJnLinkIf[j];
                if (this->chSub[linkPtr].isChecked()) {
                    setLinkInfo(i);
                    break;
                }
            }
        }
    }
    bLinkEnabled(currentSubObjLinkInfo);
}

void SignalWindow::setLinkInfo(int i) {
    int linkId = sobj->getLinkedJunctionValue(i);
    if (linkId == -1) {
        this->bSub[i].setEnabled(false);
        this->bSub[i].setStyleSheet("color: red");
        this->bSub[i].setText("NULL");
    } else if (linkId == 0) {
        this->bSub[i].setStyleSheet("");
        if (sobj->isJunctionAvailable(i)) {
            this->bSub[i].setText("Link");
            this->bSub[i].setEnabled(true);
        } else {
            this->bSub[i].setText("No Junction");
            this->bSub[i].setEnabled(false);
        }
    } else {
        this->bSub[i].setEnabled(true);
        this->bSub[i].setStyleSheet("color: green");
        this->bSub[i].setText("Linked: " + QString::number(linkId, 10));
    }
}

SignalWindow::~SignalWindow() {
}

void SignalWindow::exitNow() {
    this->close();
}