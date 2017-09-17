/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Consist.h"
#include "Eng.h"
#include "EngLib.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "ShapeLib.h"
#include "SFile.h"
#include "Game.h"
#include "GLUU.h"
#include <QDebug>
#include <QFile>
#include "GLMatrix.h"
#include "TextObj.h"
#include "TDB.h"

std::unordered_map<int, TextObj*> Consist::txtNumbers;
int Consist::lastTxtNumbersColor = 0;
TextObj * Consist::txtEngineE = NULL;
TextObj * Consist::txtEngineD = NULL;
TextObj * Consist::txtEngineS = NULL;
TextObj * Consist::txtEngineF = NULL;
TextObj * Consist::txtEngineW = NULL;
TextObj * Consist::txtEngineT = NULL;

Consist::Consist() {
    typeObj = this->consistobj;
    path = Game::root + "/trains/consists/";
    loaded = 1;
    serial = 1;
    durability = 1;
    initPos();
}

Consist::Consist(Consist * con, bool fullCopy) {
    typeObj = con->consistobj;
    path = con->path;
    loaded = 1;
    serial = 1;
    durability = 1;
    displayName = con->displayName;
    showName = displayName;
    for(int i = 0; i < con->engItems.size(); i++){
        engItems.push_back(EngItem());
        engItems.back().type = con->engItems[i].type;
        engItems.back().ename = con->engItems[i].ename;
        engItems.back().epath = con->engItems[i].epath;
        engItems.back().eng = con->engItems[i].eng;
        engItems.back().flip = con->engItems[i].flip;
    }
    
    if(fullCopy){
        lastTxtColor = con->lastTxtColor;
        name = con->name;
        pathid = con->pathid;
        conName = con->conName;
        showName = con->showName;
        serial = con->serial;
        maxVelocity[0] = con->maxVelocity[0];
        maxVelocity[1] = con->maxVelocity[1];
        nextWagonUID = con->nextWagonUID;
        durability = con->durability;
        conLength = con->conLength;
        mass = con->mass;
        emass = con->emass;
        loaded = con->loaded;
        isOnTrack = false;
        kierunek = con->kierunek;
        ref = con->ref;
        posInit = false;
        selectedIdx = -1;
        textColor[0] = con->textColor[0];
        textColor[1] = con->textColor[1];
        textColor[2] = con->textColor[2];
        newConsist = con->newConsist;
        modified = con->modified;
        defaultValue = con->defaultValue;
        maxVelocityFixed = con->maxVelocityFixed;
    }
    
    initPos();
}

Consist::~Consist() {
}

Consist::Consist(QString p, QString n) {
    typeObj = this->consistobj;
    pathid = p + "/" + n;
    pathid.replace("//", "/");
    path = p;
    name = n;
    loaded = -1;
    kierunek = false;
    load();
}

Consist::Consist(QString src, QString p, QString n) {
    typeObj = this->consistobj;
    pathid = src;
    path = p;
    name = n;
    loaded = -1;
    kierunek = false;
    load();
}

