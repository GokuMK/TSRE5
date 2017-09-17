/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Eng.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "ShapeLib.h"
#include "SFile.h"
#include <QDebug>
#include <QFile>
#include "Game.h"
#include "GLUU.h"
#include "OglObj.h"
#include "GLMatrix.h"
#include "TDB.h"
#include "Ruch.h"

Eng::Eng() {
    
}

Eng::Eng(Eng *o) {
    
    coupling.append(o->coupling);
    filePaths.append(o->filePaths);
    shape = o->shape;
    freightanimShape.append(o->freightanimShape);
    
    name = o->name;
    path = o->path;
    pathid = o->pathid;
    orpathid = o->orpathid;
    orpath = o->orpath;

    engName = o->engName;
    displayName = o->displayName;
    engType = o->engType;
    typeHash = o->typeHash;
    type = o->type;
    brakeSystemType = o->brakeSystemType;
    wagonTypeId = o->wagonTypeId;
    mass = o->mass;
    sizex = o->sizex;
    sizey = o->sizey;
    sizez = o->sizez;
    maxSpeed = o->maxSpeed;
    maxForce = o->maxForce;
    maxPower = o->maxPower;
    maxCurrent = o->maxCurrent;
    flip = o->flip;
    loaded = o->loaded;
    kierunek = o->kierunek;
    ref = o->ref;
}

Eng::~Eng() {
}

Eng::Eng(QString p, QString n) {
    pathid = p.toLower() + "/" + n.toLower();
    pathid.replace("//", "/");
    path = p;
    name = n;
    if(Game::ortsEngEnable){
        orpathid = p.toLower()+"/openrails/"+n.toLower();
        orpathid.replace("//","/");
        orpath = path+"/openrails/";
    } else {
        orpathid = pathid;
        orpath = path;
    }
    //shape.id = -1;
    sizex = 0;
    sizey = 0;
    sizez = 0;
    flip = 0;
    loaded = -1;
    kierunek = false;
    load();
}

void Eng::addToFileList(QString val){
    val.replace("\\","/");
    val.replace("//","/");
    filePaths.push_back(val);
}

Eng::Eng(QString src, QString p, QString n) {
    pathid = src;
    pathid.replace("//","/");
    path = p;
    name = n;
    if(Game::ortsEngEnable){
        orpathid = p.toLower()+"/openrails/"+n.toLower();
        orpathid.replace("//","/");
        orpath = path+"/openrails/";
    } else {
        orpathid = pathid;
        orpath = path;
    }
    //shape.id = -1;
    sizex = 0;
    sizey = 0;
    sizez = 0;
    flip = 0;
    loaded = -1;
    kierunek = false;
    load();
}

