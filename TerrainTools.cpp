/* 
 * File:   TerrainTools.cpp
 * Author: Goku
 * 
 * Created on 1 czerwca 2015, 20:39
 */

#include "TerrainTools.h"
#include "TexLib.h"
#include "Brush.h"
#include "Texture.h"
#include "GuiFunct.h"

TerrainTools::TerrainTools(QString name)
    : QWidget(){
    
    int row = 0;
    this->paintBrush = new Brush();
    this->texPreview = new QPixmap(256,256);
    this->texPreview->fill(Qt::gray);
    
    QPushButton *heightTool = new QPushButton("HeightMap +", this);
    QPushButton *paintTool = new QPushButton("Color", this);
    QPushButton *paintTool2 = new QPushButton("Texture", this);
    QPushButton *pickTexTool = new QPushButton("Pick", this);
    QPushButton *setTexTool = new QPushButton("Load", this);
    QPushButton *putTexTool = new QPushButton("Put", this);
    
    QGridLayout *vlist0 = new QGridLayout;
    vlist0->setSpacing(2);
    vlist0->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist0->addWidget(heightTool,row,0);
    vlist0->addWidget(paintTool,row,1);
    vlist0->addWidget(paintTool2,row++,2);
    QGridLayout *vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist1->addWidget(pickTexTool,row,0);
    vlist1->addWidget(putTexTool,row,1);
    vlist1->addWidget(setTexTool,row,2);
    
    colorw = new QPushButton("#000000", this);
    colorw->setStyleSheet("background-color:black;");
    
    texPreviewLabel = new QLabel("");
    texPreviewLabel->setContentsMargins(0,0,0,0);
    texPreviewLabel->setPixmap(*texPreview);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QLabel *label0 = new QLabel("Paint:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet("QLabel { color : #999999; }");
    vbox->addWidget(label0);
    vbox->addItem(vlist0);
    QLabel *label1 = new QLabel("Texture:");
    label1->setContentsMargins(3,0,0,0);
    label1->setStyleSheet("QLabel { color : #999999; }");
    vbox->addWidget(label1);
    vbox->addItem(vlist1);

    vbox->addWidget(texPreviewLabel);
    vbox->setAlignment(texPreviewLabel, Qt::AlignHCenter);
    QLabel *label2 = new QLabel("Brush settings:");
    label2->setContentsMargins(3,0,0,0);
    label2->setStyleSheet("QLabel { color : #999999; }");
    vbox->addWidget(label2);
    

    int labelWidth = 70;
    
    // brush
    sSize = new QSlider(Qt::Horizontal);
    sSize->setMinimum(1);
    sSize->setMaximum(100);
    sSize->setValue(10);
    sIntensity = new QSlider(Qt::Horizontal);
    sIntensity->setMinimum(1);
    sIntensity->setMaximum(100);
    sIntensity->setValue(100);
    hType = new QComboBox;
    hType->setStyleSheet("combobox-popup: 0;");
    hType->addItem("Add - simple");
    hType->addItem("Add - if inside 'Size' radius");
    hType->addItem("Fixed Height");
    hType->addItem("Flatten");
    fheight = new QLineEdit();
    fheight->setValidator(new QDoubleValidator(-5000, 5000, 2, this));
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,1,0);    
    leSize = GuiFunct::newQLineEdit(25,3);
    leSize->setValidator(new QIntValidator(1, 100, this));
    leIntensity = GuiFunct::newQLineEdit(25,3);
    leIntensity->setValidator(new QIntValidator(1, 100, this));
    row = 0;
    vlist->addWidget(GuiFunct::newQLabel("Color:", labelWidth),row,0);
    vlist->addWidget(colorw,row++,1,1,2);
    vlist->addWidget(GuiFunct::newQLabel("Size:", labelWidth),row,0);
    vlist->addWidget(leSize,row,1);
    vlist->addWidget(sSize,row++,2);
    vlist->addWidget(GuiFunct::newQLabel("Intensity:", labelWidth),row,0);
    vlist->addWidget(leIntensity,row,1);
    vlist->addWidget(sIntensity,row++,2);
    vlist->addWidget(GuiFunct::newQLabel("Fixed Height:", labelWidth),row,0);
    vlist->addWidget(fheight,row++,1,1,2);
    vlist->addWidget(GuiFunct::newQLabel("Height type:", labelWidth),row,0);
    vlist->addWidget(hType,row++,1,1,2);
    vbox->addItem(vlist);
    
    
    // enbankment
    sEsize = new QSlider(Qt::Horizontal);
    sEsize->setMinimum(1);
    sEsize->setMaximum(3);
    sEsize->setValue(2);
    sEemb = new QSlider(Qt::Horizontal);
    sEemb->setMinimum(1);
    sEemb->setMaximum(10);
    sEemb->setValue(2);
    sEcut = new QSlider(Qt::Horizontal);
    sEcut->setMinimum(1);
    sEcut->setMaximum(10);
    sEcut->setValue(2);
    sEradius = new QSlider(Qt::Horizontal);
    sEradius->setMinimum(1);
    sEradius->setMaximum(100);
    sEradius->setValue(10);
    leEsize = GuiFunct::newQLineEdit(25,3);
    leEsize->setValidator(new QIntValidator(1, 3, this));
    leEemb = GuiFunct::newQLineEdit(25,3);
    leEemb->setValidator(new QIntValidator(1, 10, this));
    leEcut = GuiFunct::newQLineEdit(25,3);
    leEcut->setValidator(new QIntValidator(1, 10, this));
    leEradius = GuiFunct::newQLineEdit(25,3);
    leEradius->setValidator(new QIntValidator(1, 100, this));
    QLabel *label3 = new QLabel("Embankment settings:");
    label3->setStyleSheet("QLabel { color : #999999; }");
    label3->setContentsMargins(3,0,0,0);
    vbox->addWidget(label3);
    
    QGridLayout *vlist2 = new QGridLayout;
    vlist2->setSpacing(2);
    vlist2->setContentsMargins(3,0,1,0);
    row = 0;
    vlist2->addWidget(GuiFunct::newQLabel("Size:", labelWidth),row,0);
    vlist2->addWidget(leEsize,row,1);
    vlist2->addWidget(sEsize,row++,2);
    vlist2->addWidget(GuiFunct::newQLabel("Embankment:", labelWidth),row,0);
    vlist2->addWidget(leEemb,row,1);
    vlist2->addWidget(sEemb,row++,2);
    vlist2->addWidget(GuiFunct::newQLabel("Cutting:", labelWidth),row,0);
    vlist2->addWidget(leEcut,row,1);
    vlist2->addWidget(sEcut,row++,2);
    vlist2->addWidget(GuiFunct::newQLabel("Max Radius:", labelWidth),row,0);
    vlist2->addWidget(leEradius,row,1);
    vlist2->addWidget(sEradius,row++,2);
    vbox->addItem(vlist2);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    
    // signals
    QObject::connect(heightTool, SIGNAL(released()),
                      this, SLOT(heightToolEnabled()));
    
    QObject::connect(paintTool, SIGNAL(released()),
                      this, SLOT(paintColorToolEnabled()));
    
    QObject::connect(paintTool2, SIGNAL(released()),
                      this, SLOT(paintTexToolEnabled()));
    
    QObject::connect(pickTexTool, SIGNAL(released()),
                      this, SLOT(pickTexToolEnabled()));
    
    QObject::connect(setTexTool, SIGNAL(released()),
                      this, SLOT(setTexToolEnabled()));
    
    QObject::connect(colorw, SIGNAL(released()),
                      this, SLOT(chooseColorEnabled()));
    
    // brush
    QObject::connect(sSize, SIGNAL(valueChanged(int)),
                      this, SLOT(setBrushSize(int)));
    
    QObject::connect(sIntensity, SIGNAL(valueChanged(int)),
                      this, SLOT(setBrushAlpha(int)));

    QObject::connect(leSize, SIGNAL(textChanged(QString)),
                      this, SLOT(setBrushSize(QString)));
    
    QObject::connect(leIntensity, SIGNAL(textChanged(QString)),
                      this, SLOT(setBrushAlpha(QString)));
    
    // embarkment
    QObject::connect(sEsize, SIGNAL(valueChanged(int)),
                      this, SLOT(setEsize(int)));
    
    QObject::connect(leEsize, SIGNAL(textChanged(QString)),
                      this, SLOT(setEsize(QString)));    
    
    QObject::connect(sEemb, SIGNAL(valueChanged(int)),
                      this, SLOT(setEemb(int)));

    QObject::connect(leEemb, SIGNAL(textChanged(QString)),
                      this, SLOT(setEemb(QString)));

    QObject::connect(sEcut, SIGNAL(valueChanged(int)),
                      this, SLOT(setEcut(int)));

    QObject::connect(leEcut, SIGNAL(textChanged(QString)),
                      this, SLOT(setEcut(QString)));
    
    QObject::connect(sEradius, SIGNAL(valueChanged(int)),
                      this, SLOT(setEradius(int)));

    QObject::connect(leEradius, SIGNAL(textChanged(QString)),
                      this, SLOT(setEradius(QString)));
    
    QObject::connect(putTexTool, SIGNAL(released()),
                      this, SLOT(putTexToolEnabled()));
    
    QObject::connect(fheight, SIGNAL(textChanged(QString)),
                      this, SLOT(setFheight(QString)));
    
    this->setBrushSize(this->sSize->value());
    this->setBrushAlpha(this->sIntensity->value());
    this->setEsize(this->sEsize->value());
    this->setEemb(this->sEemb->value());
    this->setEcut(this->sEcut->value());
    this->setEradius(this->sEradius->value());
    this->fheight->setText("0");
}


TerrainTools::~TerrainTools() {
}

void TerrainTools::heightToolEnabled(){
    emit setPaintBrush(this->paintBrush);
    emit enableTool("heightTool");
}

void TerrainTools::paintColorToolEnabled(){
    this->paintBrush->useTexture = false;
    emit setPaintBrush(this->paintBrush);
    emit enableTool("paintTool");
}

void TerrainTools::paintTexToolEnabled(){
    this->paintBrush->useTexture = true;
    emit setPaintBrush(this->paintBrush);
    emit enableTool("paintTool");
}

void TerrainTools::chooseColorEnabled(){
    QColor color = QColorDialog::getColor(Qt::black, this, "Text Color",  QColorDialog::DontUseNativeDialog);
    this->paintBrush->color[0] = color.red();
    this->paintBrush->color[1] = color.green();
    this->paintBrush->color[2] = color.blue();
    colorw->setStyleSheet("background-color:"+color.name()+";");
    colorw->setText(color.name());
}

void TerrainTools::pickTexToolEnabled(){
    emit enableTool("pickTerrainTexTool");
}

void TerrainTools::putTexToolEnabled(){
    emit setPaintBrush(this->paintBrush);
    emit enableTool("putTerrainTexTool");
}

void TerrainTools::setTexToolEnabled(){
    QFileDialog *fd = new QFileDialog();
    fd->setDirectory("C:/ts/routes/CMK/Terrtex");
    //QTreeView *tree = fd->findChild <QTreeView*>();
    //tree->setRootIsDecorated(true);
    //tree->setItemsExpandable(true);
    //fd->setFileMode(QFileDialog::F);
    //fd->setOption(QFileDialog::ShowDirsOnly);
    //fd->setViewMode(QFileDialog::Detail);
    int result = fd->exec();
    QString filename;
    if (!result) return;
    
    filename = fd->selectedFiles()[0];
    qDebug()<<"texture file "<<filename;

    int tid = TexLib::addTex(filename);
    this->paintBrush->texId = tid;
    this->paintBrush->tex = TexLib::mtex[tid];
    //emit enableTool("setTexTool");
    
    emit setPaintBrush(this->paintBrush);
    
    QTimer::singleShot(200, this, SLOT(updateTexPrev()));
}

// brush

void TerrainTools::setBrushSize(QString val){
    qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sSize->setValue(ival);
    this->paintBrush->size = ival;
}

void TerrainTools::setBrushAlpha(QString val){
    qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sIntensity->setValue(ival);
    this->paintBrush->alpha = (float)ival/100;
}

void TerrainTools::setBrushSize(int val){
    qDebug() << "a";
    this->leSize->setText(QString::number(val,10));
    this->paintBrush->size = val;
}

void TerrainTools::setBrushAlpha(int val){
    qDebug() << "a";
    this->leIntensity->setText(QString::number(val,10));
    this->paintBrush->alpha = (float)val/100;
}

void TerrainTools::setFheight(QString val){
    qDebug() << "a";
    float ival = val.toFloat(0);
    this->paintBrush->hFixed = ival;
}

// embarkment

void TerrainTools::setEsize(int val){
    qDebug() << "a";
    this->leEsize->setText(QString::number(val,10));
    this->paintBrush->eSize = val;
}
void TerrainTools::setEsize(QString val){
    qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEsize->setValue(ival);
    this->paintBrush->eSize = (float)ival/100;
}
void TerrainTools::setEemb(int val){
    qDebug() << "a";
    this->leEemb->setText(QString::number(val,10));
    this->paintBrush->eEmb = val;
}
void TerrainTools::setEemb(QString val){
    qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEemb->setValue(ival);
    this->paintBrush->eEmb = (float)ival/100;
}
void TerrainTools::setEcut(int val){
    qDebug() << "a";
    this->leEcut->setText(QString::number(val,10));
    this->paintBrush->eCut = val;
}
void TerrainTools::setEcut(QString val){
    qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEcut->setValue(ival);
    this->paintBrush->eCut = (float)ival/100;
}
void TerrainTools::setEradius(int val){
    qDebug() << "a";
    this->leEradius->setText(QString::number(val,10));
    this->paintBrush->eRadius = val;
}
void TerrainTools::setEradius(QString val){
    qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEradius->setValue(ival);
    this->paintBrush->eRadius = (float)ival/100;
}

//

void TerrainTools::setBrushTextureId(int val){
    if(val < 0) return;
    if(TexLib::mtex[val] == NULL) return;
    this->paintBrush->texId = val;
    this->paintBrush->tex = TexLib::mtex[val];
    updateTexPrev();
}

void TerrainTools::updateTexPrev(){
    if(!this->paintBrush->tex->loaded)
        return;
    
    unsigned char * out = this->paintBrush->tex->getImageData(256,256);
    if(this->paintBrush->tex->bytesPerPixel == 3)
        texPreviewLabel->setPixmap(QPixmap::fromImage(QImage(out,256,256,QImage::Format_RGB888)));
    if(this->paintBrush->tex->bytesPerPixel == 4)
        texPreviewLabel->setPixmap(QPixmap::fromImage(QImage(out,256,256,QImage::Format_RGBA8888)));    
}