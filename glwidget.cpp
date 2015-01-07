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

void GLWidget::initializeGL() {
    
    Game::load();
    
    gluu = GLUU::get();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    gluu->initShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);


    float * aaa = new float[2]{Game::startTileX,-Game::startTileY};//bbb
    //float * aaa = new float[2]{-5417,-15048};//traska
    camera = new Camera(aaa);
    //sFile = new SFile("F:/TrainSim/trains/trainset/pkp_sp47/pkp_sp47-001.s", "F:/TrainSim/trains/trainset/pkp_sp47");
    //sFile = new SFile("f:/train simulator/routes/cmk/shapes/cottage3.s", "cottage3.s", "f:/train simulator/routes/cmk/textures");
    //eng = new Eng("F:/Train Simulator/trains/trainset/PKP-ST44-992/","PKP-ST44-992.eng",0);
    //sFile->Load("f:/train simulator/routes/cmk/shapes/cottage3.s");
    //tile = new Tile(-5303,-14963);
    route = new Route();
    lastTime = QDateTime::currentMSecsSinceEpoch();
    timer.start(15, this);
    setFocus();
    setMouseTracking(true);
    pointer3d = new Pointer3d();
    
    emit routeLoaded(route);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //if (!selection)
    glClearColor(gluu->skyc[0], gluu->skyc[1], gluu->skyc[2], 1.0);
    //else
    //    glClearColor(0, 0, 0, 1.0);

    Mat4::perspective(gluu->pMatrix, 45.0f, float(this->width()) / this->height(), 0.2f, 4000.0);
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
    //eng->render();
    //tile->render();
    route->render(gluu, camera->pozT, camera->getPos(), camera->getTarget(), 3.14f / 3, selection);
    
    bool showPos = true;
    if (showPos && !selection) {
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
        
        //if(selection)
        //    qDebug() << wcoord[0] << ", " << wcoord[1] << ", " << wcoord[2];
        //showPos = false;
        
        //aktPointerPos[0] = 
        
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, aktPointerPos[0], aktPointerPos[1], aktPointerPos[2]);
        Mat4::identity(gluu->objStrMatrix);
        gluu->setMatrixUniforms();
        //gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        pointer3d->render();
        gluu->mvPopMatrix();
        /*gl.glDisable(GL2.GL_TEXTURE_2D);
        gl.glDisable(GL2.GL_LIGHTING);
        gl.glColor3f(1.0f, 1.0f, 0f);
        gl.glLineWidth(3);
        gl.glBegin(GL2.GL_LINES);
        gl.glVertex3f(wcoord[0], wcoord[1], wcoord[2]);
        gl.glVertex3f(wcoord[0], wcoord[1] + 2, wcoord[2]);
        gl.glVertex3f(wcoord[0] - 2, wcoord[1], wcoord[2]);
        gl.glVertex3f(wcoord[0] + 2, wcoord[1], wcoord[2]);
        gl.glVertex3f(wcoord[0], wcoord[1], wcoord[2] - 2);
        gl.glVertex3f(wcoord[0], wcoord[1], wcoord[2] + 2);
        gl.glEnd();
        gl.glLineWidth(1);
        gl.glEnable(GL2.GL_LIGHTING);*/

        ////////////
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

        qDebug() << winZ[0] << " " << winZ[1] << " " << winZ[2] << " ";
        int ww = (int) (winZ[0]*255);
        int wx = (int) (ww / 10);
        int wz = (int) (ww - (wx)*10);
        wx = camera->pozT[0] + wx - 5;
        wz = camera->pozT[1] - (wz - 5);

        int UiD = (int) (winZ[1]*255)*256 + (int) (winZ[2]*255);

        qDebug() << wx << " " << wz << " " << UiD;
        if(selectedObj != NULL) selectedObj->selected = false;
        selectedObj = route->getObj(wx, wz, UiD);
        if (!selectedObj->loaded){
            qDebug() << "brak obiektu";
            selectedObj = NULL;
        } else {
            selectedObj->selected = true;
            //selectedObj->translate(0,10,0);
        }
        //int objHash = trasa.getObjectHash(wx,wz,UiD);
        //System.out.println( objHash + " ");

        //selectedObj = trasa.getObj(wx,wz,objHash);

        //selectedObj.select();
        //System.out.println(selectedObj.selected); 
        selection = !selection;
        paintGL();
    }

    gluu->m_program->release();

}

