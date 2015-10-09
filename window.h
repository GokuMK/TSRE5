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
class PropertiesAbstract;
class WorldObj;

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();
    
public slots:
    void save();
    void createPaths();
    void about();
    void setToolbox(QString name);
    void showProperties(WorldObj* obj);
    void hideShowToolWidget(bool show);
    void hideShowPropertiesWidget(bool show);
    void viewWorldGrid(bool show);
    void viewTileGrid(bool show);
    void viewInteractives(bool show);
    void viewTrackDbLines(bool show);
    void viewTsectionLines(bool show);
    void viewPointer3d(bool show);
    
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
    NaviBox *naviBox;
    
    std::vector<PropertiesAbstract*> objProperties;
    //PropertiesUndefined * propertiesUndefined;
    
    QMenu *routeMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    
    QAction *saveAction;
    QAction *createPathsAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *propertiesAction;
    QAction *toolsAction;
    
    
    AboutWindow* aboutWindow;
};
//! [0]

#endif
