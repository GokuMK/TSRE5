#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>

//QT_BEGIN_NAMESPACE

//QT_END_NAMESPACE
//! [0]
class QSlider;
class GLWidget;
class ObjTools;
class TerrainTools;
class NaviBox;
class AboutWindow;
class NaviWindow;
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
    void setToolbox(QString name);
    void showProperties(WorldObj* obj);
    void hideShowToolWidget(bool show);
    void hideShowPropertiesWidget(bool show);
    void hideShowNaviWidget(bool);
    void viewWorldGrid(bool show);
    void viewTileGrid(bool show);
    void viewInteractives(bool show);
    void viewTrackDbLines(bool show);
    void viewTsectionLines(bool show);
    void viewPointer3d(bool show);
    void viewMarkers(bool show);
    void showToolsObject(bool show);
    void showToolsTerrain(bool show);
    void showTerrainTreeEditr();
signals:
    void exitNow();
    void sendMsg(QString text);

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent * event );
    void hideAllTools();
    
private:
    QWidget* box;
    QWidget* box2;
    
    GLWidget *glWidget;
    ObjTools *objTools;
    TerrainTools *terrainTools;
    //NaviBox *naviBox;
    
    std::vector<PropertiesAbstract*> objProperties;
    //PropertiesUndefined * propertiesUndefined;
    
    QMenu *routeMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;
    
    QAction *saveAction;
    QAction *createPathsAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *aboutAction;
    QAction *propertiesAction;
    QAction *naviAction;
    QAction *toolsAction;
    QAction *objectsAction;
    QAction *terrainAction;
    QAction *terrainTreeEditr;
    
    AboutWindow* aboutWindow;
    NaviWindow* naviWindow;
};
//! [0]

#endif
