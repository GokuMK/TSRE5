/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ShapeViewerWindow.h"
#include "ShapeTexturesWindow.h"
#include "ShapeHierarchyWindow.h"
#include "ShapeViewerNavigatorWidget.h"
#include "EngInfoWidget.h"
#include "ConInfoWidget.h"
#include "ShapeInfoWidget.h"
#include <QDebug>
#include "EngLib.h"
#include "ConLib.h"
#include "Eng.h"
#include "Consist.h"
#include "Game.h"
#include "ShapeViewerGLWidget.h"
#include "CameraFree.h"
#include "CameraConsist.h"
#include "CameraRot.h"
#include "GuiFunct.h"
#include "AboutWindow.h"
#include "GLMatrix.h"
#include <QVector>
#include "ShapeTextureInfo.h"
#include "ShapeHierarchyInfo.h"
#include "ContentHierarchyInfo.h"
#include "SFile.h"

ShapeViewerWindow::ShapeViewerWindow() : QMainWindow() {
    Game::shadowsEnabled = 0;
    Game::fogDensity = 0;
    Vec3::set((float*)Game::sunLightDirection,-1.0,0.0,0.0);
    aboutWindow = new AboutWindow(this);
    englib = new EngLib();
    Game::currentEngLib = englib;

    navigatorWidget = new ShapeViewerNavigatorWidget(this);
    glShapeWidget = new ShapeViewerGLWidget(this);
    if(Game::colorShapeView != NULL)
        glShapeWidget->setBackgroundGlColor(Game::colorShapeView->redF(), Game::colorShapeView->greenF(), Game::colorShapeView->blueF());

    hierarchyWindow = new ShapeHierarchyWindow(this);
    texturesWindow = new ShapeTexturesWindow(this);
    engInfo = new EngInfoWidget(this);
    conInfo = new ConInfoWidget(this);
    shapeInfo = new ShapeInfoWidget(this);
    engInfo->hide();
    conInfo->hide();
    shapeInfo->hide();
        
    engCamera = new CameraRot();
    engCamera->setPos(0,2.5,0);
    engCamera->setPlayerRot(M_PI/2.0,0);
    
    glShapeWidget->setCamera(engCamera);
    glShapeWidget->setMode("rot");
    glShapeWidget->setMinimumSize(100, 100);
    
    // MAIN WINDOW
    QWidget* main = new QWidget();
    QHBoxLayout *mbox = new QHBoxLayout;
    mbox->setSpacing(2);
    mbox->setContentsMargins(1,1,1,1);
    mbox->addWidget(navigatorWidget);
    connect(navigatorWidget, SIGNAL(dirFilesSelected(QString)), this, SLOT(dirFilesSelected(QString)));
    connect(navigatorWidget, SIGNAL(contentHierarchySelected(int)), this, SLOT(contentHierarchySelected(int)));
    QVBoxLayout *itemLayout = new QVBoxLayout;
    itemLayout->addWidget(glShapeWidget);
    itemLayout->addWidget(conInfo);
    itemLayout->addWidget(engInfo);
    itemLayout->addWidget(shapeInfo);
    mbox->addItem(itemLayout);

    main->setLayout(mbox);
    this->setCentralWidget(main);
    setWindowTitle(Game::AppName+" "+Game::AppVersion+" Shape Viewer");

    // MENU
    fileMenu = menuBar()->addMenu(tr("&File"));
    fNew = new QAction(tr("&Open"), this); 
    fileMenu->addAction(fNew);
    QObject::connect(fNew, SIGNAL(triggered(bool)), this, SLOT(openFileEnabled()));
    fExit = new QAction(tr("&Exit"), this); 
    fileMenu->addAction(fExit);
    QObject::connect(fExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    
    viewMenu = menuBar()->addMenu(tr("&View"));
    vHierarchyView = GuiFunct::newMenuCheckAction(tr("&Shape Hierarchy"), this, false); 
    viewMenu->addAction(vHierarchyView);
    QObject::connect(vHierarchyView, SIGNAL(triggered(bool)), this, SLOT(viewHierarchySelected(bool)));
    vTexturesView = GuiFunct::newMenuCheckAction(tr("&Shape Textures"), this, false); 
    viewMenu->addAction(vTexturesView);
    QObject::connect(vTexturesView, SIGNAL(triggered(bool)), this, SLOT(viewTexturesSelected(bool)));
    
    view3dMenu = menuBar()->addMenu(tr("&3D View"));
    vResetShapeView = new QAction(tr("&Reset"), this); 
    view3dMenu->addAction(vResetShapeView);
    QObject::connect(vResetShapeView, SIGNAL(triggered()), this, SLOT(vResetShapeViewSelected()));
    vGetImgShapeView = new QAction(tr("&Copy Image"), this); 
    view3dMenu->addAction(vGetImgShapeView);
    QObject::connect(vGetImgShapeView, SIGNAL(triggered()), this, SLOT(vGetImgShapeViewSelected()));
    vSaveImgShapeView = new QAction(tr("&Save Image"), this); 
    view3dMenu->addAction(vSaveImgShapeView);
    QObject::connect(vSaveImgShapeView, SIGNAL(triggered()), this, SLOT(vSaveImgShapeViewSelected()));    
    vSetColorShapeView = new QAction(tr("&Set Color"), this); 
    view3dMenu->addAction(vSetColorShapeView);
    QObject::connect(vSetColorShapeView, SIGNAL(triggered()), this, SLOT(vSetColorShapeViewSelected()));
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu->addAction(aboutAction);
    
    resize(1280, 800);
    
    //texturesWindow->show();
    //hierarchyWindow->show();
}

void ShapeViewerWindow::viewHierarchySelected(bool show){
    if(show) hierarchyWindow->show();
    else hierarchyWindow->hide();
}

void ShapeViewerWindow::viewTexturesSelected(bool show){
    if(show) texturesWindow->show();
    else texturesWindow->hide();
}

void ShapeViewerWindow::vSetColorShapeViewSelected(){
    QColor color = QColorDialog::getColor(Qt::black, this, "Shape View Color",  QColorDialog::DontUseNativeDialog);
    glShapeWidget->setBackgroundGlColor((float)color.redF(), (float)color.greenF(), (float)color.blueF());
}


void ShapeViewerWindow::copyImgShapeView(){
    if(glShapeWidget->screenShot != NULL)
        QApplication::clipboard()->setImage((glShapeWidget->screenShot->mirrored(false, true)), QClipboard::Clipboard);
}

void ShapeViewerWindow::saveImgShapeView(){
    if(glShapeWidget->screenShot != NULL){
        QImage img = glShapeWidget->screenShot->mirrored(false, true);
        QString path = QFileDialog::getSaveFileName(this, "Save File", "./", "Images (*.png *.jpg)");
        qDebug() << path;
        if(path.length() < 1) return;
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        img.save(&file);
    }
}

void ShapeViewerWindow::vGetImgShapeViewSelected(){
    glShapeWidget->getImg();
    QTimer::singleShot(500, this, SLOT(copyImgShapeView()));
}

void ShapeViewerWindow::vSaveImgShapeViewSelected(){
    glShapeWidget->getImg();
    QTimer::singleShot(500, this, SLOT(saveImgShapeView()));
}

void ShapeViewerWindow::vResetShapeViewSelected(){
    if(currentItemType == "shape"){
        glShapeWidget->resetRot();
    }
    if(currentItemType == "eng"){
        if(currentEng == NULL)
            return;
        float pos = -currentEng->sizez-1;
        if(pos > -15) pos = -15;
        engCamera->setPos(pos,2.5,0);
        engCamera->setPlayerRot(M_PI/2.0,0);
        glShapeWidget->resetRot();
    }
    if(currentItemType == "con"){
        if(currentCon == NULL)
            return;
        if(currentCon->engItems.size() < 1)
            return;
        float pos = -currentCon->conLength;
        if(pos > -15) pos = -15;
        engCamera->setPos(pos,2.5,pos/2.0);
        engCamera->setPlayerRot(M_PI/2.0,0);
        glShapeWidget->resetRot();
    }
}

ShapeViewerWindow::~ShapeViewerWindow() {
}

void ShapeViewerWindow::openFileEnabled(){
    QFileDialog fd;
    QString path = "";
    path.replace("//", "/");
    fd.setDirectory(path);
    fd.setFileMode(QFileDialog::ExistingFiles);
    int result = fd.exec();
    QString filename;
    if (!result) 
        return;
    if(fd.selectedFiles().size() < 1)
        return;
    
    filename = fd.selectedFiles()[0];
    //filename.split()
    texturesWindow->clearLists();
    hierarchyWindow->clearLists();
    navigatorWidget->listDirectoryFiles(filename);
    
    loadFile(filename);
}

void ShapeViewerWindow::loadFile(QString path){
    path.replace("\\", "/");
    path.replace("//", "/");
    QString dir = path.section("/",0,-2);
    qDebug() << dir;
    QString filename = path.section("/",-1,-1);
    qDebug() << filename;
    
    if(filename.endsWith(".s", Qt::CaseInsensitive)){
        if(dir.endsWith("/shapes", Qt::CaseInsensitive)){
            QString dir2 = dir.section("/", 0, -2)+"/textures";
            if(QFileInfo::exists(dir2)){
                dir = dir2;
            }
        }
        currentItemType = "shape";
        glShapeWidget->showShape(path, dir, &currentShape);
    }
    if(filename.endsWith(".eng", Qt::CaseInsensitive) || filename.endsWith(".wag", Qt::CaseInsensitive)){
        int idx = Game::currentEngLib->addEng(dir, filename);
        qDebug() << "eng id "<< idx;
        Eng *eng = Game::currentEngLib->eng[idx];
        float pos = -eng->sizez-1;
        if(pos > -15) pos = -15;
        engCamera->setPos(pos,2.5,0);
        engCamera->setPlayerRot(M_PI/2.0,0);
        currentItemType = "eng";
        glShapeWidget->showEng(eng);
        currentEng = eng;
    }
    if(filename.endsWith(".con", Qt::CaseInsensitive)){
        if(!dir.contains("/trains/consists", Qt::CaseInsensitive))
            return;
        QString gameRoot = dir.left(dir.indexOf("/trains/consists", 0, Qt::CaseInsensitive));
        Game::root = gameRoot;
        int cid = ConLib::addCon(dir, filename);
        Consist* con = ConLib::con[cid];
        if(con == NULL)
            return;
        if(con->engItems.size() < 1)
            return;
        int idx = con->engItems[0].eng;
        Eng *eng = Game::currentEngLib->eng[idx];
        float pos = -con->conLength;// -eng->sizez-1;
        if(pos > -15) pos = -15;
        engCamera->setPos(pos,2.5,pos/2.0);
        engCamera->setPlayerRot(M_PI/2.0,0);
        currentItemType = "con";
        glShapeWidget->showConSimple(cid);
        currentCon = con;
    }
    
    //List textures:
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTextureInfo()));
    connect(this, SIGNAL(stopUpdateTimer()), timer, SLOT(stop()));
    timer->start(100);
}