void Consist::load(){
    int i;
    QString sh;
    pathid.replace("//","/");
    qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    data->off = 0;
    file->close();
    ParserX::NextLine(data);
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if(sh == "train"){
            if(!load(data)){
                delete data;
                return;
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#Consist - undefined token " << sh;
        ParserX::SkipToken(data);
    }
    
    delete data;
    loaded = 1;
    initPos();
    return;
}

bool Consist::load(FileBuffer* data){
    QString sh;
    bool ok = false;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if(sh == "traincfg"){
            ok = true;
            conName = ParserX::GetString(data).trimmed();
            showName = conName;
            //qDebug() << conName;
            EngItem* eit;

            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //qDebug() << sh;
                if (sh == ("name")) {
                    displayName = ParserX::GetString(data).trimmed();
                    showName = displayName;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("serial")) {
                    serial = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("default")) {
                    defaultValue = true;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("maxvelocity")) {
                    maxVelocity[0] = ParserX::GetNumber(data);
                    maxVelocity[1] = ParserX::GetNumber(data);
                    //qDebug() << "wymiary taboru: " << sizex << " " << sizey << " " << sizez;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("nextwagonuid")) {
                    nextWagonUID = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("durability")) {
                    durability = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("engine")) {
                    engItems.push_back(EngItem());
                    engItems.back().type = 1;
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("flip")) {
                            engItems.back().flip = true;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("uid")) {
                            engItems.back().uid = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("enginedata")) {
                            engItems.back().ename = ParserX::GetString(data);
                            engItems.back().epath = ParserX::GetString(data);
                            engItems.back().eng = Game::currentEngLib->addEng(Game::root + "/TRAINS/TRAINSET/" + engItems.back().epath, engItems.back().ename + ".eng");
                            ParserX::SkipToken(data);
                            continue;
                        }
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("wagon")) {
                    engItems.push_back(EngItem());
                    engItems.back().type = 0;
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("flip")) {
                            engItems.back().flip = true;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("uid")) {
                            engItems.back().uid = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("wagondata")) {
                            engItems.back().ename = ParserX::GetString(data);
                            engItems.back().epath = ParserX::GetString(data);
                            engItems.back().eng = Game::currentEngLib->addEng(Game::root + "/TRAINS/TRAINSET/" + engItems.back().epath, engItems.back().ename + ".wag");
                            ParserX::SkipToken(data);
                            continue;
                        }
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#train - undefined token " << sh;
        ParserX::SkipToken(data);
    }
    return ok;
}

void Consist::refreshEngData(){
    QString fext;
    QString wext = ".wag";
    QString eext = ".eng";
    
    for(int i = 0; i < engItems.size(); i++){
        if(engItems[i].type == 0)
            fext = wext;
        else
            fext = eext;
        engItems[i].eng = Game::currentEngLib->addEng(Game::root + "/TRAINS/TRAINSET/" + engItems[i].epath, engItems[i].ename + fext);
    }
    initPos();
}

void Consist::initPos(){
    float length = 0;
    conLength = 0;
    mass = 0;
    emass = 0;
    if(engItems.size() == 0) {
        posInit = true;
        return;
    }
    //length += EngLib::eng[engItems[0].eng]->sizez / 2.0;
    float size = 0;
    float tmaxspeed = 0;
    //float tengmass = 0;
    float maxForce = 0;
    for(int i = 0; i < engItems.size(); i++){
        engItems[i].conLength = conLength;
        if(engItems[i].eng < 0) continue;
        size = Game::currentEngLib->eng[engItems[i].eng]->getFullWidth();
        length += size / 2.0;
        engItems[i].pos = length;
        length += size / 2.0;
        conLength += size;
        mass += Game::currentEngLib->eng[engItems[i].eng]->mass;
        if(Game::currentEngLib->eng[engItems[i].eng]->wagonTypeId >= 4){
            if(Game::currentEngLib->eng[engItems[i].eng]->maxSpeed > tmaxspeed)
                tmaxspeed = Game::currentEngLib->eng[engItems[i].eng]->maxSpeed;
            emass += Game::currentEngLib->eng[engItems[i].eng]->mass;
            maxForce += Game::currentEngLib->eng[engItems[i].eng]->maxForce;
        }
    }


    if(!maxVelocityFixed){
        //qDebug() << "mv "<< this->maxVelocity[0] <<" "<< this->maxVelocity[1];
        this->maxVelocity[0] = tmaxspeed / 3.6;
        //this->maxVelocity[1] = emass / mass;
        //qDebug() << maxForce*0.001 << " "<< mass;
        this->maxVelocity[1] = 0.8*maxForce*0.001 / mass;
        if(this->maxVelocity[1] > 1) this->maxVelocity[1] = 1;
        if(this->maxVelocity[1] < 0.001) this->maxVelocity[1] = 0.001;
    }
    //qDebug() << "mv "<< this->maxVelocity[0] <<" "<< this->maxVelocity[1];
    posInit = true;
}