void GLWidget::resizeGL(int w, int h) {
    //gluu->m_proj.setToIdentity();
    //gluu->m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::keyPressEvent(QKeyEvent * event) {
    camera->keyDown(event);
    switch (event->key()) {
        case 'M':
            route->save();
            //selection = true; //!selection;
            // paintGL();
            // selection = !selection;
            break;
        default:
            break;
    }
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        Vector2f a;

        switch (event->key()) {
            case Qt::Key_Alt:
                moveStep = moveMinStep;
                break;
            case Qt::Key_8:
                if(translateTool && selectedObj != NULL){
                    a.y = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(moveStep/10, 0, 0);
                }
                break;
            case Qt::Key_2:
                if(translateTool && selectedObj != NULL){
                    a.y = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(-moveStep/10, 0, 0);
                }
                break;    
            case Qt::Key_4:
                if(translateTool && selectedObj != NULL){
                    a.x = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, -moveStep/10, 0);
                }
                break;
            case Qt::Key_6:
                if(translateTool && selectedObj != NULL){
                    a.x = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, moveStep/10, 0);
                }
                break;                 
            case Qt::Key_9:
                if(translateTool && selectedObj != NULL){
                    this->selectedObj->translate(0,moveStep,0);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, 0, moveStep/10);
                }
                break;
            case Qt::Key_3:
                if(translateTool && selectedObj != NULL){
                    this->selectedObj->translate(0,-moveStep,0);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, 0, -moveStep/10);
                }
                break;   
            case Qt::Key_R:
                translateTool = false;
                rotateTool = true;
                break;
            case Qt::Key_T:    
                rotateTool = false;
                translateTool = true;
                break;
            case Qt::Key_Delete:
                if(selectedObj != NULL){
                    selectedObj->loaded = false;
                }
                break;                
            case Qt::Key_C:
                if(selectedObj != NULL){
                    selectedObj->selected = false;
                    selectedObj = route->placeObject(selectedObj->x, selectedObj->y, selectedObj->position, selectedObj->qDirection, selectedObj->getRefInfo());
                    if(selectedObj != NULL){
                        selectedObj->selected = true;
                    }
                }
            case Qt::Key_P:
                if(selectedObj != NULL){
                    route->ref->selected = selectedObj->getRefInfo();
                }
            case Qt::Key_L:
                route->trackDB->nextDefaultEnd();
                
            default:
                break;
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent * event) {
    camera->keyUp(event);
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        switch (event->key()) {
            case Qt::Key_Alt:
                moveStep = moveMaxStep;
                break;
            default:
                break;
        }
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
    mousePressed = true;
    
    if(event->buttons() == Qt::RightButton){
        camera->MouseDown(event);
    } else {
        if(toolEnabled == "placeTool"){
            if(selectedObj != NULL) 
                selectedObj->selected = false;
            selectedObj = route->placeObject((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            if(selectedObj != NULL)
                selectedObj->selected = true;
            //if(route->ref->selected != NULL){
            //qDebug() << route->ref->selected->description;
            //}
            //camera->MouseDown(event);
        }
        if(toolEnabled == "selectTool"){
            selection = true;
            mousePressed = false;
            if(selectedObj != NULL){
                mousePressed = true;
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
            } //else {
                //camera->MouseDown(event);
            //}
        }
        if(toolEnabled == ""){
            camera->MouseDown(event);
        }
    }
    setFocus();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event) {
    camera->MouseUp(event);
    mousePressed = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    /*int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {

    } else if (event->buttons() & Qt::RightButton) {

    }*/
    mousex = event->x();
    mousey = event->y();
    m_lastPos = event->pos();
    
    if(event->buttons() == Qt::RightButton){
        camera->MouseMove(event);
    } else {
        if(toolEnabled == "selectTool"){
            /*if(selectedObj != NULL && mousePressed){
                //selectedObj->translate(aktPointerPos[0] - lastPointerPos[0], aktPointerPos[1] - lastPointerPos[1], aktPointerPos[2] - lastPointerPos[2]);
                selectedObj->translate(0, aktPointerPos[1] - lastPointerPos[1], 0);
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
            }*/ //else {
            //    camera->MouseMove(event);
            //}
        }
        if(toolEnabled == "placeTool"){
            camera->MouseMove(event);
        }
        if(toolEnabled == ""){
            camera->MouseMove(event);
        }
    }
}

void GLWidget::enableTool(QString name){
    qDebug() << name;
    toolEnabled = name;
}

void GLWidget::jumpTo(int x, int y){
    qDebug() << "jump: "<< x << " "<< y;
    camera->setPozT(x, -y);
    
}