void ShapeViewerWindow::dirFilesSelected(QString file){
    loadFile(file);
}

void ShapeViewerWindow::contentHierarchySelected(int id){
    qDebug() << currentContent.size()<<id;
    
    if(currentContent.size() < id + 1)
        return;
    qDebug() << currentContent[id]->name;
    
    if(currentShape != NULL)
        currentShape->setCurrentDistanceLevel(0,-1);
    
    currentItemType = currentContent[id]->type;
    if(currentItemType == "shape"){
        currentItemType = "shape";
        currentShape = currentContent[id]->sfile;
        currentShape->setCurrentDistanceLevel(0, currentContent[id]->distanceLevelId);
        glShapeWidget->showShape(currentShape);
    }
    if(currentItemType == "eng"){
        Eng *eng = currentContent[id]->eng;
        float pos = -eng->sizez-1;
        if(pos > -15) pos = -15;
        engCamera->setPos(pos,2.5,0);
        engCamera->setPlayerRot(M_PI/2.0,0);
        currentItemType = "eng";
        glShapeWidget->showEng(eng);
        currentEng = eng;
    }
    if(currentItemType == "con"){
        Consist* con = currentContent[id]->con;
        if(con == NULL)
            return;
        if(con->engItems.size() < 1)
            return;
        float pos = -con->conLength;
        if(pos > -15) pos = -15;
        engCamera->setPos(pos,2.5,pos/2.0);
        engCamera->setPlayerRot(M_PI/2.0,0);
        currentItemType = "con";
        glShapeWidget->showConSimple(con);
        currentCon = con;
    }
    
    updateTextureInfo(false);
}

