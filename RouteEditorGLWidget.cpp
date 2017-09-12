/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "RouteEditorGLWidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QDateTime>
#include <math.h>
#include "GLUU.h"
#include "SFile.h"
#include "ReadFile.h"
#include "FileBuffer.h"
#include "Route.h"
#include "GLMatrix.h"
#include "Eng.h"
#include "Tile.h"
#include "Game.h"
#include "GLH.h"
#include "Vector2f.h"
#include "TerrainLib.h"
#include "Brush.h"
#include "IghCoords.h"
#include "MapWindow.h"
#include "TerrainTreeWindow.h"
#include "ShapeLib.h"
#include "EngLib.h"
#include "QOpenGLFunctions_3_3_Core"
#include "Undo.h"
#include "Environment.h"
#include "Terrain.h"

RouteEditorGLWidget::RouteEditorGLWidget(QWidget *parent)
: QOpenGLWidget(parent),
m_xRot(0),
m_yRot(0),
m_zRot(0) {

}

RouteEditorGLWidget::~RouteEditorGLWidget() {
    cleanup();
}

QSize RouteEditorGLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize RouteEditorGLWidget::sizeHint() const {
    return QSize(1000, 700);
}

void RouteEditorGLWidget::cleanup() {
    makeCurrent();
    //delete gluu->m_program;
    //gluu->m_program = 0;
    doneCurrent();
}

void RouteEditorGLWidget::timerEvent(QTimerEvent * event) {
    Game::currentShapeLib = currentShapeLib;
    timeNow = QDateTime::currentMSecsSinceEpoch();
    if (timeNow - lastTime < 1)
        fps = 1;
    else
        fps = 1000.0 / (timeNow - lastTime);
    if (fps < 10) fps = 10;

    if (timeNow % 200 < lastTime % 200) {
        //qDebug() << "new second" << timeNow;
        if (selectedObj != NULL)
            emit updateProperties(selectedObj);
        Undo::StateEndIfLongTime();
    }

    route->updateSim(camera->pozT, (float) (timeNow - lastTime) / 1000.0);

    lastTime = timeNow;

    if (Game::allowObjLag < Game::maxObjLag)
        Game::allowObjLag += 2;

    camera->update(fps);
    update();
}

void RouteEditorGLWidget::DefaultMenuActions::init(RouteEditorGLWidget *widget){
    undo = new QAction(tr("&Undo"), widget); 
    QObject::connect(undo, SIGNAL(triggered()), widget, SLOT(editUndo()));
    copy = new QAction(tr("&Copy"), widget); 
    QObject::connect(copy, SIGNAL(triggered()), widget, SLOT(editCopy()));
    paste = new QAction(tr("&Paste"), widget); 
    QObject::connect(paste, SIGNAL(triggered()), widget, SLOT(editPaste()));
    find1x1 = new QAction(tr("&Select Similar 1x1"), widget); 
    QObject::connect(find1x1, SIGNAL(triggered()), widget, SLOT(editFind1x1()));
    find3x3 = new QAction(tr("&Select Similar 3x3"), widget); 
    QObject::connect(find3x3, SIGNAL(triggered()), widget, SLOT(editFind3x3()));
}

void RouteEditorGLWidget::initializeGL() {
    //this->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
    //        this, SLOT(showContextMenu(const QPoint &)));
    this->defaultMenuActions.init(this);
    
    currentShapeLib = new ShapeLib();
    Game::currentShapeLib = currentShapeLib;
    //qDebug() << "GLUU::get();";
    gluu = GLUU::get();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &RouteEditorGLWidget::cleanup);
    //qDebug() << "initializeOpenGLFunctions();";
    initializeOpenGLFunctions();
    //funcs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    //if (!funcs) {
    //    qWarning() << "Could not obtain required OpenGL context version";
    //    exit(1);
    //}
    //funcs->initializeOpenGLFunctions();/**/
    glClearColor(0, 0, 0, 1);
    //qDebug() << "gluu->initShader();";
    gluu->initShader();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glLineWidth(Game::oglDefaultLineWidth);

    //sFile = new SFile("F:/TrainSim/trains/trainset/pkp_sp47/pkp_sp47-001.s", "F:/TrainSim/trains/trainset/pkp_sp47");
    //sFile = new SFile("f:/train simulator/routes/cmk/shapes/cottage3.s", "cottage3.s", "f:/train simulator/routes/cmk/textures");
    //eng = new Eng("F:/Train Simulator/trains/trainset/PKP-ST44-992/","PKP-ST44-992.eng",0);
    //sFile->Load("f:/train simulator/routes/cmk/shapes/cottage3.s");
    //tile = new Tile(-5303,-14963);
    //qDebug() << "route = new Route();";
    engLib = new EngLib();
    Game::currentEngLib = engLib;

    route = new Route();
    if (!route->loaded) return;
    QObject::connect(route, SIGNAL(objSelected(GameObj*)), this, SLOT(objSelected(GameObj*)));

    float * aaa = new float[2] {
        0, 0
    };
    camera = new CameraFree(aaa);
    float spos[3];
    if (Game::start == 2) {
        camera->setPozT(Game::startTileX, -Game::startTileY);
    } else {
        camera->setPozT(route->getStartTileX(), -route->getStartTileZ());
        spos[0] = route->getStartpX();
        spos[2] = -route->getStartpZ();
    }
    if (TerrainLib::load(route->getStartTileX(), -route->getStartTileZ())) {
        spos[1] = 20 + TerrainLib::getHeight(route->getStartTileX(), -route->getStartTileZ(), route->getStartpX(), -route->getStartpZ());
    } else {
        spos[1] = 0;
    }
    camera->setPos((float*) &spos);

    lastTime = QDateTime::currentMSecsSinceEpoch();
    int timerStep = 15;
    if (Game::fpsLimit > 0)
        timerStep = 1000 / Game::fpsLimit;
    timer.start(timerStep, this);
    setFocus();
    setMouseTracking(true);
    pointer3d = new Pointer3d();
    //selectedObj = NULL;
    setSelectedObj(NULL);
    groupObj = new GroupObj();
    copyPasteGroupObj = new GroupObj();
    defaultPaintBrush = new Brush();
    mapWindow = new MapWindow();
    Quat::fill(this->placeRot);

    emit routeLoaded(route);
    emit mkrList(route->getMkrList());

    gluu->makeShadowFramebuffer(FramebufferName1, depthTexture1, Game::shadowMapSize, GL_TEXTURE2);
    gluu->makeShadowFramebuffer(FramebufferName2, depthTexture2, 1024, GL_TEXTURE3);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
}

