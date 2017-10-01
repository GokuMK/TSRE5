/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ShapeViewerGLWidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QDateTime>
#include <math.h>
#include "GLUU.h"
#include "SFile.h"
#include "Eng.h"
#include "EngLib.h"
#include "Game.h"
#include "GLH.h"
#include "CameraFree.h"
#include "CameraConsist.h"
#include "GLMatrix.h"
#include "EngLib.h"
#include "ConLib.h"
#include "Consist.h" 
#include "ShapeLib.h"
#include "EngLib.h"
#include "ActLib.h"
#include "Activity.h"

ShapeViewerGLWidget::ShapeViewerGLWidget(QWidget *parent)
: QOpenGLWidget(parent),
m_xRot(0),
m_yRot(0),
m_zRot(0) {
    backgroundGlColor[0] = -2;
    currentShapeLib = new ShapeLib();
}

ShapeViewerGLWidget::~ShapeViewerGLWidget() {

}

void ShapeViewerGLWidget::cleanup() {
    makeCurrent();
    //delete gluu->m_program;
    //gluu->m_program = 0;
    doneCurrent();
}

QSize ShapeViewerGLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize ShapeViewerGLWidget::sizeHint() const {
    return QSize(1500, 700);
}

void ShapeViewerGLWidget::timerEvent(QTimerEvent * event) {

    timeNow = QDateTime::currentMSecsSinceEpoch();
    if (timeNow - lastTime < 1)
        fps = 1;
    else
        fps = 1000.0 / (timeNow - lastTime);
    if (fps < 10) fps = 10;
    lastTime = timeNow;


    if (Game::allowObjLag < Game::maxObjLag)
        Game::allowObjLag += 2;

    camera->update(fps);
    update();
    
}

void ShapeViewerGLWidget::setCamera(Camera* cam){
    camera = cam;
}

void ShapeViewerGLWidget::initializeGL() {
    Game::currentShapeLib = currentShapeLib;
    /*if(currentEngLib == NULL){
         currentEngLib = new EngLib();
         Game::currentEngLib = currentEngLib;
    }*/
    //qDebug() << "GLUU::get();";
    gluu = GLUU::get();
    //context()->set
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &ShapeViewerGLWidget::cleanup);
    //qDebug() << "initializeOpenGLFunctions();";
    initializeOpenGLFunctions();
    if(backgroundGlColor[0] == -2){
        backgroundGlColor[0] = 25.0/255;
        backgroundGlColor[1] = 25.0/255;
        backgroundGlColor[2] = 25.0/255;
        if(Game::systemTheme){
            backgroundGlColor[0] = 1;
            backgroundGlColor[1] = 1;
            backgroundGlColor[2] = 1;
        }
    }
    glClearColor(backgroundGlColor[0], backgroundGlColor[1], backgroundGlColor[2], 1);

    gluu->initShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);


    //sFile = new SFile("F:/TrainSim/trains/trainset/pkp_sp47/pkp_sp47-001.s", "F:/TrainSim/trains/trainset/pkp_sp47");
    //sFile = new SFile("f:/train simulator/routes/cmk/shapes/cottage3.s", "cottage3.s", "f:/train simulator/routes/cmk/textures");

    //eng = new Eng("F:/Train Simulator/trains/trainset/PKP-ST44-992/","PKP-ST44-992.eng");

    //qDebug() << eng->loaded;
    //sFile->Load("f:/train simulator/routes/cmk/shapes/cottage3.s");
    //tile = new Tile(-5303,-14963);
    //qDebug() << "route = new Route();";
    
    float * aaa = new float[2]{0,0};
    if(camera == NULL){
        camera = new CameraFree(aaa);
        float spos[3];
        spos[0] = 0; spos[1] = 0; spos[2] = 0;
        camera->setPos((float*)&spos);
    }
    
    lastTime = QDateTime::currentMSecsSinceEpoch();
    timer.start(15, this);
    setFocus();
    setMouseTracking(true);
}

