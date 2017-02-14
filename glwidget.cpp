/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "glwidget.h"
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

GLWidget::GLWidget(QWidget *parent)
: QOpenGLWidget(parent),
m_xRot(0),
m_yRot(0),
m_zRot(0) {

}

GLWidget::~GLWidget() {
    cleanup();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(1000, 700);
}

void GLWidget::cleanup() {
    makeCurrent();
    //delete gluu->m_program;
    //gluu->m_program = 0;
    doneCurrent();
}

void GLWidget::timerEvent(QTimerEvent * event) {
    Game::currentShapeLib = currentShapeLib;
    timeNow = QDateTime::currentMSecsSinceEpoch();
    if (timeNow - lastTime < 1)
        fps = 1;
    else
        fps = 1000.0 / (timeNow - lastTime);
    if (fps < 10) fps = 10;
    
    if (timeNow % 200 < lastTime % 200){
        //qDebug() << "new second" << timeNow;
        if(selectedObj != NULL)
            emit updateProperties(selectedObj);
        Undo::StateEndIfLongTime();
    }
    
    route->updateSim(camera->pozT, (float)(timeNow-lastTime)/1000.0);
    
    lastTime = timeNow;

    if (Game::allowObjLag < Game::maxObjLag)
        Game::allowObjLag += 2;
    
    camera->update(fps);
    update();
}