void RouteEditorGLWidget::paintGL() {
    Game::currentShapeLib = currentShapeLib;
    if (route == NULL) return;
    if (!route->loaded) return;

    // Render Shadows
    if (Game::shadowsEnabled > 0)
        renderShadowMaps();

    // Render Scene
    gluu->currentShader = gluu->shaders["StandardBloom"];
    //gluu->currentShader = gluu->shaders["StandardFog"];
    gluu->currentShader->bind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int renderMode = GLUU::RENDER_DEFAULT;
    if (selection)
        renderMode = GLUU::RENDER_SELECTION;

    glClearColor(gluu->skyc[0], gluu->skyc[1], gluu->skyc[2], 1.0);

    glViewport(0, 0, (float) this->width() * Game::PixelRatio, (float) this->height() * Game::PixelRatio);
    Mat4::perspective(gluu->pMatrix, Game::cameraFov * M_PI / 180, float(this->width()) / this->height(), 0.2f, Game::objectLod);
    Mat4::multiply(gluu->pMatrix, gluu->pMatrix, camera->getMatrix());
    Mat4::identity(gluu->mvMatrix);
    gluu->setMatrixUniforms();
    
    TerrainLib::render(gluu, camera->pozT, camera->getPos(), camera->getTarget(), 3.14f / 3, renderMode);

    if (stickPointerToTerrain && Game::viewTerrainShape)
        if (!selection) drawPointer();

    route->render(gluu, camera->pozT, camera->getPos(), camera->getTarget(), camera->getRotX(), 3.14f / 3, renderMode);
    
    if (!selection)
        for(int i = 0; i < route->env->waterCount; i++)
            TerrainLib::renderWater(gluu, camera->pozT, camera->getPos(), camera->getTarget(), 3.14f / 3, renderMode, i);
    
    if (!stickPointerToTerrain || !Game::viewTerrainShape)
        if (!selection) drawPointer();
    
    gluu->currentShader->release();

    // Handle Selection
    handleSelection();

    // Set Info
    if (this->isActiveWindow()) {
        emit this->naviInfo(route->getTileObjCount((int) camera->pozT[0], (int) camera->pozT[1]), route->getTileHiddenObjCount((int) camera->pozT[0], (int) camera->pozT[1]));
        emit this->posInfo(camera->getCurrentPos());
        emit this->pointerInfo(aktPointerPos);
    }
}

void RouteEditorGLWidget::renderShadowMaps() {
    float* lookAt = Mat4::create();
    float* out1 = Vec3::create();
    Vec3::set(out1, 0, 1, 0);
    float *ld = Vec3::create();
    Vec3::set(ld, -1.0, 2.0, 1.0);
    float *aaa = camera->getPos();
    //float *lt = camera->getTarget();
    //Vec3::sub(lt, lt, aaa);
    //lt[0] = lt[0]*100;
    //lt[1] = 0;
    //lt[2] = lt[2]*100;
    //Vec3::add(aaa, aaa, lt);
    //aaa[2] = -aaa[2];
    //aaa[0] = -aaa[0];
    Vec3::add(ld, ld, aaa);
    Mat4::ortho(gluu->pShadowMatrix, -150, 150, -150, 150, -200, 200);
    Mat4::ortho(gluu->pShadowMatrix2, -700, 700, -700, 700, -700, 700);
    Mat4::lookAt(lookAt, ld, aaa, out1);
    Mat4::multiply(gluu->pShadowMatrix, gluu->pShadowMatrix, lookAt);
    Mat4::multiply(gluu->pShadowMatrix2, gluu->pShadowMatrix2, lookAt);

    gluu->currentShader = gluu->shaders["Shadows"];
    gluu->currentShader->bind();
    Mat4::identity(gluu->mvMatrix);
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName1);
    glActiveTexture(GL_TEXTURE0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, Game::shadowMapSize, Game::shadowMapSize);
    int tempLod = Game::objectLod;
    Game::objectLod = 600;
    TerrainLib::renderEmpty(gluu, camera->pozT, camera->getPos(), camera->getTarget(), 3.14f / 3);
    route->renderShadowMap(gluu, camera->pozT, camera->getPos(), camera->getTarget(), camera->getRotX(), 3.14f / 3, selection);

    Mat4::identity(gluu->mvMatrix);
    Mat4::identity(gluu->objStrMatrix);
    float *tmatrix = gluu->pShadowMatrix;
    gluu->pShadowMatrix = gluu->pShadowMatrix2;
    gluu->setMatrixUniforms();
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName2);
    glActiveTexture(GL_TEXTURE0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1024, 1024);
    Game::objectLod = 1000;
    route->renderShadowMap(gluu, camera->pozT, camera->getPos(), camera->getTarget(), camera->getRotX(), 3.14f / 3, selection);
    gluu->pShadowMatrix2 = gluu->pShadowMatrix;
    gluu->pShadowMatrix = tmatrix;
    Game::objectLod = tempLod;
    gluu->currentShader->release();
}

