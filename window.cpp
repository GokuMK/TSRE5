/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <QtWidgets>
#include "glwidget.h"
#include "window.h"
#include "Game.h"
#include "AceLib.h"
#include <QDebug>
#include "GuiFunct.h"
#include "ObjTools.h"
#include "TerrainTools.h"
#include "GeoTools.h"
#include "ActivityTools.h"
#include "NaviBox.h"
#include "ShapeViewWindow.h"
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
#include "PropertiesForest.h"
#include "PropertiesSoundSource.h"
#include "PropertiesSpeedpost.h"
#include "PropertiesTrackObj.h"
#include "PropertiesGroup.h"
#include "PropertiesRuler.h"
#include "PropertiesSoundRegion.h"
#include "NaviWindow.h"
#include "UnsavedDialog.h"

Window::Window() {
    
    objTools = new ObjTools("ObjTools");
    terrainTools = new TerrainTools("TerrainTools");
    geoTools = new GeoTools("GeoTools");
    activityTools = new ActivityTools("ActivityTools");
    //naviBox = new NaviBox();
    glWidget = new GLWidget(this);
    shapeViewWindow = new ShapeViewWindow(this);
    aboutWindow = new AboutWindow();
    naviWindow = new NaviWindow();
    
    objProperties.push_back(new PropertiesStatic);
    objProperties.push_back(new PropertiesTransfer);
    objProperties.push_back(new PropertiesPlatform);
    objProperties.push_back(new PropertiesSiding);
    objProperties.push_back(new PropertiesCarspawner);
    PropertiesDyntrack * propertiesDyntrack = new PropertiesDyntrack();
    objProperties.push_back(propertiesDyntrack);
    objProperties.push_back(new PropertiesSignal);
    objProperties.push_back(new PropertiesPickup);
    objProperties.push_back(new PropertiesForest);
    objProperties.push_back(new PropertiesSpeedpost);
    objProperties.push_back(new PropertiesSoundSource);
    objProperties.push_back(new PropertiesTrackObj);
    objProperties.push_back(new PropertiesGroup);
    objProperties.push_back(new PropertiesRuler);
    objProperties.push_back(new PropertiesSoundRegion);
    // last 
    objProperties.push_back(new PropertiesUndefined);
    
    QWidget* main = new QWidget();
    box = new QWidget();
    box2 = new QWidget();
    box->setMaximumWidth(250);
    box->setMinimumWidth(250);
    box2->setMaximumWidth(160);
    box2->setMinimumWidth(160);
    //box2->setMaximumWidth(250);
    //box2->setMinimumWidth(250);
    QVBoxLayout *mainLayout2 = new QVBoxLayout;
    mainLayout2->setMargin(0);
    mainLayout2->setSpacing(0);
    mainLayout2->setContentsMargins(0,0,0,0);
    mainLayout2->addWidget(objTools);
    mainLayout2->addWidget(terrainTools);
    mainLayout2->addWidget(geoTools);
    mainLayout2->addWidget(activityTools);
    //mainLayout2->addWidget(naviBox);
    //mainLayout2->setAlignment(naviBox, Qt::AlignBottom);
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
    
    this->setCentralWidget(main);
    setWindowTitle(Game::AppName+" "+Game::AppVersion+" Route Editor");
    
    // MENUBAR
    // Route
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    QObject::connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    createPathsAction = new QAction(tr("&Save Paths"), this);
    QObject::connect(createPathsAction, SIGNAL(triggered()), this, SLOT(createPaths()));
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));
    QObject::connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    trkEditr = new QAction(tr("&Edit route settings"), this);
    QObject::connect(trkEditr, SIGNAL(triggered()), glWidget, SLOT(showTrkEditr()));
    routeMenu = menuBar()->addMenu(tr("&Route"));
    routeMenu->addAction(saveAction);
    routeMenu->addAction(createPathsAction);
    routeMenu->addAction(trkEditr);
    routeMenu->addAction(exitAction);
    // Edit
    editMenu = menuBar()->addMenu(tr("&Edit"));
    undoAction = new QAction(tr("&Undo"), this); 
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    QObject::connect(undoAction, SIGNAL(triggered()), glWidget, SLOT(editUndo()));
    editMenu->addAction(undoAction);
    copyAction = new QAction(tr("&Copy"), this); 
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    QObject::connect(copyAction, SIGNAL(triggered()), glWidget, SLOT(editCopy()));
    editMenu->addAction(copyAction);
    pasteAction = new QAction(tr("&Paste"), this); 
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    QObject::connect(pasteAction, SIGNAL(triggered()), glWidget, SLOT(editPaste()));
    editMenu->addAction(pasteAction);
    // View
    viewMenu = menuBar()->addMenu(tr("&View"));
    //toolsAction = GuiFunct::newMenuCheckAction(tr("&Tools"), this); 
    //viewMenu->addAction(toolsAction);
    //QObject::connect(toolsAction, SIGNAL(triggered(bool)), this, SLOT(hideShowToolWidget(bool)));
    QAction* viewWorldGrid = GuiFunct::newMenuCheckAction(tr("&World Grid"), this); 
    viewMenu->addAction(viewWorldGrid);
    QObject::connect(viewWorldGrid, SIGNAL(triggered(bool)), this, SLOT(viewWorldGrid(bool)));
    QAction* viewTileGrid = GuiFunct::newMenuCheckAction(tr("&Tile Grid"), this); 
    viewMenu->addAction(viewTileGrid);
    QObject::connect(viewTileGrid, SIGNAL(triggered(bool)), this, SLOT(viewTileGrid(bool)));    
    QAction* viewTerrainGrid = GuiFunct::newMenuCheckAction(tr("&Terrain Grid"), this, false); 
    viewMenu->addAction(viewTerrainGrid);
    QObject::connect(viewTerrainGrid, SIGNAL(triggered(bool)), this, SLOT(viewTerrainGrid(bool)));    
    QAction* showWorldObjPivotPoints = GuiFunct::newMenuCheckAction(tr("&WorldObj Markers"), this, false); 
    viewMenu->addAction(showWorldObjPivotPoints);
    QObject::connect(showWorldObjPivotPoints, SIGNAL(triggered(bool)), this, SLOT(showWorldObjPivotPointsEnabled(bool)));
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
    QAction* viewMarkers = GuiFunct::newMenuCheckAction(tr("&Markers"), this, false); 
    viewMenu->addAction(viewMarkers);
    QObject::connect(viewMarkers, SIGNAL(triggered(bool)), this, SLOT(viewMarkers(bool)));
    QAction* viewSnapable = GuiFunct::newMenuCheckAction(tr("&Snapable Points"), this, false); 
    viewMenu->addAction(viewSnapable);
    QObject::connect(viewSnapable, SIGNAL(triggered(bool)), this, SLOT(viewSnapable(bool)));
    // Tools
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    propertiesAction = GuiFunct::newMenuCheckAction(tr("&Properties"), this); 
    toolsMenu->addAction(propertiesAction);
    QObject::connect(propertiesAction, SIGNAL(triggered(bool)), this, SLOT(hideShowPropertiesWidget(bool)));
    naviAction = GuiFunct::newMenuCheckAction(tr("&Navi Window"), this); 
    toolsMenu->addAction(naviAction);
    QObject::connect(naviAction, SIGNAL(triggered(bool)), this, SLOT(hideShowNaviWidget(bool)));
    shapeViewAction = GuiFunct::newMenuCheckAction(tr("&Shape View Window"), this, false); 
    toolsMenu->addAction(shapeViewAction);
    QObject::connect(shapeViewAction, SIGNAL(triggered(bool)), this, SLOT(hideShowShapeViewWidget(bool)));
    toolsMenu->addSeparator();
    objectsAction = GuiFunct::newMenuCheckAction(tr("&Objects"), this); 
    objectsAction->setShortcut(QKeySequence("F1"));
    toolsMenu->addAction(objectsAction);
    QObject::connect(objectsAction, SIGNAL(triggered(bool)), this, SLOT(showToolsObject(bool)));
    terrainAction = GuiFunct::newMenuCheckAction(tr("&Terrain"), this); 
    terrainAction->setChecked(false);    
    terrainAction->setShortcut(QKeySequence("F2"));
    toolsMenu->addAction(terrainAction);
    QObject::connect(terrainAction, SIGNAL(triggered(bool)), this, SLOT(showToolsTerrain(bool)));
    geoAction = GuiFunct::newMenuCheckAction(tr("&Geo"), this); 
    geoAction->setChecked(false);    
    geoAction->setShortcut(QKeySequence("F3"));
    toolsMenu->addAction(geoAction);
    QObject::connect(geoAction, SIGNAL(triggered(bool)), this, SLOT(showToolsGeo(bool)));
    activityAction = GuiFunct::newMenuCheckAction(tr("&Activity"), this); 
    activityAction->setChecked(false);    
    activityAction->setShortcut(QKeySequence("F4"));
    toolsMenu->addAction(activityAction);
    QObject::connect(activityAction, SIGNAL(triggered(bool)), this, SLOT(showToolsActivity(bool)));
    // Settigs
    terrainCameraAction = GuiFunct::newMenuCheckAction(tr("&Stick Camera To Terrain"), this); 
    terrainCameraAction->setChecked(false);
    QObject::connect(terrainCameraAction, SIGNAL(triggered(bool)), this, SLOT(terrainCamera(bool)));
    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(terrainCameraAction);
    // Help
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    
    hideAllTools();
    objTools->show();
    
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
    
    QObject::connect(glWidget, SIGNAL(sendMsg(QString)), objTools, SLOT(msg(QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, bool)), objTools, SLOT(msg(QString, bool)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, int)), objTools, SLOT(msg(QString, int)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, float)), objTools, SLOT(msg(QString, float)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), objTools, SLOT(msg(QString, QString)));
    
    QObject::connect(naviWindow, SIGNAL(sendMsg(QString)), glWidget, SLOT(msg(QString)));
    QObject::connect(naviWindow, SIGNAL(sendMsg(QString, bool)), glWidget, SLOT(msg(QString, bool)));
    QObject::connect(naviWindow, SIGNAL(sendMsg(QString, int)), glWidget, SLOT(msg(QString, int)));
    QObject::connect(naviWindow, SIGNAL(sendMsg(QString, float)), glWidget, SLOT(msg(QString, float)));
    QObject::connect(naviWindow, SIGNAL(sendMsg(QString, QString)), glWidget, SLOT(msg(QString, QString)));
    ///
    QObject::connect(glWidget, SIGNAL(sendMsg(QString)), shapeViewWindow, SLOT(msg(QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, bool)), shapeViewWindow, SLOT(msg(QString, bool)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, int)), shapeViewWindow, SLOT(msg(QString, int)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, float)), shapeViewWindow, SLOT(msg(QString, float)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), shapeViewWindow, SLOT(msg(QString, QString)));
    
    QObject::connect(glWidget, SIGNAL(naviInfo(int, int)),
                      naviWindow, SLOT(naviInfo(int, int)));
    
    QObject::connect(glWidget, SIGNAL(posInfo(PreciseTileCoordinate*)),
                      naviWindow, SLOT(posInfo(PreciseTileCoordinate*)));
    
    QObject::connect(glWidget, SIGNAL(pointerInfo(float*)),
                      naviWindow, SLOT(pointerInfo(float*)));
    
    QObject::connect(glWidget, SIGNAL(mkrList(std::unordered_map<std::string, Coords*>)),
                      naviWindow, SLOT(mkrList(std::unordered_map<std::string, Coords*>)));
    
    QObject::connect(glWidget, SIGNAL(routeLoaded(Route*)),
                      objTools, SLOT(routeLoaded(Route*)));
    
    QObject::connect(glWidget, SIGNAL(routeLoaded(Route*)),
                      activityTools, SLOT(routeLoaded(Route*)));
    
    QObject::connect(objTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(terrainTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));   
    
    QObject::connect(geoTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));   
    
    for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
        if(*it == NULL) continue;
        QObject::connect((*it), SIGNAL(enableTool(QString)),
            glWidget, SLOT(enableTool(QString)));   
    }
    
    QObject::connect(propertiesDyntrack, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(glWidget, SIGNAL(flexData(int, int, float*)),
                      propertiesDyntrack, SLOT(flexData(int, int, float*)));
    
    QObject::connect(terrainTools, SIGNAL(setPaintBrush(Brush*)),
                      glWidget, SLOT(setPaintBrush(Brush*)));   
    
    QObject::connect(glWidget, SIGNAL(setBrushTextureId(int)),
                      terrainTools, SLOT(setBrushTextureId(int)));   
    
    QObject::connect(naviWindow, SIGNAL(jumpTo(PreciseTileCoordinate*)),
                      glWidget, SLOT(jumpTo(PreciseTileCoordinate*)));
    
    QObject::connect(glWidget, SIGNAL(itemSelected(Ref::RefItem*)),
                      objTools, SLOT(itemSelected(Ref::RefItem*)));

    QObject::connect(glWidget, SIGNAL(showProperties(WorldObj*)),
                      this, SLOT(showProperties(WorldObj*)));
    
    QObject::connect(glWidget, SIGNAL(updateProperties(WorldObj*)),
                      this, SLOT(updateProperties(WorldObj*)));
    
    QObject::connect(this, SIGNAL(exitNow()),
                      aboutWindow, SLOT(exitNow())); 
    
    QObject::connect(naviWindow, SIGNAL(windowClosed()),
                      this, SLOT(naviWindowClosed())); 
    
    QObject::connect(shapeViewWindow, SIGNAL(windowClosed()),
                      this, SLOT(shapeVeiwWindowClosed())); 
    
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
    std::vector<QString> unsavedItems;
    glWidget->getUnsavedInfo(unsavedItems);
    if(unsavedItems.size() == 0){
        qDebug() << "nic do zapisania";
        emit exitNow();
        event->accept();
        qApp->quit();
        return;
    }
    
    UnsavedDialog unsavedDialog;
    unsavedDialog.setWindowTitle("Save changes?");
    unsavedDialog.setMsg("Save changes in route?");
    for(int i = 0; i < unsavedItems.size(); i++){
        unsavedDialog.items.addItem(unsavedItems[i]);
    }
    unsavedDialog.exec();
    if(unsavedDialog.changed == 0){
        event->ignore();
        return;
    }
    if(unsavedDialog.changed == 2){
        emit exitNow();
        event->accept();
        qApp->quit();
        return;
    }
    
    emit save();

    emit exitNow();
    event->accept();
    qApp->quit();
}

