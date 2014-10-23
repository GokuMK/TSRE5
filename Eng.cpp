#include "Eng.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "ShapeLib.h"
#include "SFile.h"
#include <QDebug>
#include <QFile>
Eng::Eng() {
}

Eng::Eng(const Eng& orig) {
}

Eng::~Eng() {
}

Eng::Eng(QString src, QString n, int f) {
    path = src;
    name = n;
    sfile[0] = -1; sfile[1] = -1;
    sizex = 0;
    sizey = 0;
    sizez = 0;
    flip = f;
    loaded = -1;
    kierunek = false;

    load();//(src + "/" + name, this);
}

void Eng::load(){
    int i;
    QString sh;
    QString pathid = (path+"/"+name);
    pathid.replace("//","/");
    qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    data->off = 0;
    sh = "Wagon";
    ParserX::szukajsekcji1(sh, data);
    
    qDebug() << "========znaleziono sekcje " << sh << " na " << data->off;
    sh = "WagonShape";
    ParserX::szukajsekcji1(sh, data);
    QString tname = ParserX::odczytajtc(data);
    qDebug() << "=====znaleziono s " << path << tname;
    sfile[0] = ShapeLib::addShape(path, tname, path);//new Sfile(this.path, tname);

    ParserX::pominsekcjec(data);

    for(int j = 0; j<2; j++){
        i = ParserX::szukajsekcji2("freightanim", "size", data);
        //console.log(i);
        if (i == 1){
            qDebug() << "======znaleziono sekcje " << "FreightAnim" << " na ";
            tname = ParserX::odczytajtc(data);
            qDebug() << "==========znaleziono s " << path << tname;

            sfile[1] = ShapeLib::addShape(path, tname, path);//new Sfile(this.path, tname);
            ParserX::pominsekcjec(data);
        }
        if (i == 2){
            qDebug() << "jest size na ";
            //Parse.pominsekcjec(bufor);
            sizex = ParserX::parsujr(data);
            sizey = ParserX::parsujr(data);
            sizez = ParserX::parsujr(data);
            qDebug() << "wymiary taboru: " << sizex << " " << sizey << " " << sizez;
        }
    }

    loaded = 1;
    return;
}

void Eng::render() {
    render(0, 0);
}

void Eng::render(int aktwx, int aktwz) {
    //gl.glTranslatef(0, 0.2f, 0);
    if (loaded != 1) return;

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
     if(sfile[0] != -1) ShapeLib::shape[sfile[0]]->render();
     if(sfile[1] != -1) ShapeLib::shape[sfile[1]]->render();
     //gluu.mvPopMatrix();
     //
}