void RouteEditorGLWidget::handleSelection() {
    if (selection) {
        int x = mousex;
        int y = mousey;

        float winZ[4];

        int* viewport = new int[4];
        float* mvmatrix = new float[16];
        float* projmatrix = new float[16];
        float* wcoord = new float[4];

        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetFloatv(GL_MODELVIEW_MATRIX, mvmatrix);
        glGetFloatv(GL_PROJECTION_MATRIX, projmatrix);
        int realy = viewport[3] - (int) y - 1;
        glReadPixels(x, realy, 1, 1, GL_RGBA, GL_FLOAT, &winZ);

        qDebug() << winZ[0] << " " << winZ[1] << " " << winZ[2] << " " << winZ[3];
        int colorHash = (int) (winZ[0]*255)*256 * 256 + (int) (winZ[1]*255)*256 + (int) (winZ[2]*255);
        qDebug() << colorHash;
        int ww = (colorHash >> 20) & 0xF;
        qDebug() << "ww"<< ww;

        // WorldObj Selected
        if(ww == 0){
            if (selectedObj != NULL) {
                selectedObj->unselect();
                if (autoAddToTDB)
                    route->addToTDBIfNotExist((WorldObj*)selectedObj);
                setSelectedObj(NULL);
            }
        } else if( ww >= 1 && ww <= 9 ){
            int UiD = (colorHash >> 4) & 0xFFFF;
            //if(UiD >= 50000)
            //    UiD += 50000;
            int cdata = colorHash & 0xF;
            int wx = 0;
            int wz = 0;
            if (ww == 1 || ww == 2 || ww == 3) wx = camera->pozT[0] - 1;
            if (ww == 4 || ww == 5 || ww == 6) wx = camera->pozT[0];
            if (ww == 7 || ww == 8 || ww == 9) wx = camera->pozT[0] + 1;
            if (ww == 1 || ww == 4 || ww == 7) wz = camera->pozT[1] - 1;
            if (ww == 2 || ww == 5 || ww == 8) wz = camera->pozT[1];
            if (ww == 3 || ww == 6 || ww == 9) wz = camera->pozT[1] + 1;
            qDebug() << "color data: " << cdata;
            qDebug() << wx << " " << wz << " " << UiD;
            WorldObj *selectedWorldObj = (WorldObj*) selectedObj;
            if (keyControlEnabled) {
                if (selectedWorldObj == NULL)
                    setSelectedObj(groupObj);
                if (selectedWorldObj->typeObj != GameObj::worldobj){
                    selectedWorldObj->unselect();
                    setSelectedObj(groupObj);
                }
                if (selectedWorldObj->typeObj == GameObj::worldobj) {
                    groupObj->addObject(selectedWorldObj);
                    setSelectedObj(groupObj);
                }
                groupObj->addObject(route->getObj(wx, wz, UiD));
                if (groupObj->count() == 0) {
                    qDebug() << "brak obiektu";
                    groupObj->unselect();
                    setSelectedObj(NULL);
                }
            } else {
                WorldObj* twobj = route->getObj(wx, wz, UiD);
                if (selectedWorldObj != NULL && twobj != selectedWorldObj) {
                    selectedWorldObj->unselect();
                    if (autoAddToTDB) {
                        route->addToTDBIfNotExist(selectedWorldObj);
                    }
                }
                lastSelectedObj = selectedObj;
                setSelectedObj(twobj);
                if (selectedObj == NULL) {
                    qDebug() << "brak obiektu";
                } else {
                    selectedObj->select(cdata);
                }
            }
        } else if( ww == 10 ){
            int wx = camera->pozT[0] - 1 + ((colorHash >> 10) & 0x3);
            int wz = camera->pozT[1] - 1 + ((colorHash >> 8) & 0x3);
            int UiD = (colorHash) & 0xFF;
            qDebug() << wx << wz << UiD;
            if (selectedObj != NULL) {
                if (keyControlEnabled && selectedObj->typeObj == GameObj::terrainobj ) {
                    Terrain * tt = (Terrain*) selectedObj;
                    if(tt->mojex != wx || tt->mojez != wz){
                        selectedObj->unselect();
                        if (autoAddToTDB)
                            route->addToTDBIfNotExist((WorldObj*)selectedObj);
                        setSelectedObj(NULL);
                    }
                } else {
                    selectedObj->unselect();
                    if (autoAddToTDB)
                        route->addToTDBIfNotExist((WorldObj*)selectedObj);
                    setSelectedObj(NULL);
                }
            }
            setSelectedObj((GameObj*)TerrainLib::terrain[wx*10000+wz]);
            if (selectedObj == NULL) {
                qDebug() << "brak obiektu";
            } else {
                selectedObj->select(UiD);
            }
        } else if( ww == 11 ){
            if (selectedObj != NULL) {
                selectedObj->unselect();
                if (autoAddToTDB)
                    route->addToTDBIfNotExist((WorldObj*)selectedObj);
                setSelectedObj(NULL);
            }
            int CID = ((colorHash) >> 8) & 0xFF;
            int EID = ((colorHash)) & 0xFF;
            qDebug() << CID << EID;
            setSelectedObj((GameObj*)route->getActivityConsist(CID));
            if (selectedObj == NULL) {
                qDebug() << "brak obiektu";
            } else {
                //selectedObj->select(EID);
            }
        } else if( ww == 12 ){
            if (selectedObj != NULL) {
                selectedObj->unselect();
                if (autoAddToTDB)
                    route->addToTDBIfNotExist((WorldObj*)selectedObj);
                setSelectedObj(NULL);
            }
            int TID = ((colorHash) >> 19) & 0x1;
            int UID = ((colorHash)) & 0xFFFF;
            qDebug() << TID << UID;
            setSelectedObj((GameObj*)route->getTrackItem(TID, UID));
            if (selectedObj == NULL) {
                qDebug() << "brak obiektu";
            } else {
                //selectedObj->select(EID);
            }
        } else {
            if (selectedObj != NULL) {
                selectedObj->unselect();
                if (autoAddToTDB)
                    route->addToTDBIfNotExist((WorldObj*)selectedObj);
                setSelectedObj(NULL);
            }
        }

        //qDebug() << "selection" << selection;
        selection = false;// !selection;
        paintGL();
    }
}

