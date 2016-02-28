#include "TerrainTreeWindow.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "CoordsMkr.h"
#include "IghCoords.h"

TerrainTreeWindow::TerrainTreeWindow() : QDialog() {
    QImage myImage(512, 512, QImage::Format_RGB888);
    imageLabel = new QLabel("");
    imageLabel->setContentsMargins(0,0,0,0);
    imageLabel->setPixmap(QPixmap::fromImage(myImage));
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageLabel);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);
}

TerrainTreeWindow::~TerrainTreeWindow() {
}

