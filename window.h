#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "ToolBox.h"
#include "NaviBox.h"

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE
//! [0]
class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    
signals:
    void exitNow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent * event );
private:
    GLWidget *glWidget;
    ToolBox *groupBox;
    NaviBox *naviBox;
};
//! [0]

#endif