void GLWidget::initializeGL() {
    currentShapeLib = new ShapeLib();
    Game::currentShapeLib = currentShapeLib;
    //qDebug() << "GLUU::get();";
    gluu = GLUU::get();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
    //qDebug() << "initializeOpenGLFunctions();";
    initializeOpenGLFunctions();
    /*QOpenGLFunctions_3_3_Core* funcs = 0;
    funcs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    if (!funcs) {
        qWarning() << "Could not obtain required OpenGL context version";
        exit(1);
    }
    funcs->initializeOpenGLFunctions();*/
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
    if(!route->loaded) return;
    QObject::connect(route, SIGNAL(objSelected(WorldObj*)), this, SLOT(objSelected(WorldObj*)));
    
    float * aaa = new float[2]{0,0};
    camera = new CameraFree(aaa);
    float spos[3];
    if(Game::start == 2){
        camera->setPozT(Game::startTileX, -Game::startTileY);
    } else {
        camera->setPozT(route->getStartTileX(), -route->getStartTileZ());
        spos[0] = route->getStartpX();
        spos[2] = -route->getStartpZ();
    }
    if(TerrainLib::load(route->getStartTileX(), -route->getStartTileZ())){
        spos[1] = 20 + TerrainLib::getHeight(route->getStartTileX(), -route->getStartTileZ(), route->getStartpX(), -route->getStartpZ());
    } else {
        spos[1] = 0;
    }
    camera->setPos((float*)&spos);
    
    lastTime = QDateTime::currentMSecsSinceEpoch();
    int timerStep = 15;
    if(Game::fpsLimit > 0)
        timerStep = 1000/Game::fpsLimit;
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

void GLWidget::paintGL() {
    Game::currentShapeLib = currentShapeLib;
    if(route == NULL) return;
    if(!route->loaded) return;

    // Render Shadows
    if(Game::shadowsEnabled > 0)
        renderShadowMaps();

    // Render Scene
    gluu->currentShader = gluu->shaders["StandardBloom"];
    //gluu->currentShader = gluu->shaders["StandardFog"];
    gluu->currentShader->bind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(gluu->skyc[0], gluu->skyc[1], gluu->skyc[2], 1.0);

    glViewport(0,0,this->width(),this->height());
    Mat4::perspective(gluu->pMatrix, Game::cameraFov*M_PI/180, float(this->width()) / this->height(), 0.2f, Game::objectLod);
    Mat4::multiply(gluu->pMatrix, gluu->pMatrix, camera->getMatrix());
    Mat4::identity(gluu->mvMatrix);
    gluu->setMatrixUniforms();

    if (!selection){
        TerrainLib::render(gluu, camera->pozT, camera->getPos(), camera->getTarget(), 3.14f / 3);
    }
    if(stickPointerToTerrain && Game::viewTerrainShape)
        if (!selection) drawPointer();
    
    int renderMode = GLUU::RENDER_DEFAULT;
    if(selection)
        renderMode = GLUU::RENDER_SELECTION;
    route->render(gluu, camera->pozT, camera->getPos(), camera->getTarget(), camera->getRotX(), 3.14f / 3, renderMode);
    
    if(!stickPointerToTerrain || !Game::viewTerrainShape)
        if (!selection) drawPointer();

    gluu->currentShader->release();
    
    // Handle Selection
    handleSelection();

    // Set Info
    if(this->isActiveWindow()){
        emit this->naviInfo(route->getTileObjCount((int)camera->pozT[0], (int)camera->pozT[1]), route->getTileHiddenObjCount((int)camera->pozT[0], (int)camera->pozT[1]));
        emit this->posInfo(camera->getCurrentPos());
        emit this->pointerInfo(aktPointerPos);
    }
}

void GLWidget::renderShadowMaps(){
    float* lookAt = Mat4::create();
    float* out1 = Vec3::create();    
    Vec3::set(out1, 0,1,0);
    float *ld = Vec3::create();
    Vec3::set(ld, -1.0,2.0,1.0);
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
    glViewport(0,0,Game::shadowMapSize,Game::shadowMapSize);
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
    glViewport(0,0,1024,1024);
    Game::objectLod = 1000;
    route->renderShadowMap(gluu, camera->pozT, camera->getPos(), camera->getTarget(), camera->getRotX(), 3.14f / 3, selection);
    gluu->pShadowMatrix2 = gluu->pShadowMatrix;
    gluu->pShadowMatrix = tmatrix;
    Game::objectLod = tempLod;
    gluu->currentShader->release();
}

void GLWidget::handleSelection(){
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
        int colorHash = (int)(winZ[0]*255)*256*256 + (int)(winZ[1]*255)*256 + (int)(winZ[2]*255);
        int wdata = colorHash / 131072;
        int UiD = colorHash - wdata*131072;
        
        int ww1 = wdata;//(int) (winZ[0]*255);
        //int ww = ww1/10;
        int cdata = (int) (ww1 / 8);
        int ww  = (ww1 - cdata*8);
        qDebug() << "selected " <<ww1 <<" "<<ww;
        int wx = 0;//(int) (ww / 4);
        int wz = 0;//(int) (ww - (wx)*4);
        if(ww == 0 || ww == 1 || ww == 2) wx = camera->pozT[0] - 1;
        if(ww == 3 || ww == 4 || ww == 5) wx = camera->pozT[0];
        if(ww == 6 || ww == 7 || ww == 8) wx = camera->pozT[0] + 1;
        if(ww == 0 || ww == 3 || ww == 6) wz = camera->pozT[1] - 1;
        if(ww == 1 || ww == 4 || ww == 7) wz = camera->pozT[1];
        if(ww == 2 || ww == 5 || ww == 8) wz = camera->pozT[1] + 1;
        //wx = camera->pozT[0] + (wx - 1);
        //wz = camera->pozT[1] + (wz - 1);
        qDebug() << "color data: " << cdata;

        //int UiD = (int) (winZ[1]*255)*256 + (int) (winZ[2]*255);

        qDebug() << wx << " " << wz << " " << UiD;
        
        if(keyControlEnabled){
            if(selectedObj == NULL) 
                setSelectedObj(groupObj);
            if(selectedObj->typeID != WorldObj::groupobject){
                groupObj->addObject(selectedObj);
                setSelectedObj(groupObj);
            }
            groupObj->addObject(route->getObj(wx, wz, UiD));
            if (groupObj->count() == 0){
                qDebug() << "brak obiektu";
                groupObj->unselect();
                setSelectedObj(NULL);
            }
        } else {
            WorldObj* twobj = route->getObj(wx, wz, UiD);
            if(selectedObj != NULL && twobj != selectedObj) {
                selectedObj->unselect();
                if(autoAddToTDB){
                    route->addToTDBIfNotExist(selectedObj);
                }
            }
            lastSelectedObj = selectedObj;
            setSelectedObj(twobj);
            if (selectedObj == NULL){
                qDebug() << "brak obiektu";
            } else {
                selectedObj->select(cdata);
            }
        }

        selection = !selection;
        paintGL();
    }
}

