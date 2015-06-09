#include <QtWidgets>

#include "glwidget.h"
#include "window.h"
#include "Game.h"
#include "AceLib.h"
#include <QDebug>

Window::Window() {
    objTools = new ObjTools("ObjTools");
    terrainTools = new TerrainTools("TerrainTools");
    naviBox = new NaviBox();
    glWidget = new GLWidget;
    
    QWidget* main = new QWidget();
    QWidget* box = new QWidget();
    QWidget* box2 = new QWidget();
    box->setMaximumWidth(250);
    box->setMinimumWidth(250);
    //box2->setMaximumWidth(250);
    //box2->setMinimumWidth(250);
    QVBoxLayout *mainLayout2 = new QVBoxLayout;
    mainLayout2->setContentsMargins(0,0,0,0);
    mainLayout2->addWidget(objTools);
    mainLayout2->addWidget(terrainTools);
    mainLayout2->addWidget(naviBox);
    mainLayout2->setAlignment(naviBox, Qt::AlignBottom);
    box->setLayout(mainLayout2);
    
    //QVBoxLayout *mainLayout3 = new QVBoxLayout;
    //mainLayout3->setContentsMargins(0,0,0,0);
    //mainLayout3->addWidget(terrainTools);
    //mainLayout3->setAlignment(naviBox, Qt::AlignBottom);
    //box2->setLayout(mainLayout3);
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(box);
    //mainLayout->addWidget(box2);
    //terrainTools->hide();
    //mainLayout->addWidget(naviBox);
    main->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    
    hideAllTools();
    objTools->show();
    
    this->setCentralWidget(main);
    setWindowTitle(tr("TSRE5 v0.501"));
    
    saveAction = new QAction(tr("&Save"), this);
    QObject::connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    createPathsAction = new QAction(tr("&Save Paths"), this);
    QObject::connect(createPathsAction, SIGNAL(triggered()), this, SLOT(createPaths()));
    exitAction = new QAction(tr("&Exit"), this);
    QObject::connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    routeMenu = menuBar()->addMenu(tr("&Route"));
    routeMenu->addAction(saveAction);
    routeMenu->addAction(createPathsAction);
    routeMenu->addAction(exitAction);
    
    editMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu = menuBar()->addMenu(tr("&View"));
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    
    if(Game::toolsHidden){
        box->hide();
        menuBar()->hide();
    }
    
    QObject::connect(glWidget, SIGNAL(naviInfo(int, int, int, int)),
                      naviBox, SLOT(naviInfo(int, int, int, int)));
    
    QObject::connect(glWidget, SIGNAL(routeLoaded(Route*)),
                      objTools, SLOT(routeLoaded(Route*)));
    
    QObject::connect(objTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(terrainTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));   

    QObject::connect(naviBox, SIGNAL(jumpTo(int, int)),
                      glWidget, SLOT(jumpTo(int, int)));
    
    QObject::connect(glWidget, SIGNAL(itemSelected(int)),
                      objTools, SLOT(itemSelected(int)));

    QObject::connect(this, SIGNAL(sendMsg(QString)),
                      glWidget, SLOT(msg(QString)));
    
    QObject::connect(this, SIGNAL(exitNow()),
                      &aboutWindow, SLOT(exitNow()));
    
    QObject::connect(glWidget, SIGNAL(setToolbox(QString)),
                      this, SLOT(setToolbox(QString)));
}

void Window::keyPressEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::closeEvent(QCloseEvent * event ){
    //qDebug() << "Aaaa";
    emit exitNow();
    QWidget::closeEvent(event);
}

void Window::save(){
    emit sendMsg(QString("save"));
}

void Window::createPaths(){
    emit sendMsg(QString("createPaths"));
}

void Window::about(){
    aboutWindow.show();
}

void Window::setToolbox(QString name){
    if(name == "objTools"){
        hideAllTools();
        objTools->show();
    }
    if(name == "terrainTools"){
        hideAllTools();
        terrainTools->show();
    }
}

void Window::hideAllTools(){
    objTools->hide();
    terrainTools->hide();
}

//void Window::exitNow(){
//    this->hide();
//}