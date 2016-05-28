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
    backgroundGlColor[0] = -2;
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

void GlShapeWidget::setBackgroundGlColor(float r, float g, float b){
    backgroundGlColor[0] = r;
    backgroundGlColor[1] = g;
    backgroundGlColor[2] = b;
    if(renderItem == 3 && con != NULL){
        con->setTextColor(backgroundGlColor);
    }
}

void GlShapeWidget::paintGL() {
    Game::currentShapeLib = currentShapeLib;
    Game::currentEngLib = currentEngLib;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(backgroundGlColor[0], backgroundGlColor[1], backgroundGlColor[2], 1);

    float aspect = float(this->width()) / float(this->height());
    Mat4::perspective(gluu->pMatrix, camera->fov*M_PI/180*(1/aspect), aspect, 0.2f, Game::objectLod);
    float* lookAt = camera->getMatrix();
    Mat4::multiply(gluu->pMatrix, gluu->pMatrix, lookAt);
    Mat4::identity(gluu->mvMatrix);

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
        eng->render((int)selection*65536);
    }
    if(renderItem == 3 && con != NULL){
        con->render((int)selection*65536);
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
    
    gluu->m_program->release();

}

void GlShapeWidget::getImg() {
    getImage = true;
    return;
}

void GlShapeWidget::resizeGL(int w, int h) {
}

void GlShapeWidget::keyPressEvent(QKeyEvent * event) {
    camera->keyDown(event);
    Game::currentShapeLib = currentShapeLib;
    Game::currentEngLib = currentEngLib;
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

void GlShapeWidget::keyReleaseEvent(QKeyEvent * event) {
    camera->keyUp(event);
}

void GlShapeWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
    mousePressed = true;
    selection = true;
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

void GlShapeWidget::resetRot(){
    rotY = 0;
    rotZ = 0;
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
    con->setTextColor(backgroundGlColor);
    renderItem = 3;
}

void GlShapeWidget::setMode(QString n){
    mode = n;
}