void ShapeViewerWindow::updateTextureInfo(){
    updateTextureInfo(true);
}

void ShapeViewerWindow::updateTextureInfo(bool refreshContentList){
    
    QHash<int, ShapeTextureInfo*> textureInfo;
    glShapeWidget->fillCurrentShapeTextureInfo(textureInfo);
    foreach(ShapeTextureInfo* i, textureInfo){
        if(i->loading)
            return;
    }
    
    engInfo->hide();
    conInfo->hide();
    shapeInfo->hide();
    
    emit stopUpdateTimer();
    texturesWindow->setTextureList(textureInfo);
        
    ShapeHierarchyInfo* hierarchyInfo = new ShapeHierarchyInfo();
    glShapeWidget->fillCurrentShapeHierarchyInfo(hierarchyInfo);
    hierarchyWindow->setHierarchyList(hierarchyInfo);
    hierarchyWindow->currentShape = currentShape;

    if(refreshContentList){
        currentContent.clear();
        glShapeWidget->fillCurrentContentHierarchyInfo(currentContent);
        navigatorWidget->listHierarchy(currentContent);
    }
    
    if(currentItemType == "shape"){
        //shapeInfo->show();
    }
    if(currentItemType == "eng"){
        engInfo->show();
        engInfo->setInfo(currentEng);
    }
    if(currentItemType == "con"){
        //conInfo->show();
    }
}

void ShapeViewerWindow::about(){
    aboutWindow->show();
}