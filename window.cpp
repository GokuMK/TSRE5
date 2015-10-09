#include <QtWidgets>
#include "glwidget.h"
#include "window.h"
#include "Game.h"
#include "AceLib.h"
#include <QDebug>
#include "GuiFunct.h"
#include "ObjTools.h"
#include "TerrainTools.h"
#include "NaviBox.h"
#include "AboutWindow.h"
#include "PropertiesAbstract.h"
#include "PropertiesUndefined.h"
#include "PropertiesStatic.h"
#include "PropertiesTransfer.h"
#include "PropertiesPlatform.h"
#include "PropertiesSiding.h"
#include "PropertiesCarspawner.h"
#include "PropertiesDyntrack.h"
#include "PropertiesSignal.h"
#include "PropertiesPickup.h"

Window::Window() {
    
    objTools = new ObjTools("ObjTools");
    terrainTools = new TerrainTools("TerrainTools");
    naviBox = new NaviBox();
    glWidget = new GLWidget;
    aboutWindow = new AboutWindow();
    
    objProperties.push_back(new PropertiesStatic);
    objProperties.push_back(new PropertiesTransfer);
    objProperties.push_back(new PropertiesPlatform);
    objProperties.push_back(new PropertiesSiding);
    objProperties.push_back(new PropertiesCarspawner);
    PropertiesDyntrack * propertiesDyntrack = new PropertiesDyntrack();
    objProperties.push_back(propertiesDyntrack);
    objProperties.push_back(new PropertiesSignal);
    objProperties.push_back(new PropertiesPickup);
    // last 
    objProperties.push_back(new PropertiesUndefined);
    
    QWidget* main = new QWidget();
    box = new QWidget();
    box2 = new QWidget();
    box->setMaximumWidth(250);
    box->setMinimumWidth(250);
    box2->setMaximumWidth(150);
    box2->setMinimumWidth(150);
    //box2->setMaximumWidth(250);
    //box2->setMinimumWidth(250);
    QVBoxLayout *mainLayout2 = new QVBoxLayout;
    mainLayout2->setMargin(0);
    mainLayout2->setSpacing(0);
    mainLayout2->setContentsMargins(0,0,0,0);
    mainLayout2->addWidget(objTools);
    mainLayout2->addWidget(terrainTools);
    mainLayout2->addWidget(naviBox);
    mainLayout2->setAlignment(naviBox, Qt::AlignBottom);
    box->setLayout(mainLayout2);
    
    
    QVBoxLayout *mainLayout3 = new QVBoxLayout;
    mainLayout3->setContentsMargins(0,0,0,0);
    mainLayout2->setMargin(0);
    mainLayout2->setSpacing(0);
    //mainLayout3->addWidget(propertiesUndefined);
    
    for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
        if(*it == NULL) continue;
        (*it)->hide();
        //console.log(obj.type);
        mainLayout3->addWidget(*it);
    }
    
    //mainLayout3->addWidget(terrainTools);
    //mainLayout3->setAlignment(naviBox, Qt::AlignBottom);
    box2->setLayout(mainLayout3);
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(3);
    mainLayout->setSpacing(3);
    mainLayout->addWidget(box2);
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
    setWindowTitle(tr("TSRE5 v0.601"));
    
    // MENUBAR
    // Route
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
    // Edit
    editMenu = menuBar()->addMenu(tr("&Edit"));
    // View
    viewMenu = menuBar()->addMenu(tr("&View"));
    propertiesAction = GuiFunct::newMenuCheckAction(tr("&Properties"), this); 
    viewMenu->addAction(propertiesAction);
    QObject::connect(propertiesAction, SIGNAL(triggered(bool)), this, SLOT(hideShowPropertiesWidget(bool)));
    toolsAction = GuiFunct::newMenuCheckAction(tr("&Tools"), this); 
    viewMenu->addAction(toolsAction);
    QObject::connect(toolsAction, SIGNAL(triggered(bool)), this, SLOT(hideShowToolWidget(bool)));
    viewMenu->addSeparator();
    QAction* viewWorldGrid = GuiFunct::newMenuCheckAction(tr("&World Grid"), this); 
    viewMenu->addAction(viewWorldGrid);
    QObject::connect(viewWorldGrid, SIGNAL(triggered(bool)), this, SLOT(viewWorldGrid(bool)));
    QAction* viewTileGrid = GuiFunct::newMenuCheckAction(tr("&Tile Grid"), this); 
    viewMenu->addAction(viewTileGrid);
    QObject::connect(viewTileGrid, SIGNAL(triggered(bool)), this, SLOT(viewTileGrid(bool)));    
    QAction* viewInteractives = GuiFunct::newMenuCheckAction(tr("&Interactives"), this); 
    viewMenu->addAction(viewInteractives);
    QObject::connect(viewInteractives, SIGNAL(triggered(bool)), this, SLOT(viewInteractives(bool)));
    QAction* viewTrackDbLines = GuiFunct::newMenuCheckAction(tr("&TrackDB Lines"), this); 
    viewMenu->addAction(viewTrackDbLines);
    QObject::connect(viewTrackDbLines, SIGNAL(triggered(bool)), this, SLOT(viewTrackDbLines(bool)));    
    QAction* viewTsectionLines = GuiFunct::newMenuCheckAction(tr("&Tsection Lines"), this); 
    viewMenu->addAction(viewTsectionLines);
    QObject::connect(viewTsectionLines, SIGNAL(triggered(bool)), this, SLOT(viewTsectionLines(bool)));
    QAction* viewPointer3d = GuiFunct::newMenuCheckAction(tr("&3D Pointer"), this); 
    viewMenu->addAction(viewPointer3d);
    QObject::connect(viewPointer3d, SIGNAL(triggered(bool)), this, SLOT(viewPointer3d(bool)));

    // Help
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    
    if(Game::toolsHidden){
        box->hide();
        box2->hide();
        menuBar()->hide();
    }
    //box2->hide();
    
    
    QObject::connect(this, SIGNAL(sendMsg(QString)),
                      glWidget, SLOT(msg(QString)));
    
    //ObjTools <-> qlWidget
    QObject::connect(objTools, SIGNAL(sendMsg(QString)), glWidget, SLOT(msg(QString)));
    QObject::connect(objTools, SIGNAL(sendMsg(QString, bool)), glWidget, SLOT(msg(QString, bool)));
    QObject::connect(objTools, SIGNAL(sendMsg(QString, int)), glWidget, SLOT(msg(QString, int)));
    QObject::connect(objTools, SIGNAL(sendMsg(QString, float)), glWidget, SLOT(msg(QString, float)));
    QObject::connect(objTools, SIGNAL(sendMsg(QString, QString)), glWidget, SLOT(msg(QString, QString)));
    
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), objTools, SLOT(msg(QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString)), objTools, SLOT(msg(QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, bool)), objTools, SLOT(msg(QString, bool)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, int)), objTools, SLOT(msg(QString, int)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, float)), objTools, SLOT(msg(QString, float)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), objTools, SLOT(msg(QString, QString)));
    
    ///
    
    QObject::connect(glWidget, SIGNAL(naviInfo(int, int, int, int)),
                      naviBox, SLOT(naviInfo(int, int, int, int)));
    
    QObject::connect(glWidget, SIGNAL(routeLoaded(Route*)),
                      objTools, SLOT(routeLoaded(Route*)));
    
    QObject::connect(objTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(terrainTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));   
    
    QObject::connect(propertiesDyntrack, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(glWidget, SIGNAL(flexData(int, int, float*)),
                      propertiesDyntrack, SLOT(flexData(int, int, float*)));
    
    QObject::connect(terrainTools, SIGNAL(setPaintBrush(Brush*)),
                      glWidget, SLOT(setPaintBrush(Brush*)));   
    
    QObject::connect(glWidget, SIGNAL(setBrushTextureId(int)),
                      terrainTools, SLOT(setBrushTextureId(int)));   
    
    QObject::connect(naviBox, SIGNAL(jumpTo(int, int)),
                      glWidget, SLOT(jumpTo(int, int)));
    
    QObject::connect(glWidget, SIGNAL(itemSelected(Ref::RefItem*)),
                      objTools, SLOT(itemSelected(Ref::RefItem*)));

    QObject::connect(glWidget, SIGNAL(showProperties(WorldObj*)),
                      this, SLOT(showProperties(WorldObj*)));
    
    QObject::connect(this, SIGNAL(exitNow()),
                      aboutWindow, SLOT(exitNow())); 
    
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
    aboutWindow->show();
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

void Window::showProperties(WorldObj* obj){
    // hide all
    for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
        if(*it == NULL) continue;
        (*it)->hide();
    }
    // show 
    for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
        if(*it == NULL) continue;
        if(!(*it)->support(obj)) continue;
        (*it)->show();
        (*it)->showObj(obj);
        return;
    }
}

void Window::hideShowPropertiesWidget(bool show){
    if(show) box2->show();
    else box2->hide();
}

void Window::hideShowToolWidget(bool show){
    if(show) box->show();
    else box->hide();
}

void Window::viewWorldGrid(bool show){
    Game::viewWorldGrid = show;
}
void Window::viewTileGrid(bool show){
    Game::viewTileGrid = show;
}
void Window::viewInteractives(bool show){
    Game::viewInteractives = show;
}
void Window::viewTrackDbLines(bool show){
    Game::viewTrackDbLines = show;
}
void Window::viewTsectionLines(bool show){
    Game::viewTsectionLines = show;
}
void Window::viewPointer3d(bool show){
    Game::viewPointer3d = show;
}
//void Window::exitNow(){
//    this->hide();
//}