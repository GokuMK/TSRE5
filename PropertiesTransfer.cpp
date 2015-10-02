#include "PropertiesTransfer.h"
#include "WorldObj.h"
#include "TexLib.h"
#include "TransferObj.h"

PropertiesTransfer::PropertiesTransfer() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Transfer:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    
    texPreview = new QPixmap(150,150);
    texPreview->fill(Qt::gray);
    texPreviewLabel = new QLabel("");
    texPreviewLabel->setContentsMargins(0,0,0,0);
    texPreviewLabel->setPixmap(*texPreview);
    
    vbox->addWidget(infoLabel);
    QFormLayout *vlistt = new QFormLayout;
    vlistt->setSpacing(2);
    vlistt->setContentsMargins(3,0,3,0);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlistt->addRow("Tile X:",&this->tX);
    vlistt->addRow("Tile Z:",&this->tY);
    vbox->addItem(vlistt);
    
    QLabel * label0 = new QLabel("Texture:");
    label0->setStyleSheet("QLabel { color : #999999; }");
    label0->setContentsMargins(3,0,0,0);
    vbox->addWidget(label0);
    QLabel * label1 = new QLabel("FileName:");
    label1->setContentsMargins(3,0,0,0);
    vbox->addWidget(label1);
    this->fileName.setDisabled(true);
    this->fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->fileName);
    QPushButton *copyF = new QPushButton("Copy FileName", this);
    vbox->addWidget(copyF);
    vbox->addWidget(texPreviewLabel);
    vbox->setAlignment(texPreviewLabel, Qt::AlignHCenter);
    QPushButton *texLoad = new QPushButton("Load Texture", this);
    QPushButton *texPick = new QPushButton("Pick This", this);
    QPushButton *texPut = new QPushButton("Put Here", this);
    vbox->addWidget(texLoad);
    vbox->addWidget(texPick);
    vbox->addWidget(texPut);
    
    QLabel * label12 = new QLabel("Size:");
    label12->setStyleSheet("QLabel { color : #999999; }");
    label12->setContentsMargins(3,0,0,0);
    vbox->addWidget(label12);
    QFormLayout *vlist0 = new QFormLayout;
    vlist0->setSpacing(2);
    vlist0->setContentsMargins(3,0,3,0);
    vlist0->addRow("Width:",&this->sizeX);
    vlist0->addRow("Height:",&this->sizeY);
    vbox->addItem(vlist0);
    
    QLabel * label2 = new QLabel("Position:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    QFormLayout *vlist = new QFormLayout;
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
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesTransfer::~PropertiesTransfer() {
}

void PropertiesTransfer::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }

    TransferObj* tobj = (TransferObj*)obj;
        
    this->infoLabel->setText("Object: "+obj->type);
    this->fileName.setText(tobj->texture);
        
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->sizeX.setText(QString::number(tobj->width, 'G', 4));
    this->sizeY.setText(QString::number(tobj->height, 'G', 4));
    this->posX.setText(QString::number(obj->position[0], 'G', 4));
    this->posY.setText(QString::number(obj->position[1], 'G', 4));
    this->posZ.setText(QString::number(obj->position[2], 'G', 4));
    this->quat.setText(
            QString::number(obj->qDirection[0], 'G', 4) + " " +
            QString::number(obj->qDirection[1], 'G', 4) + " " +
            QString::number(obj->qDirection[2], 'G', 4) + " " +
            QString::number(obj->qDirection[3], 'G', 4)
            );
    

    
    Texture* tex = TexLib::mtex[tobj->getTexId()];
    
    unsigned char * out = tex->getImageData(128,128);
    if(tex->bytesPerPixel == 3)
        texPreviewLabel->setPixmap(QPixmap::fromImage(QImage(out,128,128,QImage::Format_RGB888)));
    if(tex->bytesPerPixel == 4)
        texPreviewLabel->setPixmap(QPixmap::fromImage(QImage(out,128,128,QImage::Format_RGBA8888)));
}

bool PropertiesTransfer::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "transfer")
        return true;
    return false;
}