void Window::save(){
    emit sendMsg(QString("save"));
}

void Window::createPaths(){
    QMessageBox msgBox;
    msgBox.setText("This will delete all your existing paths and create new simple paths! Continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    switch (msgBox.exec()) {
      case QMessageBox::Yes:
          emit sendMsg(QString("createPaths"));
          break;
      case QMessageBox::No:
          break;
      default:
          break;
    }
}

void Window::terrainCamera(bool val){
    Game::cameraStickToTerrain = val;
}

void Window::about(){
    aboutWindow->show();
}

void Window::showTerrainTreeEditr(){
    emit sendMsg(QString("showTerrainTreeEditr"));
}

void Window::showToolsObject(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("objTools");
    } else {
        hideShowToolWidget(false);
    }
}

void Window::showToolsTerrain(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("terrainTools");
    } else {
        hideShowToolWidget(false);
    }
}

void Window::showToolsGeo(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("geoTools");
    } else {
        hideShowToolWidget(false);
    }
}

void Window::showToolsActivity(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("activityTools");
    } else {
        hideShowToolWidget(false);
    }
}

void Window::setToolbox(QString name){
    if(name == "objTools"){
        hideAllTools();
        objTools->show();
        objectsAction->setChecked(true);
    }
    if(name == "terrainTools"){
        hideAllTools();
        terrainTools->show();
        terrainAction->setChecked(true);       
    }
    if(name == "geoTools"){
        hideAllTools();
        geoTools->show();
        geoAction->setChecked(true);
    }
    if(name == "activityTools"){
        hideAllTools();
        activityTools->show();
        activityAction->setChecked(true);
    }
}

