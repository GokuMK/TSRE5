/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "LoadWindow.h"
#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "Game.h"
#include <QDebug>
#include "NewRouteWindow.h"
#include "GeoCoordinates.h"

LoadWindow::LoadWindow() {
    //this->setWindowFlags( Qt::CustomizeWindowHint );
    setWindowTitle(Game::AppName+" "+Game::AppVersion+" Route Editor");
    this->setFixedSize(600, 700);
    QImage* myImage = new QImage();
    myImage->load(QString("tsre_appdata/")+Game::AppDataVersion+"/load.png");

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
    load->setStyleSheet(QString("background-color: ")+Game::StyleGreenButton);
    connect(load, SIGNAL (released()), this, SLOT (routeLoad()));
    neww = new QPushButton("New");
    neww->setStyleSheet(QString("background-color: ")+Game::StyleYellowButton);
    connect(neww, SIGNAL (released()), this, SLOT (setNewRoute()));
    exit = new QPushButton("Exit");
    exit->setStyleSheet(QString("background-color: ")+Game::StyleRedButton);
    
    
    nowaTrasa = new QLineEdit();
    QRegExp rx("^[a-zA-Z0-9\\_\\-\\ ]*$");
    //QRegExp rx("[\\/<>|\":?*].");
    QRegExpValidator* v = new QRegExpValidator(rx);
    nowaTrasa->setValidator(v);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(myLabel);
    mainLayout->addWidget(myLabel2);
    mainLayout->addWidget(browse);
    QFormLayout *recentLayout = new QFormLayout;
    recentLayout->setContentsMargins(0,0,0,0);
    cRecent.setMaxVisibleItems(10);
    cRecent.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    cRecent.setStyleSheet("combobox-popup: 0;");
    QObject::connect(&cRecent, SIGNAL(activated(QString)),
                      this, SLOT(cRecentEnabled(QString)));
    recentLayout->addRow("Recent: ", &cRecent);
    mainLayout->addItem(recentLayout);
    mainLayout->addWidget(&routeList);
    
    /*nowa = new QWidget();
    QHBoxLayout *vbox1 = new QHBoxLayout;
    vbox1->addWidget(myLabel3);
    vbox1->addWidget(nowaTrasa);
    vbox1->setContentsMargins(0,0,0,0);
    nowa->setLayout(vbox1);
    mainLayout->addWidget(nowa);*/
    
    QWidget* box = new QWidget();
    QHBoxLayout *vbox = new QHBoxLayout;
    vbox->addWidget(load);
    vbox->addWidget(neww);
    vbox->addWidget(exit);
    vbox->setContentsMargins(0,0,0,0);
    box->setLayout(vbox);
    mainLayout->addWidget(box);
    
    mainLayout->setAlignment(myLabel, Qt::AlignTop);
    mainLayout->setAlignment(myLabel2, Qt::AlignTop);
    mainLayout->setAlignment(browse, Qt::AlignTop);
    mainLayout->setAlignment(load, Qt::AlignTop);
    mainLayout->setAlignment(box, Qt::AlignBottom);
    //mainLayout->setAlignment(nowa, Qt::AlignBottom);
    mainLayout->setContentsMargins(1,1,1,1);
    //mainLayout->addWidget(naviBox);
    this->setLayout(mainLayout);
    
    
    //nowaTrasa->hide();

    QObject::connect(exit, SIGNAL (released()), this, SLOT (close()));
    QObject::connect(&routeList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(setLoadRoute()));
    //QObject::connect(nowaTrasa, SIGNAL(textChanged(QString)),
    //                  this, SLOT(setNewRoute()));
    
    listRoots();
    
    if(Game::checkRoot(Game::root)){
        qDebug()<<"ok";
        load->show();
        neww->show();
        neww->setFixedWidth(100);
        exit->setFixedWidth(100);
        browse->setText(Game::root);
        browse->setStyleSheet(QString("color: ")+Game::StyleGreenText);
        this->listRoutes();
    } else {
        exit->setFixedWidth(600);
        load->hide();
        //nowa->hide();
        neww->hide();
    }
}

