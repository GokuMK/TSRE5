/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ConEditorWindow.h"
#include <QDebug>
#include "EngLib.h"
#include "ConLib.h"
#include "Eng.h"
#include "Consist.h"
#include "Game.h"
#include "EngListWidget.h"
#include "ConListWidget.h"
#include "glShapeWidget.h"
#include "CameraFree.h"
#include "CameraConsist.h"
#include "CameraRot.h"
#include "GuiFunct.h"
#include "ConUnitsWidget.h"
#include "AboutWindow.h"
#include "OverwriteDialog.h"
#include "UnsavedDialog.h"
#include "ChooseFileDialog.h"
#include "RandomConsist.h"
#include "ActLib.h"
#include "Activity.h"
#include "GLMatrix.h"
#include <QVector>

ConEditorWindow::ConEditorWindow() : QMainWindow() {
    Game::shadowsEnabled = 0;
    Vec3::set((float*)Game::sunLightDirection,-1.0,0.0,0.0);
    aboutWindow = new AboutWindow();
    randomConsist = new RandomConsist();
    englib = new EngLib();
    englib->loadAll(Game::root);
    Game::currentEngLib = englib;
    ConLib::loadAll(Game::root);
    ActLib::loadAll(Game::root);
    glShapeWidget = new GlShapeWidget(this);
    if(Game::colorShapeView != NULL)
        glShapeWidget->setBackgroundGlColor(Game::colorShapeView->redF(), Game::colorShapeView->greenF(), Game::colorShapeView->blueF());
    //glShapeWidget->currentEngLib = englib;
    glConWidget = new GlShapeWidget(this);
    if(Game::colorConView != NULL)
        glConWidget->setBackgroundGlColor(Game::colorConView->redF(), Game::colorConView->greenF(), Game::colorConView->blueF());

    conCamera = new CameraConsist();
    conCamera->setPos(-100,2.5,42);
    conCamera->setPlayerRot(M_PI/2.0,0);
    engCamera = new CameraRot();
    engCamera->setPos(0,2.5,0);
    engCamera->setPlayerRot(M_PI/2.0,0);
    
    cDurability.setDecimals(2);
    cDurability.setMinimum(0);
    cDurability.setMaximum(2);
    cDurability.setSingleStep(0.05);
    
    glConWidget->setCamera(conCamera);
    glShapeWidget->setCamera(engCamera);
    glShapeWidget->setMode("rot");
    //qDebug()<<"aaa";
    eng1 = new EngListWidget();
    //eng1->englib = englib;
    eng1->fillEngList();
    eng2 = new EngListWidget();
    //eng2->englib = englib;
    eng2->fillEngList();
    units = new ConUnitsWidget();
    //units->englib = englib;
    con1 = new ConListWidget();
    //con1->englib = englib;
    con1->fillConList();
    //qDebug()<<"aaa";
    conSlider = new QScrollBar(Qt::Horizontal);
    conSlider->setMaximum(0);
    conSlider->setMinimum(0);
    
    QWidget* main = new QWidget();
    
    QVBoxLayout *mbox = new QVBoxLayout;
    mbox->setSpacing(2);
    mbox->setContentsMargins(1,1,1,1);
    QGridLayout *vbox = new QGridLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    vbox->addWidget(con1,0,0);
    vbox->addWidget(units,0,1);
    vbox->addWidget(eng1,0,2);
    vbox->addWidget(eng2,0,3);
    engInfo = new QWidget(this);
    QVBoxLayout *engInfoLayout = new QVBoxLayout;
    engInfoLayout->addWidget(glShapeWidget);
    glShapeWidget->setMinimumSize(100, 100);
    QGridLayout *engInfoForm = new QGridLayout;
    engInfoForm->setSpacing(2);
    engInfoForm->setContentsMargins(1,1,1,1);    
    engInfoForm->addWidget(new QLabel("Name:"),0,0);
    engInfoForm->addWidget(new QLabel("File Name:"),1,0);
    engInfoForm->addWidget(new QLabel("Dir Name:"),2,0);
    engInfoForm->addWidget(new QLabel("Shape:"),3,0);
    engInfoForm->addWidget(new QLabel("Type:"),0,2);
    engInfoForm->addWidget(new QLabel("Brakes:"),1,2);
    engInfoForm->addWidget(new QLabel("Couplings:"),2,2);
    engInfoForm->addWidget(new QLabel("Size:"),3,2);
    engInfoForm->addWidget(new QLabel("Mass:"),0,4);
    engInfoForm->addWidget(new QLabel("Max. Speed:"),1,4);
    engInfoForm->addWidget(new QLabel("Max. Force:"),2,4);
    engInfoForm->addWidget(new QLabel("Max. Power:"),3,4);    
    
    engInfoForm->addWidget(&eName,0,1);
    engInfoForm->addWidget(&eFileName,1,1);
    engInfoForm->addWidget(&eDirName,2,1);
    engInfoForm->addWidget(&eShape,3,1);
    engInfoForm->addWidget(&eType,0,3);
    engInfoForm->addWidget(&eBrakes,1,3);
    engInfoForm->addWidget(&eCouplings,2,3);
    engInfoForm->addWidget(&eSize,3,3);
    engInfoForm->addWidget(&eMass,0,5);
    engInfoForm->addWidget(&eMaxSpeed,1,5);
    engInfoForm->addWidget(&eMaxForce,2,5);
    engInfoForm->addWidget(&eMaxPower,3,5);
    eMass.setMaximumWidth(70);
    eMaxSpeed.setMaximumWidth(70);
    eMaxForce.setMaximumWidth(70);
    eMaxPower.setMaximumWidth(70);
    engInfoLayout->addItem(engInfoForm);
    engSetsWidget = new QWidget(this);
    QGridLayout *engSetsWidgetForm = new QGridLayout;
    engSetsWidgetForm->setSpacing(0);
    engSetsWidgetForm->setContentsMargins(0,0,0,0);    
    engSetsWidget->setLayout(engSetsWidgetForm);
    QLabel *engSetsLabel = GuiFunct::newTQLabel("Eng Sets Detected:");
    QPushButton *engSetShowButton = new QPushButton("Show");
    engSetShowButton->setFixedWidth(60);
    QPushButton *engSetHideButton = new QPushButton("Hide");
    engSetHideButton->setFixedWidth(60);
    QPushButton *engSetAddButton = new QPushButton("Add to Consist");
    engSetAddButton->setFixedWidth(120);
    QPushButton *engSetAddFlipButton = new QPushButton("Flip and add to Consist");
    engSetAddFlipButton->setFixedWidth(135);
    engSetsList.setFixedWidth(250);
    engSetsWidgetForm->addWidget(engSetsLabel,0,0);
    engSetsWidgetForm->addWidget(&engSetsList,0,1);
    engSetsList.setStyleSheet("combobox-popup: 0;");
    engSetsWidgetForm->addWidget(engSetShowButton,0,2);
    engSetsWidgetForm->addWidget(engSetHideButton,0,3);
    engSetsWidgetForm->addWidget(engSetAddButton,0,4);
    engSetsWidgetForm->addWidget(engSetAddFlipButton,0,5);
    engInfoLayout->addWidget(engSetsWidget);
    engInfoLayout->setSpacing(0);
    engInfoLayout->setContentsMargins(0,0,0,0);
    engInfo->setLayout(engInfoLayout);
    vbox->addWidget(engInfo,0,4);
    //vbox->addStretch(1);
    mbox->addItem(vbox);
    conInfo = new QWidget(this);
    QGridLayout *conInfoForm = new QGridLayout;
    conInfoForm->setSpacing(2);
    conInfoForm->setContentsMargins(1,1,1,1);    
    conInfoForm->addWidget(new QLabel("File Name:"),0,0);
    conInfoForm->addWidget(new QLabel("Display Name:"),1,0);
    conInfoForm->addWidget(new QLabel("Total Mass:"),0,2);
    conInfoForm->addWidget(new QLabel("Length:"),1,2);
    conInfoForm->addWidget(new QLabel("Eng Mass:"),0,4);
    conInfoForm->addWidget(new QLabel("Wag Mass:"),1,4);
    conInfoForm->addWidget(new QLabel("Units:"),0,6);
    conInfoForm->addWidget(new QLabel("Durability:"),1,6);
    conInfoForm->addWidget(&cFileName,0,1);
    conInfoForm->addWidget(&cDisplayName,1,1);
    conInfoForm->addWidget(&cMass,0,3);
    conInfoForm->addWidget(&cLength,1,3);
    conInfoForm->addWidget(&cEmass,0,5);
    conInfoForm->addWidget(&cWmass,1,5);
    conInfoForm->addWidget(&cUnits,0,7);
    conInfoForm->addWidget(&cDurability,1,7);
    cMass.setFixedWidth(100);
    cEmass.setFixedWidth(100);
    cWmass.setFixedWidth(100);
    cLength.setFixedWidth(100);
    cUnits.setFixedWidth(100);
    cDurability.setFixedWidth(100);
    conInfo->setLayout(conInfoForm);
    mbox->addWidget(conInfo);
    mbox->addWidget(glConWidget);
    mbox->addWidget(conSlider);
    //glConWidget->setFixedHeight(150);
    glConWidget->setMinimumSize(1000, 100);
    QSizePolicy policy(glConWidget->sizePolicy());
    policy.setHeightForWidth(true);
    glConWidget->setSizePolicy(policy);
    
    main->setLayout(mbox);
    this->setCentralWidget(main);
    
    setWindowTitle(Game::AppName+" "+Game::AppVersion+" Consist Editor"+"   [ "+Game::root+" ]");
    fileMenu = menuBar()->addMenu(tr("&File"));
    fNew = new QAction(tr("&New"), this); 
    fileMenu->addAction(fNew);
    QObject::connect(fNew, SIGNAL(triggered(bool)), this, SLOT(newConsist()));
    fSave = new QAction(tr("&Save"), this); 
    fileMenu->addAction(fSave);
    QObject::connect(fSave, SIGNAL(triggered(bool)), this, SLOT(save()));
    fExit = new QAction(tr("&Exit"), this); 
    fileMenu->addAction(fExit);
    QObject::connect(fExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    consistMenu = menuBar()->addMenu(tr("&Consist"));
    cReverse = new QAction(tr("&Reverse"), this); 
    consistMenu->addAction(cReverse);
    QObject::connect(cReverse, SIGNAL(triggered(bool)), this, SLOT(cReverseSelected()));
    cClone = new QAction(tr("&Clone"), this); 
    consistMenu->addAction(cClone);
    QObject::connect(cClone, SIGNAL(triggered(bool)), this, SLOT(cCloneSelected()));
    cDelete = new QAction(tr("&Delete"), this); 
    consistMenu->addAction(cDelete);
    QObject::connect(cDelete, SIGNAL(triggered(bool)), this, SLOT(cDeleteSelected()));
    cOpenInExtEditor = new QAction(tr("&Open in external editor"), this); 
    consistMenu->addAction(cOpenInExtEditor);
    QObject::connect(cOpenInExtEditor, SIGNAL(triggered(bool)), this, SLOT(cOpenInExternalEditor()));
    cSaveAsEngSet = new QAction(tr("&Save as Eng Set"), this); 
    consistMenu->addAction(cSaveAsEngSet);
    QObject::connect(cSaveAsEngSet, SIGNAL(triggered()), this, SLOT(cSaveAsEngSetSelected()));
    engMenu = menuBar()->addMenu(tr("&Eng"));
    eFindCons = new QAction(tr("&Find Consists"), this); 
    engMenu->addAction(eFindCons);
    QObject::connect(eFindCons, SIGNAL(triggered(bool)), this, SLOT(eFindConsistsByEng()));
    eOpenInExtEditor = new QAction(tr("&Open in external editor"), this); 
    engMenu->addAction(eOpenInExtEditor);
    QObject::connect(eOpenInExtEditor, SIGNAL(triggered(bool)), this, SLOT(eOpenInExternalEditor()));
    eOpenLegacyInExtEditor = new QAction(tr("&Open legacy ENG in ext. editor"), this); 
    engMenu->addAction(eOpenLegacyInExtEditor);
    QObject::connect(eOpenLegacyInExtEditor, SIGNAL(triggered(bool)), this, SLOT(eOpenLegacyInExternalEditor()));
    viewMenu = menuBar()->addMenu(tr("&View"));
    vConList = GuiFunct::newMenuCheckAction(tr("&Consist List"), this); 
    viewMenu->addAction(vConList);
    QObject::connect(vConList, SIGNAL(triggered(bool)), this, SLOT(viewConList(bool)));
    vEngList1 = GuiFunct::newMenuCheckAction(tr("&Eng List 1"), this); 
    viewMenu->addAction(vEngList1);
    QObject::connect(vEngList1, SIGNAL(triggered(bool)), this, SLOT(viewEngList1(bool)));
    vEngList2 = GuiFunct::newMenuCheckAction(tr("&Eng List 2"), this); 
    viewMenu->addAction(vEngList2);
    QObject::connect(vEngList2, SIGNAL(triggered(bool)), this, SLOT(viewEngList2(bool)));
    vConUnits = GuiFunct::newMenuCheckAction(tr("&Consist Units"), this); 
    viewMenu->addAction(vConUnits);
    QObject::connect(vConUnits, SIGNAL(triggered(bool)), this, SLOT(viewConUnits(bool)));
    vEngView = GuiFunct::newMenuCheckAction(tr("&Eng View"), this); 
    viewMenu->addAction(vEngView);
    QObject::connect(vEngView, SIGNAL(triggered(bool)), this, SLOT(viewEngView(bool)));
    vConView = GuiFunct::newMenuCheckAction(tr("&Con View"), this); 
    viewMenu->addAction(vConView);
    QObject::connect(vConView, SIGNAL(triggered(bool)), this, SLOT(viewConView(bool)));
    view3dMenu = menuBar()->addMenu(tr("&3D View"));
    vResetShapeView = new QAction(tr("&Shape View: Reset"), this); 
    view3dMenu->addAction(vResetShapeView);
    QObject::connect(vResetShapeView, SIGNAL(triggered()), this, SLOT(vResetShapeViewSelected()));
    vGetImgShapeView = new QAction(tr("&Shape View: Copy Image"), this); 
    view3dMenu->addAction(vGetImgShapeView);
    QObject::connect(vGetImgShapeView, SIGNAL(triggered()), this, SLOT(vGetImgShapeViewSelected()));
    vSaveImgShapeView = new QAction(tr("&Shape View: Save Image"), this); 
    view3dMenu->addAction(vSaveImgShapeView);
    QObject::connect(vSaveImgShapeView, SIGNAL(triggered()), this, SLOT(vSaveImgShapeViewSelected()));    
    vSetColorShapeView = new QAction(tr("&Shape View: Set Color"), this); 
    view3dMenu->addAction(vSetColorShapeView);
    QObject::connect(vSetColorShapeView, SIGNAL(triggered()), this, SLOT(vSetColorShapeViewSelected()));
    vSetColorConView = new QAction(tr("&Con View: Set Color"), this); 
    view3dMenu->addAction(vSetColorConView);
    QObject::connect(vSetColorConView, SIGNAL(triggered()), this, SLOT(vSetColorConViewSelected()));
    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    sLoadEngSetsByDefault = GuiFunct::newMenuCheckAction(tr("&Auto load Eng Sets"), this);
    QObject::connect(sLoadEngSetsByDefault, SIGNAL(triggered(bool)), this, SLOT(sLoadEngSetsByDefaultSelected(bool)));
    settingsMenu->addAction(sLoadEngSetsByDefault);
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu->addAction(aboutAction);
    
    QObject::connect(eng1, SIGNAL(engListSelected(int)),
                      this, SLOT(engListSelected(int)));
    QObject::connect(eng2, SIGNAL(engListSelected(int)),
                      this, SLOT(engListSelected(int)));
    
    QObject::connect(eng1, SIGNAL(addToConSelected(int, int, int)),
                      this, SLOT(addToConSelected(int, int, int)));
    QObject::connect(eng2, SIGNAL(addToConSelected(int, int, int)),
                      this, SLOT(addToConSelected(int, int, int)));
    
    QObject::connect(randomConsist, SIGNAL(addToConSelected(int, int, int)),
                      this, SLOT(addToConSelected(int, int, int)));
    
    QObject::connect(eng1, SIGNAL(addToRandomConsist(int)),
                      this, SLOT(addToRandomConsist(int)));
    QObject::connect(eng2, SIGNAL(addToRandomConsist(int)),
                      this, SLOT(addToRandomConsist(int)));
    
    QObject::connect(con1, SIGNAL(conListSelected(int)),
                      this, SLOT(conListSelected(int)));
    
    QObject::connect(con1, SIGNAL(conListSelected(int,int)),
                      this, SLOT(conListSelected(int,int)));
    
    QObject::connect(this, SIGNAL(showEng(QString, QString)),
                      glShapeWidget, SLOT(showEng(QString, QString))); 
    
    QObject::connect(this, SIGNAL(showEng(Eng*)),
                      glShapeWidget, SLOT(showEng(Eng*))); 
    
    QObject::connect(this, SIGNAL(showEngSet(int)),
                      glShapeWidget, SLOT(showEngSet(int))); 
    
    QObject::connect(this, SIGNAL(showCon(int)),
                      glConWidget, SLOT(showCon(int))); 
    QObject::connect(this, SIGNAL(showCon(int, int)),
                      glConWidget, SLOT(showCon(int, int))); 
    
    QObject::connect(conSlider, SIGNAL(valueChanged(int)),
                      this, SLOT(conSliderValueChanged(int))); 
    
    QObject::connect(units, SIGNAL(selected(int)),
                      this, SLOT(conUnitSelected(int))); 
    
    QObject::connect(glConWidget, SIGNAL(selected(int)),
                      this, SLOT(conUnitSelected(int))); 
    
    QObject::connect(glConWidget, SIGNAL(refreshItem()),
                      this, SLOT(refreshCurrentCon())); 
    
    QObject::connect(units, SIGNAL(refreshItem()),
                      this, SLOT(refreshCurrentCon())); 
    
    QObject::connect(&cFileName, SIGNAL(textEdited(QString)),
                      this, SLOT(cFileNameSelected(QString))); 
    
    QObject::connect(&cDisplayName, SIGNAL(textEdited(QString)),
                      this, SLOT(cDisplayNameSelected(QString))); 
    
    QObject::connect(&cDurability, SIGNAL(editingFinished()),
                      this, SLOT(cDurabilitySelected())); 

    QObject::connect(&engSetsList, SIGNAL(activated(QString)),
                      this, SLOT(engSetShowSet(QString)));
    
    QObject::connect(engSetShowButton, SIGNAL(released()),
        this, SLOT(engSetShowSelected()));
    QObject::connect(engSetHideButton, SIGNAL(released()),
        this, SLOT(engSetHideSelected()));
    QObject::connect(engSetAddButton, SIGNAL(released()),
        this, SLOT(engSetAddSelected()));
    QObject::connect(engSetAddFlipButton, SIGNAL(released()),
        this, SLOT(engSetFlipAndAddSelected()));
    
    vEngList2->trigger();
    vConUnits->trigger();
}

ConEditorWindow::~ConEditorWindow() {
}

void ConEditorWindow::vSetColorConViewSelected(){
    QColor color = QColorDialog::getColor(Qt::black, this, "Shape View Color",  QColorDialog::DontUseNativeDialog);
    glConWidget->setBackgroundGlColor((float)color.redF(), (float)color.greenF(), (float)color.blueF());
}

void ConEditorWindow::vSetColorShapeViewSelected(){
    QColor color = QColorDialog::getColor(Qt::black, this, "Shape View Color",  QColorDialog::DontUseNativeDialog);
    glShapeWidget->setBackgroundGlColor((float)color.redF(), (float)color.greenF(), (float)color.blueF());
}

void ConEditorWindow::eFindConsistsByEng(){
    if(currentEng == NULL) return;
    int eid = englib->getEngByPathid(currentEng->pathid);
    if(eid < 0) return;
    con1->findConsistsByEng(eid);
}

void ConEditorWindow::cOpenInExternalEditor(){
    if(currentCon == NULL) return;
    QFileInfo fileInfo(currentCon->pathid);
    qDebug() << currentCon->pathid;
    if(fileInfo.exists())
        QDesktopServices::openUrl(QUrl::fromLocalFile(currentCon->pathid));
}

void ConEditorWindow::eOpenLegacyInExternalEditor(){
    if(currentEng == NULL) return;
    QFileInfo fileInfo(currentEng->pathid);
    if(fileInfo.exists())
        QDesktopServices::openUrl(QUrl::fromLocalFile(currentEng->pathid));
}

void ConEditorWindow::eOpenInExternalEditor(){
    if(currentEng == NULL) return;
    if(currentEng->filePaths.size() == 1){
        QFileInfo fileInfo(currentEng->filePaths[0]);
        if(fileInfo.exists())
            QDesktopServices::openUrl(QUrl::fromLocalFile(currentEng->filePaths[0]));
    } else {
        ChooseFileDialog chooseFileDialog;
        chooseFileDialog.setMsg("This ENG contains more than one file:");
        chooseFileDialog.setWindowTitle("Choose file:");
        for(int i = 0; i < currentEng->filePaths.size(); i++){
            chooseFileDialog.items.addItem(""+currentEng->filePaths[i]);
        }
        chooseFileDialog.exec();
        if(chooseFileDialog.changed == 1){
            QFileInfo fileInfo(currentEng->filePaths[chooseFileDialog.items.currentRow()]);
            if(fileInfo.exists())
                QDesktopServices::openUrl(QUrl::fromLocalFile(currentEng->filePaths[chooseFileDialog.items.currentRow()]));
        }
    }
}

void ConEditorWindow::copyImgShapeView(){
    if(glShapeWidget->screenShot != NULL)
        QApplication::clipboard()->setImage((glShapeWidget->screenShot->mirrored(false, true)), QClipboard::Clipboard);
}

void ConEditorWindow::saveImgShapeView(){
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

void ConEditorWindow::vGetImgShapeViewSelected(){
    if(currentEng == NULL) return;
    glShapeWidget->getImg();
    QTimer::singleShot(500, this, SLOT(copyImgShapeView()));
}/**/

void ConEditorWindow::vSaveImgShapeViewSelected(){
    if(currentEng == NULL) return;
    glShapeWidget->getImg();
    QTimer::singleShot(500, this, SLOT(saveImgShapeView()));
}/**/

void ConEditorWindow::vResetShapeViewSelected(){
    if(currentEng == NULL) return;
    float pos = -currentEng->sizez-1;
    if(pos > -15) pos = -15;
    engCamera->setPos(pos,2.5,0);
    engCamera->setPlayerRot(M_PI/2.0,0);
    glShapeWidget->resetRot();
}

void ConEditorWindow::save(){
    if(con1->isActivity())
        saveCurrentActivity();
    else
        saveCurrentConsist();
}

void ConEditorWindow::saveCurrentActivity(){
    int id = con1->getCurrentActivityId();
    if(ActLib::act[id] == NULL)
        return;
    ActLib::act[id]->save();
}

void ConEditorWindow::saveCurrentConsist(){
    if(currentCon == NULL) return;
    if(currentCon->isNewConsist()){
        OverwriteDialog owerwriteDialog;
        owerwriteDialog.setWindowTitle("Overwrite \""+currentCon->conName+"\" ?");
        //owerwriteDialog.name.setText(currentCon->conName);
        QString spath;
        do {
            spath = currentCon->path + "/" + currentCon->name;
            spath.replace("//", "/");
            qDebug() << spath;
            QFile file(spath);
            if(!file.exists())
                break;
            owerwriteDialog.exec();
            if(owerwriteDialog.changed == 0)
                return;
            if(owerwriteDialog.changed == 1)
                break;
            //currentCon->conName = owerwriteDialog.name.text();
            //currentCon->name = currentCon->conName + ".con";
            //cFileName.setText(currentCon->conName);
        } while(owerwriteDialog.changed == 2);
    }
    Game::currentEngLib = englib;
    currentCon->save();
}

void ConEditorWindow::newConsist(){
    Game::currentEngLib = englib;
    con1->newConsist();
}

void ConEditorWindow::cCloneSelected(){
    Game::currentEngLib = englib;
    if(currentCon == NULL) return;
    con1->newConsist(currentCon);
}

void ConEditorWindow::cDeleteSelected(){
    Game::currentEngLib = englib;
    if(currentCon == NULL) return;
    con1->deleteCurrentCon();
    showCon(-1);
}


void ConEditorWindow::about(){
    aboutWindow->show();
}

void ConEditorWindow::cDurabilitySelected(){
    if(currentCon == NULL) return;
    currentCon->setDurability(cDurability.value());
}

void ConEditorWindow::cFileNameSelected(QString n){
    if(currentCon == NULL) return;
    if(!currentCon->isNewConsist()){
        cFileName.setText(currentCon->conName);
        return;
    }
    currentCon->conName = n;
    if(currentCon->displayName == "")
        currentCon->showName = n;
    currentCon->name = n+".con";
    con1->updateCurrentCon();
}

void ConEditorWindow::cDisplayNameSelected(QString n){
    if(currentCon == NULL) return;
    currentCon->setDisplayName(n);
    con1->updateCurrentCon();
}

void ConEditorWindow::cReverseSelected(){
    if(currentCon == NULL) return;
    Game::currentEngLib = englib;
    currentCon->reverse();
    refreshCurrentCon();
}

void ConEditorWindow::conUnitSelected(int uid){
    if(currentCon == NULL) return;
    currentCon->select(uid);
    setCurrentEng(currentCon->engItems[uid].eng);
}

void ConEditorWindow::viewConList(bool show){
    if(show) con1->show();
    else con1->hide();
}
void ConEditorWindow::viewEngList1(bool show){
    if(show) eng1->show();
    else eng1->hide();
}
void ConEditorWindow::viewEngList2(bool show){
    if(show) eng2->show();
    else eng2->hide();
}
void ConEditorWindow::viewConUnits(bool show){
    if(show) units->show();
    else units->hide();
}
void ConEditorWindow::viewEngView(bool show){
    if(show) engInfo->show();
    else engInfo->hide();
}
void ConEditorWindow::viewConView(bool show){
    if(show) glConWidget->show();
    else glConWidget->hide();
    if(show) conInfo->show();
    else conInfo->hide();
    if(show) conSlider->show();
    else conSlider->hide();
}

void ConEditorWindow::setCurrentEng(int id, int engSetId){
    currentEng = englib->eng[id];
    qDebug() << currentEng->engName;

    engSets.clear();
    con1->getEngSets(currentEng, engSets);
    if(engSets.size() > 0){
        this->engSetsWidget->show();
        engSetsList.clear();
        for(int i = 0; i < engSets.size(); i++){
            engSetsList.addItem(ConLib::con[engSets[i]]->showName, i);
        }
    } else {
        this->engSetsWidget->hide();
        this->engSetsList.clear();
        engSetId = -1;
    }
    
    fillCurrentEng(engSetId);
}

void ConEditorWindow::fillCurrentEng(int engSetId){
    if(currentEng == NULL)
        return;
    
    float pos = 0;
    
    if(engSetId >= 0 ){
        pos = -ConLib::con[engSets[engSetId]]->conLength - 1;
        if(pos > -15) pos = -15;
        emit showEngSet(engSets[engSetId]);
    } else {
        pos = -currentEng->sizez-1;
        if(pos > -15) pos = -15;
        emit showEng(currentEng);
    }
    engCamera->setPos(pos,2.5,0);
    engCamera->setPlayerRot(M_PI/2.0,0);
    
    eName.setText(currentEng->displayName);
    eFileName.setText(currentEng->name);
    eDirName.setText(currentEng->path.split("/").last());
    QString ttype = currentEng->type;
    if(currentEng->engType.length() > 1)
        ttype += " ( "+currentEng->engType+" )";
    eType.setText(ttype);
    //eBrakes;
    //eCouplings;
    eMass.setText(QString::number(currentEng->mass) + " t");
    if(currentEng->wagonTypeId >= 4){
        eMaxSpeed.setText(QString::number((int)currentEng->maxSpeed) + " km/h");
        eMaxForce.setText(QString::number((int)currentEng->maxForce / 1000.0) + " kN");
        eMaxPower.setText(QString::number((int)currentEng->maxPower ) + " kW");
    } else {
        eMaxSpeed.setText("--");
        eMaxForce.setText("--");
        eMaxPower.setText("--");
    }
    eShape.setText(currentEng->shape.name);
    eSize.setText(QString::number(currentEng->sizex)+" "+QString::number(currentEng->sizey)+" "+QString::number(currentEng->sizez)+" ");
    eCouplings.setText(currentEng->getCouplingsName());
    eBrakes.setText(currentEng->brakeSystemType);
}

void ConEditorWindow::engSetAddSelected(){
    if(currentCon == NULL) return;
    int cid = engSetsList.currentIndex();
    if(cid > engSets.size()) return;
    cid = engSets[cid];
    if(currentCon == ConLib::con[cid])
        return;
    
    for(int i = 0; i < ConLib::con[cid]->engItems.size(); i++)
        currentCon->appendEngItem(ConLib::con[cid]->engItems[i].eng, 2, ConLib::con[cid]->engItems[i].flip);
    refreshCurrentCon();
    conSlider->setValue(currentCon->engItems.size()-2);
}

void ConEditorWindow::engSetFlipAndAddSelected(){
    if(currentCon == NULL) return;
    int cid = engSetsList.currentIndex();
    if(cid > engSets.size()) return;
    cid = engSets[cid];
    
    for(int i = ConLib::con[cid]->engItems.size() - 1; i >= 0 ; i--){
        currentCon->appendEngItem(ConLib::con[cid]->engItems[i].eng, 2);
        currentCon->engItems[currentCon->engItems.size()-1].flip = !ConLib::con[cid]->engItems[i].flip;
    }
    refreshCurrentCon();
    conSlider->setValue(currentCon->engItems.size()-2);
}

void ConEditorWindow::engSetHideSelected(){
    this->fillCurrentEng(-1);
}

void ConEditorWindow::engSetShowSelected(){
    this->fillCurrentEng(engSetsList.currentIndex());
}

void ConEditorWindow::engSetShowSet(QString n){
    this->fillCurrentEng(engSetsList.currentIndex());
}

void ConEditorWindow::cSaveAsEngSetSelected(){
    qDebug() << "new eng set";
    if(currentCon == NULL) return;
    if(!currentCon->isNewConsist()){
        QMessageBox msgBox;
        msgBox.setText("Consist must be new.");
        msgBox.exec();
        return;
    }
    QString fileName = currentCon->name.split("#").last().split(".con").first();
    QString engName = currentCon->getFirstEngName();
    
    if(fileName == "")
        fileName = engName;
    else
        fileName = engName + "#" + fileName;
    
    currentCon->conName = fileName;
    currentCon->showName = fileName;
    cFileName.setText(currentCon->conName);
    currentCon->name = fileName+".con";
    
    OverwriteDialog owerwriteDialog;
    owerwriteDialog.setWindowTitle("Overwrite \""+currentCon->conName+"\" ?");
    
    QString spath;
    spath = currentCon->path + "/" + currentCon->name;
    spath.replace("//", "/");
    qDebug() << spath;
    QFile file(spath);
    if(file.exists()){
        owerwriteDialog.exec();
        if(owerwriteDialog.changed == 0)
            return;
    }
    currentCon->save();
    con1->updateCurrentCon();
}

void ConEditorWindow::sLoadEngSetsByDefaultSelected(bool show){
    loadEngSetsByDefault = show;
}

void ConEditorWindow::engListSelected(int id){
    if(loadEngSetsByDefault)
        setCurrentEng(id, 0);
    else
        setCurrentEng(id, -1);
    //currentEng = englib->eng[id];
    qDebug() << currentEng->engName;
    //float pos = -currentEng->sizez-1;
    //if(pos > -15) pos = -15;
    //engCamera->setPos(pos,2.5,0);
    //engCamera->setPlayerRot(M_PI/2.0,0);

    //emit showEng(englib->eng[id]->path, englib->eng[id]->name);

}

void ConEditorWindow::addToConSelected(int id, int pos, int count){
    if(currentCon == NULL) return;
    Game::currentEngLib = englib;
    for(int i = 0; i < count; i++)
        currentCon->appendEngItem(id, pos);
    refreshCurrentCon();
    if(pos == 0)
        conSlider->setValue(0);
    if(pos == 2)
        conSlider->setValue(currentCon->engItems.size()-2);
    if(pos == 1)
        conSlider->setValue(currentCon->selectedIdx);
}

void ConEditorWindow::conListSelected(int id){
    currentCon = ConLib::con[id];
    qDebug() << currentCon->conName;
    refreshCurrentCon();
    conSlider->setValue(0);
    emit showCon(id);
}

void ConEditorWindow::conListSelected(int aid, int id){
    currentCon = ActLib::act[aid]->activityObjects[id].con;
    qDebug() << currentCon->showName;
    refreshCurrentCon();
    conSlider->setValue(0);
    emit showCon(aid, id);
}

void ConEditorWindow::refreshCurrentCon(){
    units->setCon(currentCon);
    conSlider->setMinimum(0);
    conSlider->setMaximum(currentCon->engItems.size());
    if(conSlider->value() > conSlider->maximum())
        conSlider->setValue(conSlider->maximum());
    cFileName.setText(currentCon->conName);
    cDisplayName.setText(currentCon->displayName);
    cMass.setText(QString::number(currentCon->mass) + " t");
    cEmass.setText(QString::number(currentCon->emass) + " t");
    cWmass.setText(QString::number(currentCon->mass - currentCon->emass) + " t");
    cLength.setText(QString::number(currentCon->conLength) + " m");
    cUnits.setText(QString::number(currentCon->engItems.size()));
    cDurability.setValue(currentCon->durability);
}

void ConEditorWindow::conSliderValueChanged(int val){
    if(currentCon == NULL) return;
    if(currentCon->engItems.size() < 1) return;
    if(val > currentCon->engItems.size() - 1)
        val = currentCon->engItems.size() - 1;
    float len = currentCon->engItems[val].conLength;
    conCamera->setPos(-100,2.5,42 + len);
}
//addToRandomConsist
void ConEditorWindow::addToRandomConsist(int id){
    if(englib->eng[id] == NULL) return;
    randomConsist->show();
    new QListWidgetItem ( englib->eng[id]->displayName, &randomConsist->items, id);
}

void ConEditorWindow::closeEvent( QCloseEvent *event )
{
    std::vector<int> unsavedConIds;
    std::vector<int> unsavedActIds;
    con1->getUnsaed(unsavedConIds);
    con1->getUnsaedAct(unsavedActIds);
    if(unsavedConIds.size()+unsavedActIds.size() == 0){
        qDebug() << "nic do zapisania";
        event->accept();
        return;
    }
    
    UnsavedDialog unsavedDialog;
    unsavedDialog.setMsg("Save changes in consists?");
    unsavedDialog.setWindowTitle("Save changes?");
    for(int i = 0; i < unsavedConIds.size(); i++){
        if(ConLib::con[unsavedConIds[i]] == NULL) continue;
        unsavedDialog.items.addItem("[C] "+ConLib::con[unsavedConIds[i]]->showName);
    }
    for(int i = 0; i < unsavedActIds.size(); i++){
        if(ActLib::act[unsavedActIds[i]] == NULL) continue;
        unsavedDialog.items.addItem("[A] "+ActLib::act[unsavedActIds[i]]->header->name);
    }
    unsavedDialog.exec();
    if(unsavedDialog.changed == 0){
        event->ignore();
        return;
    }
    if(unsavedDialog.changed == 2){
        event->accept();
        return;
    }
    
    for(int i = 0; i < unsavedConIds.size(); i++){
        currentCon = ConLib::con[unsavedConIds[i]];
        if(currentCon == NULL) continue;
        if(currentCon->isUnSaved())
            this->saveCurrentConsist();
    }
    for(int i = 0; i < unsavedActIds.size(); i++){
        if(ActLib::act[unsavedActIds[i]] == NULL) continue;
        if(ActLib::act[unsavedActIds[i]]->isUnSaved())
            ActLib::act[unsavedActIds[i]]->save();
    }
    //qDebug() << "aaa2";
    event->accept();
}