/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QMap>

class QSlider;
class RouteEditorGLWidget;
class ObjTools;
class GeoTools;
class TerrainTools;
class ActivityTools;
class NaviBox;
class AboutWindow;
class NaviWindow;
class ShapeViewWindow;
class PropertiesAbstract;
class GameObj;
class PreciseTileCoordinate;
class ActivityEventWindow;
class ActivityServiceWindow;
class ActivityTrafficWindow;
class ActivityTimetableWindow;
class ErrorMessagesWindow;
class ClientUsersWindow;

class RouteEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    RouteEditorWindow();
    
public slots:
    void save();
    void showRoute();
    void show();
    void createPaths();
    void reloadRef();
    void about();
    void terrainCamera(bool val);
    void mstsShadows(bool val);
    void detailedTerrainEnabled();
    void distantTerrainEnabled();
    void setToolbox(QString name);
    void showProperties(GameObj* obj);
    void updateProperties(GameObj* obj);
    void hideShowToolWidget(bool show);
    void hideShowPropertiesWidget(bool show);
    void hideShowNaviWidget(bool);
    void hideShowShapeViewWidget(bool);
    void hideShowErrorMsgWidget(bool);
    void viewWorldGrid(bool show);
    void viewTileGrid(bool show);
    void viewTerrainShape(bool show);
    void viewTerrainGrid(bool show);
    void viewInteractives(bool show);
    void viewTrackDbLines(bool show);
    void viewTsectionLines(bool show);
    void viewTrackItems(bool show);
    void viewPointer3d(bool show);
    void viewMarkers(bool show);
    void viewSnapable(bool show);
    void viewCompass(bool show);
    void showToolsObjectAndTerrain(bool show);
    void showToolsObject(bool show);
    void showToolsTerrain(bool show);
    void showToolsGeo(bool show);
    void showToolsActivity(bool show);
    void showTerrainTreeEditr();
    void showWorldObjPivotPointsEnabled(bool show);
    void naviWindowClosed();
    void errorMessagesWindowClosed();
    void shapeVeiwWindowClosed();
    void viewUnselectAll();
    void showActivityEventEditor();
    void showActivityServiceEditor();
    void showActivityTrafficEditor();
    void showActivityTimetableEditor();

signals:
    void exitNow();
    void sendMsg(QString text);
    void reloadRefFile();
    
protected:
    void keyPressEvent(QKeyEvent *event);
    virtual void closeEvent(QCloseEvent * event );
    void hideAllTools();

private:
    QWidget* box;
    QWidget* box2;
    
    RouteEditorGLWidget *glWidget;
    ObjTools *objTools;
    GeoTools *geoTools;
    TerrainTools *terrainTools;
    ActivityTools *activityTools;
    //NaviBox *naviBox;
    
    QMap<QString, PropertiesAbstract*> objProperties;
    
    QMenu *routeMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;
    QMenu *settingsMenu;
    
    QAction *saveAction;
    QAction *createPathsAction;
    QAction *reloadRefAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *undoAction;
    QAction *pasteAction;
    QAction *selectAction;
    QAction *aboutAction;
    QAction *terrainCameraAction;
    QAction *mstsShadowsAction;
    QAction *propertiesAction;
    QAction *naviAction;
    QAction *shapeViewAction;
    QAction *errorViewAction;
    QAction *toolsAction;
    QAction *objectsAction;
    QAction *objectsAndTerrainAction;
    QAction *terrainAction;
    QAction *terrainTreeEditr;
    QAction *trkEditr;
    QAction *geoAction;
    QAction *activityAction;
    
    QAction* vViewWorldGrid;
    QAction* vViewTileGrid;
    QAction* vViewTerrainGrid;
    QAction* vViewTerrainShape;
    QAction* vShowWorldObjPivotPoints;
    QAction* vViewInteractives;
    QAction* vViewTrackDbLines;
    QAction* vViewTsectionLines;
    QAction* vViewTrackItems;
    QAction* vViewPointer3d;
    QAction* vViewMarkers;
    QAction* vViewSnapable;
    
    AboutWindow* aboutWindow;
    NaviWindow* naviWindow;
    ErrorMessagesWindow* errorMessagesWindow;
    ClientUsersWindow* clientUsersWindow;
    ActivityEventWindow* activityEventWindow;
    ActivityServiceWindow* activityServiceWindow;
    ActivityTrafficWindow* activityTrafficWindow;
    ActivityTimetableWindow* activityTimetableWindow;
    ShapeViewWindow* shapeViewWindow;
};
//! [0]

#endif