bool Consist::isBroken(){
    for(int i = 0; i < engItems.size(); i++){
        if(engItems[i].eng < 0) continue;
        if(Game::currentEngLib->eng[engItems[i].eng] == NULL) continue;
        if(Game::currentEngLib->eng[engItems[i].eng]->loaded != 1) return true;
    }
    return false;
}

bool Consist::isUnSaved(){
    if(isNewConsist()) return true;
    return modified;
}

void Consist::setModified(bool val){
    modified = val;
}

bool Consist::select(int idx){
    selectedIdx = idx;
    return true;
}

bool Consist::unselect(){
    selectedIdx = -1;
    return false;
}

void Consist::deteleSelected(){
    if(selectedIdx < 0)
        return;
    if(selectedIdx >= engItems.size())
        return;
    engItems.erase(engItems.begin() + selectedIdx);
    initPos();
    modified = true;
}

void Consist::appendEngItem(int id, int pos, bool flip){
    Eng * eng = Game::currentEngLib->eng[id];
    if(eng == NULL) return;
    std::vector<int>::iterator it;
    EngItem* newE;
    if(pos == 0){
        engItems.emplace(engItems.begin());
        newE = &engItems[0];
        selectedIdx++;
    } else if(pos == 2){
        engItems.emplace_back();
        newE = &engItems[engItems.size()-1];
    } else if(pos == 1){
        if(selectedIdx > engItems.size() - 1 )
            selectedIdx = engItems.size() - 1;
        if(engItems.size() == 0 )
            selectedIdx = -1;
        if(selectedIdx < -1 )
            selectedIdx = -1;
        qDebug() << "selectedIdx "<< selectedIdx;
        engItems.emplace(engItems.begin()+selectedIdx+1);
        newE = &engItems[selectedIdx+1];
    } else {
        return;
    }

    newE->type = eng->wagonTypeId / 4;
    newE->eng = id;
    newE->ename = eng->name.section(".", 0, -2);
    newE->epath = eng->path.split("/").last();
    newE->uid = this->nextWagonUID++;
    newE->flip = flip;

    initPos();
    modified = true;
}

void Consist::flipSelected(){
    if(selectedIdx < 0)
        return;
    if(selectedIdx >= engItems.size())
        return;
    engItems[selectedIdx].flip = !engItems[selectedIdx].flip;
    modified = true;
}

void Consist::reverse(){
    for(int i = 0; i < engItems.size() / 2; i++){
        engItems[i].flip = !engItems[i].flip;
        engItems[engItems.size()-1-i].flip = !engItems[engItems.size()-1-i].flip;
        std::swap(engItems[i],engItems[engItems.size()-1-i]);
    }
    for(int i = 0; i < engItems.size(); i++){
        engItems[i].uid = i;
    }
    initPos();
    modified = true;
}

void Consist::moveLeftSelected(){
    if(selectedIdx < 1)
        return;
    if(selectedIdx >= engItems.size())
        return;
    engItems[selectedIdx-1].uid++;
    engItems[selectedIdx].uid--;
    std::swap(engItems[selectedIdx-1],engItems[selectedIdx]);
    selectedIdx--;
    initPos();
    modified = true;
}

void Consist::moveRightSelected(){
    if(selectedIdx < 0)
        return;
    if(selectedIdx + 2 > engItems.size())
        return;
    engItems[selectedIdx].uid++;
    engItems[selectedIdx+1].uid--;
    std::swap(engItems[selectedIdx],engItems[selectedIdx+1]);
    selectedIdx++;
    initPos();
    modified = true;
}

void  Consist::setFileName(QString n){
    this->conName = n;
    this->name = n+".con";
    modified = true;
}
void  Consist::setDisplayName(QString n){
    this->displayName = n;
    this->showName = n;
    modified = true;
}

void Consist::render(int selectionColor, bool renderText) {
    render(0, 0, selectionColor, renderText);
}