void LoadWindow::handleBrowseButton(QString directory){
    if(directory == ""){
        QFileDialog *fd = new QFileDialog;
        //QTreeView *tree = fd->findChild <QTreeView*>();
        //tree->setRootIsDecorated(true);
        //tree->setItemsExpandable(true);
        fd->setFileMode(QFileDialog::Directory);
        fd->setOption(QFileDialog::ShowDirsOnly);
        //fd->setViewMode(QFileDialog::Detail);
        int result = fd->exec();
        if (result)
        {
            directory = fd->selectedFiles()[0];
            qDebug()<<directory;
        }
    }
    //Game::root = directory;
    browse->setText(directory);
    browse->setStyleSheet(QString("color: ")+Game::StyleRedText);
    load->hide();
    //nowa->hide();
    neww->hide();
    exit->setFixedWidth(600);
    routeList.clear();
    if(Game::checkRoot(directory)){
        qDebug()<<"ok";
        load->show();
        neww->show();
        neww->setFixedWidth(100);
        exit->setFixedWidth(100);
        browse->setStyleSheet(QString("color: ")+Game::StyleGreenText);
        Game::root = directory;
        this->listRoutes();
        
        int i = 0;
        for(i = 0; i < cRecent.count(); i++){
            if(cRecent.itemText(i) == directory.toLower())
                break;
        }
        if(i == cRecent.count())
            cRecent.addItem(directory.toLower());
        
        QString path;
        path = "cerecent.txt";
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly))
            return;
        QTextStream in(&file);
        QString line;
        for(int i = 0; i < cRecent.count(); i++){
            in << cRecent.itemText(i) << "\n";
        }
        in.flush();
        file.close();
    }
}

void LoadWindow::routeLoad(){
    if(this->newRoute){
        if(!Game::checkRoot(Game::root)) return;
        Game::routeName = Game::route;
        Game::trkName = Game::route;
        Game::writeEnabled = true;
        Game::createNewRoutes = true;
        Game::writeTDB = true;
    }else{
        if(routeList.currentRow() < 0) return;
        Game::route = routeList.currentItem()->text();
        Game::checkRoute(Game::route);
    }
    qDebug() << Game::route;
    this->hide();
    emit showMainWindow();
}

void LoadWindow::listRoutes(){
    QDir dir(Game::root+"/routes");
    dir.setFilter(QDir::Dirs);
    
    foreach(QString dirFile, dir.entryList()){
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

void LoadWindow::cRecentEnabled(QString val){
    handleBrowseButton(val);
}

void LoadWindow::setNewRoute(){
    //qDebug() << "new";
    //this->load->setText("New");
    
    //Check if template route available.
    QString path = "./tsre_assets/templateRoute_0.6";
    QFile appFile(path);
    if (!appFile.exists()){
        downloadTemplateRoute(path);
    }
    
    NewRouteWindow newWindow;
    newWindow.setWindowTitle("New route");
    newWindow.name.setText("");
    newWindow.lat.setText("50.0");
    newWindow.lon.setText("20.0");
    newWindow.exec();
    if(newWindow.changed){
        if(newWindow.name.text().length() < 2) return;
        Game::route = newWindow.name.text();
        double lat = newWindow.lat.text().toDouble();
        double lon = newWindow.lon.text().toDouble();
        
        Game::GeoCoordConverter = new GeoMstsCoordinateConverter();
        
        igh = Game::GeoCoordConverter->ConvertToInternal(lat, lon, igh);
        aCoords = Game::GeoCoordConverter->ConvertToTile(igh, aCoords);
        aCoords->setWxyz();
        Game::newRouteX = aCoords->TileX;
        Game::newRouteZ = aCoords->TileZ;
        qDebug() << Game::newRouteX << " " << Game::newRouteZ;
        this->newRoute = true;
        routeLoad();
    }
}

void LoadWindow::exitNow(){
    this->hide();
}

void LoadWindow::downloadTemplateRoute(QString path){
    QDir().mkdir(path);
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    qDebug() << "Wait ..";
    
    QString Url = "http://koniec.org/tsre5/data/appdata/templateroute_0.6.cab";
    qDebug() << Url;
    QNetworkRequest req;//(QUrl(Url));
    req.setUrl(QUrl(Url));
    qDebug() << req.url();
    QNetworkReply* r = mgr->get(req);
    QEventLoop loop;
    QObject::connect(r, SIGNAL(finished()), &loop, SLOT(quit()));
    //connect(r, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    //connect(r, SIGNAL(finished()), this, SLOT(downloadFinished()));
    loop.exec();
    qDebug() << "Network Reply Loop End";
    QByteArray data = r->readAll();

    QFile file("./temp.cab");
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
    
    QProcess proc;
    QString command = "expand .\\temp.cab .\\tsre_assets\\templateroute_0.6 -F:*";
    proc.start(command);
    if(proc.waitForStarted()){
        qDebug() << "Windows .cab Epand Started";
    }
    proc.waitForFinished(-1);
        qDebug() << "Finished";
        
    qDebug() << file.remove();
}

void LoadWindow::listRoots(){
    QString sh;
    QString path;
    path = "cerecent.txt";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    qDebug() << path;

    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine().toLower();
        cRecent.addItem(line);
    }
    file.close();
} 