void GLWidget::drawPointer(){
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
    if(Game::viewPointer3d){
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, aktPointerPos[0], aktPointerPos[1], aktPointerPos[2]);
        Mat4::identity(gluu->objStrMatrix);
        gluu->setMatrixUniforms();
        //gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        pointer3d->render();
        gluu->mvPopMatrix();
    }
}

void GLWidget::resizeGL(int w, int h) {
    //gluu->m_proj.setToIdentity();
    //gluu->m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::keyPressEvent(QKeyEvent * event) {
    Game::currentShapeLib = currentShapeLib;
    if(!route->loaded) return;
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
        case Qt::Key_B: {
            QMessageBox msgBox;
            msgBox.setText("Create new Tile here?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            if(msgBox.exec() == QMessageBox::Yes)
                route->newTile((int)camera->pozT[0], (int)camera->pozT[1]);
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
            if(keyControlEnabled)
                autoAddToTDB = !autoAddToTDB;
            else if(keyShiftEnabled)
                stickPointerToTerrain = !stickPointerToTerrain;
            else
                enableTool("placeTool");
            break;
        case Qt::Key_Home:
            aktPointerPos[1]+=40;
            jumpTo(camera->pozT, aktPointerPos);
            aktPointerPos[1]-=40;
            break;
        default:
            break;
    }
    if(toolEnabled == "heightTool"){
        switch (event->key()) {
            case Qt::Key_Z:
                if(!keyControlEnabled){
                    this->defaultPaintBrush->direction = -this->defaultPaintBrush->direction;
                    if(this->defaultPaintBrush->direction == 1)
                        emit sendMsg(QString("brushDirection"),QString("+"));
                    else
                        emit sendMsg(QString("brushDirection"),QString("-"));
                }
                break;
            default:
                break;
        }
    }
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        Vector2f a;

        switch (event->key()) {
            case Qt::Key_Up:    
                if(Game::usenNumPad) break;
            case Qt::Key_8:
                Undo::PushWorldObjData(selectedObj);
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(moveStep, 0, 0);
                } else if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(moveStep/10, 0, 0);
                } else if(selectedObj != NULL){
                    a.y = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }  
                break;
            case Qt::Key_Down:    
                if(Game::usenNumPad) break;    
            case Qt::Key_2:
                Undo::PushWorldObjData(selectedObj);
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(-moveStep, 0, 0);
                } else if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(-moveStep/10, 0, 0);
                } else if(selectedObj != NULL){
                    a.y = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                } 
                break;    
            case Qt::Key_Left:    
                if(Game::usenNumPad) break;    
            case Qt::Key_4:
                Undo::PushWorldObjData(selectedObj);
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, moveStep, 0);
                } else if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, -moveStep/10, 0);
                } else if(selectedObj != NULL){
                    a.x = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                break;
            case Qt::Key_Right:    
                if(Game::usenNumPad) break;     
            case Qt::Key_6:
                Undo::PushWorldObjData(selectedObj);
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, -moveStep, 0);
                } else if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, moveStep/10, 0);
                } else if(selectedObj != NULL){
                    a.x = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                break;
            case Qt::Key_PageUp:
                //Game::cameraFov += 1;
                //qDebug() << Game::cameraFov;
            case Qt::Key_9:
                Undo::PushWorldObjData(selectedObj);
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, 0, moveStep);
                } else if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, 0, moveStep/10);
                } else if(selectedObj != NULL){
                    this->selectedObj->translate(0,moveStep,0);
                }
                break;
            case Qt::Key_PageDown:
                //Game::cameraFov -= 1;
                //qDebug() << Game::cameraFov;
            case Qt::Key_3:
            case Qt::Key_7:    
                Undo::PushWorldObjData(selectedObj);
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, 0, -moveStep/10);
                } else if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, 0, -moveStep);
                } else if(selectedObj != NULL){
                    this->selectedObj->translate(0,-moveStep,0);
                }
                break;                
            case Qt::Key_F:
                Undo::StateBegin();
                if(selectedObj != NULL)
                    route->setTerrainToTrackObj(selectedObj, defaultPaintBrush);
                else
                    route->setTerrainToTrackObj(lastSelectedObj, defaultPaintBrush);
                Undo::StateEnd();
                break;
            case Qt::Key_H:
                if(selectedObj != NULL)
                    selectedObj->adjustPositionToTerrain();
                break;
            case Qt::Key_N:
                if(selectedObj != NULL)
                    selectedObj->adjustRotationToTerrain();
                break;
            case Qt::Key_Delete:
                if(selectedObj != NULL){
                    route->deleteObj(selectedObj);
                    selectedObj->unselect();
                    setSelectedObj(NULL);
                    lastSelectedObj = NULL;
                }
                break;                
            case Qt::Key_C:
                if(selectedObj != NULL){
                    selectedObj->unselect();
                    setSelectedObj(route->placeObject(selectedObj->x, selectedObj->y, selectedObj->position, selectedObj->qDirection, selectedObj->getRefInfo()));
                    if(selectedObj != NULL){
                        selectedObj->select();
                    }
                }
                break;
            case Qt::Key_P:
                if(selectedObj != NULL){
                    Quat::copy(this->placeRot, selectedObj->qDirection);
                    route->ref->selected = selectedObj->getRefInfo();
                    emit itemSelected(route->ref->selected);
                }
                break;
            case Qt::Key_L:
                //route->nextDefaultEnd();
                break;
            case Qt::Key_Z:
                //route->refreshObj(selectedObj);
                //route->trackDB->setDefaultEnd(0);
                //route->addToTDB(selectedObj, (float*)&lastNewObjPosT, (float*)&selectedObj->position);
                Undo::StateBegin();
                Undo::PushTrackDB(Game::trackDB, false);
                Undo::PushTrackDB(Game::roadDB, true);
                route->toggleToTDB(selectedObj);
                Undo::StateEnd();
                if(selectedObj != NULL) selectedObj->unselect();
                lastSelectedObj = selectedObj;
                setSelectedObj(NULL);
                break;
            case Qt::Key_X:
                if(selectedObj == NULL)
                    return;
                //route->refreshObj(selectedObj);
                route->flipObject(selectedObj);
                break;
            default:
                break;
        }
    }
    
    //switch (event->key()) {
    //    default:
    //        break;
    //}
}