void Eng::load(){
    filePaths.clear();
    QString mstsincpath = path.toLower();
    QString incpath = orpath.toLower();
    QString sh;
    QFile *file = new QFile(orpathid);
    if (!file->open(QIODevice::ReadOnly)){
        incpath = path.toLower();
        file = new QFile(pathid);
        if (!file->open(QIODevice::ReadOnly)){
            qDebug() << pathid << "not exist";
            return;
        } else {
            qDebug() << pathid;
            addToFileList(pathid);
        }
    } else {
        qDebug() << orpathid;
        addToFileList(orpathid);
    }

    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    data->skipBOM();
    //ParserX::NextLine(data);
    QString loadedPath;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("simisa@@@@@@@@@@jinx0d0t______")) {
            continue;
        }
        if (sh == ("include")) {
            QString incPath = ParserX::GetStringInside(data).toLower();
            ParserX::SkipToken(data);
            if(data->insertFile(incpath + "/" + incPath, mstsincpath + "/" + incPath, &loadedPath))
                addToFileList(loadedPath);
            continue;
        }
        if (sh == ("wagon")) {
            if(engName.length() == 0){
                engName = ParserX::GetString(data).trimmed();
                displayName = engName;
            }
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //qDebug() << sh;
                if (sh == ("include")) {
                    QString incPath = ParserX::GetStringInside(data).toLower();
                    ParserX::SkipToken(data);
                    if(data->insertFile(incpath + "/" + incPath, mstsincpath + "/" + incPath, &loadedPath))
                        addToFileList(loadedPath);
                    continue;
                }
                if (sh == ("name")) {
                    displayName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("freightanim")) {
                    freightanimShape.push_back(EngShape());
                    freightanimShape.back().name = ParserX::GetString(data);
                    //freightanimShape.back().id = -2;
                    //qDebug() << "=====znaleziono s2 " << path << sNames[1];
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("size")) {
                    sizex = ParserX::GetNumberInside(data);
                    sizey = ParserX::GetNumberInside(data);
                    sizez = ParserX::GetNumberInside(data);
                    //qDebug() << "wymiary taboru: " << sizex << " " << sizey << " " << sizez;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("wagonshape")) {
                    shape.name = ParserX::GetString(data);
                    //shape.id = -2;
                    //qDebug() << "=====znaleziono s1 " << path << sNames[0];
                    //sfile[0] = ShapeLib::addShape(path, tname, path);//new Sfile(this.path, tname);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("mass")) {
                    mass = ParserX::GetNumberInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("coupling")) {
                    coupling.push_back(Coupling());
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("type")) {
                            coupling.back().type = ParserX::GetString(data);
                            //qDebug() << "c: "<< coupling.back().type;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("velocity")) {
                            coupling.back().velocity = ParserX::GetNumber(data);
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("spring")) {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if (sh == ("r0")) {
                                    coupling.back().r0[0] = ParserX::GetNumberInside(data);
                                    coupling.back().r0[1] = ParserX::GetNumberInside(data);
                                    //qDebug() <<"r0 " << coupling.back().r0[0] << coupling.back().r0[1];
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
                if (sh == ("ortsfreightanims")) {
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        //qDebug() << "orts " << sh;
                        if (sh == ("mstsfreightanimenabled")) {
                            int val = ParserX::GetNumber(data);
                            if(val == 0){
                                freightanimShape.clear();
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("freightanimstatic")) {
                            freightanimShape.push_back(EngShape());
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                //qDebug() << "orts " << sh;
                                if (sh == ("subtype")) {
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if (sh == ("freightweight")) {
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if (sh == ("shape")) {
                                    freightanimShape.back().name = ParserX::GetString(data);
                                    //qDebug() << QString::fromStdString(freightanimShapeName.back();
                                    //freightanimShape.back().id = -2;
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if (sh == ("offset")) {
                                    freightanimShape.back().x = ParserX::GetNumber(data);
                                    freightanimShape.back().y = ParserX::GetNumber(data);
                                    freightanimShape.back().z = ParserX::GetNumber(data);
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
                if (sh == ("type")) {
                    type = ParserX::GetString(data);
                    //qDebug() << "type "<< type;
                    if(type.toLower() == "engine")
                        wagonTypeId = 4;
                    if(type.toLower() == "carriage")
                        wagonTypeId = 1;
                    if(type.toLower() == "freight")
                        wagonTypeId = 2;
                    if(type.toLower() == "tender")
                        wagonTypeId = 3;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("brakesystemtype")) {
                    brakeSystemType = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
            }
            typeHash = type;
            ParserX::SkipToken(data);
            continue;
        }
        if(sh == "engine"){
            ParserX::GetString(data);
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //qDebug() << sh;
                if (sh == ("include")) {
                    QString incPath = ParserX::GetStringInside(data).toLower();
                    ParserX::SkipToken(data);
                    if(data->insertFile(incpath + "/" + incPath, mstsincpath + "/" + incPath, &loadedPath))
                        addToFileList(loadedPath);
                    continue;
                }
                if (sh == ("type")) {
                    engType = ParserX::GetString(data);
                    typeHash+="-"+engType;
                    //qDebug() << engType;
                    if(engType.toLower() == "electric")
                        wagonTypeId += 0;
                    if(engType.toLower() == "diesel")
                        wagonTypeId += 1;
                    if(engType.toLower() == "steam")
                        wagonTypeId += 2;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("name")) {
                    displayName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("maxvelocity")) {
                    maxSpeed = ParserX::GetNumberInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("maxpower")) {
                    maxPower = ParserX::GetNumberInside(data);
                    //qDebug() << "maxPower "<<maxPower;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("maxcurrent")) {
                    maxCurrent = ParserX::GetNumberInside(data);
                    //qDebug() << "maxCurrent "<<maxCurrent;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("maxforce")) {
                    maxForce = ParserX::GetNumberInside(data);
                    //qDebug() << "maxForce "<<maxForce / 1000;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("description")) {
                    ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("name")) {
            displayName = ParserX::GetString(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("brakesystemtype")) {
            brakeSystemType = ParserX::GetString(data);
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
        continue;
    }
    //qDebug() << typeHash;
    delete data;
    loaded = 1;
    //qDebug() << loaded;
    return;
}

bool Eng::engFilter(QString q){
    if(q == "") return true;
    if(q == "electric" && wagonTypeId == 4)
        return true;
    if(q == "diesel" && wagonTypeId == 5)
        return true;
    if(q == "steam" && wagonTypeId == 6)
        return true;    
    if(q == "carriage" && wagonTypeId == 1)
        return true;
    if(q == "freight" && wagonTypeId == 2)
        return true;
    if(q == "tender" && wagonTypeId == 3)
        return true;   
    return false;
}

bool Eng::couplingFilter(QString q){
    if(q == "") return true;
    for(int i = 0; i < coupling.size(); i++){
       if(q.toLower() == coupling[i].type.toLower()) return true; 
    }
    return false;
}
bool Eng::searchFilter(QString q){
    if(q == "") return true;
    if(this->displayName.contains(q, Qt::CaseInsensitive)) return true;
    if(this->engName.contains(q, Qt::CaseInsensitive)) return true;
    return false;
}

QString Eng::getCouplingsName(){
    QString name;
    for(int i = 0; i < coupling.size(); i++){
       if(i != 0) name += " , ";
       name += coupling[i].type; 
    }
    return name;
}

float Eng::getFullWidth(){
    if(this->coupling.size() > 0)
        return this->coupling[0].r0[0] + this->sizez;
    else
        return this->sizez + 0.0;
}

void Eng::select(){
    selected = true;
}

void Eng::unselect(){
    selected = false;
}

bool Eng::isSelected(){
    return selected;
}

void Eng::drawBorder(){

    if (borderObj == NULL) {
        borderObj = new OglObj();
        
        float width = this->getFullWidth() / 2;
        
        float* punkty = new float[24];
        float* ptr = punkty;
        
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = -width;
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = width;
        *ptr++ = 0;
        *ptr++ = 6;
        *ptr++ = -width;
        *ptr++ = 0;
        *ptr++ = 6;
        *ptr++ = width;
        
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = -width;
        *ptr++ = 0;
        *ptr++ = 6;
        *ptr++ = -width;
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = width;
        *ptr++ = 0;
        *ptr++ = 6;
        *ptr++ = width;
        borderObj->setMaterial(1.0, 0.0, 0.0);
        borderObj->init(punkty, ptr-punkty, borderObj->V, GL_LINES);
        delete[] punkty;
    }

    borderObj->render();
};

void Eng::drawBorder3d(){

    if (borderObj3d == NULL) {
        borderObj3d = new OglObj();
        
        float width = this->getFullWidth() / 2;
        
        float* punkty = new float[96];
        float* ptr = punkty;
        
        float sizexx = sizex / 2.0;
        for(int j = 0; j <= 1; j++)
            for(int i = -1; i <= 1; i+=2){
                *ptr++ = sizexx*i;
                *ptr++ = sizey*j;
                *ptr++ = -width;
                *ptr++ = sizexx*i;
                *ptr++ = sizey*j;
                *ptr++ = width;
            }
        for(int j = 0; j <=1; j++)
            for(int i = -1; i <= 1; i+=2){
                *ptr++ = sizexx;
                *ptr++ = sizey*j;
                *ptr++ = width*i;
                *ptr++ = -sizexx;
                *ptr++ = sizey*j;
                *ptr++ = width*i;
        }
        for(int j = -1; j <=1; j+=2)
            for(int i = -1; i <= 1; i+=2){
                *ptr++ = sizexx*j;
                *ptr++ = 0;
                *ptr++ = width*i;
                *ptr++ = sizexx*j;
                *ptr++ = sizey;
                *ptr++ = width*i;
        }
        borderObj3d->setMaterial(1.0, 0.0, 0.0);
        borderObj3d->init(punkty, ptr-punkty, borderObj3d->V, GL_LINES);
        delete[] punkty;
    }

    borderObj3d->render();
};

void Eng::render(int selectionColor) {
    render(0, 0, selectionColor);
}

void Eng::render(int aktwx, int aktwz, int selectionColor) {
    //gl.glTranslatef(0, 0.2f, 0);
    //qDebug() << loaded;
    if (loaded != 1) return;

    GLUU *gluu = GLUU::get();
    
     if(selectionColor != 0){
        gluu->disableTextures(selectionColor);
    } else {
        gluu->enableTextures();
    }
    
    long long int shapeLibId = reinterpret_cast<long long int>(Game::currentShapeLib);
    if(!shape.id.keys().contains(shapeLibId)){
        if(shape.name.length() > 1)
            shape.id[shapeLibId] = Game::currentShapeLib->addShape(path +"/"+ shape.name, path);
        else 
            shape.id[shapeLibId] = -1;
    }

    for(int i = 0; i < freightanimShape.size(); i++){
        if(!freightanimShape[i].id.keys().contains(shapeLibId)){
            if(freightanimShape[i].name.length() > 1)
                freightanimShape[i].id[shapeLibId] = Game::currentShapeLib->addShape(path +"/"+ freightanimShape[i].name, path);
            else 
                freightanimShape[i].id[shapeLibId] = -1;
        }
    }

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
    if(shape.id[shapeLibId] >= 0) 
        Game::currentShapeLib->shape[shape.id[shapeLibId]]->render();

    for(int i = 0; i < freightanimShape.size(); i++){
        if(freightanimShape[i].id[shapeLibId] >= 0) {
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, freightanimShape[i].x, freightanimShape[i].y, -freightanimShape[i].z);
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            Game::currentShapeLib->shape[freightanimShape[i].id[shapeLibId]]->render();
            gluu->mvPopMatrix();
        }
    }
     

     //gluu.mvPopMatrix();
     //

}
void Eng::reload(){
    long long int shapeLibId = reinterpret_cast<long long int>(Game::currentShapeLib);
    if(shape.id[shapeLibId] >= 0) 
        Game::currentShapeLib->shape[shape.id[shapeLibId]]->reload();
    
    for(int i = 0; i < freightanimShape.size(); i++){
        if(freightanimShape[i].id[shapeLibId] >= 0) {
            Game::currentShapeLib->shape[freightanimShape[i].id[shapeLibId]]->reload();
        }
    }
}

float *Eng::getCurrentPositionOnTrack(){
    if (loaded != 1) return NULL;
    return ruch1->getCurrentPosition();
}

void Eng::renderOnTrack(GLUU* gluu, float* playerT, int selectionColor) {
    if (loaded != 1) return;

    long long int shapeLibId = reinterpret_cast<long long int>(Game::currentShapeLib);
    if(!shape.id.keys().contains(shapeLibId)){
        if(shape.name.length() > 1)
            shape.id[shapeLibId] = Game::currentShapeLib->addShape(path +"/"+ shape.name, path);
        else 
            shape.id[shapeLibId] = -1;
    }

    for(int i = 0; i < freightanimShape.size(); i++){
        if(!freightanimShape[i].id.keys().contains(shapeLibId)){
            if(freightanimShape[i].name.length() > 1)
                freightanimShape[i].id[shapeLibId] = Game::currentShapeLib->addShape(path +"/"+ freightanimShape[i].name, path);
            else 
                freightanimShape[i].id[shapeLibId] = -1;
        }
    }
    
    if (ruchPoint == NULL) {
        ruchPoint = new OglObj();
        float *punkty = new float[3 * 2];
        int ptr = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 30;
        punkty[ptr++] = 0;
        ruchPoint->setMaterial(0.0, 1.0, 0.0);
        ruchPoint->init(punkty, ptr, ruchPoint->V, GL_LINES);
        delete[] punkty;
    }
    
    float *drawPosition1 = ruch1->getCurrentPosition();
    float *drawPosition2 = ruch2->getCurrentPosition();
    /*gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition2[0] + 2048 * (drawPosition2[5] - playerT[0]), drawPosition2[1], -drawPosition2[2] + 2048 * (-drawPosition2[6] - playerT[1]));
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    ruchPoint->render();
    gluu->mvPopMatrix();
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition1[0] + 2048 * (drawPosition1[5] - playerT[0]), drawPosition1[1], -drawPosition1[2] + 2048 * (-drawPosition1[6] - playerT[1]));
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    ruchPoint->render();
    gluu->mvPopMatrix();*/

    float pos[5];
    pos[3] = drawPosition1[5];
    pos[4] = drawPosition1[6];
    drawPosition2[0] += 2048*(drawPosition2[5]-drawPosition1[5]);
    drawPosition2[2] += 2048*(drawPosition2[6]-drawPosition1[6]);
    Vec3::add(pos, drawPosition1, drawPosition2);
    Vec3::scale(pos, pos, 0.5);
    float dlugosc = Vec3::distance(drawPosition1, drawPosition2);
    
    int someval = (((drawPosition1[2]-drawPosition2[2])+0.00001f)/fabs((drawPosition1[2]-drawPosition2[2])+0.00001f));
    float rotY = ((float)someval+1.0)*(M_PI/2.0)+(float)(atan((drawPosition1[0]-drawPosition2[0])/(drawPosition1[2]-drawPosition2[2]))); 
    float rotX = -(float)(asin((drawPosition1[1]-drawPosition2[1])/(dlugosc))); 
            
     if(selectionColor != 0){
        gluu->disableTextures(selectionColor);
    } else {
        gluu->enableTextures();
    }
        
    gluu->mvPushMatrix();
 
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, pos[0] + 2048 * (pos[3] - playerT[0]), pos[1]+0.28, -pos[2] + 2048 * (-pos[4] - playerT[1]));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, -rotY /* + flip*M_PI+ M_PI*/);
    Mat4::rotateX(gluu->mvMatrix, gluu->mvMatrix, rotX );
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, flip*M_PI /*+ M_PI*/);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));

    if(shape.id[shapeLibId] >= 0) 
        Game::currentShapeLib->shape[shape.id[shapeLibId]]->render();
    
    
    for(int i = 0; i < freightanimShape.size(); i++){
        if(freightanimShape[i].id[shapeLibId] >= 0) {
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, freightanimShape[i].x, freightanimShape[i].y, -freightanimShape[i].z);
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            Game::currentShapeLib->shape[freightanimShape[i].id[shapeLibId]]->render();
            gluu->mvPopMatrix();
        }
    }
    gluu->mvPopMatrix();
    
}

void Eng::move(float m){
    if(ruch1 != NULL)
        ruch1->next(m);
    if(ruch2 != NULL)
        ruch2->next(m);
}

void Eng::initOnTrack(float *tpos, int direction){
    TDB* tdb = Game::trackDB;
    
    if(ruch1 == NULL)
        ruch1 = new Ruch();
    ruch1->set(tpos[0], tpos[1], direction);
    if(ruch2 == NULL)
        ruch2 = new Ruch();
    ruch2->set(tpos[0], tpos[1], direction);
    ruch2->next(-getFullWidth());
    //ruch1->getCurrentPosition();
    
    //qDebug() << drawPosition[0] << drawPosition[1] << drawPosition[2];
    //qDebug() << drawPosition[5] << drawPosition[6];

    //    qDebug() << "fail";
    //    this->loaded = -1;
    this->loaded = 1;
    qDebug() << "ok";
}