void Window::hideAllTools(){
    objTools->hide();
    terrainTools->hide();
    geoTools->hide();
    activityTools->hide();
    objectsAction->setChecked(false);
    terrainAction->setChecked(false);     
    geoAction->setChecked(false);
    activityAction->setChecked(false);
}

void Window::showProperties(WorldObj* obj){
    // hide all
    for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
        if(*it == NULL) continue;
        (*it)->hide();
    }
    if(obj == NULL) return;
    // show 
    for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
        if(*it == NULL) continue;
        if(!(*it)->support(obj)) continue;
        (*it)->show();
        (*it)->showObj(obj);
        return;
    }
}

void Window::updateProperties(WorldObj* obj){
    if(obj == NULL) return;
    // show 
    for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
        if(*it == NULL) continue;
        if((*it)->isVisible() && (*it)->support(obj)){
            (*it)->updateObj(obj);
            return;
        }
    }
}

void Window::hideShowPropertiesWidget(bool show){
    if(show) box2->show();
    else box2->hide();
}

void Window::hideShowShapeViewWidget(bool show){
    if(show) shapeViewWindow->show();
    else shapeViewWindow->hide();
}

void Window::hideShowNaviWidget(bool show){
    if(show) naviWindow->show();
    else naviWindow->hide();
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
void Window::viewTerrainGrid(bool show){
    Game::viewTerrainGrid = show;
}
void Window::showWorldObjPivotPointsEnabled(bool show){
    Game::showWorldObjPivotPoints = show;
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
void Window::viewMarkers(bool show){
    Game::viewMarkers = show;
}
void Window::viewSnapable(bool show){
    Game::viewSnapable = show;
}
void Window::show(){
    naviWindow->move(0, this->height() - naviWindow->height() );
    naviWindow->show();
    QMainWindow::show();
}
void Window::naviWindowClosed(){
    naviAction->blockSignals(true);
    naviAction->setChecked(false);
    naviAction->blockSignals(false);
}
void Window::shapeVeiwWindowClosed(){
    shapeViewAction->blockSignals(true);
    shapeViewAction->setChecked(false);
    shapeViewAction->blockSignals(false);
}
//void Window::exitNow(){
//    this->hide();
//}