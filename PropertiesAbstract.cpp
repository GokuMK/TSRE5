/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesAbstract.h"
#include "WorldObj.h"
#include "TransformWorldObjDialog.h"
#include "GLMatrix.h"
#include "Undo.h"

PropertiesAbstract::PropertiesAbstract() : QWidget() {
    foreach (QObject *child, children()) {
        if (child->isWidgetType()) {
            child->installEventFilter(this);
        }
    }
}

PropertiesAbstract::~PropertiesAbstract() {
}

void PropertiesAbstract::showObj(WorldObj* obj){
}

void PropertiesAbstract::updateObj(WorldObj* obj){
}

bool PropertiesAbstract::support(WorldObj* obj){
    return false;
}

void PropertiesAbstract::copyFileNameEnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    //QString originalText = clipboard->text();
    clipboard->setText(this->fileName.text());
}

void PropertiesAbstract::resetRotEnabled(){
    if(worldObj == NULL)
        return;
    worldObj->setNewQdirection();
    worldObj->modified = true;
    worldObj->setMartix();
}
/*
void PropertiesAbstract::enableTool(){
    
}*/

void PropertiesAbstract::copyREnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->quat.text());
}

void PropertiesAbstract::copyPEnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->posX.text()+" "+this->posY.text()+" "+this->posZ.text());
}

void PropertiesAbstract::copyFEnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->flags.text());
}

void PropertiesAbstract::pasteFEnabled(){
    if(worldObj == NULL)
        return;
    QClipboard *clipboard = QApplication::clipboard();
    int val = clipboard->text().toUInt(NULL, 16);
    worldObj->set("staticflags", val);
    quat.setText(clipboard->text());
}


void PropertiesAbstract::copyPREnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->posX.text()+" "+this->posY.text()+" "+this->posZ.text()+" "+this->quat.text());
}

void PropertiesAbstract::rotYEnabled(){
    if(worldObj == NULL)
        return;
    
    Undo::SinglePushWorldObjData(worldObj);
    worldObj->rotate(0,M_PI/2,0);
    worldObj->modified = true;
    worldObj->setMartix();
}

void PropertiesAbstract::pasteREnabled(){
    if(worldObj == NULL)
        return;
    QClipboard *clipboard = QApplication::clipboard();
    QStringList args = clipboard->text().split(" ");
    if(args.length() != 4)
        return;
    float nq[4];
    nq[0] = args[0].toFloat();
    nq[1] = args[1].toFloat();
    nq[2] = args[2].toFloat();
    nq[3] = args[3].toFloat();
    
    Undo::SinglePushWorldObjData(worldObj);
    worldObj->setQdirection((float*)&nq);
    worldObj->modified = true;
    worldObj->setMartix();
    quat.setText(clipboard->text());
}

void PropertiesAbstract::pastePEnabled(){
    if(worldObj == NULL)
        return;
    QClipboard *clipboard = QApplication::clipboard();
    QStringList args = clipboard->text().split(" ");
    if(args.length() != 3)
        return;
    float nq[3];
    nq[0] = args[0].toFloat();
    nq[1] = args[1].toFloat();
    nq[2] = -args[2].toFloat();
    
    Undo::SinglePushWorldObjData(worldObj);
    worldObj->setPosition((float*)&nq);
    worldObj->modified = true;
    worldObj->setMartix();
    this->posX.setText(args[0]);
    this->posY.setText(args[1]);
    this->posZ.setText(args[2]);
}

void PropertiesAbstract::pastePREnabled(){
    if(worldObj == NULL)
        return;
    QClipboard *clipboard = QApplication::clipboard();
    QStringList args = clipboard->text().split(" ");
    if(args.length() != 7)
        return;
    float nq[3];
    float rq[4];
    nq[0] = args[0].toFloat();
    nq[1] = args[1].toFloat();
    nq[2] = -args[2].toFloat();
    rq[0] = args[3].toFloat();
    rq[1] = args[4].toFloat();
    rq[2] = -args[5].toFloat();
    rq[3] = args[6].toFloat();
    
    Undo::SinglePushWorldObjData(worldObj);
    worldObj->setPosition((float*)&nq);
    worldObj->setQdirection((float*)&rq);
    worldObj->modified = true;
    worldObj->setMartix();
    this->posX.setText(args[0]);
    this->posY.setText(args[1]);
    this->posZ.setText(args[2]);
    quat.setText(args[3]+" "+args[4]+" "+args[5]+" "+args[6]);
}

void PropertiesAbstract::transformEnabled(){
    if(worldObj == NULL)
        return;
    TransformWorldObjDialog transformWindow;
    transformWindow.exec();
    //qDebug() << waterWindow->changed;
    if(transformWindow.isOk){
        Undo::SinglePushWorldObjData(worldObj);
        if(transformWindow.x != 0 || transformWindow.y != 0 || + transformWindow.z != 0){
            float pos[3];
            pos[0] = transformWindow.x;
            pos[1] = transformWindow.y;
            pos[2] = -transformWindow.z;
            if(transformWindow.isUseObjRot){
                Vec3::transformQuat((float*)pos, (float*)pos, (float*)worldObj->qDirection);
            }
            worldObj->translate(pos[0], pos[1], pos[2]);
            worldObj->modified = true;
            worldObj->setMartix();
        }
        if(transformWindow.rx != 0 || transformWindow.ry != 0 || + transformWindow.rz != 0){
            worldObj->rotate(transformWindow.rx*M_PI/180.0, transformWindow.ry*M_PI/180.0, transformWindow.rz*M_PI/180.0);
            worldObj->modified = true;
            worldObj->setMartix();
        }
    }
}