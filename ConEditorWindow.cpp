#include "ConEditorWindow.h"
#include <QDebug>
#include "EngLib.h"
#include "ConLib.h"
#include "Eng.h"
#include "Consist.h"
#include "Game.h"
#include "EngListWidget.h"
#include "ConListWidget.h"
#include "glShapeWidget.h"
#include "CameraFree.h"
#include "CameraConsist.h"

ConEditorWindow::ConEditorWindow() : QMainWindow() {
    englib = new EngLib();
    //conEngLib = new EngLib();
    englib->loadAll(Game::root);
    Game::currentEngLib = englib;
    ConLib::loadAll(Game::root);
    glShapeWidget = new GlShapeWidget(this);
    //glShapeWidget->currentEngLib = englib;
    glConWidget = new GlShapeWidget(this);
    glConWidget->currentEngLib = englib;
    conCamera = new CameraConsist();
    conCamera->setPos(-100,2.5,42);
    conCamera->setPlayerRot(M_PI/2.0,0);
    
    glConWidget->setCamera(conCamera);
    //qDebug()<<"aaa";
    eng1 = new EngListWidget();
    eng1->englib = englib;
    eng1->fillEngList();
    con1 = new ConListWidget();
    con1->fillConList();
    //qDebug()<<"aaa";
    conSlider = new QScrollBar(Qt::Horizontal);
    conSlider->setMaximum(0);
    conSlider->setMinimum(0);
    
    QWidget* main = new QWidget();

    QVBoxLayout *mbox = new QVBoxLayout;
    mbox->setSpacing(2);
    mbox->setContentsMargins(1,1,1,1);
    QHBoxLayout *vbox = new QHBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    vbox->addWidget(con1);
    vbox->addWidget(eng1);
    QVBoxLayout *engInfoLayout = new QVBoxLayout;
    engInfoLayout->addWidget(glShapeWidget);
    QGridLayout *engInfoForm = new QGridLayout;
    engInfoForm->setSpacing(2);
    engInfoForm->setContentsMargins(1,1,1,1);    
    engInfoForm->addWidget(new QLabel("Name:"),0,0);
    engInfoForm->addWidget(new QLabel("Type:"),0,2);
    engInfoForm->addWidget(new QLabel("Mass:"),1,0);
    engInfoForm->addWidget(new QLabel("Max. Speed:"),1,2);
    engInfoForm->addWidget(new QLabel("Brakes:"),2,0);
    engInfoForm->addWidget(new QLabel("Couplings:"),3,0);
    engInfoForm->addWidget(new QLabel("Shape:"),2,2);
    engInfoForm->addWidget(new QLabel("Size:"),3,2);
    engInfoForm->addWidget(&eName,0,1);
    engInfoForm->addWidget(&eType,0,3);
    engInfoForm->addWidget(&eMass,1,1);
    engInfoForm->addWidget(&eMaxSpeed,1,3);
    engInfoForm->addWidget(&eBrakes,2,1);
    engInfoForm->addWidget(&eCouplings,3,1);
    engInfoForm->addWidget(&eShape,2,3);
    engInfoForm->addWidget(&eSize,3,3);
    engInfoLayout->addItem(engInfoForm);
    engInfoLayout->setSpacing(0);
    engInfoLayout->setContentsMargins(0,0,0,0);
    vbox->addItem(engInfoLayout);
    vbox->addStretch(1);
    mbox->addItem(vbox);
    QGridLayout *conInfoForm = new QGridLayout;
    conInfoForm->setSpacing(2);
    conInfoForm->setContentsMargins(1,1,1,1);    
    conInfoForm->addWidget(new QLabel("File Name:"),0,0);
    conInfoForm->addWidget(new QLabel("Display Name:"),1,0);
    conInfoForm->addWidget(new QLabel("Mass:"),0,2);
    conInfoForm->addWidget(new QLabel("Length:"),1,2);
    conInfoForm->addWidget(new QLabel("Units:"),0,4);
    conInfoForm->addWidget(new QLabel("Durability:"),1,4);
    conInfoForm->addWidget(&cFileName,0,1);
    conInfoForm->addWidget(&cDisplayName,1,1);
    conInfoForm->addWidget(&cMass,0,3);
    conInfoForm->addWidget(&cLength,1,3);
    conInfoForm->addWidget(&cUnits,0,5);
    conInfoForm->addWidget(&cDurability,1,5);
    cMass.setFixedWidth(100);
    cLength.setFixedWidth(100);
    cUnits.setFixedWidth(100);
    cDurability.setFixedWidth(100);
    mbox->addItem(conInfoForm);
    mbox->addWidget(glConWidget);
    mbox->addWidget(conSlider);
    //glConWidget->setFixedHeight(150);
    glConWidget->setMinimumSize(1000, 100);
    QSizePolicy policy(glConWidget->sizePolicy());
    policy.setHeightForWidth(true);
    glConWidget->setSizePolicy(policy);
    
    main->setLayout(mbox);
    this->setCentralWidget(main);
    
    setWindowTitle(tr("TSRE5 v0.612 Consist Editor"));
    fileMenu = menuBar()->addMenu(tr("&File"));
    menuBar()->addMenu(tr("&Edit"));
    menuBar()->addMenu(tr("&Help"));
    
    QObject::connect(eng1, SIGNAL(engListSelected(int)),
                      this, SLOT(engListSelected(int)));
    
    QObject::connect(con1, SIGNAL(conListSelected(int)),
                      this, SLOT(conListSelected(int)));
    
    QObject::connect(this, SIGNAL(showEng(QString, QString)),
                      glShapeWidget, SLOT(showEng(QString, QString))); 
    
    QObject::connect(this, SIGNAL(showCon(int)),
                      glConWidget, SLOT(showCon(int))); 
    
    QObject::connect(conSlider, SIGNAL(valueChanged(int)),
                      this, SLOT(conSliderValueChanged(int))); 
}

