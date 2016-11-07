/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
//#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QBasicTimer>
#include "CameraFree.h"
#include "WorldObj.h"
#include "GroupObj.h"
#include "Pointer3d.h"
#include "Ref.h"
#include <unordered_map>

class Tile;
class SFile;
class Eng;
class GLUU;
class Route;
class Brush;
class PreciseTileCoordinate;
class Coords;
class MapWindow;
class ShapeLib;
class EngLib;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    
    void getUnsavedInfo(std::vector<QString> &items);

public slots:
    void cleanup();
    void enableTool(QString name);
    void setPaintBrush(Brush* brush);
    void jumpTo(PreciseTileCoordinate*);
    
    void msg(QString text);
    void msg(QString name, bool val);
    void msg(QString name, int val);
    void msg(QString name, float val);
    void msg(QString name, QString val);
    
    void editCopy();
    void editPaste();
    void showTrkEditr();
    
signals:
    void routeLoaded(Route * a);
    void itemSelected(Ref::RefItem* pointer);
    void naviInfo(int all, int hidden);
    void posInfo(PreciseTileCoordinate* pos);
    void setToolbox(QString name);
    void setBrushTextureId(int val);
    void showProperties(WorldObj* obj);
    void updateProperties(WorldObj* obj);
    void flexData(int x, int z, float* p);
    void mkrList(std::unordered_map<std::string, Coords*> list);
    
    void sendMsg(QString name);
    void sendMsg(QString name, bool val);
    void sendMsg(QString name, int val);
    void sendMsg(QString name, float val);
    void sendMsg(QString name, QString val);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void renderShadowMaps();
    void handleSelection();
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void drawPointer();
private:
    void setupVertexAttribs();
    void setSelectedObj(WorldObj* o);
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
    Route* route = NULL;
    GLUU* gluu;
    bool m_transparent;
    Camera* camera;
    bool selection = false;
    int mousex, mousey;
    WorldObj* selectedObj = NULL;
    WorldObj* lastSelectedObj = NULL;
    WorldObj* copyPasteObj = NULL;
    GroupObj* groupObj = NULL;
    GroupObj* copyPasteGroupObj = NULL;
    Pointer3d* pointer3d;
    float lastPointerPos[3];
    float aktPointerPos[3];
    bool mouseLPressed = false;
    bool mouseRPressed = false;
    QString toolEnabled = "";
    float moveStep = 0.25;
    float moveUltraStep = 2.0;
    float moveMaxStep = 0.25;
    float moveMinStep = 0.01;
    bool resizeTool = false;
    bool rotateTool = false;
    bool translateTool = false;
    bool stickPointerToTerrain = true;
    bool autoAddToTDB = true;
    float lastNewObjPos[3];
    float lastNewObjPosT[2];
    float placeRot[4];
    long long int lastMousePressTime = 0;
    bool keyControlEnabled = false;
    bool keyShiftEnabled = false;
    bool keyAltEnabled = false;
    GLuint FramebufferName1 = 0;
    GLuint depthTexture1 = 0;
    GLuint FramebufferName2 = 0;
    GLuint depthTexture2 = 0;
    Brush* defaultPaintBrush;
    MapWindow* mapWindow;
    ShapeLib *currentShapeLib = NULL;
    EngLib *engLib = NULL;
};

#endif