void RouteEditorGLWidget::objectSelected(GameObj* obj){
    if (selectedObj != NULL) {
        selectedObj->unselect();
        if (autoAddToTDB)
            route->addToTDBIfNotExist((WorldObj*)selectedObj);
        setSelectedObj(NULL);
    }
    if(obj == NULL)
        return;
    obj->select();
    setSelectedObj(obj);
    
}

void RouteEditorGLWidget::drawPointer() {
    int x = mousex;
    int y = mousey;

    float winZ[4];
    int viewport[4];
    //float wcoord[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    //glGetFloatv(GL_MODELVIEW_MATRIX, mvmatrix);
    //glGetFloatv(GL_PROJECTION_MATRIX, projmatrix);
    int realy = viewport[3] - (int) y - 1;
    glReadPixels(x, realy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    GLH::glhUnProjectf((float) x, (float) realy, winZ[0], // 
            gluu->mvMatrix,
            gluu->pMatrix,
            viewport,
            aktPointerPos);
    //qDebug()<<aktPointerPos[0]<< aktPointerPos[1]<< aktPointerPos[2];
    if (Game::viewPointer3d) {
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, aktPointerPos[0], aktPointerPos[1], aktPointerPos[2]);
        Mat4::identity(gluu->objStrMatrix);
        gluu->setMatrixUniforms();
        //gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        pointer3d->render();
        gluu->mvPopMatrix();
    }
}