void Consist::setTextColor(float* bgColor) {
    //qDebug() << "new color";
    if(bgColor[0]< 30.0/255.0 && bgColor[1] < 30.0/255.0 && bgColor[2] < 30.0/255.0){
        textColor[0] = 255;
        textColor[1] = 255;
        textColor[2] = 0;
    } else {
        textColor[0] = fabs(bgColor[0] - 1.0)*255.0;
        textColor[1] = fabs(bgColor[1] - 1.0)*255.0;
        textColor[2] = fabs(bgColor[2] - 1.0)*255.0;
    }
    int colorHash = textColor[0]*255 + textColor[1]*255 + textColor[2]*255;
    
    if(colorHash != lastTxtColor){
        lastTxtColor = colorHash;
        for(int i = 0; i < engItems.size(); i++){
            if(engItems[i].txt != NULL) 
                delete engItems[i].txt;
            engItems[i].txt = NULL;
        }
    }
    
    if(colorHash != lastTxtNumbersColor){
        //qDebug() << "new global color";
        if(txtEngineT != NULL) delete txtEngineT;
        if(txtEngineF != NULL) delete txtEngineF;
        if(txtEngineW != NULL) delete txtEngineW;
        txtEngineT = NULL;
        txtEngineF = NULL;
        txtEngineW = NULL;
        lastTxtNumbersColor = colorHash;
        for(int i = 0; i < txtNumbers.size(); i++){
            if(txtNumbers[i] != NULL) 
                delete txtNumbers[i];
            txtNumbers[i] = NULL;
        }
    }
}

QString Consist::getFirstEngName(){
    if(engItems.size() < 1)
        return "";
    for(int i = 0; i < engItems.size(); i++){
        if(engItems[i].type == 1)
            return engItems[i].ename.toLower();
    }
    return engItems[0].ename.toLower();
}