ConEditorWindow::~ConEditorWindow() {
}

void ConEditorWindow::engListSelected(int id){
    currentEng = englib->eng[id];
    qDebug() << currentEng->engName;
    eName.setText(currentEng->displayName);
    QString ttype = currentEng->type;
    if(currentEng->engType.length() > 1)
        ttype += "( "+currentEng->engType+" )";
    eType.setText(ttype);
    //eBrakes;
    //eCouplings;
    eMass.setText(QString::number(currentEng->mass));
    eMaxSpeed.setText(QString::number(currentEng->maxSpeed));
    eShape.setText(currentEng->sNames[0]);
    eSize.setText(QString::number(currentEng->sizex)+" "+QString::number(currentEng->sizey)+" "+QString::number(currentEng->sizez)+" ");
    
    emit showEng(englib->eng[id]->path, englib->eng[id]->name);
}

void ConEditorWindow::conListSelected(int id){
    currentCon = ConLib::con[id];
    qDebug() << currentCon->conName;
    conSlider->setMinimum(0);
    conSlider->setMaximum(currentCon->engItems.size());
    cFileName.setText(currentCon->conName);
    cDisplayName.setText(currentCon->displayName);
    cMass.setText(QString::number(currentCon->mass));
    cLength.setText(QString::number(currentCon->conLength));
    cUnits.setText(QString::number(currentCon->engItems.size()));
    cDurability.setText(QString::number(currentCon->durability));

    emit showCon(id);

}

void ConEditorWindow::conSliderValueChanged(int val){
    if(currentCon == NULL) return;
    if(currentCon->engItems.size() < 1) return;
    if(val > currentCon->engItems.size() - 1)
        val = currentCon->engItems.size() - 1;
    float len = currentCon->engItems[val].conLength;
    conCamera->setPos(-100,2.5,42 + len);
}