void RouteEditorGLWidget::resizeGL(int w, int h) {
    //gluu->m_proj.setToIdentity();
    //gluu->m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void RouteEditorGLWidget::keyPressEvent(QKeyEvent * event) {
    Game::currentShapeLib = currentShapeLib;
    if (!route->loaded) return;
    camera->keyDown(event);

    Undo::StateBeginIfNotExist();

    switch (event->key()) {
        case Qt::Key_Control:
            moveStep = moveMinStep;
            keyControlEnabled = true;
            break;
        case Qt::Key_Shift:
            keyShiftEnabled = true;
            break;
        case Qt::Key_Alt:
            moveStep = moveUltraStep;
            keyAltEnabled = true;
            break;
            //case Qt::Key_N:
            //if(selectedObj != NULL)
            //    route->deleteTDBTree(selectedObj);
            /*if(this->selectedObj != NULL){
                this->selectedObj->unselect();
                this->selectedObj = NULL;
            }
            this->selectedObj = route->makeFlexTrack((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            if(this->selectedObj != NULL){
                this->selectedObj->select();
                lastNewObjPosT[0] = this->selectedObj->x;
                lastNewObjPosT[1] = this->selectedObj->y;
                lastNewObjPos[0] = this->selectedObj->position[0];
                lastNewObjPos[1] = this->selectedObj->position[1];
                lastNewObjPos[2] = this->selectedObj->position[2];
            }*/
            //    break;
        case Qt::Key_B:
        {
            QMessageBox msgBox;
            msgBox.setText("Create new Tile here?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            if (msgBox.exec() == QMessageBox::Yes)
                route->newTile((int) camera->pozT[0], (int) camera->pozT[1]);
        }
            break;
        case Qt::Key_E:
            enableTool("selectTool");
            break;
        case Qt::Key_R:
            enableTool("selectTool");
            rotateTool = true;
            break;
        case Qt::Key_T:
            enableTool("selectTool");
            translateTool = true;
            break;
        case Qt::Key_Y:
            enableTool("selectTool");
            resizeTool = true;
            break;
        case Qt::Key_Q:
            if (keyControlEnabled)
                autoAddToTDB = !autoAddToTDB;
            else if (keyShiftEnabled)
                stickPointerToTerrain = !stickPointerToTerrain;
            else
                enableTool("placeTool");
            break;
        case Qt::Key_Home:
            aktPointerPos[1] += 40;
            jumpTo(camera->pozT, aktPointerPos);
            aktPointerPos[1] -= 40;
            break;
        default:
            break;
    }
    if (toolEnabled == "heightTool") {
        switch (event->key()) {
            case Qt::Key_Z:
                if (!keyControlEnabled) {
                    this->defaultPaintBrush->direction = -this->defaultPaintBrush->direction;
                    if (this->defaultPaintBrush->direction == 1)
                        emit sendMsg(QString("brushDirection"), QString("+"));
                    else
                        emit sendMsg(QString("brushDirection"), QString("-"));
                }
                break;
            default:
                break;
        }
    }
    if (toolEnabled == "selectTool" || toolEnabled == "placeTool") {
        Vector2f a;

        switch (event->key()) {
            case Qt::Key_Up:
                if (Game::usenNumPad) break;
            case Qt::Key_8:
                Undo::PushGameObjData(selectedObj);
                if (resizeTool && selectedObj != NULL) {
                    selectedObj->resize(moveStep, 0, 0);
                } else if (rotateTool && selectedObj != NULL) {
                    selectedObj->rotate(moveStep / 10, 0, 0);
                } else if (selectedObj != NULL) {
                    a.y = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    selectedObj->translate(a.x, 0, a.y);
                }
                break;
            case Qt::Key_Down:
                if (Game::usenNumPad) break;
            case Qt::Key_2:
                Undo::PushGameObjData(selectedObj);
                if (resizeTool && selectedObj != NULL) {
                    selectedObj->resize(-moveStep, 0, 0);
                } else if (rotateTool && selectedObj != NULL) {
                    selectedObj->rotate(-moveStep / 10, 0, 0);
                } else if (selectedObj != NULL) {
                    a.y = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    selectedObj->translate(a.x, 0, a.y);
                }
                break;
            case Qt::Key_Left:
                if (Game::usenNumPad) break;
            case Qt::Key_4:
                Undo::PushGameObjData(selectedObj);
                if (resizeTool && selectedObj != NULL) {
                    selectedObj->resize(0, moveStep, 0);
                } else if (rotateTool && selectedObj != NULL) {
                    selectedObj->rotate(0, -moveStep / 10, 0);
                } else if (selectedObj != NULL) {
                    a.x = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    selectedObj->translate(a.x, 0, a.y);
                }
                break;
            case Qt::Key_Right:
                if (Game::usenNumPad) break;
            case Qt::Key_6:
                Undo::PushGameObjData(selectedObj);
                if (resizeTool && selectedObj != NULL) {
                    selectedObj->resize(0, -moveStep, 0);
                } else if (rotateTool && selectedObj != NULL) {
                    selectedObj->rotate(0, moveStep / 10, 0);
                } else if (selectedObj != NULL) {
                    a.x = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    selectedObj->translate(a.x, 0, a.y);
                }
                break;
            case Qt::Key_PageUp:
                //Game::cameraFov += 1;
                //qDebug() << Game::cameraFov;
            case Qt::Key_9:
                Undo::PushGameObjData(selectedObj);
                if (resizeTool && selectedObj != NULL) {
                    selectedObj->resize(0, 0, moveStep);
                } else if (rotateTool && selectedObj != NULL) {
                    selectedObj->rotate(0, 0, moveStep / 10);
                } else if (selectedObj != NULL) {
                    selectedObj->translate(0, moveStep, 0);
                }
                break;
            case Qt::Key_PageDown:
                //Game::cameraFov -= 1;
                //qDebug() << Game::cameraFov;
            case Qt::Key_3:
            case Qt::Key_7:
                Undo::PushGameObjData(selectedObj);
                if (rotateTool && selectedObj != NULL) {
                    selectedObj->rotate(0, 0, -moveStep / 10);
                } else if (resizeTool && selectedObj != NULL) {
                    selectedObj->resize(0, 0, -moveStep);
                } else if (selectedObj != NULL) {
                    selectedObj->translate(0, -moveStep, 0);
                }
                break;
            case Qt::Key_F:
                Undo::StateBegin();
                if (selectedObj != NULL)
                    route->setTerrainToTrackObj((WorldObj*)selectedObj, defaultPaintBrush);
                else
                    route->setTerrainToTrackObj((WorldObj*)lastSelectedObj, defaultPaintBrush);
                Undo::StateEnd();
                break;
            case Qt::Key_H:
                if (selectedObj != NULL)
                    if(selectedObj->typeObj == GameObj::worldobj)
                        ((WorldObj*)selectedObj)->adjustPositionToTerrain();
                break;
            case Qt::Key_N:
                if (selectedObj != NULL)
                    if(selectedObj->typeObj == GameObj::worldobj)
                        ((WorldObj*)selectedObj)->adjustRotationToTerrain();
                break;
            case Qt::Key_Delete:
                if (selectedObj != NULL) {
                    route->deleteObj((WorldObj*)selectedObj);
                    selectedObj->unselect();
                    setSelectedObj(NULL);
                    lastSelectedObj = NULL;
                }
                break;
            case Qt::Key_C:
                if (selectedObj != NULL) {
                    selectedObj->unselect();
                    if(selectedObj->typeObj == GameObj::worldobj){
                        setSelectedObj(route->placeObject(((WorldObj*)selectedObj)->x, ((WorldObj*)selectedObj)->y, ((WorldObj*)selectedObj)->position, ((WorldObj*)selectedObj)->qDirection, ((WorldObj*)selectedObj)->getRefInfo()));
                        if (selectedObj != NULL) {
                            selectedObj->select();
                        }
                    }
                }
                break;
            case Qt::Key_P:
                if (selectedObj != NULL) {
                    if(selectedObj->typeObj == GameObj::worldobj){
                        Quat::copy(this->placeRot, ((WorldObj*)selectedObj)->qDirection);
                        route->ref->selected = ((WorldObj*)selectedObj)->getRefInfo();
                        emit itemSelected(route->ref->selected);
                    }
                }
                break;
            case Qt::Key_L:
                //route->nextDefaultEnd();
                break;
            case Qt::Key_Z:
                //route->refreshObj(selectedWorldObj);
                //route->trackDB->setDefaultEnd(0);
                //route->addToTDB(selectedWorldObj, (float*)&lastNewObjPosT, (float*)&selectedWorldObj->position);
                Undo::StateBegin();
                Undo::PushTrackDB(Game::trackDB, false);
                Undo::PushTrackDB(Game::roadDB, true);
                route->toggleToTDB((WorldObj*)selectedObj);
                Undo::StateEnd();
                if (selectedObj != NULL) selectedObj->unselect();
                lastSelectedObj = selectedObj;
                setSelectedObj(NULL);
                break;
            case Qt::Key_X:
                if (selectedObj == NULL)
                    return;
                //route->refreshObj(selectedWorldObj);
                route->flipObject((WorldObj*)selectedObj);
                break;
            default:
                break;
        }
    }
}

void RouteEditorGLWidget::keyReleaseEvent(QKeyEvent * event) {
    Game::currentShapeLib = currentShapeLib;
    if (!route->loaded) return;
    camera->keyUp(event);
    switch (event->key()) {
            //case Qt::Key_Alt:
        case Qt::Key_Control:
            moveStep = moveMaxStep;
            keyControlEnabled = false;
            break;
        case Qt::Key_Shift:
            keyShiftEnabled = false;
            break;
        case Qt::Key_Alt:
            moveStep = moveMaxStep;
            keyAltEnabled = false;
            break;
        default:
            break;
    }
    if (toolEnabled == "selectTool" || toolEnabled == "placeTool") {
        switch (event->key()) {
            default:
                break;
        }
    }
}

void RouteEditorGLWidget::mousePressEvent(QMouseEvent *event) {
    Game::currentShapeLib = currentShapeLib;
    if (!route->loaded) return;
    m_lastPos = event->pos();
    m_lastPos *= Game::PixelRatio;
    mouseClick = true;
    if ((event->button()) == Qt::RightButton) {
        mouseRPressed = true;
        camera->MouseDown(event);
    }
    if ((event->button()) == Qt::LeftButton) {
        Undo::StateBegin();
        mouseLPressed = true;
        lastMousePressTime = QDateTime::currentMSecsSinceEpoch();
        if (toolEnabled == "placeTool") {
            if (selectedObj != NULL) {
                selectedObj->unselect();
                if (autoAddToTDB)
                    if (selectedObj->typeObj == GameObj::worldobj)
                        route->addToTDBIfNotExist((WorldObj*) selectedObj);
            }
            Undo::StateBeginIfNotExist();
            lastNewObjPosT[0] = camera->pozT[0];
            lastNewObjPosT[1] = camera->pozT[1];
            lastNewObjPos[0] = aktPointerPos[0];
            lastNewObjPos[1] = aktPointerPos[1];
            lastNewObjPos[2] = aktPointerPos[2];
            float *q = Quat::create();
            Quat::copy(q, this->placeRot);
            setSelectedObj(route->placeObject((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos, q));
            if (selectedObj != NULL)
                selectedObj->select();
        }
        if (toolEnabled == "autoPlaceSimpleTool") {
            if (selectedObj != NULL) {
                selectedObj->unselect();
                if (autoAddToTDB)
                    if (selectedObj->typeObj == GameObj::worldobj)
                        route->addToTDBIfNotExist((WorldObj*) selectedObj);
            }
            Undo::StateBeginIfNotExist();
            lastNewObjPosT[0] = camera->pozT[0];
            lastNewObjPosT[1] = camera->pozT[1];
            lastNewObjPos[0] = aktPointerPos[0];
            lastNewObjPos[1] = aktPointerPos[1];
            lastNewObjPos[2] = aktPointerPos[2];
            int mode = 0;
            if (keyControlEnabled)
                mode = 1;
            if (keyShiftEnabled)
                mode = 2;
            setSelectedObj(route->autoPlaceObject((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos, mode));
            if (selectedObj != NULL)
                selectedObj->select();
        }
        if (toolEnabled == "selectTool") {
            if (!translateTool && !rotateTool && !resizeTool)
                selection = true;
            if (selectedObj != NULL) {
                mouseLPressed = true;
                if (translateTool) {
                    if (selectedObj->typeObj == GameObj::worldobj) {
                        Undo::PushGameObjData((WorldObj*) selectedObj);
                        ((WorldObj*) selectedObj)->setPosition(camera->pozT[0], camera->pozT[1], aktPointerPos);
                        ((WorldObj*) selectedObj)->setMartix();
                    }
                }
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];

            }
        }
        if (toolEnabled == "signalLinkTool") {
            Undo::PushGameObjData((WorldObj*) selectedObj);
            Undo::PushTrackDB(Game::trackDB);
            route->linkSignal((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos, (WorldObj*) selectedObj);
            enableTool("");
        }

        if (toolEnabled == "FlexTool") {
            emit flexData((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "heightTool") {
            // qDebug() << aktPointerPos[0] << " " << aktPointerPos[2];
            route->paintHeightMap(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled.startsWith("paintTool")) {
            // qDebug() << aktPointerPos[0] << " " << aktPointerPos[2];
            if (keyControlEnabled)
                route->setTerrainTextureToObj((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos, defaultPaintBrush, (WorldObj*) selectedObj);
            else
                TerrainLib::paintTexture(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "pickTerrainTexTool") {
            // qDebug() << aktPointerPos[0] << " " << aktPointerPos[2];
            int textureId = TerrainLib::getTexture((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
            emit setBrushTextureId(textureId);
        }
        if (toolEnabled == "putTerrainTexTool") {
            TerrainLib::setTerrainTexture(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
            lastPointerPos[0] = aktPointerPos[0];
            lastPointerPos[1] = aktPointerPos[1];
            lastPointerPos[2] = aktPointerPos[2];
        }
        if (toolEnabled == "waterTerrTool") {
            TerrainLib::setWaterDraw((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "drawTerrTool") {
            TerrainLib::setDraw((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "waterHeightTileTool") {
            TerrainLib::setWaterLevelGui((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "fixedTileTool") {
            TerrainLib::setFixedTileHeight(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "mapTileShowTool") {
            TerrainLib::setTileBlob((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "mapTileLoadTool") {
            int x = (int) camera->pozT[0];
            int z = (int) camera->pozT[1];
            float posx = aktPointerPos[0];
            float posz = aktPointerPos[2];
            Game::check_coords(x, z, posx, posz);
            mapWindow->tileX = x;
            mapWindow->tileZ = z;
            mapWindow->exec();
        }
        if (toolEnabled == "heightTileLoadTool") {
            TerrainLib::setHeightFromGeoGui((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "lockTexTool") {
            TerrainLib::lockTexture(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "gapsTerrainTool") {
            TerrainLib::toggleGaps((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "makeTileTextureTool") {
            TerrainLib::makeTextureFromMap((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "removeTileTextureTool") {
            TerrainLib::removeTileTextureFromMap((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "actNewLooseConsistTool") {
            route->actNewLooseConsist((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
            //TerrainLib::removeTileTextureFromMap((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
        }
        if (toolEnabled == "") {
            camera->MouseDown(event);
        }
    }
    setFocus();
}

void RouteEditorGLWidget::wheelEvent(QWheelEvent *event) {
    float numDegrees = 0.01 * event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (toolEnabled == "selectTool" || toolEnabled == "placeTool") {
            if (selectedObj != NULL) {
                if (selectedObj->typeObj == GameObj::worldobj) {
                    Undo::StateBeginIfNotExist();
                    Undo::PushGameObjData(selectedObj);
                    ((WorldObj*) selectedObj)->translate(0, numDegrees*moveStep, 0);
                }
            }
        }
    } else {

    }
    event->accept();
}

void RouteEditorGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    Game::currentShapeLib = currentShapeLib;
    if (!route->loaded) return;
    camera->MouseUp(event);
    if ((event->button()) == Qt::RightButton) {
        mouseRPressed = false;
        if(mouseClick)
            showContextMenu(event->pos());
    }
    if ((event->button()) == Qt::LeftButton) {
        mouseLPressed = false;
        Undo::StateEnd();
    }
    mouseClick = false;
}

void RouteEditorGLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseClick = false;
    Game::currentShapeLib = currentShapeLib;
    if (!route->loaded) return;
    /*int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {

    } else if (event->buttons() & Qt::RightButton) {

    }*/
    mousex = event->x() * Game::PixelRatio;
    mousey = event->y() * Game::PixelRatio;

    if ((event->buttons() & 2) == Qt::RightButton) {
        camera->MouseMove(event);
    }
    if ((event->buttons() & 1) == Qt::LeftButton) {
        if (toolEnabled.startsWith("paintTool") && mouseLPressed == true) {
            if (mousex != m_lastPos.x() || mousey != m_lastPos.y()) {
                TerrainLib::paintTexture(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
            }
        }
        if (toolEnabled == "heightTool" && mouseLPressed == true) {
            if (mousex != m_lastPos.x() || mousey != m_lastPos.y()) {
                route->paintHeightMap(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
            }
        }
        if (toolEnabled == "putTerrainTexTool" && mouseLPressed == true) {
            if (fabs(lastPointerPos[0] - aktPointerPos[0]) > 32 || fabs(lastPointerPos[2] - aktPointerPos[2]) > 32) {
                TerrainLib::setTerrainTexture(defaultPaintBrush, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
            }
        }
        if (toolEnabled == "selectTool") {
            if (selectedObj != NULL && mouseLPressed) {
                if (!translateTool && !rotateTool && !resizeTool) {
                long long int ntime = QDateTime::currentMSecsSinceEpoch();
                if (ntime - lastMousePressTime > 200) {
                        Undo::PushGameObjData(selectedObj);
                        if (keyShiftEnabled) {
                            float val = mousex - m_lastPos.x();
                            selectedObj->rotate(0, val * moveStep * 0.1, 0);
                        } else {
                            if(selectedObj->typeObj == GameObj::worldobj)
                                route->dragWorldObject((WorldObj*)selectedObj, camera->pozT[0], camera->pozT[1], aktPointerPos);
                            if(selectedObj->typeObj != GameObj::consistobj)
                                selectedObj->setPosition((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
                        }
                    }
                }
                if (translateTool) {
                    Undo::PushGameObjData(selectedObj);
                    selectedObj->setPosition(camera->pozT[0], camera->pozT[1], aktPointerPos);
                    selectedObj->setMartix();
                }
                if (rotateTool) {
                    Undo::PushGameObjData(selectedObj);
                    float val = mousex - m_lastPos.x();
                    selectedObj->rotate(0, val * moveStep * 0.1, 0);
                }
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
            }
        }
        if (toolEnabled == "placeTool") {
            if (selectedObj != NULL && mouseLPressed) {
                long long int ntime = QDateTime::currentMSecsSinceEpoch();
                if (ntime - lastMousePressTime > 200) {
                    Undo::PushGameObjData(selectedObj);
                    if (keyShiftEnabled) {
                        float val = mousex - m_lastPos.x();
                        selectedObj->rotate(0, val * moveStep * 0.1, 0);
                    } else {
                        route->dragWorldObject((WorldObj*)selectedObj, camera->pozT[0], camera->pozT[1], aktPointerPos);
                    }
                }
            }
        }
        if (toolEnabled == "") {
            camera->MouseMove(event);
        }
    }
    m_lastPos = event->pos();
    m_lastPos *= Game::PixelRatio;
}

void RouteEditorGLWidget::enableTool(QString name) {
    qDebug() << name;
    toolEnabled = name;
    //if(toolEnabled == "placeTool" || toolEnabled == "selectTool" || toolEnabled == "autoPlaceSimpleTool"){
    resizeTool = false;
    translateTool = false;
    rotateTool = false;
    //}
    emit sendMsg("toolEnabled", name);
}

void RouteEditorGLWidget::jumpTo(PreciseTileCoordinate* c) {
    jumpTo(c->TileX, -c->TileZ, c->wX, c->wY, -c->wZ);
}

void RouteEditorGLWidget::jumpTo(float *posT, float *pos) {
    int X = posT[0];
    int Z = posT[1];
    float x = pos[0];
    float z = pos[2];
    Game::check_coords(X, Z, x, z);
    jumpTo(X, Z, x, pos[1], z);
}

void RouteEditorGLWidget::jumpTo(int X, int Z, float x, float y, float z) {
    qDebug() << "jump: " << X << " " << Z;
    TerrainLib::load(X, Z);
    float h = TerrainLib::getHeight(X, Z, x, y, z);
    if (y < h || y > h + 100) y = h + 20;
    camera->setPozT(X, Z);
    camera->setPos(x, y, z);

}

void RouteEditorGLWidget::objSelected(GameObj* o) {
    if (selectedObj == NULL)
        return;
    selectedObj->unselect();
    setSelectedObj(o);
    if (selectedObj != NULL)
        selectedObj->select();
}

void RouteEditorGLWidget::setPaintBrush(Brush* brush) {
    this->defaultPaintBrush = brush;
}

void RouteEditorGLWidget::setSelectedObj(GameObj* o) {
    this->selectedObj = o;
    emit showProperties(selectedObj);
    if (o != NULL)
        if (o->typeObj == o->worldobj)
            emit sendMsg("showShape", ((WorldObj*) o)->getShapePath());
}

void RouteEditorGLWidget::editCopy() {
    if (toolEnabled == "selectTool" || toolEnabled == "placeTool") {
        if (selectedObj != NULL) {
            if (selectedObj->typeObj == GameObj::worldobj) {
                WorldObj *selectedWorldObj = (WorldObj*) selectedObj;
                if (selectedWorldObj->typeID == WorldObj::groupobject) {
                    delete copyPasteGroupObj;
                    copyPasteGroupObj = new GroupObj(*groupObj);
                    copyPasteObj = copyPasteGroupObj;
                } else {
                    copyPasteObj = selectedWorldObj;
                }
            }
        }
    }
}

void RouteEditorGLWidget::editPaste() {
    qDebug() << "EditPaste Start";
    Undo::StateBeginIfNotExist();
    if (toolEnabled == "selectTool" || toolEnabled == "placeTool") {
        if (copyPasteObj != NULL) {
            //qDebug() << "EditPaste selectedObj->unselect";
            if (selectedObj != NULL)
                selectedObj->unselect();
            lastNewObjPosT[0] = camera->pozT[0];
            lastNewObjPosT[1] = camera->pozT[1];
            lastNewObjPos[0] = aktPointerPos[0];
            lastNewObjPos[1] = aktPointerPos[1];
            lastNewObjPos[2] = aktPointerPos[2];
            //qDebug() << "EditPaste copyPasteObj->typeObj";
            if(copyPasteObj->typeObj == GameObj::worldobj) {
               // qDebug() << "EditPaste copyPasteObj->typeID";
                if (copyPasteObj->typeID == WorldObj::groupobject) {
                    //qDebug() << "EditPaste groupobject";
                    groupObj->fromNewObjects((GroupObj*) copyPasteObj, route, (int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos);
                    //qDebug() << "EditPaste setSelectedObj";
                    setSelectedObj(groupObj);
                } else {
                    //qDebug() << "EditPaste object";
                    float *q = Quat::create();
                    Quat::copy(q, copyPasteObj->qDirection);
                    setSelectedObj(route->placeObject((int) camera->pozT[0], (int) camera->pozT[1], aktPointerPos, q, copyPasteObj->getRefInfo()));
                    //qDebug() << "EditPaste setSelectedObj";
                    if (selectedObj != NULL)
                        selectedObj->select();
                }
            }
        }
    }
    qDebug() << "EditPaste End";
}

void RouteEditorGLWidget::editFind1x1() {
    editFind(0);
}

void RouteEditorGLWidget::editFind3x3() {
    editFind(1);
}

void RouteEditorGLWidget::editFind(int radius) {
    if (selectedObj != NULL)
        if (selectedObj->typeObj == GameObj::worldobj){
            selectedObj->unselect();
            route->findSimilar((WorldObj*)selectedObj, groupObj, camera->pozT, radius);
            setSelectedObj(groupObj);
            if (groupObj->count() == 0) {
                groupObj->unselect();
                setSelectedObj(NULL);
            }
        }
}

void RouteEditorGLWidget::editUndo() {
    Undo::UndoLast();
}

void RouteEditorGLWidget::showTrkEditr() {
    if (route != NULL)
        route->showTrkEditr();
}

void RouteEditorGLWidget::showContextMenu(const QPoint & point) {
    QMenu menu;
    if(selectedObj != NULL){
        menu.addSection("Object");
        selectedObj->pushContextMenuActions(&menu);
        
        if(selectedObj->typeObj == selectedObj->worldobj){
            menu.addAction(defaultMenuActions.find1x1);
            menu.addAction(defaultMenuActions.find3x3);
        }
    }
    //menu.addSeparator();
    menu.addSection("Edit");
    menu.addAction(defaultMenuActions.undo);
    menu.addAction(defaultMenuActions.copy);
    menu.addAction(defaultMenuActions.paste);
    menu.exec(mapToGlobal(point));
}

void RouteEditorGLWidget::createNewTiles(QMap<int, QPair<int, int>*> list){
    int x, z;
    QMapIterator<int, QPair<int, int>*> i2(list);
    while (i2.hasNext()) {
        i2.next();
        if(i2.value() == NULL)
            continue;
        x = i2.value()->first;
        z = i2.value()->second;
        qDebug() << x << z;
        route->newTile(x, -z);
    }
    
}

void RouteEditorGLWidget::getUnsavedInfo(std::vector<QString> &items) {
    if (this->route == NULL)
        return;
    route->getUnsavedInfo(items);
}

void RouteEditorGLWidget::msg(QString text) {
    qDebug() << text;
    if (text == "save") {
        route->save();
        return;
    }
    if (text == "createPaths") {
        route->createNewPaths();
        return;
    }
    if (text == "resetPlaceRotation") {
        Quat::fill(this->placeRot);
        return;
    }
    if (text == "showTerrainTreeEditr") {
        TerrainTreeWindow ttWindow;
        ttWindow.exec();
        return;
    }
    if (text == "engItemSelected") {
        //QString pathid = ;
        //QString name = pathid.split("/").last();
        //QString texpath = pathid.left(pathid.length() - name.length());
        emit sendMsg("showShape", route->ref->selected->getShapePath());
    }
    if (text == "autoPlacementDeleteLast") {
        route->autoPlacementDeleteLast();
    }
}

void RouteEditorGLWidget::msg(QString text, bool val) {
    qDebug() << text;
    if (text == "stickToTDB") {
        this->route->placementStickToTarget = val;
        return;
    }
}

void RouteEditorGLWidget::msg(QString text, int val) {
}

void RouteEditorGLWidget::msg(QString text, float val) {
    qDebug() << text;
    if (text == "autoPlacementLength") {
        this->route->placementAutoLength = val;
        return;
    }
}

void RouteEditorGLWidget::msg(QString text, QString val) {
    //qDebug() << text;
    if (text == "mkrFile") {
        this->route->setMkrFile(val);
        return;
    }
}