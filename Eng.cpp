#include "Eng.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "ShapeLib.h"
#include "SFile.h"
#include <QDebug>
#include <QFile>
#include "Game.h"

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
    int i;
    QString sh;
    pathid.replace("//","/");
    //qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    data->off = 0;
    sh = "Wagon";
    ParserX::szukajsekcji1(sh, data);
    //qDebug() << "========znaleziono sekcje " << sh << " na " << data->off;
    engName = ParserX::odczytajtc(data).trimmed();
    displayName = engName;
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << sh;
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
            sizex = ParserX::parsujr(data);
            sizey = ParserX::parsujr(data);
            sizez = ParserX::parsujr(data);
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
            mass = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("coupling")) {
            coupling.push_back(Coupling());
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == ("type")) {
                    coupling.back().type = ParserX::odczytajtc(data);
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
                            coupling.back().r0[0] = ParserX::parsujr(data);
                            coupling.back().r0[1] = ParserX::parsujr(data);
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
            ParserX::pominsekcje(data);
            continue;
        }
        
        ParserX::pominsekcje(data);
    }
    typeHash = type;
    if(type.toLower() == "engine"){
        sh = "Engine";
        ParserX::szukajsekcji1(sh, data);
        //qDebug() << "========znaleziono sekcje " << sh << " na " << data->off;
        ParserX::odczytajtc(data);
        while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
            //qDebug() << sh;
            if (sh == ("type")) {
                engType = ParserX::odczytajtc(data);
                typeHash+="-"+engType;
                //qDebug() << engType;
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("name")) {
                displayName = ParserX::odczytajtc(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if (sh == ("maxvelocity")) {
                maxSpeed = ParserX::parsujr(data);
                ParserX::pominsekcje(data);
                continue;
            }
            ParserX::pominsekcje(data);
        }
    }
    //qDebug() << typeHash;
    delete data;
    loaded = 1;
    //qDebug() << loaded;
    return;
}

float Eng::getFullWidth(){
    if(this->coupling.size() > 0)
        return this->coupling[0].r0[0] + this->sizez;
    else
        return this->sizez + 0.0;
}

void Eng::render() {
    render(0, 0);
}

void Eng::render(int aktwx, int aktwz) {
    //gl.glTranslatef(0, 0.2f, 0);
    //qDebug() << loaded;
    if (loaded != 1) return;

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