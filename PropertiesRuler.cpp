/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesRuler.h"
#include "RulerObj.h"
#include "Undo.h"
#include "Game.h"
#include "ProceduralShape.h"
#include "ShapeTemplates.h"

PropertiesRuler::PropertiesRuler() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Ruler:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
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
    QLabel *label = new QLabel("Game Length:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);

    vlist->addRow("Meters: ",&this->lengthM);
    vbox->addItem(vlist);
    
    label = new QLabel("Geo Length:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);

    vlist->addRow("Meters: ",&this->lengthGM);
    vbox->addItem(vlist);
    
    
    label = new QLabel("Average Elevation:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(0);
    vlist->setContentsMargins(0,0,0,0);
    QDoubleValidator* doubleValidator = new QDoubleValidator(-10000, 10000, 6, this); 
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    QDoubleValidator* doubleValidator1 = new QDoubleValidator(-1000, 1000, 6, this); 
    doubleValidator1->setNotation(QDoubleValidator::StandardNotation);
    
    //‰
    vlist->addRow("Value: ",&this->elevType);
    elevType.addItem("Permille ‰");
    elevType.addItem("Percent %");
    elevType.addItem("1 in 'X' m");
    elevType.addItem("Angle º");
    elevType.setStyleSheet("combobox-popup: 0;");
    QObject::connect(&elevType, SIGNAL(currentTextChanged(QString)),
                      this, SLOT(elevTypeEdited(QString)));
    
    elevPromLabel.setText("‰");
    vlist->addRow(&elevPromLabel,&elevProm);
    //oneInXm
    elev1inXmLabel.setText("1 in 'x' m");
    vlist->addRow(&elev1inXmLabel,&elev1inXm);
    //º
    elevProgLabel.setText("º");
    vlist->addRow(&elevProgLabel,&elevProg);
    //%
    elevPropLabel.setText("%");
    vlist->addRow(&elevPropLabel,&elevProp);
    hideElevBoxes();
    elevType.setCurrentIndex(Game::DefaultElevationBox);
    showElevBox(elevType.currentText());
    vbox->addItem(vlist);
    
    
    label = new QLabel("Default Settings:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    checkboxTwoPoint.setText("Only Two-Point Ruler");
    checkboxTwoPoint.setChecked(false);
    vbox->addWidget(&checkboxTwoPoint);
    QObject::connect(&checkboxTwoPoint, SIGNAL(stateChanged(int)),
                      this, SLOT(checkboxTwoPointEdited(int)));
    checkboxDrawPoints.setText("Render points");
    checkboxDrawPoints.setChecked(false);
    vbox->addWidget(&checkboxDrawPoints);
    QObject::connect(&checkboxDrawPoints, SIGNAL(stateChanged(int)),
                      this, SLOT(checkboxDrawPointsEdited(int)));
    label = new QLabel("Experimental:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *button = new QPushButton("Create Road Paths");
    vbox->addWidget(button);
    QObject::connect(button, SIGNAL(released()),
                      this, SLOT(createRoadPathsEdited()));
    button = new QPushButton("Remove Road Paths");
    vbox->addWidget(button);
    QObject::connect(button, SIGNAL(released()),
                      this, SLOT(removeRoadPathsEdited()));
    
    label = new QLabel("Shape Template:");
    //label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&eTemplate);
    eTemplate.setStyleSheet("combobox-popup: 0;");
    eTemplate.addItem("DEFAULT");
    eTemplate.addItem("DISABLED");
    
    ProceduralShape::Load();
    if(ProceduralShape::ShapeTemplateFile != NULL){
        QMapIterator<QString, ShapeTemplate*> i(ProceduralShape::ShapeTemplateFile->templates);
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            eTemplate.addItem(i.value()->name);
        }
    }
    QObject::connect(&eTemplate, SIGNAL(currentTextChanged(QString)),
                      this, SLOT(eTemplateEdited(QString)));
    button = new QPushButton("Add Shape");
    vbox->addWidget(button);
    QObject::connect(button, SIGNAL(released()),
                      this, SLOT(addShapeEdited()));
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesRuler::~PropertiesRuler() {
}

void PropertiesRuler::elevTypeEdited(QString val){
    hideElevBoxes();
    showElevBox(val);
    ElevTypeName = val;
}

void PropertiesRuler::eTemplateEdited(QString val){
    if(worldObj == NULL){
        return;
    }
    Undo::SinglePushWorldObjData(worldObj);
    worldObj->setTemplate(val);
    Undo::StateEnd();
}

void PropertiesRuler::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = (WorldObj*)obj;
    RulerObj* robj = (RulerObj*)obj;
    this->uid.setText(QString::number(robj->UiD, 10));
    this->tX.setText(QString::number(robj->x, 10));
    this->tY.setText(QString::number(-robj->y, 10));
    lengthM.setText(QString::number(robj->getLength(), 'G', 4));
    lengthGM.setText(QString::number(robj->getGeoLength(), 'G', 4));
    
    QString templateName = worldObj->getTemplate();
    if(templateName.length() == 0)
        eTemplate.setCurrentText("DEFAULT");
    else
        eTemplate.setCurrentText(templateName);
    
    elevType.setCurrentText(ElevTypeName);
    float elev = sin(robj->getElevation())*1000;
    float oneInXm = 0.0;
    float prog = qRadiansToDegrees(qAtan(elev/1000.0));
    float prop = elev/10.0;
    oneInXm = 1000.0/elev;
    this->elevProm.setText(QString::number(elev));
    this->elevProg.setText(QString::number(prog));
    this->elevProp.setText(QString::number(prop));
    this->elev1inXm.setText(QString::number(oneInXm));
}

void PropertiesRuler::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    worldObj = (WorldObj*)obj;
    RulerObj* robj = (RulerObj*)obj;

    if(!lengthM.hasFocus())
        lengthM.setText(QString::number(robj->getLength(), 'G', 4));
    if(!lengthGM.hasFocus())
        lengthGM.setText(QString::number(robj->getGeoLength(), 'G', 4));
    
    float elev = sin(robj->getElevation())*1000;
    float oneInXm = 0.0;
    float prog = qRadiansToDegrees(qAtan(elev/1000.0));
    float prop = elev/10.0;
    oneInXm = 1000.0/elev;
    if(!this->elevProm.hasFocus() && !this->elev1inXm.hasFocus() && !this->elevProg.hasFocus() && !this->elevProp.hasFocus()){
        this->elevProm.setText(QString::number(elev));
        this->elevProg.setText(QString::number(prog));
        this->elevProp.setText(QString::number(prop));
        this->elev1inXm.setText(QString::number(oneInXm));
    }
}

void PropertiesRuler::checkboxTwoPointEdited(int val){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    if(val == 2){
        robj->TwoPointRuler = true;
    } else {
        robj->TwoPointRuler = false;
    }
}

void PropertiesRuler::checkboxDrawPointsEdited(int val){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    if(val == 2){
        robj->DrawPoints = true;
    } else {
        robj->DrawPoints = false;
    }
}

void PropertiesRuler::createRoadPathsEdited(){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    //Undo::SinglePushWorldObjData(worldObj);
    robj->createRoadPaths();
}

void PropertiesRuler::addShapeEdited(){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    //Undo::SinglePushWorldObjData(worldObj);
    robj->enableShape();
}

void PropertiesRuler::removeRoadPathsEdited(){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    //Undo::SinglePushWorldObjData(worldObj);
    robj->removeRoadPaths();
}
bool PropertiesRuler::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj != GameObj::worldobj)
        return false;
    if(((WorldObj*)obj)->type == "ruler")
        return true;
    return false;
}

void PropertiesRuler::showElevBox(QString val){
    if(val == "Permille ‰"){
        elevProm.show();
        elevPromLabel.show();
    }
    if(val == "Percent %"){
        elevProp.show();
        elevPropLabel.show();
    }
    if(val == "1 in 'X' m"){
        elev1inXm.show();
        elev1inXmLabel.show();
    }
    if(val == "Angle º"){
        elevProg.show();
        elevProgLabel.show();
    }    
}

void PropertiesRuler::hideElevBoxes(){
    elevProm.hide();
    elevProg.hide();
    elevProp.hide();
    elev1inXm.hide();
    elevPromLabel.hide();
    elevProgLabel.hide();
    elevPropLabel.hide();
    elev1inXmLabel.hide();
}