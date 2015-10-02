#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "ObjTools.h"
#include "TerrainTools.h"
#include "NaviBox.h"
#include "AboutWindow.h"
#include "PropertiesUndefined.h"

//QT_BEGIN_NAMESPACE

//QT_END_NAMESPACE
//! [0]
class QSlider;
class GLWidget;

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
    
signals:
    void exitNow();
    void sendMsg(QString text);

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent * event );
    void hideAllTools();
private:
    GLWidget *glWidget;
    ObjTools *objTools;
    TerrainTools *terrainTools;
    NaviBox *naviBox;
    
    PropertiesUndefined * propertiesUndefined;
    
    QMenu *routeMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    
    QAction *saveAction;
    QAction *createPathsAction;
    QAction *exitAction;
    QAction *aboutAction;
    
    AboutWindow aboutWindow;
};
//! [0]

#endif
