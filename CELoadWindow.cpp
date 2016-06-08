#include "CELoadWindow.h"
#include <QtWidgets>
#include "Game.h"
#include <QDebug>
#include "ConEditorWindow.h"

CELoadWindow::CELoadWindow() {
    this->setWindowFlags( Qt::CustomizeWindowHint );
    this->setFixedSize(600, 600);
    QImage* myImage = new QImage();
    myImage->load("resources/load.png");

    QLabel* myLabel = new QLabel("");
    myLabel->setContentsMargins(0,0,0,0);
    //QLabel* myLabel2 = new QLabel("Choose folder containing 'Trains': ");
    //myLabel2->setContentsMargins(5,0,0,0);
    QLabel* myLabel3 = new QLabel("Choose folder containing 'Trains' above, or select recent below: ");
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
    //mainLayout->addWidget(myLabel2);
    mainLayout->addWidget(browse);
    mainLayout->addWidget(myLabel3);
    mainLayout->addWidget(&recentDirs);
    info = new QWidget();
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Consists:", &conNumber);
    vlist->addRow("Rolling Stock:", &trainsNumber);
    info->setLayout(vlist);
    mainLayout->addWidget(info);
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
    vbox->addWidget(exit);
    vbox->setContentsMargins(0,0,0,0);
    box->setLayout(vbox);
    mainLayout->addWidget(box);
    
    mainLayout->setAlignment(myLabel, Qt::AlignTop);
    //mainLayout->setAlignment(myLabel2, Qt::AlignTop);
    mainLayout->setAlignment(browse, Qt::AlignTop);
    mainLayout->setAlignment(load, Qt::AlignTop);
    mainLayout->setAlignment(box, Qt::AlignBottom);
    //mainLayout->setAlignment(nowa, Qt::AlignBottom);
    mainLayout->setContentsMargins(1,1,1,1);
    //mainLayout->addWidget(naviBox);
    this->setLayout(mainLayout);

    
    //nowaTrasa->hide();

    QObject::connect(exit, SIGNAL (released()), this, SLOT (close()));
    QObject::connect(&recentDirs, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(dirSelected()));
    //QObject::connect(nowaTrasa, SIGNAL(textChanged(QString)),
    //                  this, SLOT(setNewRoute()));
    
    if(Game::checkCERoot(Game::root)){
        qDebug()<<"ok";
        load->show();
        exit->setFixedWidth(100);
        browse->setText(Game::root);
        browse->setStyleSheet("color: #00AA00");
        info->show();
        listInfo();
    } else {
        exit->setFixedWidth(600);
        load->hide();
        info->hide();
    }
    
    this->listRoutes();
}

void CELoadWindow::dirSelected(){
    QString directory = recentDirs.currentItem()->text();
    //Game::root = directory;
    browse->setText(directory);
    browse->setStyleSheet("color: #AA0000");
    load->hide();
    info->hide();
    //nowa->hide();
    exit->setFixedWidth(600);
    if(Game::checkCERoot(directory)){
        qDebug()<<"ok";
        load->show();
        exit->setFixedWidth(100);
        browse->setStyleSheet("color: #00AA00");
        Game::root = directory;
        info->show();
        listInfo();        
    }
}

void CELoadWindow::handleBrowseButton(){
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
    info->hide();
    //nowa->hide();
    exit->setFixedWidth(600);
    if(Game::checkCERoot(directory)){
        qDebug()<<"ok";
        load->show();
   
        exit->setFixedWidth(100);
        browse->setStyleSheet("color: #00AA00");
        Game::root = directory;
        //this->listRoutes();
        recentDirs.addItem(directory.toLower());
        for(int i = 0; i < recentDirs.count() - 1; i++){
            if(recentDirs.item(i)->text() == directory){
                recentDirs.removeItemWidget(recentDirs.item(i));
                i--;
            }
        }
        QString path;
        path = "cerecent.txt";
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly))
            return;
        QTextStream in(&file);
        QString line;
        for(int i = 0; i < recentDirs.count(); i++){
            in << recentDirs.item(i)->text() << "\n";
        }
        in.flush();
        file.close();
        info->show();
        listInfo();    
    }
}

void CELoadWindow::routeLoad(){
    ConEditorWindow* cwindow = new ConEditorWindow();
    cwindow->resize(1280, 720);
    this->hide();
    cwindow->show();
}

void CELoadWindow::listInfo(){
    QString num = 0;
    QDir dir(Game::root + "/trains/consists/");
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.con");
    num = QString::number(dir.count());
    conNumber.setText(num+" consists");    
    QDir dir2(Game::root + "/trains/trainset/");
    dir2.setFilter(QDir::Dirs);
    num = QString::number(dir2.count()-2);
    trainsNumber.setText(num+" dirs");    
}

void CELoadWindow::listRoutes(){
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
        recentDirs.addItem(line);
    }
    file.close();
} 

void CELoadWindow::exitNow(){
    this->hide();
}