void GLWidget::keyReleaseEvent(QKeyEvent * event) {
    Game::currentShapeLib = currentShapeLib;
    if(!route->loaded) return;
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
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        switch (event->key()) {
            default:
                break;
        }
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    Game::currentShapeLib = currentShapeLib;
    if(!route->loaded) return;
    m_lastPos = event->pos();
    
    if((event->button()) == Qt::RightButton){
        mouseRPressed = true;
        camera->MouseDown(event);
    }
    if((event->button()) == Qt::LeftButton){
        Undo::StateBegin();
        mouseLPressed = true;
        lastMousePressTime = QDateTime::currentMSecsSinceEpoch();
        if(toolEnabled == "placeTool"){
            if(selectedObj != NULL){
                selectedObj->unselect();
                if(autoAddToTDB){
                    route->addToTDBIfNotExist(selectedObj);
                }
            }
            Undo::StateBeginIfNotExist();
            lastNewObjPosT[0] = camera->pozT[0];
            lastNewObjPosT[1] = camera->pozT[1];
            lastNewObjPos[0] = aktPointerPos[0];
            lastNewObjPos[1] = aktPointerPos[1];
            lastNewObjPos[2] = aktPointerPos[2];
            float *q = Quat::create();
            Quat::copy(q, this->placeRot);
            setSelectedObj(route->placeObject((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos, q));
            if(selectedObj != NULL)
                selectedObj->select();
            //if(route->ref->selected != NULL){
            //qDebug() << route->ref->selected->description;
            //}
            //camera->MouseDown(event);
        }
        if(toolEnabled == "autoPlaceSimpleTool"){
            if(selectedObj != NULL){
                selectedObj->unselect();
                if(autoAddToTDB){
                    route->addToTDBIfNotExist(selectedObj);
                }
            }
            Undo::StateBeginIfNotExist();
            lastNewObjPosT[0] = camera->pozT[0];
            lastNewObjPosT[1] = camera->pozT[1];
            lastNewObjPos[0] = aktPointerPos[0];
            lastNewObjPos[1] = aktPointerPos[1];
            lastNewObjPos[2] = aktPointerPos[2];
            int mode = 0;
            if(keyControlEnabled) 
                mode = 1;
            if(keyShiftEnabled) 
                mode = 2;
            setSelectedObj(route->autoPlaceObject((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos, mode));
            if(selectedObj != NULL)
                selectedObj->select();
            //if(route->ref->selected != NULL){
            //qDebug() << route->ref->selected->description;
            //}
            //camera->MouseDown(event);
        }
        if(toolEnabled == "selectTool"){
            if(!translateTool && !rotateTool && !resizeTool)
                selection = true;
            //mousePressed = false;
            if(selectedObj != NULL){
                mouseLPressed = true;
                if(translateTool){
                    Undo::PushWorldObjData(selectedObj);
                    selectedObj->setPosition(camera->pozT[0], camera->pozT[1], aktPointerPos);
                    selectedObj->setMartix();
                }
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
                
            } //else {
                //camera->MouseDown(event);
            //}
        }
        if(toolEnabled == "FlexTool"){
            emit flexData((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "heightTool"){
            // qDebug() << aktPointerPos[0] << " " << aktPointerPos[2];
            route->paintHeightMap(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled.startsWith("paintTool")){
            // qDebug() << aktPointerPos[0] << " " << aktPointerPos[2];
            if(keyControlEnabled)
                route->setTerrainTextureToObj((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos, defaultPaintBrush, selectedObj);
            else
                TerrainLib::paintTexture(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "pickTerrainTexTool"){
           // qDebug() << aktPointerPos[0] << " " << aktPointerPos[2];
           int textureId = TerrainLib::getTexture((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
           emit setBrushTextureId(textureId);
        }
        if(toolEnabled == "putTerrainTexTool"){
            TerrainLib::setTerrainTexture(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            lastPointerPos[0] = aktPointerPos[0];
            lastPointerPos[1] = aktPointerPos[1];
            lastPointerPos[2] = aktPointerPos[2];
        }
        if(toolEnabled == "waterTerrTool"){
            TerrainLib::setWaterDraw((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "drawTerrTool"){
            TerrainLib::setDraw((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "waterHeightTileTool"){
            TerrainLib::setWaterLevelGui((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "fixedTileTool"){
            TerrainLib::setFixedTileHeight(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "mapTileShowTool"){
            TerrainLib::setTileBlob((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "mapTileLoadTool"){
            int x = (int)camera->pozT[0];
            int z = (int)camera->pozT[1];
            float posx = aktPointerPos[0];
            float posz = aktPointerPos[2];
            Game::check_coords(x, z, posx, posz);
            mapWindow->tileX = x;
            mapWindow->tileZ = z;
            mapWindow->exec();
        }
        if(toolEnabled == "heightTileLoadTool"){
            TerrainLib::setHeightFromGeoGui((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "lockTexTool"){
            TerrainLib::lockTexture(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "gapsTerrainTool"){
            TerrainLib::toggleGaps((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "signalLinkTool"){
            Undo::PushWorldObjData(selectedObj);
            Undo::PushTrackDB(Game::trackDB);
            route->linkSignal((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos, selectedObj);
            enableTool("");
        }
        if(toolEnabled == "makeTileTextureTool"){
            TerrainLib::makeTextureFromMap((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "removeTileTextureTool"){
            TerrainLib::removeTileTextureFromMap((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == "actNewLooseConsistTool"){
            route->actNewLooseConsist((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            TerrainLib::removeTileTextureFromMap((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
        }
        if(toolEnabled == ""){
            camera->MouseDown(event);
        }
    }
    setFocus();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    float numDegrees = 0.01*event->delta();

    if (event->orientation() == Qt::Vertical) {
        if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
            if(selectedObj != NULL){
                Undo::StateBeginIfNotExist();
                Undo::PushWorldObjData(selectedObj);
                this->selectedObj->translate(0,numDegrees*moveStep,0);
            }
        }
    } else {

    }
    event->accept();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event) {
    Game::currentShapeLib = currentShapeLib;
    if(!route->loaded) return;
    camera->MouseUp(event);
    if((event->button()) == Qt::RightButton){
        mouseRPressed = false;
    }
    if((event->button()) == Qt::LeftButton){
        mouseLPressed = false;
        Undo::StateEnd();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    Game::currentShapeLib = currentShapeLib;
    if(!route->loaded) return;
    /*int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {

    } else if (event->buttons() & Qt::RightButton) {

    }*/
    mousex = event->x();
    mousey = event->y();

    if((event->buttons() & 2) == Qt::RightButton){
        camera->MouseMove(event);
    }
    if((event->buttons() & 1) == Qt::LeftButton){
        if(toolEnabled.startsWith("paintTool") && mouseLPressed == true){
            if(mousex != m_lastPos.x() || mousey != m_lastPos.y()){
                TerrainLib::paintTexture(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            }
        }
        if(toolEnabled == "heightTool" && mouseLPressed == true){
            if(mousex != m_lastPos.x() || mousey != m_lastPos.y()){
                route->paintHeightMap(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            }
        }
        if(toolEnabled == "putTerrainTexTool" && mouseLPressed == true){
            if(fabs(lastPointerPos[0] - aktPointerPos[0]) > 32 || fabs(lastPointerPos[2] - aktPointerPos[2]) > 32 ){
                TerrainLib::setTerrainTexture(defaultPaintBrush, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
            }
        }
        if(toolEnabled == "selectTool"){
            if(selectedObj != NULL && mouseLPressed){
                if(!translateTool && !rotateTool && !resizeTool){
                    long long int ntime = QDateTime::currentMSecsSinceEpoch();
                    if(ntime - lastMousePressTime > 200){
                        Undo::PushWorldObjData(selectedObj);
                        if(keyShiftEnabled){
                            float val = mousex - m_lastPos.x();
                            selectedObj->rotate(0,val*moveStep*0.1,0);
                        } else {
                            route->dragWorldObject(selectedObj, camera->pozT[0], camera->pozT[1], aktPointerPos);
                            //selectedObj->setPosition(camera->pozT[0], camera->pozT[1], aktPointerPos);
                            //selectedObj->setMartix();
                        }
                    }
                }
                if(translateTool){
                    Undo::PushWorldObjData(selectedObj);
                    selectedObj->setPosition(camera->pozT[0], camera->pozT[1], aktPointerPos);
                    selectedObj->setMartix();
                }
                if(rotateTool){
                    Undo::PushWorldObjData(selectedObj);
                    float val = mousex - m_lastPos.x();
                    selectedObj->rotate(0,val*moveStep*0.1,0);
                }
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
                
            } //else {
            //    camera->MouseMove(event);
            //}
        }
        if(toolEnabled == "placeTool"){
            if(selectedObj != NULL && mouseLPressed){
                long long int ntime = QDateTime::currentMSecsSinceEpoch();
                if(ntime - lastMousePressTime > 200){
                    Undo::PushWorldObjData(selectedObj);
                    if(keyShiftEnabled){
                        float val = mousex - m_lastPos.x();
                        selectedObj->rotate(0,val*moveStep*0.1,0);
                    } else {
                        route->dragWorldObject(selectedObj, camera->pozT[0], camera->pozT[1], aktPointerPos);
                        //selectedObj->setPosition(camera->pozT[0], camera->pozT[1], aktPointerPos);
                        //selectedObj->setMartix();
                    }
                }
            }
        }
        if(toolEnabled == ""){
            camera->MouseMove(event);
        }
    }
    m_lastPos = event->pos();
}

void GLWidget::enableTool(QString name){
    qDebug() << name;
    toolEnabled = name;
    //if(toolEnabled == "placeTool" || toolEnabled == "selectTool" || toolEnabled == "autoPlaceSimpleTool"){
    resizeTool = false;
    translateTool = false;
    rotateTool = false;
    //}
    emit sendMsg("toolEnabled", name);
}

void GLWidget::jumpTo(PreciseTileCoordinate* c){
    jumpTo(c->TileX, -c->TileZ, c->wX, c->wY, -c->wZ);
}

void GLWidget::jumpTo(float *posT, float *pos){
    int X = posT[0];
    int Z = posT[1];
    float x = pos[0];
    float z = pos[2];
    Game::check_coords(X, Z, x, z);
    jumpTo(X, Z, x, pos[1], z);
}

void GLWidget::jumpTo(int X, int Z, float x, float y, float z){
    qDebug() << "jump: "<< X << " "<< Z;
    TerrainLib::load(X, Z);
    float h = TerrainLib::getHeight(X, Z, x, y, z);
    if(y < h || y > h+100) y = h + 20;
    camera->setPozT(X, Z);
    camera->setPos(x, y, z);
    
}

void GLWidget::objSelected(WorldObj* o){
    if(selectedObj == NULL)
        return;
    selectedObj->unselect();
    setSelectedObj(o);
    if(selectedObj != NULL)
        selectedObj->select();
}

void GLWidget::setPaintBrush(Brush* brush){
    this->defaultPaintBrush = brush;
}

void GLWidget::setSelectedObj(WorldObj* o){
    this->selectedObj = o;
    emit showProperties(selectedObj);
    if(o != NULL)
        emit sendMsg("showShape", o->getShapePath());
}

void GLWidget::editCopy(){
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        if(selectedObj != NULL){
            if(selectedObj->typeID == WorldObj::groupobject){
                delete copyPasteGroupObj;
                copyPasteGroupObj = new GroupObj(*groupObj);
                copyPasteObj = copyPasteGroupObj;
            } else {
                copyPasteObj = selectedObj;
            }
        }
    }
}

void GLWidget::editPaste(){
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        if(copyPasteObj != NULL){
            if(selectedObj != NULL)
                selectedObj->unselect();
            lastNewObjPosT[0] = camera->pozT[0];
            lastNewObjPosT[1] = camera->pozT[1];
            lastNewObjPos[0] = aktPointerPos[0];
            lastNewObjPos[1] = aktPointerPos[1];
            lastNewObjPos[2] = aktPointerPos[2];
            if(copyPasteObj->typeID == WorldObj::groupobject){
                groupObj->fromNewObjects((GroupObj*)copyPasteObj, route, (int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
                setSelectedObj(groupObj);
            } else {
                float *q = Quat::create();
                Quat::copy(q, copyPasteObj->qDirection);
                setSelectedObj(route->placeObject((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos, q, copyPasteObj->getRefInfo()));
                if(selectedObj != NULL)
                    selectedObj->select();
            }
        }
    }
}

void GLWidget::editUndo(){
    Undo::UndoLast();
}

void GLWidget::showTrkEditr(){
    if(route != NULL)
        route->showTrkEditr();
}

void GLWidget::getUnsavedInfo(std::vector<QString> &items){
    if(this->route == NULL)
        return;
    route->getUnsavedInfo(items);
}

void GLWidget::msg(QString text){
    qDebug() << text;
    if(text == "save"){
        route->save();
        return;
    }
    if(text == "createPaths"){
        route->createNewPaths();
        return;
    }
    if(text == "resetPlaceRotation"){
        Quat::fill(this->placeRot);
        return;
    }
    if(text == "showTerrainTreeEditr"){
        TerrainTreeWindow ttWindow;
        ttWindow.exec();
        return;
    }
    if(text == "engItemSelected"){
        //QString pathid = ;
        //QString name = pathid.split("/").last();
        //QString texpath = pathid.left(pathid.length() - name.length());
        emit sendMsg("showShape", route->ref->selected->getShapePath());
    }
    if(text == "autoPlacementDeleteLast"){
        route->autoPlacementDeleteLast();
    }
}

void GLWidget::msg(QString text, bool val){
    qDebug() << text;
    if(text == "stickToTDB"){
        this->route->placementStickToTarget = val;
        return;
    }
}

void GLWidget::msg(QString text, int val){
}

void GLWidget::msg(QString text, float val){
    qDebug() << text;
    if(text == "autoPlacementLength"){
        this->route->placementAutoLength = val;
        return;
    }
}


void GLWidget::msg(QString text, QString val){
    //qDebug() << text;
    if(text == "mkrFile"){
        this->route->setMkrFile(val);
        return;
    }
}