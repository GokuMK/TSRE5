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

Eng::Eng() {
}

Eng::~Eng() {
}

Eng::Eng(QString p, QString n) {
    pathid = p + "/" + n;
    pathid.replace("//", "/");
    path = p;
    name = n;
    sfile[0] = -1; sfile[1] = -1;
    sizex = 0;
    sizey = 0;
    sizez = 0;
    flip = 0;
    loaded = -1;
    kierunek = false;
    load();
}

Eng::Eng(QString src, QString p, QString n) {
    pathid = src;
    path = p;
    name = n;
    sfile[0] = -1; sfile[1] = -1;
    sizex = 0;
    sizey = 0;
    sizez = 0;
    flip = 0;
    loaded = -1;
    kierunek = false;
    load();
}

void Eng::load(){
    QString sh;
    pathid.replace("//","/");
    //qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    data->off = 48;
    
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("include")) {
            QString incPath = ParserX::odczytajtcInside(data);
            ParserX::pominsekcje(data);
            data->insertFile(path + "/" + incPath);
            continue;
        }
        if (sh == ("wagon")) {
            engName = ParserX::odczytajtc(data).trimmed();
            displayName = engName;
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                //qDebug() << sh;
                if (sh == ("include")) {
                    QString incPath = ParserX::odczytajtcInside(data);
                    ParserX::pominsekcje(data);
                    data->insertFile(path + "/" + incPath);
                    continue;
                }
                if (sh == ("name")) {
                    displayName = ParserX::odczytajtc(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("freightanim")) {
                    sNames[1] = ParserX::odczytajtc(data);
                    sfile[1] = -2;
                    //qDebug() << "=====znaleziono s2 " << path << sNames[1];
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("size")) {
                    sizex = ParserX::parsujrInside(data);
                    sizey = ParserX::parsujrInside(data);
                    sizez = ParserX::parsujrInside(data);
                    //qDebug() << "wymiary taboru: " << sizex << " " << sizey << " " << sizez;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("wagonshape")) {
                    sNames[0] = ParserX::odczytajtc(data);
                    sfile[0] = -2;
                    //qDebug() << "=====znaleziono s1 " << path << sNames[0];
                    //sfile[0] = ShapeLib::addShape(path, tname, path);//new Sfile(this.path, tname);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("mass")) {
                    mass = ParserX::parsujrInside(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("coupling")) {
                    coupling.push_back(Coupling());
                    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                        if (sh == ("type")) {
                            coupling.back().type = ParserX::odczytajtc(data);
                            //qDebug() << "c: "<< coupling.back().type;
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("velocity")) {
                            coupling.back().velocity = ParserX::parsujr(data);
                            ParserX::pominsekcje(data);
                            continue;
                        }
                        if (sh == ("spring")) {
                            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                                if (sh == ("r0")) {
                                    coupling.back().r0[0] = ParserX::parsujrInside(data);
                                    coupling.back().r0[1] = ParserX::parsujrInside(data);
                                    //qDebug() <<"r0 " << coupling.back().r0[0] << coupling.back().r0[1];
                                    ParserX::pominsekcje(data);
                                    continue;
                                }
                                ParserX::pominsekcje(data);
                            }
                            ParserX::pominsekcje(data);
                            continue;
                        }

                        ParserX::pominsekcje(data);
                    }
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("type")) {
                    type = ParserX::odczytajtc(data);
                    //qDebug() << "type "<< type;
                    if(type.toLower() == "engine")
                        wagonTypeId = 4;
                    if(type.toLower() == "carriage")
                        wagonTypeId = 1;
                    if(type.toLower() == "freight")
                        wagonTypeId = 2;
                    if(type.toLower() == "tender")
                        wagonTypeId = 3;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("brakesystemtype")) {
                    brakeSystemType = ParserX::odczytajtc(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                ParserX::pominsekcje(data);
            }
            typeHash = type;
            ParserX::pominsekcje(data);
            continue;
        }
        if(sh == "engine"){
            ParserX::odczytajtc(data);
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                //qDebug() << sh;
                if (sh == ("include")) {
                    QString incPath = ParserX::odczytajtcInside(data);
                    ParserX::pominsekcje(data);
                    data->insertFile(path + "/" + incPath);
                    continue;
                }
                if (sh == ("type")) {
                    engType = ParserX::odczytajtc(data);
                    typeHash+="-"+engType;
                    //qDebug() << engType;
                    if(engType.toLower() == "electric")
                        wagonTypeId += 0;
                    if(engType.toLower() == "diesel")
                        wagonTypeId += 1;
                    if(engType.toLower() == "steam")
                        wagonTypeId += 2;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("name")) {
                    displayName = ParserX::odczytajtc(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("maxvelocity")) {
                    maxSpeed = ParserX::parsujrInside(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                ParserX::pominsekcje(data);
            }
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("name")) {
            displayName = ParserX::odczytajtc(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("brakesystemtype")) {
            brakeSystemType = ParserX::odczytajtc(data);
            ParserX::pominsekcje(data);
            continue;
        }
        ParserX::pominsekcje(data);
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

void Eng::render(int selectionColor) {
    render(0, 0, selectionColor);
}

void Eng::render(int aktwx, int aktwz, int selectionColor) {
    //gl.glTranslatef(0, 0.2f, 0);
    //qDebug() << loaded;
    if (loaded != 1) return;

    GLUU *gluu = GLUU::get();
    
     if(selectionColor != 0){
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
    } else {
        gluu->enableTextures();
    }
    
    if(sfile[0] == -2){
        sfile[0] = Game::currentShapeLib->addShape(path, sNames[0], path);
    }
    if(sfile[1] == -2){
        sfile[1] = Game::currentShapeLib->addShape(path, sNames[1], path);
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
     if(sfile[0] != -1) Game::currentShapeLib->shape[sfile[0]]->render();
     if(sfile[1] != -1) Game::currentShapeLib->shape[sfile[1]]->render();
     //gluu.mvPopMatrix();
     //
}