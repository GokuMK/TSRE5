#include "PropertiesStatic.h"
#include "WorldObj.h"

PropertiesStatic::PropertiesStatic(){
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Static:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->uid.setDisabled(true);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlist->addRow("UiD:",&this->uid);
    vlist->addRow("Tile X:",&this->tX);
    vlist->addRow("Tile Z:",&this->tY);
    vbox->addItem(vlist);
    
    QLabel * label1 = new QLabel("FileName:");
    label1->setContentsMargins(3,0,0,0);
    vbox->addWidget(label1);
    this->fileName.setDisabled(true);
    this->fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->fileName);
    QPushButton *copyF = new QPushButton("Copy FileName", this);
    vbox->addWidget(copyF);
    
    QLabel * label2 = new QLabel("Position:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("X:",&this->posX);
    vlist->addRow("Y:",&this->posY);
    vlist->addRow("Z:",&this->posZ);
    vbox->addItem(vlist);
    QPushButton *copyPos = new QPushButton("Copy Position", this);
    QPushButton *pastePos = new QPushButton("Paste Position", this);
    vbox->addWidget(copyPos);
    vbox->addWidget(pastePos);
    QLabel * label3 = new QLabel("QDirection:");
    label3->setStyleSheet("QLabel { color : #999999; }");
    label3->setContentsMargins(3,0,0,0);
    vbox->addWidget(label3);
    this->quat.setDisabled(true);
    this->quat.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->quat);
    QPushButton *copyQrot = new QPushButton("Copy Rotation", this);
    QPushButton *pasteQrot = new QPushButton("Paste Rotation", this);
    QPushButton *resetQrot = new QPushButton("Reset Rotation", this);
    QPushButton *qRot90 = new QPushButton("Rotate Y 90°", this);
    vbox->addWidget(copyQrot);
    vbox->addWidget(pasteQrot);
    vbox->addWidget(resetQrot);
    vbox->addWidget(qRot90);
    QLabel * label4 = new QLabel("Flags:");
    label4->setStyleSheet("QLabel { color : #999999; }");
    label4->setContentsMargins(3,0,0,0);
    vbox->addWidget(label4);
    this->flags.setDisabled(true);
    this->flags.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->flags);
    QPushButton *copyFlags = new QPushButton("Copy Flags", this);
    QPushButton *pasteFlags = new QPushButton("Paste Flags", this);
    vbox->addWidget(copyFlags);
    vbox->addWidget(pasteFlags);
    checkboxAnim.setText("Animations");
    checkboxTerrain.setText("Terrain Object");
    vbox->addWidget(&checkboxAnim);
    vbox->addWidget(&checkboxTerrain);
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(copyF, SIGNAL(released()),
                      this, SLOT(copyFEnabled()));
}

PropertiesStatic::~PropertiesStatic() {
}

void PropertiesStatic::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }

    this->infoLabel->setText("Object: "+obj->type);
    this->fileName.setText(obj->fileName);
    
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->posX.setText(QString::number(obj->position[0], 'G', 4));
    this->posY.setText(QString::number(obj->position[1], 'G', 4));
    this->posZ.setText(QString::number(-obj->position[2], 'G', 4));
    this->quat.setText(
            QString::number(obj->qDirection[0], 'G', 4) + " " +
            QString::number(obj->qDirection[1], 'G', 4) + " " +
            QString::number(-obj->qDirection[2], 'G', 4) + " " +
            QString::number(obj->qDirection[3], 'G', 4)
            );
    
    
    int l;
    QString flags;
    flags = QString::number(obj->staticFlags, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++)
        flags = "0"+flags;
    this->flags.setText(flags);
}

bool PropertiesStatic::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "static")
        return true;
    if(obj->type == "gantry")
        return true;
    return false;
}

void PropertiesStatic::copyFEnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    //QString originalText = clipboard->text();
    clipboard->setText(this->fileName.text());
}