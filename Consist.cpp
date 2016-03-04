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

Consist::Consist() {
}

Consist::~Consist() {
}

Consist::Consist(QString p, QString n) {
    pathid = p + "/" + n;
    pathid.replace("//", "/");
    path = p;
    name = n;
    loaded = -1;
    kierunek = false;
    load();
}

Consist::Consist(QString src, QString p, QString n) {
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
    //qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    data->off = 0;
    sh = "Train";
    ParserX::szukajsekcji1(sh, data);
    sh = "TrainCfg";
    ParserX::szukajsekcji1(sh, data);
    //qDebug() << "========znaleziono sekcje " << sh << " na " << data->off;
    conName = ParserX::odczytajtc(data).trimmed();
    //qDebug() << conName;
    EngItem* eit;
    QString epath;
    QString ename;
    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("name")) {
            displayName = ParserX::odczytajtc(data).trimmed();
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("serial")) {
            serial = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("maxvelocity")) {
            maxVelocity[0] = ParserX::parsujr(data);
            maxVelocity[1] = ParserX::parsujr(data);
            //qDebug() << "wymiary taboru: " << sizex << " " << sizey << " " << sizez;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("nextwagonuid")) {
            nextWagonUID = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("durability")) {
            durability = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("engine")) {
            engItems.push_back(EngItem());
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == ("flip")) {
                    engItems.back().flip = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("uid")) {
                    engItems.back().uid = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("enginedata")) {
                    ename = ParserX::odczytajtc(data);
                    epath = ParserX::odczytajtc(data);
                    engItems.back().eng = Game::currentEngLib->addEng(Game::root + "/TRAINS/TRAINSET/" + epath, ename + ".eng");
                    ParserX::pominsekcje(data);
                    continue;
                }
                ParserX::pominsekcje(data);
            }
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("wagon")) {
            engItems.push_back(EngItem());
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == ("flip")) {
                    engItems.back().flip = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("uid")) {
                    engItems.back().uid = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("wagondata")) {
                    ename = ParserX::odczytajtc(data);
                    epath = ParserX::odczytajtc(data);
                    engItems.back().eng = Game::currentEngLib->addEng(Game::root + "/TRAINS/TRAINSET/" + epath, ename + ".wag");
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

    delete data;
    loaded = 1;
    initPos();
    return;
}

void Consist::initPos(){
    float length = 0;
    conLength = 0;
    mass = 0;
    if(engItems.size() == 0) return;
    //length += EngLib::eng[engItems[0].eng]->sizez / 2.0;
    float size = 0;
    for(int i = 0; i < engItems.size(); i++){
        engItems[i].conLength = conLength;
        if(engItems[i].eng < 0) continue;
        size = Game::currentEngLib->eng[engItems[i].eng]->getFullWidth();
        length += size / 2.0;
        engItems[i].pos = length;
        length += size / 2.0;
        conLength += size;
        mass += Game::currentEngLib->eng[engItems[i].eng]->mass;
    }
    
    posInit = true;
}

void Consist::render() {
    render(0, 0);
}

void Consist::render(int aktwx, int aktwz) {
    //gl.glTranslatef(0, 0.2f, 0);
    //qDebug() << loaded;
    if (loaded != 1) return;

    GLUU *gluu = GLUU::get();
    for(int i = 0; i < engItems.size(); i++){
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0, engItems[i].pos);
        if(!engItems[i].flip)
            Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI, 0, 1, 0);
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        Game::currentEngLib->eng[engItems[i].eng]->render(aktwx, aktwz);
        gluu->mvPopMatrix();
        //qDebug() << engItems[i].eng;
        //qDebug() << EngLib::eng[engItems[i].eng]->engName;
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