void ShapeViewerGLWidget::setBackgroundGlColor(float r, float g, float b){
    backgroundGlColor[0] = r;
    backgroundGlColor[1] = g;
    backgroundGlColor[2] = b;
    if(renderItem == 3 && con != NULL){
        con->setTextColor(backgroundGlColor);
    }
}

void ShapeViewerGLWidget::paintGL() {
    Game::currentShapeLib = currentShapeLib;
    //Game::currentEngLib = currentEngLib;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // disable shadows temporaily
    int shadowsState = Game::shadowsEnabled;
    Game::shadowsEnabled = 0;
    
    glClearColor(backgroundGlColor[0], backgroundGlColor[1], backgroundGlColor[2], 1);
    
    float aspect = float(this->width()) / float(this->height());
    Mat4::perspective(gluu->pMatrix, camera->fov*M_PI/180*(1/aspect), aspect, 0.2f, Game::objectLod);
    float* lookAt = camera->getMatrix();
    Mat4::multiply(gluu->pMatrix, gluu->pMatrix, lookAt);
    Mat4::identity(gluu->mvMatrix);

    Mat4::identity(gluu->objStrMatrix);
    
    gluu->currentShader->bind();
    gluu->setMatrixUniforms();
    //sFile->render();
    if(mode == "rot"){
        Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, rotY, 0,1,0);
        Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, rotZ, 0,0,1);
    }
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    if(renderItem == 2 && eng != NULL){
        eng->render((int)selection*65536);
    }
    if(renderItem == 3 && con != NULL){
        con->render((int)selection*65536, true);
    }
    if(renderItem == 2 && con != NULL){
        Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI, 0,1,0);
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0, -con->conLength/2);
        con->render((int)selection*65536);
    }
    if(renderItem == 4 && sFile != NULL){
        GLUU *gluu = GLUU::get();
        gluu->enableTextures();
        sFile->render();
        if(cameraInit){
            cameraInit = false;
            float max = 0;
            if(fabs(sFile->bound[0]-sFile->bound[1]) > max)
                max = fabs(sFile->bound[0]-sFile->bound[1]);
            if(fabs(sFile->bound[2]-sFile->bound[3]) > max)
                max = fabs(sFile->bound[2]-sFile->bound[3]);
            if(fabs(sFile->bound[4]-sFile->bound[5]) > max)
                max = fabs(sFile->bound[4]-sFile->bound[5]);
            qDebug() << fabs(sFile->bound[0]-sFile->bound[1]);
            qDebug() << fabs(sFile->bound[2]-sFile->bound[3]);
            qDebug() << fabs(sFile->bound[4]-sFile->bound[5]);
            camera->setPos(-max*1.5,fabs(sFile->bound[2]-sFile->bound[3])/2.0,0.0);
        }
    }

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
        //int colorHash = (int)(winZ[0]*255)*256*256 + (int)(winZ[1]*255)*256 + (int)(winZ[2]*255);
        //qDebug() << colorHash;
        int isSelection = (int)(winZ[0]*255);
        if(isSelection == 1 && renderItem == 3 && con != NULL){
            con->select((int)(winZ[1]*255)*256 + (int)(winZ[2]*255));
            emit selected((int)(winZ[1]*255)*256 + (int)(winZ[2]*255));
        }
        
        selection = !selection;
        paintGL();
    }
    
    if(getImage){
        qDebug() << "get image";
        if(screenShot != NULL)
            delete screenShot;
        qDebug() << "new image";
        screenShot = NULL;
        getImage = false;
        int* viewport = new int[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        unsigned char* winZ = new unsigned char[viewport[2]*viewport[3]*4];
        glReadPixels(0, 0, viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, winZ);
        screenShot = new QImage(winZ, viewport[2], viewport[3], QImage::Format_RGBA8888 );
        
    }
    
    gluu->currentShader->release();
    Game::shadowsEnabled = shadowsState;
}

void ShapeViewerGLWidget::getImg() {
    getImage = true;
    return;
}

void ShapeViewerGLWidget::resizeGL(int w, int h) {
}

