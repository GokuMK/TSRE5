#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "ToolBox.h"
#include "NaviBox.h"
#include "AboutWindow.h"

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE
//! [0]
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
    
signals:
    void exitNow();
    void sendMsg(QString text);

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent * event );
private:
    GLWidget *glWidget;
    ToolBox *groupBox;
    NaviBox *naviBox;
    
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
