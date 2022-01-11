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
#include "RouteEditorGLWidget.h"
#include "RouteEditorWindow.h"
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
#include "SoundManager.h"
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
#include "PropertiesLevelCr.h"
#include "PropertiesSoundRegion.h"
#include "PropertiesTerrain.h"
#include "PropertiesActivityObject.h"
#include "PropertiesTrackItem.h"
#include "PropertiesActivityPath.h"
#include "PropertiesConsist.h"
#include "NaviWindow.h"
#include "ErrorMessagesWindow.h"
#include "ClientUsersWindow.h"
#include "ErrorMessagesLib.h"
#include "UnsavedDialog.h"
#include "ActivityEventWindow.h"
#include "ActivityEventProperties.h"
#include "ActivityServiceWindow.h"
#include "ActivityServiceProperties.h"
#include "ActivityTrafficWindow.h"
#include "ActivityTrafficProperties.h"
#include "ActivityTimetableWindow.h"
#include "ActivityTimetableProperties.h"
#include "RouteEditorClient.h"

RouteEditorWindow::RouteEditorWindow() {

    objTools = new ObjTools("ObjTools");
    terrainTools = new TerrainTools("TerrainTools");
    geoTools = new GeoTools("GeoTools");
    activityTools = new ActivityTools("ActivityTools");
    //naviBox = new NaviBox();
    glWidget = new RouteEditorGLWidget(this);
    shapeViewWindow = new ShapeViewWindow(this);
    aboutWindow = new AboutWindow(this);
    naviWindow = new NaviWindow(this);
    errorMessagesWindow = ErrorMessagesLib::GetWindow(this);
    clientUsersWindow = new ClientUsersWindow(this);
    activityEventWindow = new ActivityEventWindow(this);
    activityServiceWindow = new ActivityServiceWindow(this);
    activityTrafficWindow = new ActivityTrafficWindow(this);
    activityTimetableWindow = new ActivityTimetableWindow(this);
    
    objProperties["Static"] = new PropertiesStatic;
    objProperties["Transfer"] = new PropertiesTransfer;
    objProperties["Platform"] = new PropertiesPlatform;
    objProperties["Siding"] = new PropertiesSiding;
    objProperties["Carspawner"] = new PropertiesCarspawner;
    objProperties["Dyntrack"] = new PropertiesDyntrack;
    objProperties["Signal"] = new PropertiesSignal;
    objProperties["Pickup"] = new PropertiesPickup;
    objProperties["Forest"] = new PropertiesForest;
    objProperties["Speedpost"] = new PropertiesSpeedpost;
    objProperties["SoundSource"] = new PropertiesSoundSource;
    objProperties["TrackObj"] = new PropertiesTrackObj;
    objProperties["Group"] = new PropertiesGroup;
    objProperties["Ruler"] = new PropertiesRuler;
    objProperties["SoundRegion"] = new PropertiesSoundRegion;
    objProperties["LevelCr"] = new PropertiesLevelCr;
    objProperties["Terrain"] = new PropertiesTerrain;
    objProperties["ActivityObject"] = new PropertiesActivityObject;
    objProperties["TrackItem"] = new PropertiesTrackItem;
    objProperties["ActivityPath"] = new PropertiesActivityPath;
    objProperties["ActivityConsist"] = new PropertiesConsist;
    // last 
    objProperties["Undefined"] = new PropertiesUndefined;
    
    QWidget* main = new QWidget();
    box = new QWidget(this);
    box2 = new QWidget(this);
    box->setFixedWidth(250);
    box->setWindowTitle("Tools Window");
    box2->setWindowFilePath("Properties Window");
    box2->setMaximumWidth(160);
    box2->setMinimumWidth(160);
    //box2->setMaximumWidth(250);
    //box2->setMinimumWidth(250);
    QHBoxLayout *mainLayout2 = new QHBoxLayout; 
    mainLayout2->setMargin(0);
    mainLayout2->setSpacing(0);
    mainLayout2->setContentsMargins(0,0,0,0);
    mainLayout2->addWidget(objTools);
    mainLayout2->addWidget(terrainTools);
    mainLayout2->addWidget(geoTools);
    if(Game::serverClient == NULL){
        mainLayout2->addWidget(activityTools);
    }
    //mainLayout2->addWidget(naviBox);
    //mainLayout2->setAlignment(naviBox, Qt::AlignBottom);
    box->setLayout(mainLayout2);
    
    
    QVBoxLayout *mainLayout3 = new QVBoxLayout;
    mainLayout3->setContentsMargins(0,0,0,0);
    mainLayout2->setMargin(0);
    mainLayout2->setSpacing(0);
    //mainLayout3->addWidget(propertiesUndefined);
    
    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        it->hide();
        //console.log(obj.type);
        mainLayout3->addWidget(it);
    }
    
    //mainLayout3->addWidget(terrainTools);
    //mainLayout3->setAlignment(naviBox, Qt::AlignBottom);
    box2->setLayout(mainLayout3);

    glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(3);
    mainLayout->setSpacing(3);
    
    QString mainWindowLayout = Game::mainWindowLayout;
    if(!mainWindowLayout.contains('W')){
        mainWindowLayout += 'W';
    }
    for(int i = 0; i < mainWindowLayout.length(); i++){
        if(mainWindowLayout[i] == 'P')
            mainLayout->addWidget(box2);
        if(mainWindowLayout[i] == 'T')
            mainLayout->addWidget(box);
        if(mainWindowLayout[i] == 'W')
            mainLayout->addWidget(glWidget);
    }
    if(!mainWindowLayout.contains('T')){
        box->move(this->pos());
        box->setWindowFlags(Qt::WindowType::Tool);
    }
    if(!mainWindowLayout.contains('P')){
        box2->move(this->pos());
        box2->setWindowFlags(Qt::WindowType::Tool);
    }
    
    main->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    
    this->setCentralWidget(main);
    setWindowTitle(Game::AppName+" "+Game::AppVersion+" Route Editor");
    
    // MENUBAR
    // Route
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    QObject::connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    createPathsAction = new QAction(tr("&Create Debug Paths"), this);
    QObject::connect(createPathsAction, SIGNAL(triggered()), this, SLOT(createPaths()));
    reloadRefAction = new QAction(tr("&Reload Ref File"), this);
    QObject::connect(reloadRefAction, SIGNAL(triggered()), this, SLOT(reloadRef()));
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));
    QObject::connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    trkEditr = new QAction(tr("&Edit route settings"), this);
    QObject::connect(trkEditr, SIGNAL(triggered()), glWidget, SLOT(showTrkEditr()));
    if(Game::serverClient == NULL){
        routeMenu = menuBar()->addMenu(tr("&Route"));
        routeMenu->addAction(saveAction);
        routeMenu->addAction(reloadRefAction);
        routeMenu->addAction(createPathsAction);
        routeMenu->addAction(trkEditr);
        routeMenu->addAction(exitAction);
    } else {
        routeMenu = menuBar()->addMenu(tr("&Server"));
        routeMenu->addAction(exitAction);
    }
    // Edit
    editMenu = menuBar()->addMenu(tr("&Edit"));
    if(Undo::UndoEnabled){
        undoAction = new QAction(tr("&Undo"), this); 
        undoAction->setShortcut(QKeySequence("Ctrl+Z"));
        QObject::connect(undoAction, SIGNAL(triggered()), glWidget, SLOT(editUndo()));
        editMenu->addAction(undoAction);
    }
    copyAction = new QAction(tr("&Copy"), this); 
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    QObject::connect(copyAction, SIGNAL(triggered()), glWidget, SLOT(editCopy()));
    editMenu->addAction(copyAction);
    pasteAction = new QAction(tr("&Paste"), this); 
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    QObject::connect(pasteAction, SIGNAL(triggered()), glWidget, SLOT(editPaste()));
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    selectAction = new QAction(tr("&Select Tool"), this); 
    selectAction->setShortcut(QKeySequence("E"));
    QObject::connect(selectAction, SIGNAL(triggered()), glWidget, SLOT(editSelect()));
    editMenu->addAction(selectAction);
    // View
    viewMenu = menuBar()->addMenu(tr("&View"));
    //toolsAction = GuiFunct::newMenuCheckAction(tr("&Tools"), this); 
    //viewMenu->addAction(toolsAction);
    //QObject::connect(toolsAction, SIGNAL(triggered(bool)), this, SLOT(hideShowToolWidget(bool)));
    QAction* viewUnselectAll = new QAction(tr("&Unselect All"), this); 
    viewMenu->addAction(viewUnselectAll);
    QObject::connect(viewUnselectAll, SIGNAL(triggered()), this, SLOT(viewUnselectAll()));
    viewMenu->addSeparator();
    vViewWorldGrid = GuiFunct::newMenuCheckAction(tr("&World Grid"), this); 
    viewMenu->addAction(vViewWorldGrid);
    QObject::connect(vViewWorldGrid, SIGNAL(triggered(bool)), this, SLOT(viewWorldGrid(bool)));
    vViewTileGrid = GuiFunct::newMenuCheckAction(tr("&Tile Grid"), this); 
    viewMenu->addAction(vViewTileGrid);
    QObject::connect(vViewTileGrid, SIGNAL(triggered(bool)), this, SLOT(viewTileGrid(bool)));    
    vViewTerrainGrid = GuiFunct::newMenuCheckAction(tr("&Terrain Grid"), this, false); 
    viewMenu->addAction(vViewTerrainGrid);
    QObject::connect(vViewTerrainGrid, SIGNAL(triggered(bool)), this, SLOT(viewTerrainGrid(bool)));   
    vViewTerrainShape = GuiFunct::newMenuCheckAction(tr("&Hide Terrain Shape"), this, false); 
    viewMenu->addAction(vViewTerrainShape);
    QObject::connect(vViewTerrainShape, SIGNAL(triggered(bool)), this, SLOT(viewTerrainShape(bool)));   
    vShowWorldObjPivotPoints = GuiFunct::newMenuCheckAction(tr("&WorldObj Markers"), this, false); 
    viewMenu->addAction(vShowWorldObjPivotPoints);
    QObject::connect(vShowWorldObjPivotPoints, SIGNAL(triggered(bool)), this, SLOT(showWorldObjPivotPointsEnabled(bool)));
    vViewInteractives = GuiFunct::newMenuCheckAction(tr("&Interactives"), this); 
    viewMenu->addAction(vViewInteractives);
    QObject::connect(vViewInteractives, SIGNAL(triggered(bool)), this, SLOT(viewInteractives(bool)));
    vViewTrackDbLines = GuiFunct::newMenuCheckAction(tr("&TrackDB Lines"), this); 
    viewMenu->addAction(vViewTrackDbLines);
    QObject::connect(vViewTrackDbLines, SIGNAL(triggered(bool)), this, SLOT(viewTrackDbLines(bool)));    
    vViewTsectionLines = GuiFunct::newMenuCheckAction(tr("&Tsection Lines"), this); 
    viewMenu->addAction(vViewTsectionLines);
    QObject::connect(vViewTsectionLines, SIGNAL(triggered(bool)), this, SLOT(viewTsectionLines(bool)));
    vViewTrackItems = GuiFunct::newMenuCheckAction(tr("&TrackDB Items"), this, Game::renderTrItems); 
    viewMenu->addAction(vViewTrackItems);
    QObject::connect(vViewTrackItems, SIGNAL(triggered(bool)), this, SLOT(viewTrackItems(bool)));
    
    vViewPointer3d = GuiFunct::newMenuCheckAction(tr("&3D Pointer"), this); 
    viewMenu->addAction(vViewPointer3d);
    QObject::connect(vViewPointer3d, SIGNAL(triggered(bool)), this, SLOT(viewPointer3d(bool)));
    vViewMarkers = GuiFunct::newMenuCheckAction(tr("&Markers"), this, false); 
    viewMenu->addAction(vViewMarkers);
    QObject::connect(vViewMarkers, SIGNAL(triggered(bool)), this, SLOT(viewMarkers(bool)));
    vViewSnapable = GuiFunct::newMenuCheckAction(tr("&Snapable Points"), this, false); 
    viewMenu->addAction(vViewSnapable);
    QObject::connect(vViewSnapable, SIGNAL(triggered(bool)), this, SLOT(viewSnapable(bool)));
    QAction* vViewCompass = GuiFunct::newMenuCheckAction(tr("&Compass"), this, false); 
    viewMenu->addAction(vViewCompass);
    QObject::connect(vViewCompass, SIGNAL(triggered(bool)), this, SLOT(viewCompass(bool)));
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
    errorViewAction = GuiFunct::newMenuCheckAction(tr("&Errors and Messages"), this, false); 
    toolsMenu->addAction(errorViewAction);
    QObject::connect(errorViewAction, SIGNAL(triggered(bool)), this, SLOT(hideShowErrorMsgWidget(bool)));
    toolsMenu->addSeparator();
    objectsAndTerrainAction = GuiFunct::newMenuCheckAction(tr("&Objects and Terrain"), this); 
    toolsMenu->addAction(objectsAndTerrainAction);
    QObject::connect(objectsAndTerrainAction, SIGNAL(triggered(bool)), this, SLOT(showToolsObjectAndTerrain(bool)));
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
    terrainCameraAction->setChecked(Game::cameraStickToTerrain);
    terrainCameraAction->setShortcut(QKeySequence("/"));
    QObject::connect(terrainCameraAction, SIGNAL(triggered(bool)), this, SLOT(terrainCamera(bool)));
    mstsShadowsAction = GuiFunct::newMenuCheckAction(tr("&MSTS Shadows"), this); 
    mstsShadowsAction->setChecked(Game::mstsShadows);
    QObject::connect(mstsShadowsAction, SIGNAL(triggered(bool)), this, SLOT(mstsShadows(bool)));
    QMenu *terrainMenu = new QMenu("&Terrain Editing:");
    QAction *detailTerrainAction = new QAction(tr("&Detailed Terrain"), this);
    QObject::connect(detailTerrainAction, SIGNAL(triggered()), this, SLOT(detailedTerrainEnabled()));
    QAction *distantTerrainAction = new QAction(tr("&Distant Terrain"), this);
    QObject::connect(distantTerrainAction, SIGNAL(triggered()), this, SLOT(distantTerrainEnabled()));
    terrainMenu->addAction(detailTerrainAction);
    terrainMenu->addAction(distantTerrainAction);
    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(terrainCameraAction);
    settingsMenu->addAction(mstsShadowsAction);
    settingsMenu->addMenu(terrainMenu);
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
    } else {
        //box->show();
        //box2->show();
    }
    
    if(Game::playerMode){
        naviWindow->hide();
        errorMessagesWindow->hide();
        box->hide();
        box2->hide();
        menuBar()->hide();
        this->viewUnselectAll();
    }
    
    if(Game::serverClient != NULL){
        clientUsersWindow->show();
    }
    
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
    
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), terrainTools, SLOT(msg(QString, QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), geoTools, SLOT(msg(QString, QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), activityTools, SLOT(msg(QString, QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString)), activityTools, SLOT(msg(QString)));
    QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), activityEventWindow->eventProperties, SLOT(msg(QString, QString)));
    
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
    
    QObject::connect(glWidget, SIGNAL(mkrList(QMap<QString, Coords*>)),
                      naviWindow, SLOT(mkrList(QMap<QString, Coords*>)));
    
    QObject::connect(glWidget, SIGNAL(mkrList(QMap<QString, Coords*>)),
                      geoTools, SLOT(mkrList(QMap<QString, Coords*>)));
    
    QObject::connect(geoTools, SIGNAL(createNewTiles(QMap<int, QPair<int, int>*>)),
                      glWidget, SLOT(createNewTiles(QMap<int, QPair<int, int>*>)));
    
    QObject::connect(geoTools, SIGNAL(createNewLoTiles(QMap<int, QPair<int, int>*>)),
                      glWidget, SLOT(createNewLoTiles(QMap<int, QPair<int, int>*>)));
    
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
    
    QObject::connect(activityTools, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));   
    
    QObject::connect(activityEventWindow->eventProperties, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));   
    
    
    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        QObject::connect(it, SIGNAL(enableTool(QString)),
            glWidget, SLOT(enableTool(QString)));   
        QObject::connect(glWidget, SIGNAL(sendMsg(QString, QString)), 
                it, SLOT(msg(QString, QString)));
    }
    
    QObject::connect(objProperties["Dyntrack"], SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(glWidget, SIGNAL(flexData(int, int, float*)),
                      objProperties["Dyntrack"], SLOT(flexData(int, int, float*)));
    
    QObject::connect(objProperties["Dyntrack"], SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(objProperties["ActivityObject"], SIGNAL(sendMsg(QString)),
                      glWidget, SLOT(msg(QString)));
    
    QObject::connect(objProperties["ActivityConsist"], SIGNAL(cameraObject(GameObj*)),
                      glWidget, SLOT(setCameraObject(GameObj*)));
    
    QObject::connect(objProperties["TrackObj"], SIGNAL(setMoveStep(float)),
                      glWidget, SLOT(setMoveStep(float)));
    
    QObject::connect(objProperties["Dyntrack"], SIGNAL(setMoveStep(float)),
                      glWidget, SLOT(setMoveStep(float)));
    
    QObject::connect(terrainTools, SIGNAL(setPaintBrush(Brush*)),
                      glWidget, SLOT(setPaintBrush(Brush*)));   
    
    QObject::connect(glWidget, SIGNAL(setBrushTextureId(int)),
                      terrainTools, SLOT(setBrushTextureId(int)));   
    
    QObject::connect(naviWindow, SIGNAL(jumpTo(PreciseTileCoordinate*)),
                      glWidget, SLOT(jumpTo(PreciseTileCoordinate*)));
    
    QObject::connect(glWidget, SIGNAL(itemSelected(Ref::RefItem*)),
                      objTools, SLOT(itemSelected(Ref::RefItem*)));

    QObject::connect(glWidget, SIGNAL(showProperties(GameObj*)),
                      this, SLOT(showProperties(GameObj*)));
    
    QObject::connect(glWidget, SIGNAL(updateProperties(GameObj*)),
                      this, SLOT(updateProperties(GameObj*)));
    
    QObject::connect(this, SIGNAL(exitNow()),
                      aboutWindow, SLOT(exitNow())); 
    
    QObject::connect(naviWindow, SIGNAL(windowClosed()),
                      this, SLOT(naviWindowClosed())); 
    
    QObject::connect(errorMessagesWindow, SIGNAL(windowClosed()),
                      this, SLOT(errorMessagesWindowClosed())); 

    QObject::connect(shapeViewWindow, SIGNAL(windowClosed()),
                      this, SLOT(shapeVeiwWindowClosed())); 
    
    QObject::connect(glWidget, SIGNAL(setToolbox(QString)),
                      this, SLOT(setToolbox(QString)));
    
    QObject::connect(activityTools, SIGNAL(objectSelected(GameObj*)),
                      glWidget, SLOT(objectSelected(GameObj*)));
    
    QObject::connect(activityTools, SIGNAL(showActivityEventEditor()),
                      this, SLOT(showActivityEventEditor()));
    
    QObject::connect(activityTools, SIGNAL(showActivityServiceEditor()),
                      this, SLOT(showActivityServiceEditor()));
    
    QObject::connect(activityTools, SIGNAL(showActivityTrafficEditor()),
                      this, SLOT(showActivityTrafficEditor()));
    
    QObject::connect(activityTools, SIGNAL(showActivityTimetableEditor()),
                      this, SLOT(showActivityTimetableEditor()));
    
    QObject::connect(activityTools, SIGNAL(showEvents(Activity*)),
                      activityEventWindow, SLOT(showEvents(Activity*)));
    
    QObject::connect(activityTools, SIGNAL(showServices(Route*)),
                      activityServiceWindow, SLOT(showServices(Route*)));
    
    QObject::connect(activityTools, SIGNAL(showTraffic(Route*)),
                      activityTrafficWindow, SLOT(showTraffic(Route*)));
    
    QObject::connect(activityServiceWindow, SIGNAL(reloadServicesList()),
                      activityTools, SLOT(reloadServicesList()));
    
    QObject::connect(activityTrafficWindow, SIGNAL(reloadTrafficsList()),
                       activityTools, SLOT(reloadTrafficsList()));

    QObject::connect(activityTools, SIGNAL(showTimetable(Activity*)),
                      activityTimetableWindow, SLOT(showTimetable(Activity*)));
    
    QObject::connect(activityEventWindow->eventProperties, SIGNAL(jumpTo(PreciseTileCoordinate*)),
                      glWidget, SLOT(jumpTo(PreciseTileCoordinate*)));
    
    QObject::connect(activityTools, SIGNAL(jumpTo(PreciseTileCoordinate*)),
                      glWidget, SLOT(jumpTo(PreciseTileCoordinate*)));
    
    QObject::connect(errorMessagesWindow, SIGNAL(jumpTo(PreciseTileCoordinate*)),
                      glWidget, SLOT(jumpTo(PreciseTileCoordinate*)));
    
    QObject::connect(errorMessagesWindow, SIGNAL(selectObject(GameObj*)),
                      glWidget, SLOT(objectSelected(GameObj*)));
    
    QObject::connect(activityTools, SIGNAL(sendMsg(QString)), glWidget, SLOT(msg(QString)));
    
    QObject::connect(this, SIGNAL(reloadRefFile()),
                      glWidget, SLOT(reloadRefFile()));
    
    QObject::connect(glWidget, SIGNAL(refreshObjLists()),
                      objTools, SLOT(refreshObjLists()));
    
    if(Game::serverClient != NULL)
        QObject::connect(Game::serverClient, SIGNAL(refreshObjLists()),
                      objTools, SLOT(refreshObjLists()));
}

void RouteEditorWindow::keyPressEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void RouteEditorWindow::closeEvent(QCloseEvent * event ){
    QVector<QString> unsavedItems;
    glWidget->getUnsavedInfo(unsavedItems);
    if(unsavedItems.size() == 0){
        qDebug() << "nic do zapisania";
        emit exitNow();
        event->accept();
        SoundManager::CloseAl();
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
        SoundManager::CloseAl();
        qApp->quit();
        return;
    }
    
    save();

    emit exitNow();
    event->accept();
    
    SoundManager::CloseAl();
    qApp->quit();
}

void RouteEditorWindow::save(){
    emit sendMsg(QString("save"));
}

void RouteEditorWindow::reloadRef(){
    emit reloadRefFile();
}

void RouteEditorWindow::createPaths(){
    QMessageBox msgBox;
    msgBox.setText("This action will delete all your existing activity paths and create new simple paths! Continue?");
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

void RouteEditorWindow::terrainCamera(bool val){
    Game::cameraStickToTerrain = val;
}

void RouteEditorWindow::detailedTerrainEnabled(){
    emit this->sendMsg("editDetailedTerrain");
}

void RouteEditorWindow::distantTerrainEnabled(){
    emit this->sendMsg("editDistantTerrain");
}
    
void RouteEditorWindow::mstsShadows(bool val){
    Game::mstsShadows = val;
}

void RouteEditorWindow::about(){
    aboutWindow->show();
}

void RouteEditorWindow::showTerrainTreeEditr(){
    emit sendMsg(QString("showTerrainTreeEditr"));
}

void RouteEditorWindow::showToolsObject(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("objTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsObjectAndTerrain(bool show){
    if(show){
        hideShowToolWidget(true);
        hideAllTools();
        objTools->show();
        objectsAndTerrainAction->setChecked(true);
        terrainTools->show();
        box->setFixedWidth(500);
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsTerrain(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("terrainTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsGeo(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("geoTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsActivity(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("activityTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showActivityEventEditor(){
    activityEventWindow->show();
}

void RouteEditorWindow::showActivityServiceEditor(){
    activityServiceWindow->show();
}

void RouteEditorWindow::showActivityTrafficEditor(){
    activityTrafficWindow->show();
}

void RouteEditorWindow::showActivityTimetableEditor(){
    activityTimetableWindow->show();
}

void RouteEditorWindow::setToolbox(QString name){
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
        if(Game::serverClient == NULL){
            activityTools->show();
            activityAction->setChecked(true);
        }
    }
}

void RouteEditorWindow::hideAllTools(){
    objTools->hide();
    terrainTools->hide();
    geoTools->hide();
    activityTools->hide();
    objectsAction->setChecked(false);
    terrainAction->setChecked(false);     
    geoAction->setChecked(false);
    activityAction->setChecked(false);
    objectsAndTerrainAction->setChecked(false);
    box->setFixedWidth(250);
}

void RouteEditorWindow::showProperties(GameObj* obj){
    // hide all
    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        it->hide();
    }
    if(obj == NULL) return;
    // show 
    //qDebug() << obj->typeObj;

    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        if(!it->support(obj)) continue;
        it->show();
        it->showObj(obj);
        return;
    }
}

void RouteEditorWindow::updateProperties(GameObj* obj){
    if(obj == NULL) return;
    // show 

    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        if(it->isVisible() && it->support(obj)){
            it->updateObj(obj);
            return;
        }
    }
}

void RouteEditorWindow::hideShowPropertiesWidget(bool show){
    if(show) box2->show();
    else box2->hide();
}

void RouteEditorWindow::hideShowShapeViewWidget(bool show){
    if(show) shapeViewWindow->show();
    else shapeViewWindow->hide();
}

void RouteEditorWindow::hideShowErrorMsgWidget(bool show){
    if(show) {
        errorMessagesWindow->show();
    }
    else errorMessagesWindow->hide();
}

void RouteEditorWindow::hideShowNaviWidget(bool show){
    if(show) naviWindow->show();
    else naviWindow->hide();
}

void RouteEditorWindow::hideShowToolWidget(bool show){
    if(show) box->show();
    else box->hide();
}

void RouteEditorWindow::viewWorldGrid(bool show){
    Game::viewWorldGrid = show;
}
void RouteEditorWindow::viewTileGrid(bool show){
    Game::viewTileGrid = show;
}
void RouteEditorWindow::viewTerrainShape(bool show){
    Game::viewTerrainShape = !show;
}
void RouteEditorWindow::viewTerrainGrid(bool show){
    Game::viewTerrainGrid = show;
}
void RouteEditorWindow::showWorldObjPivotPointsEnabled(bool show){
    Game::showWorldObjPivotPoints = show;
}
void RouteEditorWindow::viewInteractives(bool show){
    Game::viewInteractives = show;
}
void RouteEditorWindow::viewTrackDbLines(bool show){
    Game::viewTrackDbLines = show;
}
void RouteEditorWindow::viewTsectionLines(bool show){
    Game::viewTsectionLines = show;
}

void RouteEditorWindow::viewTrackItems(bool show){
    Game::renderTrItems = show;
}

void RouteEditorWindow::viewPointer3d(bool show){
    Game::viewPointer3d = show;
}

void RouteEditorWindow::viewMarkers(bool show){
    Game::viewMarkers = show;
}

void RouteEditorWindow::viewSnapable(bool show){
    Game::viewSnapable = show;
}

void RouteEditorWindow::viewCompass(bool show){
    Game::viewCompass = show;
}

void RouteEditorWindow::showRoute(){
    if(Game::serverClient == NULL){
        if(!glWidget->initRoute()){
            emit exitNow();
            SoundManager::CloseAl();
            qApp->quit();
            return;
        }
        show();
    } else {
        QObject::connect(glWidget, SIGNAL(showWindow()), this, SLOT(show()));
        glWidget->initRoute();
    }
}

void RouteEditorWindow::show(){
    naviWindow->move(0, this->height() - naviWindow->height() );
    
    if(!Game::playerMode){
        naviWindow->show();
        //errorMessagesWindow->show();
        //errorMessagesWindow->refreshErrorList();
    }
    
    QMainWindow::show();
}

void RouteEditorWindow::naviWindowClosed(){
    naviAction->blockSignals(true);
    naviAction->setChecked(false);
    naviAction->blockSignals(false);
}

void RouteEditorWindow::errorMessagesWindowClosed(){
    errorViewAction->blockSignals(true);
    errorViewAction->setChecked(false);
    errorViewAction->blockSignals(false);
}

void RouteEditorWindow::shapeVeiwWindowClosed(){
    shapeViewAction->blockSignals(true);
    shapeViewAction->setChecked(false);
    shapeViewAction->blockSignals(false);
}

void RouteEditorWindow::viewUnselectAll(){

    vViewWorldGrid->setChecked(false);
    vViewTileGrid->setChecked(false);
    vViewTerrainGrid->setChecked(false);
    vViewTerrainShape->setChecked(false);
    vShowWorldObjPivotPoints->setChecked(false);
    vViewInteractives->setChecked(false);
    vViewTrackDbLines->setChecked(false);
    vViewTsectionLines->setChecked(false);
    vViewTrackItems->setChecked(false);
    vViewPointer3d->setChecked(false);
    vViewMarkers->setChecked(false);
    vViewSnapable->setChecked(false);

    vViewWorldGrid->triggered(false);
    vViewTileGrid->triggered(false);
    vViewTerrainGrid->triggered(false);
    vViewTerrainShape->triggered(false);
    vShowWorldObjPivotPoints->triggered(false);
    vViewInteractives->triggered(false);
    vViewTrackDbLines->triggered(false);
    vViewTsectionLines->triggered(false);
    vViewTrackItems->triggered(false);
    vViewPointer3d->triggered(false);
    vViewMarkers->triggered(false);
    vViewSnapable->triggered(false);
}
//void Window::exitNow(){
//    this->hide();
//}