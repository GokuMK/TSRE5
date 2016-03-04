#include "glShapeWidget.h"
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

GlShapeWidget::GlShapeWidget(QWidget *parent)
: QOpenGLWidget(parent),
m_xRot(0),
m_yRot(0),
m_zRot(0) {

}

GlShapeWidget::~GlShapeWidget() {

}

void GlShapeWidget::cleanup() {
    makeCurrent();
    //delete gluu->m_program;
    //gluu->m_program = 0;
    doneCurrent();
}

QSize GlShapeWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GlShapeWidget::sizeHint() const {
    return QSize(1500, 700);
}

void GlShapeWidget::timerEvent(QTimerEvent * event) {

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

void GlShapeWidget::setCamera(Camera* cam){
    camera = cam;
}

void GlShapeWidget::initializeGL() {
    currentShapeLib = new ShapeLib();
    Game::currentShapeLib = currentShapeLib;
    if(currentEngLib == NULL){
         currentEngLib = new EngLib();
         Game::currentEngLib = currentEngLib;
    }
    //qDebug() << "GLUU::get();";
    gluu = GLUU::get();
    //context()->set
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GlShapeWidget::cleanup);
    //qDebug() << "initializeOpenGLFunctions();";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    gluu->initShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);


    //sFile = new SFile("F:/TrainSim/trains/trainset/pkp_sp47/pkp_sp47-001.s", "F:/TrainSim/trains/trainset/pkp_sp47");
    //sFile = new SFile("f:/train simulator/routes/cmk/shapes/cottage3.s", "cottage3.s", "f:/train simulator/routes/cmk/textures");

    eng = new Eng("F:/Train Simulator/trains/trainset/PKP-ST44-992/","PKP-ST44-992.eng");

    qDebug() << eng->loaded;
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

void GlShapeWidget::paintGL() {
    Game::currentShapeLib = currentShapeLib;
    Game::currentEngLib = currentEngLib;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //if (!selection)
    glClearColor(0, 0, 0, 1.0);
    //else
    //    glClearColor(0, 0, 0, 1.0);
    float aspect = float(this->width()) / float(this->height());
    Mat4::perspective(gluu->pMatrix, camera->fov*M_PI/180*(1/aspect), aspect, 0.2f, Game::objectLod);
    float* lookAt = camera->getMatrix();
    Mat4::multiply(gluu->pMatrix, gluu->pMatrix, lookAt);
    Mat4::identity(gluu->mvMatrix);

    //mat4.translate(gluu.mvMatrix, gluu.mvMatrix, [0.0, -2.0, -15.0]);
    //mat4.rotate(gluu.mvMatrix, gluu.mvMatrix, gluu.degToRad(yRot), [0,1,0]);
    //mat4.translate(gluu.mvMatrix, gluu.mvMatrix, [0.0, 0.0, -30.0]);
    //mat4.rotate(gluu.mvMatrix, gluu.mvMatrix, gluu.degToRad(yRot), [0,1,0]);
    //Mat4::identity(gluu->objStrMatrix);
    //gluu->objStrMatrix = Mat4::create();
    //gluu->m_proj.setToIdentity();
    //gluu->m_proj.perspective(45.0f, GLfloat(this->width()) / this->height(), 0.1f, 6000.0f);

    //gluu->m_world.setToIdentity();
    //gluu->m_world.translate(0.0, 0.0, -20.0);
    //gluu->m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    //gluu->m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    //gluu->m_program->bind();
    //gluu->m_program->setUniformValue(gluu->m_projMatrixLoc, gluu->m_proj);
    //gluu->m_program->setUniformValue(gluu->m_mvMatrixLoc, gluu->m_camera * gluu->m_world);
    //QMatrix3x3 normalMatrix = gluu->m_world.normalMatrix();
    //gluu->m_program->setUniformValue(gluu->m_normalMatrixLoc, normalMatrix);

    //Mat4::identity(gluu->pMatrix);
    //Mat4::identity(gluu->mvMatrix);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.0, 0.0, -20.0);
    Mat4::identity(gluu->objStrMatrix);
    
    gluu->m_program->bind();
    gluu->setMatrixUniforms();
    //sFile->render();
    if(mode == "rot"){
        Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, rotY, 0,1,0);
        Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, rotZ, 0,0,1);
    }
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));

    if(renderItem == 2 && eng != NULL){
        eng->render();
    }
    if(renderItem == 3 && con != NULL){
        con->render();
    }

    gluu->m_program->release();

}

void GlShapeWidget::resizeGL(int w, int h) {
}

void GlShapeWidget::keyPressEvent(QKeyEvent * event) {
    camera->keyDown(event);
}

void GlShapeWidget::keyReleaseEvent(QKeyEvent * event) {
    camera->keyUp(event);
}

void GlShapeWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
    mousePressed = true;
    if(event->button() == Qt::RightButton)
        mouseRPressed = true;
    if(event->button() == Qt::LeftButton)
        mouseLPressed = true;
    camera->MouseDown(event);
    setFocus();
}

void GlShapeWidget::mouseReleaseEvent(QMouseEvent* event) {
    camera->MouseUp(event);
    mousePressed = false;
    mouseRPressed = false;
    mouseLPressed = false;
}

void GlShapeWidget::mouseMoveEvent(QMouseEvent *event) {
    mousex = event->x();
    mousey = event->y();
    
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
}

void GlShapeWidget::showEng(QString path, QString name){
    qDebug() << "eng id "<< -5;
    int idx = currentEngLib->addEng(path, name);
    eng = currentEngLib->eng[idx];
    renderItem = 2;
}

void GlShapeWidget::showCon(int id){
    qDebug() << "con id "<< id;
    con = ConLib::con[id];
    renderItem = 3;
}

void GlShapeWidget::setMode(QString n){
    mode = n;
}