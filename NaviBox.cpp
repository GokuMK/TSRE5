#include "NaviBox.h"

NaviBox::NaviBox() : QWidget(){
    QPushButton *jumpButton = new QPushButton("Jump", this);
    QLabel *xLabel = new QLabel("x", this);
    QLabel *yLabel = new QLabel("y", this);
    
    QFormLayout *vbox = new QFormLayout;
    vbox->addRow(xLabel,&xBox);
    vbox->addRow(yLabel,&yBox);
    vbox->addWidget(jumpButton);
    //vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(jumpButton, SIGNAL(released()),
                      this, SLOT(jumpTileSelected()));

}

void NaviBox::jumpTileSelected(){
    int x = 10; int y = 10;
    x = xBox.text().toInt();
    y = yBox.text().toInt();
    emit jumpTo(x, y);
}

NaviBox::~NaviBox() {
}

