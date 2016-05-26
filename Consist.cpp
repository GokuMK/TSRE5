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

std::unordered_map<int, TextObj*> Consist::txtNumbers;
TextObj * Consist::txtEngineE = NULL;
TextObj * Consist::txtEngineD = NULL;
TextObj * Consist::txtEngineS = NULL;
TextObj * Consist::txtEngineF = NULL;
TextObj * Consist::txtEngineW = NULL;
TextObj * Consist::txtEngineT = NULL;

Consist::Consist() {
    path = Game::root + "/trains/consists/";
    loaded = 1;
    serial = 1;
    durability = 1;
    initPos();
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
    qDebug() << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    data->off = 0;
    sh = "Train";
    ParserX::szukajsekcji1(sh, data);
    //qDebug() << data->off << " " << data->length;
    sh = "TrainCfg";
    int ok = ParserX::szukajsekcji1(sh, data);
    if(ok == 0) return;
    //qDebug() << "========znaleziono sekcje " << sh << " na " << data->off;
    conName = ParserX::odczytajtc(data).trimmed();
    showName = conName;
    //qDebug() << conName;
    EngItem* eit;

    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("name")) {
            displayName = ParserX::odczytajtc(data).trimmed();
            showName = displayName;
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("serial")) {
            serial = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
            continue;
        }
        if (sh == ("default")) {
            defaultValue = true;
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
            engItems.back().type = 1;
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == ("flip")) {
                    engItems.back().flip = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("uid")) {
                    engItems.back().uid = ParserX::parsujr(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("enginedata")) {
                    engItems.back().ename = ParserX::odczytajtc(data);
                    engItems.back().epath = ParserX::odczytajtc(data);
                    engItems.back().eng = Game::currentEngLib->addEng(Game::root + "/TRAINS/TRAINSET/" + engItems.back().epath, engItems.back().ename + ".eng");
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
            engItems.back().type = 0;
            while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
                if (sh == ("flip")) {
                    engItems.back().flip = true;
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("uid")) {
                    engItems.back().uid = ParserX::parsujr(data);
                    ParserX::pominsekcje(data);
                    continue;
                }
                if (sh == ("wagondata")) {
                    engItems.back().ename = ParserX::odczytajtc(data);
                    engItems.back().epath = ParserX::odczytajtc(data);
                    engItems.back().eng = Game::currentEngLib->addEng(Game::root + "/TRAINS/TRAINSET/" + engItems.back().epath, engItems.back().ename + ".wag");
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
    emass = 0;
    if(engItems.size() == 0) {
        posInit = true;
        return;
    }
    //length += EngLib::eng[engItems[0].eng]->sizez / 2.0;
    float size = 0;
    float tmaxspeed = 0;
    //float tengmass = 0;
    for(int i = 0; i < engItems.size(); i++){
        engItems[i].conLength = conLength;
        if(engItems[i].eng < 0) continue;
        size = Game::currentEngLib->eng[engItems[i].eng]->getFullWidth();
        length += size / 2.0;
        engItems[i].pos = length;
        length += size / 2.0;
        conLength += size;
        mass += Game::currentEngLib->eng[engItems[i].eng]->mass;
        if(Game::currentEngLib->eng[engItems[i].eng]->wagonTypeId > 4){
            if(Game::currentEngLib->eng[engItems[i].eng]->maxSpeed > tmaxspeed)
                tmaxspeed = Game::currentEngLib->eng[engItems[i].eng]->maxSpeed;
            emass += Game::currentEngLib->eng[engItems[i].eng]->mass;
        }
    }

    this->maxVelocity[0] = tmaxspeed / 3.6;
    this->maxVelocity[1] = emass / mass;
    
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

void Consist::select(int idx){
    selectedIdx = idx;
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

void Consist::appendEngItem(int id){
    Eng * eng = Game::currentEngLib->eng[id];
    if(eng == NULL) return;
    engItems.push_back(EngItem());
    engItems.back().type = eng->wagonTypeId / 4;
    engItems.back().eng = id;
    engItems.back().ename = eng->name.split(".")[0];
    engItems.back().epath = eng->path.split("/").last();
    engItems.back().uid = this->nextWagonUID++;
    engItems.back().flip = false;

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
    this->name = n;
    modified = true;
}
void  Consist::setDisplayName(QString n){
    this->displayName = n;
    modified = true;
}

void Consist::render(int selectionColor) {
    render(0, 0, selectionColor);
}

void Consist::render(int aktwx, int aktwz, int selectionColor) {
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
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(selectionColor != 0)
            scolor = selectionColor + i;
        if(selectedIdx == i)
            Game::currentEngLib->eng[engItems[i].eng]->drawBorder();
        Game::currentEngLib->eng[engItems[i].eng]->render(aktwx, aktwz, scolor);
        gluu->mvPopMatrix();
        
        if(selectionColor != 0)
            continue;
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, -1, engItems[i].pos);
        Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI/2, 0, 1, 0);
        Mat4::identity(gluu->objStrMatrix);
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
        if(engItems[i].txt == NULL){
            engItems[i].txt = new TextObj(Game::currentEngLib->eng[engItems[i].eng]->displayName, 16, 1.0);
            engItems[i].txt->setColor(255,255,0);
            if(Game::systemTheme)
                engItems[i].txt->setColor(0,0,0);
        }        
        engItems[i].txt->render();
        gluu->mvPopMatrix();
        
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 5, engItems[i].pos);
        //Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, M_PI/2, 0, 1, 0);
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(txtNumbers[i] == NULL){
            txtNumbers[i] = new TextObj(i+1);
            txtNumbers[i]->setColor(255,255,0);
            if(Game::systemTheme)
                txtNumbers[i]->setColor(0,0,0);
        }
        txtNumbers[i]->render(M_PI/2);
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0, -1);
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        int wt = Game::currentEngLib->eng[engItems[i].eng]->wagonTypeId;
        if(wt == 1){
            if(txtEngineW == NULL){
                txtEngineW = new TextObj("C");
                txtEngineW->setColor(255,255,0);
                if(Game::systemTheme)
                    txtEngineW->setColor(0,0,0);
            }
            txtEngineW->render(M_PI/2);
        }
        if(wt == 2){
            if(txtEngineF == NULL){
                txtEngineF = new TextObj("F");
                txtEngineF->setColor(255,255,0);
                if(Game::systemTheme)
                    txtEngineF->setColor(0,0,0);
            }
            txtEngineF->render(M_PI/2);
        }
        if(wt == 3){
            if(txtEngineT == NULL){
                txtEngineT = new TextObj("T");
                txtEngineT->setColor(255,255,0);
                if(Game::systemTheme)
                    txtEngineT->setColor(0,0,0);
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

bool Consist::isNewConsist(){
    return newConsist;
}
void Consist::setNewConsistFlag(){
    newConsist = true;
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
    out << "	TrainCfg ( \""<<conName<<"\"\n";
    if(displayName.length() > 0)
    out << "		Name ( \"" << displayName << "\" )\n";
    if(serial > 0 )
    out << "		Serial ( "<<serial<<" )\n";
    if(defaultValue)
    out << "		Default ( )\n";
    out << "		MaxVelocity ( " << maxVelocity[0] << " " << maxVelocity[1] << " )\n";
    out << "		NextWagonUID ( " << nextWagonUID << " )\n";
    out << "		Durability ( " << durability << " )\n";

    for(int i = 0; i < this->engItems.size(); i++){
        if(this->engItems[i].type == 1){
            out << "		Engine (\n";
            if(this->engItems[i].flip)
            out << "			Flip ( )\n";
            out << "			UiD ( " << engItems[i].uid << " )\n";
            out << "			EngineData ( " << ParserX::addComIfReq(engItems[i].ename) << " " << ParserX::addComIfReq(engItems[i].epath) << " )\n";
            out << "		)\n";
        } 
        if(this->engItems[i].type == 0){
            out << "		Wagon (\n";
            out << "			WagonData ( " << ParserX::addComIfReq(engItems[i].ename) << " " << ParserX::addComIfReq(engItems[i].epath) << " )\n";
            if(this->engItems[i].flip)
            out << "			Flip ( )\n";
            out << "			UiD ( " << engItems[i].uid << " )\n";
            out << "		)\n";
        }
    }
    out << "	)\n";
    out << ")\n";
    
    file.close(); 
    newConsist = false;
    modified = false;
}
