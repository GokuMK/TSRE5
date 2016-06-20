#include "TrkWindow.h"
#include <QDebug>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "Trk.h"
#include "GuiFunct.h"

TrkWindow::TrkWindow() : QDialog(){
    //QPushButton *loadButton = new QPushButton("Load", this);
    //QImage myImage(800, 800, QImage::Format_RGB888);
    //myImage->load("F:/2.png");
    imageGraphic.setFixedSize(400,250);
    imageLoad.setFixedSize(400,250);
    //imageLabel->setContentsMargins(0,0,0,0);
    //imageLabel->setPixmap(QPixmap::fromImage(myImage));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *main2Layout = new QHBoxLayout;
    mainLayout->addItem(main2Layout);
    mainLayout->addWidget(&description);
    
    QVBoxLayout *tab1 = new QVBoxLayout;
    QGridLayout *settings = new QGridLayout;
    settings->setSpacing(2);
    settings->setContentsMargins(3,0,1,0);    
    int row = 0;
    idName.setMinimumWidth(200);
    
    settings->addWidget(GuiFunct::newTQLabel("Route names"), row++, 0);
    settings->addWidget(new QLabel("Route ID: "), row++, 0);
    settings->addWidget(new QLabel("File names: "), row++, 0);
    settings->addWidget(new QLabel("Display name: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Electricity"), row++, 0);
    settings->addWidget(new QLabel("Electrified: "), row++, 0);
    settings->addWidget(new QLabel("Wire Height: "), row++, 0);
    settings->addWidget(new QLabel("Max Voltage: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Start Tile"), row++, 0);
    settings->addWidget(new QLabel("Tile X: "), row++, 0);
    settings->addWidget(new QLabel("Tile Z: "), row++, 0);
    settings->addWidget(new QLabel("Position X: "), row++, 0);
    settings->addWidget(new QLabel("Position Z: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Other"), row++, 0);
    settings->addWidget(new QLabel("Speed Limit: "), row++, 0);
    settings->addWidget(new QLabel("Restricted Speed: "), row++, 0);
    settings->addWidget(new QLabel("Speed in Miles: "), row++, 0);
    settings->addWidget(new QLabel("Terrain Error Scale: "), row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Environment"), row++, 0);
    settings->addWidget(&envName, row++, 0);
    settings->addWidget(GuiFunct::newTQLabel("Description"), row++, 0);
    row = 0;
    row++;
    settings->addWidget(&idName, row++, 1);
    settings->addWidget(&routeName, row++, 1);
    settings->addWidget(&displayName, row++, 1);
    row++;
    settings->addWidget(&electrified, row++, 1);
    settings->addWidget(&overheadWireHeight, row++, 1);
    settings->addWidget(&maxLineVoltage, row++, 1);
    row++;
    settings->addWidget(&startTileX, row++, 1);
    settings->addWidget(&startTileZ, row++, 1);
    settings->addWidget(&startpX, row++, 1);
    settings->addWidget(&startpZ, row++, 1);
    row++;
    settings->addWidget(&speedLimit, row++, 1);
    settings->addWidget(&tempRestrictedSpeed, row++, 1);
    settings->addWidget(&milepostUnitsKilometers, row++, 1);
    settings->addWidget(&terrainErrorScale, row++, 1);
    row++;
    settings->addWidget(&envValue, row++, 1);
    tab1->addItem(settings);
    QVBoxLayout *tab2 = new QVBoxLayout;
    tab2->addWidget(&imageGraphic);
    tab2->addWidget(&imageLoad);
    main2Layout->addItem(tab1);
    main2Layout->addItem(tab2);
    description.setMinimumHeight(200);
    //mainLayout->addWidget(loadButton);
    //mainLayout->addWidget(imageLabel);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);
    
    //QObject::connect(loadButton, SIGNAL(released()),
    //                  this, SLOT(load()));
}

int TrkWindow::exec() {
    if(this->trk == NULL)
        return 0;
    this->setWindowTitle("Route settings");
    
    this->idName.setText(trk->idName);
    this->routeName.setText(trk->routeName);
    this->displayName.setText(trk->displayName);
    this->maxLineVoltage.setValue(trk->maxLineVoltage);
    this->speedLimit.setValue(trk->speedLimit);
    this->overheadWireHeight.setValue(trk->overheadWireHeight);
    
    QString txt = trk->description;
    txt.replace("\\n","\n");
    this->description.setPlainText(txt);
    for (auto it = trk->environment.begin(); it != trk->environment.end(); ++it)
        this->envName.addItem(QString::fromStdString((*it).first));
    
    return QDialog::exec();
} 

TrkWindow::~TrkWindow() {
}

