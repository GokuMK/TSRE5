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
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu->addAction(aboutAction);
    
    resize(1280, 800);
    
    texturesWindow->show();
    hierarchyWindow->show();
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
    loadFile(filename);
    navigatorWidget->listDirectoryFiles(filename);
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
        glShapeWidget->showShape(path, dir);
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

void ShapeViewerWindow::updateTextureInfo(){
    
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
    
    QVector<ContentHierarchyInfo*> chlist;
    glShapeWidget->fillCurrentContentHierarchyInfo(chlist);
    navigatorWidget->listHierarchy(chlist);
    
    if(currentItemType == "shape"){
        shapeInfo->show();
    }
    if(currentItemType == "eng"){
        engInfo->show();
        engInfo->setInfo(currentEng);
    }
    if(currentItemType == "con"){
        conInfo->show();
    }
}

void ShapeViewerWindow::about(){
    aboutWindow->show();
}