#include "NaviBox.h"

NaviBox::NaviBox() : QWidget(){
    QPushButton *jumpButton = new QPushButton("Jump", this);
    QLabel *xLabel = new QLabel("x", this);
    QLabel *yLabel = new QLabel("y", this);
    
    QFormLayout *vbox = new QFormLayout;
    vbox->addRow(xLabel,&xBox);
    vbox->addRow(yLabel,&yBox);
    vbox->addRow(" ",&tileInfo);
    vbox->addWidget(jumpButton);
    //vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(jumpButton, SIGNAL(released()),
                      this, SLOT(jumpTileSelected()));

    tileInfo.setText(" ");
}

void NaviBox::jumpTileSelected(){
    int x = 10; int y = 10;
    x = xBox.text().toInt();
    y = yBox.text().toInt();
    emit jumpTo(x, y);
}

void NaviBox::naviInfo(int x, int z, int all, int hidden){
    if(lastX != x || lastZ != z || all != objCount || hidden != objHidden ){
        lastX = x;
        lastZ = z;
        objCount = all;
        objHidden = hidden;
        this->xBox.setText(QString::number(x, 10));
        this->yBox.setText(QString::number(z, 10));
        this->tileInfo.setText("Objects: "+QString::number(all, 10)+" ( including "+QString::number(hidden, 10)+" hidden )");
    }
}

NaviBox::~NaviBox() {
}

