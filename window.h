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

class QSlider;
class GLWidget;
class ObjTools;
class GeoTools;
class TerrainTools;
class ActivityTools;
class NaviBox;
class AboutWindow;
class NaviWindow;
class ShapeViewWindow;
class PropertiesAbstract;
class WorldObj;
class PreciseTileCoordinate;

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();
    
public slots:
    void save();
    void show();
    void createPaths();
    void about();
    void terrainCamera(bool val);
    void setToolbox(QString name);
    void showProperties(WorldObj* obj);
    void updateProperties(WorldObj* obj);
    void hideShowToolWidget(bool show);
    void hideShowPropertiesWidget(bool show);
    void hideShowNaviWidget(bool);
    void hideShowShapeViewWidget(bool);
    void viewWorldGrid(bool show);
    void viewTileGrid(bool show);
    void viewTerrainGrid(bool show);
    void viewInteractives(bool show);
    void viewTrackDbLines(bool show);
    void viewTsectionLines(bool show);
    void viewPointer3d(bool show);
    void viewMarkers(bool show);
    void viewSnapable(bool show);
    void showToolsObject(bool show);
    void showToolsTerrain(bool show);
    void showToolsGeo(bool show);
    void showToolsActivity(bool show);
    void showTerrainTreeEditr();
    void showWorldObjPivotPointsEnabled(bool show);
    void naviWindowClosed();
    void shapeVeiwWindowClosed();

signals:
    void exitNow();
    void sendMsg(QString text);

protected:
    void keyPressEvent(QKeyEvent *event);
    virtual void closeEvent(QCloseEvent * event );
    void hideAllTools();

private:
    QWidget* box;
    QWidget* box2;
    
    GLWidget *glWidget;
    ObjTools *objTools;
    GeoTools *geoTools;
    TerrainTools *terrainTools;
    ActivityTools *activityTools;
    //NaviBox *naviBox;
    
    std::vector<PropertiesAbstract*> objProperties;
    //PropertiesUndefined * propertiesUndefined;
    
    QMenu *routeMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;
    QMenu *settingsMenu;
    
    QAction *saveAction;
    QAction *createPathsAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *undoAction;
    QAction *pasteAction;
    QAction *aboutAction;
    QAction *terrainCameraAction;
    QAction *propertiesAction;
    QAction *naviAction;
    QAction *shapeViewAction;
    QAction *toolsAction;
    QAction *objectsAction;
    QAction *terrainAction;
    QAction *terrainTreeEditr;
    QAction *trkEditr;
    QAction *geoAction;
    QAction *activityAction;
    
    AboutWindow* aboutWindow;
    NaviWindow* naviWindow;
    ShapeViewWindow* shapeViewWindow;
};
//! [0]

#endif
