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
#include "CameraConsist.h"
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
class QOpenGLFunctions_3_3_Core;
class QAction;
class GuiGlCompass;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class RouteEditorGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    RouteEditorGLWidget(QWidget *parent = 0);
    ~RouteEditorGLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    
    bool initRoute();
    void cameraInit();
    void playInit();
    
    void getUnsavedInfo(QVector<QString> &items);

public slots:
    void cleanup();
    void enableTool(QString name);
    void setPaintBrush(Brush* brush);
    void jumpTo(PreciseTileCoordinate*);
    void jumpTo(float *posT, float *pos);
    void jumpTo(int X, int Z, float x, float y, float z);
    
    void msg(QString text);
    void msg(QString name, bool val);
    void msg(QString name, int val);
    void msg(QString name, float val);
    void msg(QString name, QString val);
    
    void editCopy();
    void editPaste();
    void editSelect();
    void editFind1x1();
    void editFind3x3();
    void editFind(int radius = 0);
    void editUndo();
    void showTrkEditr();
    void showContextMenu(const QPoint & point);
    void createNewTiles(QMap<int, QPair<int, int>*> list);
    void createNewLoTiles(QMap<int, QPair<int, int>*> list);
    void objectSelected(GameObj* obj);
    void objectSelected(QVector<GameObj*> obj);
    
    void selectToolresetMoveStep();
    void selectToolresetRot();
    void selectToolSelect();
    void selectToolRotate();
    void selectToolTranslate();
    void selectToolScale();
    void toolBrushDirectionUp();
    void toolBrushDirectionDown();
    void putTerrainTexToolSelectRandom();
    void putTerrainTexToolSelectPresent();
    void putTerrainTexToolSelect0();
    void putTerrainTexToolSelect90();
    void putTerrainTexToolSelect180();
    void putTerrainTexToolSelect270();
    void placeToolStickTerrain();
    void placeToolStickAll();
    void reloadRefFile();
    void setCameraObject(GameObj* obj);
    void setMoveStep(float val);
    void paintToolObj();
    void paintToolObjSelected();
    void paintToolTDB();
    void paintToolTDBVector();
    void setTerrainToObj();
    void adjustObjPositionToTerrainMenu();
    void adjustObjRotationToTerrainMenu();
    void pickObjForPlacement();
    void pickObjRotForPlacement();
    void pickObjRotElevForPlacement();
    
    void initRoute2();
    
signals:
    void showWindow();
    void routeLoaded(Route * a);
    void itemSelected(Ref::RefItem* pointer);
    void naviInfo(int all, int hidden);
    void posInfo(PreciseTileCoordinate* pos);
    void pointerInfo(float* pos);
    void setToolbox(QString name);
    void setBrushTextureId(int val);
    void showProperties(GameObj* obj);
    void updateProperties(GameObj* obj);
    void flexData(int x, int z, float* p);
    void mkrList(QMap<QString, Coords*> list);
    void refreshObjLists();
    
    void sendMsg(QString name);
    void sendMsg(QString name, bool val);
    void sendMsg(QString name, int val);
    void sendMsg(QString name, float val);
    void sendMsg(QString name, QString val);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void paintGL2();
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
    void pushRenderPointer();
private:
    void setupVertexAttribs();
    void setSelectedObj(GameObj* o);
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
    QOpenGLFunctions_3_3_Core* funcs = 0;
    unsigned int fbo[3];
    bool m_transparent;
    Camera* camera = NULL;
    CameraFree* cameraFree = NULL;
    CameraConsist* cameraObj = NULL;
    bool selection = false;
    int mousex, mousey;
    GameObj* selectedObj = NULL;
    GameObj* lastSelectedObj = NULL;
    WorldObj* copyPasteObj = NULL;
    GroupObj* groupObj = NULL;
    GroupObj* copyPasteGroupObj = NULL;
    Pointer3d* pointer3d;
    float lastPointerPos[3];
    float aktPointerPos[3];
    bool mouseLPressed = false;
    bool mouseRPressed = false;
    bool mouseClick = false;
    QString toolEnabled = "";
    float defaultMoveStep = 0.25;
    float moveStep = 0.25;
    //float moveUltraStep = 2.0;
    float moveMaxStep = 0.25;
    //float moveMinStep = 0.01;
    bool resizeTool = false;
    bool rotateTool = false;
    bool translateTool = false;
    bool stickPointerToTerrain = true;
    bool autoAddToTDB = true;
    float lastNewObjPos[3];
    float lastNewObjPosT[2];
    float placeRot[4];
    float placeElev = 0;
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
    
    /*struct DefaultMenuActions {
        QAction *undo;
        QAction *copy;
        QAction *paste;
        QAction *find1x1;
        QAction *find3x3;
        QAction *select;
        void init(RouteEditorGLWidget *widget);
    };*/
    //DefaultMenuActions defaultMenuActions;
    QMap<QString, QAction*> defaultMenuActions;
    bool bolckContextMenu = false;
    
    GuiGlCompass * compass = NULL;
    OglObj * compassPointer = NULL;
    
    
    
};

#endif
