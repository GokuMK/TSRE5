#include "ConEditorWindow.h"
#include <QtWidgets>
#include <QDebug>
#include "EngLib.h"
#include "ConLib.h"
#include "Eng.h"
#include "Consist.h"
#include "Game.h"
#include "EngListWidget.h"
#include "ConListWidget.h"
#include "glShapeWidget.h"

ConEditorWindow::ConEditorWindow() {
    EngLib::loadAll(Game::root);
    ConLib::loadAll(Game::root);
    glShapeWidget = new GlShapeWidget(this);
    glConWidget = new GlShapeWidget(this);
    
    eng1 = new EngListWidget();
    eng1->fillEngList();
    con1 = new ConListWidget();
    con1->fillConList();
    
    conSlider = new QScrollBar(Qt::Horizontal);
    
    QWidget* main = new QWidget();
    
    QVBoxLayout *mbox = new QVBoxLayout;
    mbox->setSpacing(2);
    mbox->setContentsMargins(1,1,1,1);
    QHBoxLayout *vbox = new QHBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    vbox->addWidget(con1);
    vbox->addWidget(eng1);
    vbox->addWidget(glShapeWidget);
    vbox->addStretch(1);
    
    mbox->addItem(vbox);
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
    
    QObject::connect(this, SIGNAL(showEng(int)),
                      glShapeWidget, SLOT(showEng(int))); 
    
    QObject::connect(this, SIGNAL(showCon(int)),
                      glConWidget, SLOT(showCon(int))); 
}

ConEditorWindow::~ConEditorWindow() {
}

void ConEditorWindow::engListSelected(int id){
    qDebug() << EngLib::eng[id]->engName;
    emit showEng(id);
}

void ConEditorWindow::conListSelected(int id){
    qDebug() << ConLib::con[id]->conName;
    emit showCon(id);
}