void Consist::render(int aktwx, int aktwz, int selectionColor, bool renderText) {
    //gl.glTranslatef(0, 0.2f, 0);
    //qDebug() << loaded;
    if (loaded != 1) return;

    GLUU *gluu = GLUU::get();
    int scolor = 0;
    for(int i = 0; i < engItems.size(); i++){
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0, engItems[i].pos);
        if(!engItems[i].flip)
            Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI, 0, 1, 0);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(selectionColor != 0)
            scolor = selectionColor + i;
        if(selectedIdx == i)
            Game::currentEngLib->eng[engItems[i].eng]->drawBorder();
        Game::currentEngLib->eng[engItems[i].eng]->render(aktwx, aktwz, scolor);
        gluu->mvPopMatrix();
        
        if(selectionColor != 0 || !renderText)
            continue;
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, -1, engItems[i].pos);
        Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI/2, 0, 1, 0);

        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
        gluu->currentMsMatrinxHash = gluu->getMatrixHash(gluu->objStrMatrix);
        if(engItems[i].txt == NULL){
            engItems[i].txt = new TextObj(Game::currentEngLib->eng[engItems[i].eng]->displayName, 16, 1.0);
            //engItems[i].txt->setColor(255,255,0);
            //if(Game::systemTheme)
            //    engItems[i].txt->setColor(0,0,0);
            engItems[i].txt->setColor(textColor[0],textColor[1],textColor[2]);
        }        
        engItems[i].txt->render();
        gluu->mvPopMatrix();
        
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 5, engItems[i].pos);
        //Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI/2, 0, 1, 0);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(txtNumbers[i] == NULL){
            txtNumbers[i] = new TextObj(i+1);
            txtNumbers[i]->setColor(textColor[0],textColor[1],textColor[2]);
        }
        txtNumbers[i]->render(M_PI/2);
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0, -1);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        int wt = Game::currentEngLib->eng[engItems[i].eng]->wagonTypeId;
        if(wt == 1){
            if(txtEngineW == NULL){
                txtEngineW = new TextObj("C");
                txtEngineW->setColor(textColor[0],textColor[1],textColor[2]);
            }
            txtEngineW->render(M_PI/2);
        }
        if(wt == 2){
            if(txtEngineF == NULL){
                txtEngineF = new TextObj("F");
                txtEngineF->setColor(textColor[0],textColor[1],textColor[2]);
            }
            txtEngineF->render(M_PI/2);
        }
        if(wt == 3){
            if(txtEngineT == NULL){
                txtEngineT = new TextObj("T");
                txtEngineT->setColor(textColor[0],textColor[1],textColor[2]);
            }
            txtEngineT->render(M_PI/2);
        }
        if(wt == 4){
            if(txtEngineE == NULL){
                txtEngineE = new TextObj("E");
                txtEngineE->setColor(255,0,0);
            }
            txtEngineE->render(M_PI/2);
        }
        if(wt == 5){
            if(txtEngineD == NULL){
                txtEngineD = new TextObj("D");
                txtEngineD->setColor(255,0,0);
            }
            txtEngineD->render(M_PI/2);
        }
        if(wt == 6){
            if(txtEngineS == NULL){
                txtEngineS = new TextObj("S");
                txtEngineS->setColor(255,0,0);
            }
            txtEngineS->render(M_PI/2);
        }
        
        gluu->mvPopMatrix();

    }
    //loaded = -1;
    //ruchy[0].renderCon(gl, aktwx, aktwz);
    //ruchy[1].renderCon(gl, aktwx, aktwz);

    /*Vector3f pos1 = ruchy[0].getPosition(aktwx, aktwz);
     Vector3f pos2 = ruchy[1].getPosition(aktwx, aktwz);
     Vector3f pos = Vector3f.add(pos1, pos2);
     float dlugosc = (float) Math.sqrt(Math.pow(pos1.z-pos2.z, 2) + Math.pow(pos1.x-pos2.x, 2));
     pos.div(2); 
     
     
     gl.glTranslatef(pos.x, pos.y+0.25f, pos.z);
     
     gl.glRotatef((Math.signum(pos2.z-pos1.z)+1)*90+
     (float)(Math.atan((pos1.x-pos2.x)/(pos1.z-pos2.z))*180/Math.PI),0,1,0); 
     gl.glRotatef(-(float)(Math.atan((pos1.y-pos2.y)/(dlugosc))*180/Math.PI),1,0,0); 
     
     //linie
     //tu sobie byly xD
     
     gl.glColor3f(1.0f, 1.0f, 1.0f);  */
     //gluu.mvPushMatrix();
     //if(sfile[0] != -1) ShapeLib::shape[sfile[0]]->render();
     //if(sfile[1] != -1) ShapeLib::shape[sfile[1]]->render();
     //gluu.mvPopMatrix();
     //
}

void Consist::initOnTrack(float *posTXZ, int direction){
    if(direction > 0)
        direction = 1;
    
    float posT[2];
    Vec2::set(posT, posTXZ[0], -posTXZ[1]);
    float pos[3];
    float tpos[3];
    Vec3::set(pos, posTXZ[2], 0, -posTXZ[3]);
    
    qDebug() << posT[0]<< posT[1];
    qDebug() << pos[0]<< pos[1]<< pos[2];
    
    int ok = Game::trackDB->findNearestPositionOnTDB(posT, pos, NULL, tpos);
    if(ok < 0) {
        qDebug() << "coninit fail";
        return;
    }
    qDebug() << "coninit init";
    qDebug() << tpos[0];
    
    float conLen = 0;
    if(engItems.size() > 0)
        conLen = engItems[0].pos;
    for(int i = 0; i < engItems.size(); i++){
        if(engItems[i].engPointer == NULL)
            engItems[i].engPointer = new Eng(Game::currentEngLib->eng[engItems[i].eng]);
        engItems[i].engPointer->initOnTrack(tpos, direction);
        engItems[i].engPointer->flip = engItems[i].flip;
        //engItems[i].engPointer->move(26.5*i);
        //engItems[i].engPointer->move(engItems[i].conLength);
        engItems[i].engPointer->move(-(engItems[i].conLength-conLen));
    }
    this->isOnTrack = true;
}