void ShapeViewerGLWidget::keyPressEvent(QKeyEvent * event) {
    camera->keyDown(event);
    Game::currentShapeLib = currentShapeLib;
    //Game::currentEngLib = currentEngLib;
    if(renderItem == 3 && con != NULL){
        switch (event->key()) {
            case Qt::Key_Delete:
                con->deteleSelected();
                break;
            case Qt::Key_F:
                con->flipSelected();
                break;
            case Qt::Key_Right:
                con->moveRightSelected();
                break;
            case Qt::Key_Left:
                con->moveLeftSelected();
                break;
        }
        
        emit refreshItem();
    }


}

void ShapeViewerGLWidget::keyReleaseEvent(QKeyEvent * event) {
    camera->keyUp(event);
}

void ShapeViewerGLWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
    m_lastPos *= Game::PixelRatio;
    mousePressed = true;
    selection = true;
    if(event->button() == Qt::RightButton)
        mouseRPressed = true;
    if(event->button() == Qt::LeftButton)
        mouseLPressed = true;
    camera->MouseDown(event);
    setFocus();
}

void ShapeViewerGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    camera->MouseUp(event);
    mousePressed = false;
    mouseRPressed = false;
    mouseLPressed = false;
}

void ShapeViewerGLWidget::mouseMoveEvent(QMouseEvent *event) {
    mousex = event->x()*Game::PixelRatio;
    mousey = event->y()*Game::PixelRatio;
    
    if(mode == "rot"){
        if (mousePressed) {
            if(mouseRPressed)
                rotZ += (float) (m_lastPos.y() - event->y()) / 60*(camera->fov/45.0);
            if(mouseLPressed)
                rotY += (float) (m_lastPos.x() - event->x()) / 60*(camera->fov/45.0);
            if (rotZ > 1.57)
                rotZ = (float) 1.57;
            if (rotZ < -1.57)
                rotZ = (float) - 1.57;
        }
    } else {
        camera->MouseMove(event);
    }
    m_lastPos = event->pos();
    m_lastPos *= Game::PixelRatio;
}

void ShapeViewerGLWidget::resetRot(){
    rotY = M_PI;
    rotZ = 0;
}


void ShapeViewerGLWidget::showEng(Eng *e){
    eng = e;
    con = NULL;
    renderItem = 2;
}

void ShapeViewerGLWidget::showEng(QString path, QString name){
    int idx = Game::currentEngLib->addEng(path, name);
    qDebug() << "eng id "<< idx;
    eng = Game::currentEngLib->eng[idx];
    con = NULL;
    renderItem = 2;
}

void ShapeViewerGLWidget::showEngSet(int id){
    qDebug() << "eng set id "<< id;
    con = ConLib::con[id];
    con->setTextColor(backgroundGlColor);
    eng = NULL;
    renderItem = 2;
}

void ShapeViewerGLWidget::showCon(int id){
    if(id < 0){
        con = NULL;
        eng = NULL;
        sFile = NULL;
        renderItem = 3;
        return;
    }
    qDebug() << "con id "<< id;
    con = ConLib::con[id];
    con->setTextColor(backgroundGlColor);
    eng = NULL;
    renderItem = 3;
}

void ShapeViewerGLWidget::showCon(int aid, int id){
    qDebug() << "con aid "<< aid<< " con id "<< id;
    con = ActLib::Act[aid]->activityObjects[id].con;
    con->setTextColor(backgroundGlColor);
    renderItem = 3;
}

void ShapeViewerGLWidget::showShape(QString path, QString texPath){
    int shapeId;
    if(texPath.length() > 0)
        shapeId = currentShapeLib->addShape(path, texPath);
    else
        shapeId = currentShapeLib->addShape(path);
    if(shapeId < 0)
        sFile = NULL;
    else {
        sFile = currentShapeLib->shape[shapeId];
        cameraInit = true;
    }
    renderItem = 4;
    con = NULL;
    eng = NULL;
}

void ShapeViewerGLWidget::setMode(QString n){
    mode = n;
}