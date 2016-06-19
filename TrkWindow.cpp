#include "TrkWindow.h"
#include <QDebug>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "Trk.h"

TrkWindow::TrkWindow() : QDialog(){
    QPushButton *loadButton = new QPushButton("Load", this);
    QImage myImage(800, 800, QImage::Format_RGB888);
    //myImage->load("F:/2.png");
    //imageLabel = new QLabel("");
    //imageLabel->setContentsMargins(0,0,0,0);
    //imageLabel->setPixmap(QPixmap::fromImage(myImage));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(loadButton);
    //mainLayout->addWidget(imageLabel);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);
    
    //QObject::connect(loadButton, SIGNAL(released()),
    //                  this, SLOT(load()));
}

int TrkWindow::exec() {
    this->setWindowTitle("Route settings");

    
    return QDialog::exec();
} 

TrkWindow::~TrkWindow() {
}

