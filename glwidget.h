#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
//#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QBasicTimer>
#include "Camera.h"
#include "WorldObj.h"
#include "Pointer3d.h"
#include "Ref.h"

class Tile;
class SFile;
class Eng;
class GLUU;
class Route;
class Brush;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void cleanup();
    void enableTool(QString name);
    void setPaintBrush(Brush* brush);
    void jumpTo(int x, int y);
    void msg(QString text);

signals:
    void routeLoaded(Route * a);
    void itemSelected(int pointer);
    void naviInfo(int x, int z, int all, int hidden);
    void setToolbox(QString name);
    void setBrushTextureId(int val);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void drawPointer();
private:
    void setupVertexAttribs();
    QBasicTimer timer;
    unsigned long long int lastTime;
    unsigned long long int timeNow;
    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    int fps;
    QPoint m_lastPos;
    SFile* sFile;
    Eng* eng;
    Tile* tile;
    Route* route;
    GLUU* gluu;
    bool m_transparent;
    Camera* camera;
    bool selection = false;
    int mousex, mousey;
    WorldObj* selectedObj = NULL;
    WorldObj* lastSelectedObj = NULL;
    Pointer3d* pointer3d;
    float lastPointerPos[3];
    float aktPointerPos[3];
    bool mousePressed = false;
    QString toolEnabled = "";
    float moveStep = 0.25;
    float moveMaxStep = 0.25;
    float moveMinStep = 0.01;
    bool resizeTool = false;
    bool rotateTool = false;
    bool translateTool = false;
    //int tdbDefaultEnd = 0;
    float lastNewObjPos[3];
    float lastNewObjPosT[2];
    
    Brush* defaultPaintBrush;
};

#endif
