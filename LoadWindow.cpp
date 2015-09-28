/* 
 * File:   loadWindow.cpp
 * Author: Goku
 * 
 * Created on 16 maja 2015, 01:22
 */

#include "LoadWindow.h"
#include <QtWidgets>
#include "Game.h"
#include <QDebug>

LoadWindow::LoadWindow() {
    this->setWindowFlags( Qt::CustomizeWindowHint );
    this->setFixedSize(600, 500);
    QImage* myImage = new QImage();
    myImage->load("resources/load.png");

    QLabel* myLabel = new QLabel("");
    myLabel->setContentsMargins(0,0,0,0);
    QLabel* myLabel2 = new QLabel("Choose folder containing 'Global' and 'Routes': ");
    myLabel2->setContentsMargins(5,0,0,0);
    QLabel* myLabel3 = new QLabel("Select route above or enter name for new route: ");
    myLabel3->setContentsMargins(5,0,0,0);
    
    myLabel->setPixmap(QPixmap::fromImage(*myImage));

    browse = new QPushButton("Browse");
    connect(browse, SIGNAL (released()), this, SLOT (handleBrowseButton()));
    load = new QPushButton("Load");
    load->setStyleSheet("background-color: #008800");
    connect(load, SIGNAL (released()), this, SLOT (routeLoad()));
    exit = new QPushButton("Exit");
    exit->setStyleSheet("background-color: #880000");

    
    nowaTrasa = new QLineEdit();
    QRegExp rx("^[a-zA-Z0-9\\_\\-\\ ]*$");
    //QRegExp rx("[\\/<>|\":?*].");
    QRegExpValidator* v = new QRegExpValidator(rx);
    nowaTrasa->setValidator(v);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(myLabel);
    mainLayout->addWidget(myLabel2);
    mainLayout->addWidget(browse);
    mainLayout->addWidget(&routeList);
    
    nowa = new QWidget();
    QHBoxLayout *vbox1 = new QHBoxLayout;
    vbox1->addWidget(myLabel3);
    vbox1->addWidget(nowaTrasa);
    vbox1->setContentsMargins(0,0,0,0);
    nowa->setLayout(vbox1);
    mainLayout->addWidget(nowa);
    
    QWidget* box = new QWidget();
    QHBoxLayout *vbox = new QHBoxLayout;
    vbox->addWidget(load);
    vbox->addWidget(exit);
    vbox->setContentsMargins(0,0,0,0);
    box->setLayout(vbox);
    mainLayout->addWidget(box);
    
    mainLayout->setAlignment(myLabel, Qt::AlignTop);
    mainLayout->setAlignment(myLabel2, Qt::AlignTop);
    mainLayout->setAlignment(browse, Qt::AlignTop);
    mainLayout->setAlignment(load, Qt::AlignTop);
    mainLayout->setAlignment(box, Qt::AlignBottom);
    mainLayout->setAlignment(nowa, Qt::AlignBottom);
    mainLayout->setContentsMargins(1,1,1,1);
    //mainLayout->addWidget(naviBox);
    this->setLayout(mainLayout);
    
    
    //nowaTrasa->hide();

    QObject::connect(exit, SIGNAL (released()), this, SLOT (close()));
    QObject::connect(&routeList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(setLoadRoute()));
    QObject::connect(nowaTrasa, SIGNAL(textChanged(QString)),
                      this, SLOT(setNewRoute()));
    
    if(Game::checkRoot(Game::root)){
        qDebug()<<"ok";
        load->show();
        exit->setFixedWidth(100);
        browse->setText(Game::root);
        browse->setStyleSheet("color: #00AA00");
        this->listRoutes();
    } else {
        exit->setFixedWidth(600);
        load->hide();
        nowa->hide();
    }
}

void LoadWindow::handleBrowseButton(){
    QFileDialog *fd = new QFileDialog;
    //QTreeView *tree = fd->findChild <QTreeView*>();
    //tree->setRootIsDecorated(true);
    //tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    //fd->setViewMode(QFileDialog::Detail);
    int result = fd->exec();
    QString directory;
    if (result)
    {
        directory = fd->selectedFiles()[0];
        qDebug()<<directory;
    }
    //Game::root = directory;
    browse->setText(directory);
    browse->setStyleSheet("color: #AA0000");
    load->hide();
    nowa->hide();
    exit->setFixedWidth(600);
    routeList.clear();
    if(Game::checkRoot(directory)){
        qDebug()<<"ok";
        load->show();
        nowa->show();
        exit->setFixedWidth(100);
        browse->setStyleSheet("color: #00AA00");
        Game::root = directory;
        this->listRoutes();
    }
}

void LoadWindow::routeLoad(){
    if(this->newRoute){
        if(!Game::checkRoot(Game::root)) return;
        if(nowaTrasa->text().length() < 2) return;
        Game::route = nowaTrasa->text();
        Game::writeEnabled = true;
        Game::createNewRoutes = true;
        Game::writeTDB = true;
    }else{
        if(routeList.currentRow() < 0) return;
        Game::route = routeList.currentItem()->text();
    }
    qDebug() << Game::route;
    this->hide();
    emit showMainWindow();
}

void LoadWindow::listRoutes(){
    QDir dir(Game::root+"/routes");
    dir.setFilter(QDir::Dirs);
    
    foreach(QString dirFile, dir.entryList())
    {
        //qDebug() << dirFile;
        if(dirFile == "." || dirFile == "..")
            continue;
        if(!Game::checkRoute(dirFile))
            continue;
        this->routeList.addItem(dirFile);
    }
}

void LoadWindow::setLoadRoute(){
    //qDebug() << "load";
    this->load->setText("Load");
    this->newRoute = false;
}

void LoadWindow::setNewRoute(){
    //qDebug() << "new";
    this->load->setText("New");
    this->newRoute = true;
}

void LoadWindow::exitNow(){
    this->hide();
}

