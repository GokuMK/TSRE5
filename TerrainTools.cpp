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

TerrainTools::TerrainTools(QString name)
    : QWidget(){
    
    this->paintBrush = new Brush();
    this->texPreview = new QPixmap(256,256);
    this->texPreview->fill(Qt::white);
    
    QPushButton *heightTool = new QPushButton("Paint HeightMap", this);
    QPushButton *paintTool = new QPushButton("Paint Texture", this);
    QPushButton *pickTexTool = new QPushButton("Pick Texture", this);
    QPushButton *setTexTool = new QPushButton("Load Texture", this);
    QPushButton *putTexTool = new QPushButton("Put Texture", this);
    colorw = new QPushButton("Choose color", this);
    colorw->setStyleSheet("background-color:black;");
    //colorw->show();
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(100);
    slider->setValue(10);
    QSlider *slider2 = new QSlider(Qt::Horizontal);
    slider2->setMinimum(0);
    slider2->setMaximum(100);
    slider2->setValue(100);
    
    texPreviewLabel = new QLabel("");
    texPreviewLabel->setContentsMargins(0,0,0,0);
    texPreviewLabel->setPixmap(*texPreview);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(heightTool);
    vbox->addWidget(paintTool);
    vbox->addWidget(pickTexTool);
    vbox->addWidget(setTexTool);
    vbox->addWidget(colorw);
    vbox->addWidget(texPreviewLabel);
    vbox->setAlignment(texPreviewLabel, Qt::AlignHCenter);
    vbox->addWidget(putTexTool);
    
    vbox->addWidget(slider);
    vbox->addWidget(slider2);
    
    vbox->setContentsMargins(0,1,5,1);
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(heightTool, SIGNAL(released()),
                      this, SLOT(heightToolEnabled()));
    
    QObject::connect(paintTool, SIGNAL(released()),
                      this, SLOT(paintToolEnabled()));
    
    QObject::connect(pickTexTool, SIGNAL(released()),
                      this, SLOT(pickTexToolEnabled()));
    
    QObject::connect(setTexTool, SIGNAL(released()),
                      this, SLOT(setTexToolEnabled()));
    
    QObject::connect(colorw, SIGNAL(released()),
                      this, SLOT(chooseColorEnabled()));
    
    QObject::connect(slider, SIGNAL(valueChanged(int)),
                      this, SLOT(setBrushSize(int)));
    
    QObject::connect(slider2, SIGNAL(valueChanged(int)),
                      this, SLOT(setBrushAlpha(int)));
    
    QObject::connect(putTexTool, SIGNAL(released()),
                      this, SLOT(putTexToolEnabled()));
}


TerrainTools::~TerrainTools() {
}

void TerrainTools::heightToolEnabled(){
    emit setPaintBrush(this->paintBrush);
    emit enableTool("heightTool");
}

void TerrainTools::paintToolEnabled(){
    emit setPaintBrush(this->paintBrush);
    emit enableTool("paintTool");
}

void TerrainTools::chooseColorEnabled(){
    QColor color = QColorDialog::getColor(Qt::black, this, "Text Color",  QColorDialog::DontUseNativeDialog);
    this->paintBrush->color[0] = color.red();
    this->paintBrush->color[1] = color.green();
    this->paintBrush->color[2] = color.blue();
    colorw->setStyleSheet("background-color:"+color.name()+";");
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

void TerrainTools::setBrushSize(int val){
    qDebug() << "a";
    this->paintBrush->size = val;
}

void TerrainTools::setBrushAlpha(int val){
    qDebug() << "a";
    this->paintBrush->alpha = (float)val/100;
}

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