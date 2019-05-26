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
#include "MstsSoundDefinition.h"
#include "SoundManager.h"
#include "SoundSource.h"
#include "SoundVariables.h"
#include "TrainNetworkEng.h"
#include "GeoCoordinates.h"
#include "ContentHierarchyInfo.h"

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
    
    souncCabFile = o->souncCabFile;
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
                if (sh == ("tsrekeywords")) {
                    searchKeywords = ParserX::GetString(data);
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
                    if(wagonTypeId == 4){
                        if(engType.toLower() == "electric")
                            wagonTypeId = wagonTypeId + 0;
                        if(engType.toLower() == "diesel")
                            wagonTypeId = wagonTypeId + 1;
                        if(engType.toLower() == "steam")
                            wagonTypeId = wagonTypeId + 2;
                    }
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
                if (sh == ("sound")) {
                    souncCabFile = ParserX::GetStringInside(data);
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
    if(this->searchKeywords.contains(q, Qt::CaseInsensitive)) return true;
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

void Eng::updateSim(float deltaTime){
    static float acceleration = 0;
    static float lastSpeed = 0;
    // Use True for fake acceleration sound
    static bool fakesound = true;
    
    if(!Game::useNetworkEng || fakesound){
    // Local Sound Speed test:
        static SoundDefinitionGroup::Stream::Curve curve1("SpeedControlled");
        static SoundDefinitionGroup::Stream::Curve curve2("SpeedControlled");
        static SoundDefinitionGroup::Stream::Curve *curve = &curve1;
        if(curve1.points.size() == 0){
            curve1.points.push_back(Vector2f(-1.0, 0.1));
            curve1.points.push_back(Vector2f(2.0, 0.5));
            curve1.points.push_back(Vector2f(25.0, 1.0));
            curve1.points.push_back(Vector2f(56.0, 0.0));
        }
        if(curve2.points.size() == 0){
            curve2.points.push_back(Vector2f(-1.0, -0.1));
            curve2.points.push_back(Vector2f(2.0, -0.5));
            curve2.points.push_back(Vector2f(25.0, -1.0));
            curve2.points.push_back(Vector2f(56.0, 0.0));
        }

        if(currentSpeed > 55 && curve == &curve1){
            curve = &curve2;
        }
        if(currentSpeed < 0.1 && curve == &curve2){
            curve = &curve1;
        }

        if(soundVariables != NULL)
            acceleration = curve->getValue(soundVariables);

        currentSpeed += acceleration*deltaTime;

        // Static speed
        currentSpeed = 40;
    
    // Network Speed
    }
    if(Game::useNetworkEng) {
        static IghCoordinate* igh = new IghCoordinate();
        static LatitudeLongitudeCoordinate* latlon = new LatitudeLongitudeCoordinate();
        static PreciseTileCoordinate* coords = new PreciseTileCoordinate();
        if(networkEng == NULL){
            networkEng = new TrainNetworkEng();
        }
        currentSpeed = networkEng->getSpeed();
        
        float data[4];
        data[0] = getCurrentElevation();
        if(isBroken())
            data[0] = 666;
        data[1] = getTotalDistanceDownPath();
        float cpos[8];
        getCameraPosition((float*)cpos);
        coords->setTWxyz(cpos[0], -cpos[1], cpos[2], cpos[3], cpos[4]);
        Game::GeoCoordConverter->ConvertToInternal(coords, igh);
        Game::GeoCoordConverter->ConvertToLatLon(igh, latlon);
        data[2] = latlon->Latitude;
        data[3] = latlon->Longitude;
        networkEng->writeData(data);
    } 
    
    if(currentSpeed != lastSpeed && !fakesound){
        acceleration = (currentSpeed - lastSpeed) / (deltaTime);
    }
    
    //qDebug() << acceleration << currentSpeed << lastSpeed;
    if(soundVariables != NULL){
        // vectron
        //soundVariables->value[SoundVariables::VARIABLE2] = currentSpeed * 100 / 55.0;
        soundVariables->value[SoundVariables::VARIABLE2] = acceleration * 100;
        soundVariables->value[SoundVariables::SPEED] = currentSpeed;
        // acela
        //soundVariables->value[SoundVariables::VARIABLE2] = currentSpeed * 3.0;
    }
    
    if(Game::soundEnabled){
        if(ruch1->onJunction > 0){
            qDebug() << "--------------";
            int soundSourceId = SoundManager::AddSoundSource(path, "j1.wav");
            float *pos = ruch1->getCurrentPosition();
            pos[2] = -pos[2];
            SoundManager::Sources[soundSourceId]->setPosition(pos[5], -pos[6], pos);
            //SoundManager::Sources[camSoundSourceId]->setRelative(true);
            ruch1->onJunction = 0;
        }
    }
    
    lastSpeed = currentSpeed;
}

float Eng::getCurrentSpeed(){
    return currentSpeed;
}

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

bool Eng::isBroken(){
    QString name = ruch1->getLastItemName();
    if(name == "E_STOP"){
        return true;
    }
    return false;
}

float *Eng::getCurrentPositionOnTrack(){
    if (loaded != 1) return NULL;
    return ruch1->getCurrentPosition();
}

void Eng::getCameraPosition(float* out){
    if(Game::soundEnabled){
        if(camSoundSourceId == -1){
            QString spath = path+"/sound";
            int sid = MstsSoundDefinition::AddDefinition(spath, souncCabFile);

            if(sid != -1){
                if(MstsSoundDefinition::Definitions[sid]->group.size() > 0){
                    camSoundSourceId = SoundManager::AddSoundSource(MstsSoundDefinition::Definitions[sid]->group.first());
                    SoundManager::Sources[camSoundSourceId]->setRelative(true);
                    soundVariables = new SoundVariables();
                    SoundManager::Sources[camSoundSourceId]->variables = soundVariables;
                }
            } else {
                camSoundSourceId = -2;
            }
        }
    }
    
    if(out == NULL)
        return;
    //qDebug() << "get position";
    float selev1, selev2;
    float *drawPosition1 = ruch1->getCurrentPosition(&selev1);
    float *drawPosition2 = ruch2->getCurrentPosition(&selev2);
    selev1 = (selev1 + selev2) / 2.0;
    
    
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
    float rotX = (float)(asin((drawPosition1[1]-drawPosition2[1])/(dlugosc))); 

    out[0] = pos[3];
    out[1] = -pos[4];
    out[2] = drawPosition1[0]; // pos?
    out[3] = drawPosition1[1];
    out[4] = -drawPosition1[2];
    out[5] = -rotY+M_PI;
    out[6] = rotX;
    //out[6] = 0; // distant camera test only
    out[7] = selev1;
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, pos[0] + 2048 * (pos[3] - playerT[0]), pos[1]+0.28, -pos[2] + 2048 * (-pos[4] - playerT[1]));
    //Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, -rotY /* + flip*M_PI+ M_PI*/);
    //Mat4::rotateX(gluu->mvMatrix, gluu->mvMatrix, rotX );
    //Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, flip*M_PI /*+ M_PI*/);
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
    
    float selev1, selev2;
    float *drawPosition1 = ruch1->getCurrentPosition(&selev1);
    float *drawPosition2 = ruch2->getCurrentPosition(&selev2);
    selev1 = (selev1 + selev2) / 2.0;
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
    Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, -selev1, 0, 0, 1 );
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

float Eng::getCurrentElevation(){
    if(ruch1 == NULL)
        return 0;
    if(ruch2 == NULL)
        return 0;
    
    float *drawPosition1 = ruch1->getCurrentPosition();
    float *drawPosition2 = ruch2->getCurrentPosition();
    drawPosition2[0] += 2048*(drawPosition2[5]-drawPosition1[5]);
    drawPosition2[2] += 2048*(drawPosition2[6]-drawPosition1[6]);
    float d = Vec3::distance(drawPosition1, drawPosition2);
    float h = drawPosition1[1] - drawPosition2[1];
    return (h/d)*1000.0;
}

float Eng::getTotalDistanceDownPath(){
    if(ruch1 == NULL)
        return 0;
    return ruch1->getDistanceDownPath();
}

void Eng::initOnTrack(float *tpos, int direction, QMap<int, int>* junctionDirections){
    TDB* tdb = Game::trackDB;
    
    if(ruch1 == NULL)
        ruch1 = new Ruch();
    ruch1->set(tpos[0], tpos[1], direction, junctionDirections);
    ruch1->trackPassingItems(true);
    if(ruch2 == NULL)
        ruch2 = new Ruch();
    ruch2->set(tpos[0], tpos[1], direction, junctionDirections);
    ruch2->next(-getFullWidth());
    //ruch1->getCurrentPosition();
    
    //qDebug() << drawPosition[0] << drawPosition[1] << drawPosition[2];
    //qDebug() << drawPosition[5] << drawPosition[6];

    //    qDebug() << "fail";
    //    this->loaded = -1;
    this->loaded = 1;
    qDebug() << "ok";
}

void Eng::fillContentHierarchyInfo(QVector<ContentHierarchyInfo*>& list, int parent){
    ContentHierarchyInfo *info = new ContentHierarchyInfo();
    info->parent = parent;
    info->name = name;
    info->eng = this;
    info->type = "eng";
    list.push_back(info);
    parent = list.size()-1;

    long long int shapeLibId = reinterpret_cast<long long int>(Game::currentShapeLib);
    if(shape.id[shapeLibId] >= 0) 
        Game::currentShapeLib->shape[shape.id[shapeLibId]]->fillContentHierarchyInfo(list, list.size()-1);

    for(int i = 0; i < freightanimShape.size(); i++){
        if(freightanimShape[i].id[shapeLibId] >= 0) {
            Game::currentShapeLib->shape[freightanimShape[i].id[shapeLibId]]->fillContentHierarchyInfo(list, parent);
        }
    }
}