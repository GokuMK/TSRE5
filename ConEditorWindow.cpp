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
#include "GuiFunct.h"
#include "ConUnitsWidget.h"
#include "AboutWindow.h"

ConEditorWindow::ConEditorWindow() : QMainWindow() {
    aboutWindow = new AboutWindow();
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
    eng2 = new EngListWidget();
    eng2->englib = englib;
    eng2->fillEngList();
    units = new ConUnitsWidget();
    units->englib = englib;
    
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
    QGridLayout *vbox = new QGridLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    vbox->addWidget(con1,0,0);
    vbox->addWidget(units,0,1);
    vbox->addWidget(eng1,0,2);
    vbox->addWidget(eng2,0,3);
    engInfo = new QWidget(this);
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
    engInfo->setLayout(engInfoLayout);
    vbox->addWidget(engInfo,0,4);
    //vbox->addStretch(1);
    mbox->addItem(vbox);
    conInfo = new QWidget(this);
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
    conInfo->setLayout(conInfoForm);
    mbox->addWidget(conInfo);
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
    viewMenu = menuBar()->addMenu(tr("&View"));
    vConList = GuiFunct::newMenuCheckAction(tr("&Consist List"), this); 
    viewMenu->addAction(vConList);
    QObject::connect(vConList, SIGNAL(triggered(bool)), this, SLOT(viewConList(bool)));
    vEngList1 = GuiFunct::newMenuCheckAction(tr("&Eng List 1"), this); 
    viewMenu->addAction(vEngList1);
    QObject::connect(vEngList1, SIGNAL(triggered(bool)), this, SLOT(viewEngList1(bool)));
    vEngList2 = GuiFunct::newMenuCheckAction(tr("&Eng List 2"), this); 
    viewMenu->addAction(vEngList2);
    QObject::connect(vEngList2, SIGNAL(triggered(bool)), this, SLOT(viewEngList2(bool)));
    vConUnits = GuiFunct::newMenuCheckAction(tr("&Consist Units"), this); 
    viewMenu->addAction(vConUnits);
    QObject::connect(vConUnits, SIGNAL(triggered(bool)), this, SLOT(viewConUnits(bool)));
    vEngView = GuiFunct::newMenuCheckAction(tr("&Eng View"), this); 
    viewMenu->addAction(vEngView);
    QObject::connect(vEngView, SIGNAL(triggered(bool)), this, SLOT(viewEngView(bool)));
    vConView = GuiFunct::newMenuCheckAction(tr("&Con View"), this); 
    viewMenu->addAction(vConView);
    QObject::connect(vConView, SIGNAL(triggered(bool)), this, SLOT(viewConView(bool)));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu->addAction(aboutAction);
    
    QObject::connect(eng1, SIGNAL(engListSelected(int)),
                      this, SLOT(engListSelected(int)));
    
    QObject::connect(eng2, SIGNAL(engListSelected(int)),
                      this, SLOT(engListSelected(int)));
    
    QObject::connect(con1, SIGNAL(conListSelected(int)),
                      this, SLOT(conListSelected(int)));
    
    QObject::connect(this, SIGNAL(showEng(QString, QString)),
                      glShapeWidget, SLOT(showEng(QString, QString))); 
    
    QObject::connect(this, SIGNAL(showCon(int)),
                      glConWidget, SLOT(showCon(int))); 
    
    QObject::connect(conSlider, SIGNAL(valueChanged(int)),
                      this, SLOT(conSliderValueChanged(int))); 
    
    vEngList2->trigger();
    vConUnits->trigger();
}

ConEditorWindow::~ConEditorWindow() {
}

void ConEditorWindow::about(){
    aboutWindow->show();
}

void ConEditorWindow::viewConList(bool show){
    if(show) con1->show();
    else con1->hide();
}
void ConEditorWindow::viewEngList1(bool show){
    if(show) eng1->show();
    else eng1->hide();
}
void ConEditorWindow::viewEngList2(bool show){
    if(show) eng2->show();
    else eng2->hide();
}
void ConEditorWindow::viewConUnits(bool show){
    if(show) units->show();
    else units->hide();
}
void ConEditorWindow::viewEngView(bool show){
    if(show) engInfo->show();
    else engInfo->hide();
}
void ConEditorWindow::viewConView(bool show){
    if(show) glConWidget->show();
    else glConWidget->hide();
    if(show) conInfo->show();
    else conInfo->hide();
    if(show) conSlider->show();
    else conSlider->hide();
    
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
    units->setCon(currentCon);
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