void Consist::renderOnTrack(GLUU* gluu, float* playerT, int selectionColor) {
    if (loaded != 1) return;

    int scolor = 0;
    for(int i = 0; i < engItems.size(); i++){
        gluu->mvPushMatrix();
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0, engItems[i].pos);
        //if(!engItems[i].flip)
        //    Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI, 0, 1, 0);
        //gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(selectionColor != 0){
            scolor = selectionColor | i;
        }
        engItems[i].engPointer->renderOnTrack(gluu, playerT, scolor);
        if(selectedIdx == i)
            engItems[i].engPointer->drawBorder3d();
        gluu->mvPopMatrix();
    }
}

bool Consist::getWagonWorldPosition(int id, float *posTW){
    if (loaded != 1) return false;
    
    if(id >= engItems.size())
        return false;
    
    float *drawPosition = engItems[id].engPointer->getCurrentPositionOnTrack();
    if(drawPosition == NULL)
        return false;
    
    posTW[0] = drawPosition[5];
    posTW[1] = drawPosition[6];
    posTW[2] = drawPosition[0];
    posTW[3] = 0;
    posTW[4] = drawPosition[2];
    
    return true;
}

bool Consist::isNewConsist(){
    return newConsist;
}
void Consist::setNewConsistFlag(){
    newConsist = true;
}
void Consist::setMaxVelocityFixed(bool val){
    maxVelocityFixed = val;
}

bool Consist::isMaxVelocityFixed(){
    return maxVelocityFixed;
}

void Consist::setDurability(float val){
    if(val < 0) val = 0;
    if(val > 1) val = 1;
    this->durability = val;
    modified = true;
}

void Consist::save(){
    QString sh;
    QString spath;
    spath = path + "/" + name;
    spath.replace("//", "/");
    qDebug() << spath;
    QFile file(spath);
    
    
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()) {
        qDebug() << "con file write error. file not open " << spath;
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);

    out << "SIMISA@@@@@@@@@@JINX0D0t______\n";
    out << "\n";
    out << "Train (\n";
    save("", &out);
    out << ")\n";
    
    file.close(); 
    newConsist = false;
    modified = false;
}

void Consist::save(QString woff, QTextStream* out){
    *out << woff <<"	TrainCfg ( \""<<conName<<"\"\n";
    if(displayName.length() > 0)
    *out << woff <<"		Name ( \"" << displayName << "\" )\n";
    if(serial > 0 )
    *out << woff <<"		Serial ( "<<serial<<" )\n";
    if(defaultValue)
    *out << woff <<"		Default ( )\n";
    *out << woff <<"		MaxVelocity ( " << QString::number(maxVelocity[0], 'f', 5) << " " << QString::number(maxVelocity[1], 'f', 5) << " )\n";
    *out << woff <<"		NextWagonUID ( " << nextWagonUID << " )\n";
    *out << woff <<"		Durability ( " << QString::number(durability, 'f', 5) << " )\n";

    for(int i = 0; i < this->engItems.size(); i++){
        if(this->engItems[i].type == 1){
            *out << woff <<"		Engine (\n";
            if(this->engItems[i].flip)
            *out << woff <<"			Flip ( )\n";
            *out << woff <<"			UiD ( " << engItems[i].uid << " )\n";
            *out << woff <<"			EngineData ( " << ParserX::AddComIfReq(engItems[i].ename) << " " << ParserX::AddComIfReq(engItems[i].epath) << " )\n";
            *out << woff <<"		)\n";
        } 
        if(this->engItems[i].type == 0){
            *out << woff <<"		Wagon (\n";
            *out << woff <<"			WagonData ( " << ParserX::AddComIfReq(engItems[i].ename) << " " << ParserX::AddComIfReq(engItems[i].epath) << " )\n";
            if(this->engItems[i].flip)
            *out << woff <<"			Flip ( )\n";
            *out << woff <<"			UiD ( " << engItems[i].uid << " )\n";
            *out << woff <<"		)\n";
        }
    }
    *out << woff <<"	)\n";
}
