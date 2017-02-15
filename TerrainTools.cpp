/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TerrainTools.h"
#include "TexLib.h"
#include "Brush.h"
#include "Texture.h"
#include "GuiFunct.h"
#include "TransferObj.h"
#include "ClickableLabel.h"
#include "Game.h"

TerrainTools::TerrainTools(QString name)
    : QWidget(){
    
    int row = 0;
    paintBrush = new Brush();
    
    texPreview = new QPixmap(192,192);
    defaultTexPreview = new QPixmap(64,64);
    defaultTexPreview->fill(Qt::transparent);
    texPreview->fill(Qt::gray);
    texPreviewLabel = new ClickableLabel("");
    texPreviewLabel->setContentsMargins(0,0,0,0);
    texPreviewLabel->setPixmap(*texPreview);
    for(int i = 0; i < 7; i++){
        texPreviewLabels.push_back(new ClickableLabel(""));
        texPreviewLabels.back()->setContentsMargins(0,0,0,0);
        texPreviewLabels.back()->setPixmap(*defaultTexPreview);
        texPreviewSignals.setMapping(texPreviewLabels.back(), i);
        connect(texPreviewLabels.back(), SIGNAL(clicked()), &texPreviewSignals, SLOT(map()));
    }
    texPreviewSignals.setMapping(texPreviewLabel, 7);
    connect(texPreviewLabel, SIGNAL(clicked()), &texPreviewSignals, SLOT(map()));
    connect(&texPreviewSignals, SIGNAL(mapped(int)), this, SLOT(texPreviewEnabled(int)));

    buttonTools["heightTool"] = new QPushButton("HeightMap +", this);
    buttonTools["pickTerrainTexTool"] = new QPushButton("Pick", this);
    buttonTools["putTerrainTexTool"] = new QPushButton("Put", this);
    buttonTools["lockTexTool"] = new QPushButton("Lock", this);
    buttonTools["waterTerrTool"] = new QPushButton("Show/H Water", this);
    buttonTools["drawTerrTool"] = new QPushButton("Show/H Tile", this);
    buttonTools["gapsTerrainTool"] = new QPushButton("Gaps", this);
    buttonTools["waterHeightTileTool"] = new QPushButton("Water level", this);
    buttonTools["fixedTileTool"] = new QPushButton("Fixed Height", this);
    buttonTools["paintToolColor"] = new QPushButton("Color", this);
    buttonTools["paintToolTexture"] = new QPushButton("Texture", this);
    QMapIterator<QString, QPushButton*> i(buttonTools);
    while (i.hasNext()) {
        i.next();
        i.value()->setCheckable(true);
    }
    
    QPushButton *loadTerrainTexTool = new QPushButton("Load...", this);
    
    QGridLayout *vlist3 = new QGridLayout;
    vlist3->setSpacing(2);
    vlist3->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist3->addWidget(buttonTools["heightTool"],row,0);
    vlist3->addWidget(buttonTools["fixedTileTool"],row,1);
    vlist3->addWidget(buttonTools["waterHeightTileTool"],row,2);
    //vlist3->addWidget(mapTileShowTool,row,0);
    //vlist3->addWidget(mapTileLoadTool,row,1);
    //vlist3->addWidget(heightTileLoadTool,row++,2);
    
    QGridLayout *vlist4 = new QGridLayout;
    vlist4->setSpacing(2);
    vlist4->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist4->addWidget(buttonTools["waterTerrTool"],row,0);
    vlist4->addWidget(buttonTools["drawTerrTool"],row,1);
    vlist4->addWidget(buttonTools["gapsTerrainTool"],row++,2);
    
    QGridLayout *vlist0 = new QGridLayout;
    vlist0->setSpacing(2);
    vlist0->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist0->addWidget(buttonTools["paintToolColor"],row,0);
    vlist0->addWidget(buttonTools["paintToolTexture"],row,1);
    vlist0->addWidget(buttonTools["lockTexTool"],row++,2);
    
    QGridLayout *vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist1->addWidget(buttonTools["pickTerrainTexTool"],row,0);
    vlist1->addWidget(buttonTools["putTerrainTexTool"],row,1);
    vlist1->addWidget(loadTerrainTexTool,row,2);
    
    colorw = new QPushButton("#000000", this);
    colorw->setStyleSheet("background-color:black;");

    QLabel *label0;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    label0 = new QLabel("World Tile:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addItem(vlist3);
    label0 = new QLabel("Tile:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addItem(vlist4);
    label0 = new QLabel("Paint:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addItem(vlist0);
    label0 = new QLabel("Texture:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addItem(vlist1);

    vlist1 = new QGridLayout;
    vlist1->setSpacing(0);
    vlist1->setContentsMargins(0,0,0,0);    
    vlist1->addWidget(texPreviewLabel, 0, 0, 3, 3);
    vlist1->addWidget(texPreviewLabels[0], 0, 3);
    vlist1->addWidget(texPreviewLabels[1], 1, 3);
    vlist1->addWidget(texPreviewLabels[2], 2, 3);
    vlist1->addWidget(texPreviewLabels[3], 3, 2);
    vlist1->addWidget(texPreviewLabels[4], 3, 1);
    vlist1->addWidget(texPreviewLabels[5], 3, 0);
    vlist1->addWidget(texPreviewLabels[6], 3, 3);
    vbox->addItem(vlist1);
    vbox->setAlignment(vlist1, Qt::AlignHCenter);
    //vbox->addWidget(texPreviewLabel);
    //vbox->setAlignment(texPreviewLabel, Qt::AlignHCenter);
    QLabel *label2 = new QLabel("Brush settings:");
    label2->setContentsMargins(3,0,0,0);
    label2->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label2);
    

    int labelWidth = 70;
    
    // brush
    sSize = new QSlider(Qt::Horizontal);
    sSize->setMinimum(1);
    sSize->setMaximum(100);
    sSize->setValue(paintBrush->size);
    sIntensity = new QSlider(Qt::Horizontal);
    sIntensity->setMinimum(1);
    sIntensity->setMaximum(100);
    sIntensity->setValue(paintBrush->alpha*100);
    hType = new QComboBox;
    hType->setStyleSheet("combobox-popup: 0;");
    hType->addItem("Add - simple");
    hType->addItem("Add - if inside 'Size' radius");
    hType->addItem("Fixed Height");
    hType->addItem("Flatten");
    hType->setCurrentIndex(paintBrush->hType);
    fheight = new QLineEdit();
    QDoubleValidator* doubleValidator = new QDoubleValidator(-5000, 5000, 2, this); 
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    fheight->setValidator(doubleValidator);
    
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
    sEsize->setValue(paintBrush->eSize);
    sEemb = new QSlider(Qt::Horizontal);
    sEemb->setMinimum(1);
    sEemb->setMaximum(10);
    sEemb->setValue(paintBrush->eEmb);
    sEcut = new QSlider(Qt::Horizontal);
    sEcut->setMinimum(1);
    sEcut->setMaximum(10);
    sEcut->setValue(paintBrush->eCut);
    sEradius = new QSlider(Qt::Horizontal);
    sEradius->setMinimum(1);
    sEradius->setMaximum(100);
    sEradius->setValue(paintBrush->eRadius);
    leEsize = GuiFunct::newQLineEdit(25,3);
    leEsize->setValidator(new QIntValidator(1, 3, this));
    leEemb = GuiFunct::newQLineEdit(25,3);
    leEemb->setValidator(new QIntValidator(1, 10, this));
    leEcut = GuiFunct::newQLineEdit(25,3);
    leEcut->setValidator(new QIntValidator(1, 10, this));
    leEradius = GuiFunct::newQLineEdit(25,3);
    leEradius->setValidator(new QIntValidator(1, 100, this));
    QLabel *label3 = new QLabel("Embankment settings:");
    label3->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
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
    QObject::connect(buttonTools["heightTool"], SIGNAL(toggled(bool)),
                      this, SLOT(heightToolEnabled(bool)));
    
    QObject::connect(buttonTools["paintToolColor"], SIGNAL(toggled(bool)),
                      this, SLOT(paintColorToolEnabled(bool)));
    
    QObject::connect(buttonTools["paintToolTexture"], SIGNAL(toggled(bool)),
                      this, SLOT(paintTexToolEnabled(bool)));
    
    QObject::connect(buttonTools["lockTexTool"], SIGNAL(toggled(bool)),
                      this, SLOT(lockTexToolEnabled(bool)));
    
    QObject::connect(buttonTools["pickTerrainTexTool"], SIGNAL(toggled(bool)),
                      this, SLOT(pickTexToolEnabled(bool)));
    
    QObject::connect(buttonTools["waterTerrTool"], SIGNAL(toggled(bool)),
                      this, SLOT(waterTerrToolEnabled(bool)));
    
    QObject::connect(buttonTools["waterHeightTileTool"], SIGNAL(toggled(bool)),
                      this, SLOT(waterHeightTileToolEnabled(bool)));
    
    QObject::connect(buttonTools["fixedTileTool"], SIGNAL(toggled(bool)),
                      this, SLOT(fixedTileToolEnabled(bool)));
    
    QObject::connect(buttonTools["gapsTerrainTool"], SIGNAL(toggled(bool)),
                      this, SLOT(gapsTerrToolEnabled(bool)));
    
    QObject::connect(buttonTools["drawTerrTool"], SIGNAL(toggled(bool)),
                      this, SLOT(drawTerrToolEnabled(bool)));
    
    QObject::connect(buttonTools["putTerrainTexTool"], SIGNAL(toggled(bool)),
                      this, SLOT(putTexToolEnabled(bool)));
    
    QObject::connect(loadTerrainTexTool, SIGNAL(released()),
                      this, SLOT(setTexToolEnabled()));
    
    QObject::connect(colorw, SIGNAL(released()),
                      this, SLOT(chooseColorEnabled()));
    
    // brush
    QObject::connect(sSize, SIGNAL(valueChanged(int)),
                      this, SLOT(setBrushSize(int)));
    
    QObject::connect(sIntensity, SIGNAL(valueChanged(int)),
                      this, SLOT(setBrushAlpha(int)));

    QObject::connect(leSize, SIGNAL(textEdited(QString)),
                      this, SLOT(setBrushSize(QString)));
    
    QObject::connect(leIntensity, SIGNAL(textEdited(QString)),
                      this, SLOT(setBrushAlpha(QString)));
    
    // embarkment
    QObject::connect(sEsize, SIGNAL(valueChanged(int)),
                      this, SLOT(setEsize(int)));
    
    QObject::connect(leEsize, SIGNAL(textEdited(QString)),
                      this, SLOT(setEsize(QString)));    
    
    QObject::connect(sEemb, SIGNAL(valueChanged(int)),
                      this, SLOT(setEemb(int)));

    QObject::connect(leEemb, SIGNAL(textEdited(QString)),
                      this, SLOT(setEemb(QString)));

    QObject::connect(sEcut, SIGNAL(valueChanged(int)),
                      this, SLOT(setEcut(int)));

    QObject::connect(leEcut, SIGNAL(textEdited(QString)),
                      this, SLOT(setEcut(QString)));
    
    QObject::connect(sEradius, SIGNAL(valueChanged(int)),
                      this, SLOT(setEradius(int)));

    QObject::connect(leEradius, SIGNAL(textEdited(QString)),
                      this, SLOT(setEradius(QString)));
    
    QObject::connect(fheight, SIGNAL(textEdited(QString)),
                      this, SLOT(setFheight(QString)));
    
    QObject::connect(hType, SIGNAL(currentIndexChanged(int)),
                      this, SLOT(setHtype(int)));
    
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

void TerrainTools::heightToolEnabled(bool val){
    if(val){
        emit setPaintBrush(this->paintBrush);
        emit enableTool("heightTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::paintColorToolEnabled(bool val){
     if(val){
        this->paintBrush->useTexture = false;
        emit setPaintBrush(this->paintBrush);
        emit enableTool("paintToolColor");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::gapsTerrToolEnabled(bool val){
    if(val){
        emit enableTool("gapsTerrainTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::paintTexToolEnabled(bool val){
    if(val){
        this->paintBrush->useTexture = true;
        emit setPaintBrush(this->paintBrush);
        emit enableTool("paintToolTexture");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::chooseColorEnabled(){
    QColor aColor(paintBrush->color[0], paintBrush->color[1], paintBrush->color[2]);
    QColor color = QColorDialog::getColor(aColor, this, "Text Color",  QColorDialog::DontUseNativeDialog);
    paintBrush->color[0] = color.red();
    paintBrush->color[1] = color.green();
    paintBrush->color[2] = color.blue();
    colorw->setStyleSheet("background-color:"+color.name()+";");
    colorw->setText(color.name());
}

void TerrainTools::pickTexToolEnabled(bool val){
    if(val){
        emit enableTool("pickTerrainTexTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::lockTexToolEnabled(bool val){
    if(val){
        emit enableTool("lockTexTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::waterTerrToolEnabled(bool val){
    if(val){
        emit enableTool("waterTerrTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::drawTerrToolEnabled(bool val){
    if(val){
        emit enableTool("drawTerrTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::waterHeightTileToolEnabled(bool val){
    if(val){
        emit enableTool("waterHeightTileTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::putTexToolEnabled(bool val){
    if(val){
        emit setPaintBrush(this->paintBrush);
        emit enableTool("putTerrainTexTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::fixedTileToolEnabled(bool val){
    if(val){
        emit setPaintBrush(this->paintBrush);
        emit enableTool("fixedTileTool");
    } else {
        emit enableTool("");
    }
}

void TerrainTools::setTexToolEnabled(){
    QFileDialog fd;
    QString path = Game::root+"/routes/"+Game::route+"/terrtex";
    path.replace("//", "/");
    fd.setDirectory(path);
    fd.setFileMode(QFileDialog::ExistingFiles);
    //QTreeView *tree = fd->findChild <QTreeView*>();
    //tree->setRootIsDecorated(true);
    //tree->setItemsExpandable(true);
    //fd->setFileMode(QFileDialog::F);
    //fd->setOption(QFileDialog::ShowDirsOnly);
    //fd->setViewMode(QFileDialog::Detail);
    int result = fd.exec();
    QString filename;
    if (!result) return;
    
    for(int i = 0; i < fd.selectedFiles().length(); i++){
        filename = fd.selectedFiles()[i];
        TexLib::addTex(filename);
    }
    
    QTime cTime = QTime::currentTime().addMSecs(300);  
    while (QTime::currentTime() < cTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    
    for(int i = 0; i < fd.selectedFiles().length(); i++){
        filename = fd.selectedFiles()[i];
        qDebug()<<"texture file "<<filename;

        int tid = TexLib::addTex(filename);
        this->paintBrush->texId = tid;
        this->paintBrush->tex = TexLib::mtex[tid];

        texLastItems.push_back(qMakePair(this->paintBrush->texId, this->paintBrush->tex));
        if(texLastItems.size() > 7){
            texLastItems.removeFirst();
        }
        updateTexPrev();
    //emit enableTool("setTexTool");
    }
    emit setPaintBrush(this->paintBrush);
    
    //QTimer::singleShot(200, this, SLOT(updateTexPrev()));
}

// brush

void TerrainTools::setBrushSize(QString val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sSize->setValue(ival);
    this->paintBrush->size = ival;
}

void TerrainTools::setBrushAlpha(QString val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sIntensity->setValue(ival);
    this->paintBrush->alpha = (float)ival/100;
}

void TerrainTools::setBrushSize(int val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    this->leSize->setText(QString::number(val,10));
    this->paintBrush->size = val;
}

void TerrainTools::setBrushAlpha(int val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    this->leIntensity->setText(QString::number(val,10));
    this->paintBrush->alpha = (float)val/100;
}

void TerrainTools::setFheight(QString val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    float ival = val.toFloat(0);
    this->paintBrush->hFixed = ival;
}

void TerrainTools::setHtype(int val){
    emit setPaintBrush(this->paintBrush);
    if(val < 0) return;
    this->paintBrush->hType = val;
}

// embarkment

void TerrainTools::setEsize(int val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    this->leEsize->setText(QString::number(val,10));
    this->paintBrush->eSize = val;
}
void TerrainTools::setEsize(QString val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEsize->setValue(ival);
    this->paintBrush->eSize = (float)ival/100;
}
void TerrainTools::setEemb(int val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    this->leEemb->setText(QString::number(val,10));
    this->paintBrush->eEmb = val;
}
void TerrainTools::setEemb(QString val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEemb->setValue(ival);
    this->paintBrush->eEmb = (float)ival/100;
}
void TerrainTools::setEcut(int val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    this->leEcut->setText(QString::number(val,10));
    this->paintBrush->eCut = val;
}
void TerrainTools::setEcut(QString val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEcut->setValue(ival);
    this->paintBrush->eCut = (float)ival/100;
}
void TerrainTools::setEradius(int val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    this->leEradius->setText(QString::number(val,10));
    this->paintBrush->eRadius = val;
}
void TerrainTools::setEradius(QString val){
    emit setPaintBrush(this->paintBrush);
    //qDebug() << "a";
    int ival = val.toInt(0, 10);
    this->sEradius->setValue(ival);
    this->paintBrush->eRadius = (float)ival/100;
}

//

void TerrainTools::setBrushTextureId(int val){
    emit setPaintBrush(this->paintBrush);
    if(val < 0) return;
    if(TexLib::mtex[val] == NULL) return;
    this->paintBrush->texId = val;
    this->paintBrush->tex = TexLib::mtex[val];
    
    texLastItems.push_back(qMakePair(this->paintBrush->texId, this->paintBrush->tex));
    if(texLastItems.size() > 7){
        texLastItems.removeFirst();
    }
    updateTexPrev();
}

void TerrainTools::updateTexPrev(){
    if(!this->paintBrush->tex->loaded)
        return;

    ClickableLabel *tlabel;
    unsigned char * out;
    int idx;
    int res;
    for(int i = 1; i < 8; i++){
        idx = texLastItems.size() - i;
        if(idx < 0)
            continue;
        if(i == 1){
            tlabel = texPreviewLabel;
            res = 192;
            out = this->paintBrush->tex->getImageData(res,res);
            
        } else {
            tlabel = texPreviewLabels[i-2];
            res = 64;
            out = texLastItems[idx].second->getImageData(res,res);
        }
        if(texLastItems[idx].second->bytesPerPixel == 3)
            tlabel->setPixmap(QPixmap::fromImage(QImage(out,res,res,QImage::Format_RGB888)));
        if(texLastItems[idx].second->bytesPerPixel == 4)
            tlabel->setPixmap(QPixmap::fromImage(QImage(out,res,res,QImage::Format_RGBA8888)));   
    }
}

void TerrainTools::texPreviewEnabled(int val){
    qDebug() << val;
    int idx = 5 - val;//texLastItems.size() - val - 1;
    if(val > texLastItems.size() - 1) return;
    if(idx < 0) return;
    this->paintBrush->tex = texLastItems[idx].second;
    this->paintBrush->texId = texLastItems[idx].first;
    updateTexPrev();
}

void TerrainTools::msg(QString text, QString val){
    if(text == "toolEnabled"){
        QMapIterator<QString, QPushButton*> i(buttonTools);
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(true);
            i.value()->setChecked(false);
        }
        if(buttonTools[val] != NULL)
            buttonTools[val]->setChecked(true);
        i.toFront();
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(false);
        }
    } else if(text == "brushDirection"){
        QString t = buttonTools["heightTool"]->text().left(buttonTools["heightTool"]->text().length() - 1);
        buttonTools["heightTool"]->setText(t+val);
    }

}