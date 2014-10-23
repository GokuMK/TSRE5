#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QBasicTimer>
#include "Camera.h"

class Tile;
class SFile;
class Eng;
class GLUU;
class Route;

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

signals:

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
};

#endif
