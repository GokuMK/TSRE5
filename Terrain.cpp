/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Terrain.h"
#include <QDebug>
#include "Game.h"
#include <QFile>
#include "ReadFile.h"
#include "TexLib.h"
#include "TerrainLib.h"
#include "GLMatrix.h"
#include "Brush.h"
#include "TerrainWaterWindow.h"
#include "MapWindow.h"
#include "Route.h"
#include "Environment.h"
#include "AboutWindow.h"
#include "TerrainInfo.h"

QString Terrain::TileDir[2] = {"tiles", "lo_tiles"};
Brush* Terrain::DefaultBrush = NULL;

Terrain::Terrain(TerrainInfo *ti){
    mojex = ti->cx;
    mojez = -ti->cy;
    name = ti->name;
    lowTile = ti->low;
    load();
}

Terrain::Terrain(float x, float y) {
    mojex = x;
    mojez = y;
    name = getTileName((int) x, (int) -y);
    load();
}

void Terrain::load(){
    typeObj = this->terrainobj;
    loaded = false;
    isOgl = false;
    modified = false;
    wTexid = -1;
    for (int i = 0; i < 256; i++) {
        texid[i] = -1;
        texid2[i] = -1;
        hidden[i] = false;
        texModified[i] = false;
        texLocked[i] = false;
        uniqueTex[i] = false;
        selectedPatchs[i] = false;
    }
    VBO = new QOpenGLBuffer();
    VAO = new QOpenGLVertexArrayObject();

    texturepath = Game::root + "/routes/" + Game::route + "/terrtex/";
    QString path = Game::root + "/routes/" + Game::route + "/" + TileDir[(int)lowTile] + "/";
    tfile = new TFile();

    //QString filename = getTileName((int) x, (int) -y);
    //QString filename = getTileNameExperimental2((int) x, (int) -y);
    //qDebug() << filename << x << -y;
    if (!tfile->readT((path + name + ".t"))) {
        //qDebug() << " t fail" << name;
        return;
    }
    if(tfile->sampleYbuffer == NULL)
        return;
    if (!readRAW((path + *tfile->sampleYbuffer/* + "_y.raw"*/))) {
        //qDebug() << " y fail" << name;
        return;
    }
    if(tfile->sampleFbuffer != NULL)
        jestF = readF(path + *tfile->sampleFbuffer/* + "_f.raw"*/);
    modifiedF = false;
    //qDebug() << " ok";
    
    //QString name = this->getTileName(mojex, -mojez);
    QString name2;
    //int samples = 
    int patches = tfile->patchsetNpatches;
    for (int u = 0; u < patches; u++)
        for (int y = 0; y < patches; y++) {
            name2 = name + "_" + QString::number(y) + "_" + QString::number(u) + ".ace";
            //qDebug() << name2 << (int) tfile->tdata[(y * 16 + u)*13 + 0 + 6];
            //qDebug() << tfile->materialsCount;
            //qDebug() << tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0];
            //qDebug() << name << patches << tfile->materialsCount << (int) tfile->tdata[(y * patches + u)*13 + 0 + 6];
            if (tfile->materialsCount <= (int) tfile->tdata[(y * patches + u)*13 + 0 + 6])
                continue;
            
            if (name2 == *tfile->materials[(int) tfile->tdata[(y * patches + u)*13 + 0 + 6]].tex[0])
                this->uniqueTex[y*patches+u] = true;
        }
    
    loaded = true;
    //save();
}

bool Terrain::isModified() {
    return this->modified;
}

void Terrain::setModified(bool value) {
    this->modified = value;
}

float Terrain::setHeight(int x, int z, float posx, float posz, float val, bool add){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;

    posx -= 2048 * (mojex-x) - 1024;
    posz -= 2048 * (mojez-z) + 1024;
    posz = tileSize + posz;
    
    if(add)
        terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize] += val;
    else
        terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize] = val;
    setErrorBias(x, z, posx, posz, 0);
    setModified(true);
    return terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize];
}

void Terrain::getLowCornerTileXY(int& X, int& Y){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int tileSize = sampleSize*samples;
    int level = tileSize/2048;
    
    X = mojex;// - level + 1;
    Y = mojez - level + 1;
}


void Terrain::getCornerCoordsXY(int &x, int &z, int ox, int oz){
    x = mojex;
    z = mojez;
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int tileSize = sampleSize*samples;
    int level = tileSize/2048;
    
    //if(level == 1){
        x += level*ox;
        z += level*oz;
    //    return;
    //}
    /*if(ox == -1)
        x -= level;
    if(ox == 1)
        x += level+1;
    if(oz == -1)
        z -= level;
    if(oz == 1)
        z += level;*/
    return;
}

void Terrain::fillTerrainDataX(Terrain* adjacent){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int tileSize = sampleSize*samples;
    
    if(sampleSize != adjacent->getSampleSize())
        return;
    if(samples != adjacent->getSampleCount())
        return;
    
    for (int i = 0; i < samples; i++) {
        terrainData[i][samples] = adjacent->terrainData[i][0];
    }
}

void Terrain::fillTerrainDataY(Terrain* adjacent){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int tileSize = sampleSize*samples;
    
    if(sampleSize != adjacent->getSampleSize())
        return;
    if(samples != adjacent->getSampleCount())
        return;
    
    for (int i = 0; i < samples; i++) {
        terrainData[samples][i] = adjacent->terrainData[0][i];
    }
}

void Terrain::fillTerrainDataXY(Terrain* adjacent){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int tileSize = sampleSize*samples;
    
    if(sampleSize != adjacent->getSampleSize())
        return;
    if(samples != adjacent->getSampleCount())
        return;
    
    terrainData[samples][samples] = adjacent->terrainData[0][0];
}
    
int Terrain::getSampleCount(){
    return *tfile->nsamples;
}

void Terrain::setFixedHeight(float val){
    if (loaded == false) 
        return;
    int samples = *tfile->nsamples;
    for (int i = 0; i < samples; i++)
        for (int j = 0; j < samples; j++) {
            terrainData[i][j] = val;
        }
    refresh();
    setModified(true);
}

Terrain::Terrain(const Terrain& orig) {
}

Terrain::~Terrain() {
    long timeNow1 = QDateTime::currentMSecsSinceEpoch();
    if (this->loaded) {
        for (int i = 0; i < 257; i++) {
            delete[] terrainData[i];
            if (this->jestF)
                delete[] fData[i];
        }
        //for (int i = 0; i < 256; i++) {
        //    //delete VBO[i];
        //    //delete VAO[i];
        //    //GC::VBO.push_back(VBO[i]);
        //    //GC::VAO.push_back(VAO[i]);
        //}
        if(VBO != NULL)
            delete VBO;
        if(VAO != NULL)
            delete VAO;
        //delete[] VBO;
        //delete[] VAO;

        delete[] terrainData;
        if (this->jestF)
            delete[] fData;
    }
    long timeNow2 = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "= release terrain "<< timeNow2 - timeNow1;
}

void Terrain::SaveEmpty(QString name, int samples, int sampleSize, int patches, bool low) {
    qDebug() << "New terrain tile";
    QString path;
    if(low)
        path = Game::root + "/routes/" + Game::route + "/lo_tiles/" + name + "_y.raw";
    else
        path = Game::root + "/routes/" + Game::route + "/tiles/" + name + "_y.raw";
    QFile file(path);
    if (!file.exists()){
        file.open(QIODevice::WriteOnly);
        QDataStream write(&file);
        write.setByteOrder(QDataStream::BigEndian);
        unsigned short value = 128;
        for (int i = 0; i < samples; i++)
            for (int j = 0; j < samples; j++)
                write << value;
        file.close();
    }
    
    TFile *tfile = new TFile();
    tfile->initNew(name, samples, sampleSize, patches);
    if(low){
        if(!QDir(Game::root + "/routes/" + Game::route + "/lo_tiles/").exists())
            QDir().mkdir(Game::root + "/routes/" + Game::route + "/lo_tiles/");
        tfile->save(Game::root + "/routes/" + Game::route + "/lo_tiles/" + name + ".t");
    } else {
        tfile->save(Game::root + "/routes/" + Game::route + "/tiles/" + name + ".t");
    }
}

QString Terrain::getTileName(){
    return name;
    //return getTileName(mojex, -mojez);
}

QString Terrain::getTileNameExperimental(int x, int y){
    int offset = 16384;
    x += offset;
    y += offset;
    unsigned int numer = 0;
    for (int tx = offset, ty = tx, i = tx, sign_xs, sign_ys; i > 0; i /= 2, tx += i, ty += i, numer <<= 2) {
        sign_xs = ((unsigned)(x-tx)>>31);
        sign_ys = ((unsigned)(y-ty)>>31);
        numer |= (sign_ys*2+!(sign_xs^sign_ys));
        tx -= sign_xs*i;
        ty -= sign_ys*i;
    }
    QString name = QString::number(numer, 16);
    int len = 8 - name.length();
    for (int i = 0; i < len; i++)
        name = "0" + name;
    //qDebug() << name;
    return "-" + name;
}

QString Terrain::getTileNameExperimental2(int x, int y){
    int o = 16384;
    x += o;
    y += o;
    unsigned int n = 0;
    for (int tx=o, ty=tx, i=tx, sx, sy; i>0; sx=((unsigned)(x-tx)>>31), sy=((unsigned)(y-ty)>>31), n|=(sy*2+!(sx^sy)), tx-=sx*i-i/2, ty-=sy*i-i/2, i/=2, n<<=2);
    QString name = QString::number(n, 16);
    int len = 8 - name.length();
    for (int i = 0; i < len; i++)
        name = "0" + name;
    return "-" + name;
}

QString Terrain::getTileName(int x, int y) {
    int offset = 16384;
    int xs = offset;
    int ys = offset;
    x += offset;
    y += offset;
    unsigned int numer = 0;
    
    for (int i = offset / 2, j = 30; j > 0; i /= 2, j -= 2) {
        if (x < xs && y < ys) {
            numer = numer | (3 << j);
            xs -= i;
            ys -= i;
        } else if (x < xs) {
            xs -= i;
            ys += i;
        } else if (y < ys) {
            numer = numer | (2 << j);
            xs += i;
            ys -= i;
        } else {
            numer = numer | (1 << j);
            xs += i;
            ys += i;
        }
    }

    QString name = QString::number(numer, 16);
    int len = 8 - name.length();
    for (int i = 0; i < len; i++)
        name = "0" + name;
    //qDebug() << name;
    return "-" + name;
}

void Terrain::refresh() {
    if (!loaded) return;
    isOgl = false;
    lines.loaded = false;
    //reloadLines();
}

void Terrain::toggleGaps(int x, int z, float posx, float posz, float direction){
    if(!jestF)
        newF();
    int tx = (posz + 1024) / 8;
    int tz = (posx + 1024) / 8;
    if(tx > 256 || tx < 0)
        return;
    if(tz > 256 || tz < 0)
        return;
    if(direction == 0)
        fData[tx][tz] ^= 0x04;
    if(direction == 1)
        fData[tx][tz] |= 0x04;
    if(direction == -1){
        for(int j = -1; j <= 1; j++)
            for(int i = -1; i <= 1; i++){
                if(tx+j > 256 || tx+j < 0)
                    return;
                if(tz+i > 256 || tz+i < 0)
                    return;
                fData[tx+j][tz+i] &= ~0x04;
            }
    }
    modifiedF = true;
    modified = true;
    refresh();
}

int Terrain::getTexture(int x, int z, float posx, float posz) {
    getPatchCoords(x, z, posx, posz);
    int patches = tfile->patchsetNpatches;

    return texid[z * patches + x];
}

void Terrain::convertTexToDefaultCoords(int idx) {
    /*float x11 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y11 = (0) * tfile->tdata[(idx)*13 + 5 + 6] + (0) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    //qDebug() << x11 << " " <<y11;
    float x21 = (16) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y21 = (16) * tfile->tdata[(idx)*13 + 5 + 6] + (0) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    //qDebug() << x21 << " " <<y21;
    float x12 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (16) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y12 = (0) * tfile->tdata[(idx)*13 + 5 + 6] + (16) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    //qDebug() << x12 << " " <<y12;
    float x22 = (16) * tfile->tdata[(idx)*13 + 3 + 6] + (16) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y22 = (16) * tfile->tdata[(idx)*13 + 5 + 6] + (16) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    //qDebug() << x22 << " " <<y22;
    float t;*/
    /*if ((x11 < x21) && (y11 == y21)) {
        qDebug() << "rot1 - ok";
    } else {
        qDebug() << "rot";
        TexLib::mtex[texid[idx]]->crop(x11, y11, x22, y22);
        TexLib::mtex[texid[idx]]->advancedCrop((float*)&tfile->tdata[(idx)*13 + 6]);
    }*/
    if((fabs(tfile->tdata[(idx)*13 + 1 + 6] - 0.001)
      +fabs(tfile->tdata[(idx)*13 + 2 + 6] - 0.001)
      +fabs(tfile->tdata[(idx)*13 + 3 + 6] - 0.062375)
      +fabs(tfile->tdata[(idx)*13 + 4 + 6] - 0.0)
      +fabs(tfile->tdata[(idx)*13 + 5 + 6] - 0.0)
      +fabs(tfile->tdata[(idx)*13 + 6 + 6] - 0.062375)
      ) < 0.01){
        qDebug() << "rot ok";
        return;
    }
    qDebug() << "rot ->";
    
    TexLib::mtex[texid[idx]]->advancedCrop((float*)&tfile->tdata[(idx)*13 + 6], 512, 512);
    tfile->tdata[(idx)*13 + 1 + 6] = 0.001;
    tfile->tdata[(idx)*13 + 2 + 6] = 0.001;
    tfile->tdata[(idx)*13 + 3 + 6] = 0.062375;
    tfile->tdata[(idx)*13 + 4 + 6] = 0.0;
    tfile->tdata[(idx)*13 + 5 + 6] = 0.0;
    tfile->tdata[(idx)*13 + 6 + 6] = 0.062375;
    this->refresh();
}

void Terrain::resetPatchTexCoords(int uu){
    if( uu >= 0 && uu < 256){
        tfile->tdata[(uu)*13 + 1 + 6] = 0.001;
        tfile->tdata[(uu)*13 + 2 + 6] = 0.001;
        tfile->tdata[(uu)*13 + 3 + 6] = 0.062375;
        tfile->tdata[(uu)*13 + 4 + 6] = 0.0;
        tfile->tdata[(uu)*13 + 5 + 6] = 0.0;
        tfile->tdata[(uu)*13 + 6 + 6] = 0.062375;
    } else {
        for (uu = 0; uu < 256; uu++) {
            if(selectedPatchs[uu]){
                tfile->tdata[(uu)*13 + 1 + 6] = 0.001;
                tfile->tdata[(uu)*13 + 2 + 6] = 0.001;
                tfile->tdata[(uu)*13 + 3 + 6] = 0.062375;
                tfile->tdata[(uu)*13 + 4 + 6] = 0.0;
                tfile->tdata[(uu)*13 + 5 + 6] = 0.0;
                tfile->tdata[(uu)*13 + 6 + 6] = 0.062375;
            }
        }
    }
    modified = true;
    this->refresh();
}

void Terrain::rotateTex(int idx) {

    float x11 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y11 = (0) * tfile->tdata[(idx)*13 + 5 + 6] + (0) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    qDebug() << x11 << " " << y11;
    float x21 = (16) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y21 = (16) * tfile->tdata[(idx)*13 + 5 + 6] + (0) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    qDebug() << x21 << " " << y21;
    float x12 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (16) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y12 = (0) * tfile->tdata[(idx)*13 + 5 + 6] + (16) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    qDebug() << x12 << " " << y12;
    float x22 = (16) * tfile->tdata[(idx)*13 + 3 + 6] + (16) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y22 = (16) * tfile->tdata[(idx)*13 + 5 + 6] + (16) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    qDebug() << x22 << " " << y22;
    float t;
    if ((x11 < x21) && (y11 == y21)) {
        qDebug() << "rot1";
        tfile->tdata[(idx)*13 + 1 + 6] = x12;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = -tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = -t;
    } else if ((x11 == x21) && (y11 < y21)) {
        qDebug() << "rot2";
        tfile->tdata[(idx)*13 + 2 + 6] = y21;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = -tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = -t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = t;
    } else if ((x11 > x21) && (y11 == y21)) {
        qDebug() << "rot3";
        tfile->tdata[(idx)*13 + 1 + 6] = x12;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = -tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = -t;
    } else if ((x11 == x21) && (y11 > y21)) {
        qDebug() << "rot4";
        tfile->tdata[(idx)*13 + 2 + 6] = y21;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = -tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = -t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = t;
    }
    this->refresh();
}

void Terrain::mirrorXTex(int idx){
    float x21 = (16) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y21 = (16) * tfile->tdata[(idx)*13 + 5 + 6] + (0) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    tfile->tdata[(idx)*13 + 1 + 6] = x21;
    tfile->tdata[(idx)*13 + 2 + 6] = y21;
    tfile->tdata[(idx)*13 + 3 + 6] = -tfile->tdata[(idx)*13 + 3 + 6];
    tfile->tdata[(idx)*13 + 5 + 6] = -tfile->tdata[(idx)*13 + 5 + 6];
    modified = true;
    this->refresh();
}

void Terrain::mirrorYTex(int idx){
    float x12 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (16) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
    float y12 = (0) * tfile->tdata[(idx)*13 + 5 + 6] + (16) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
    tfile->tdata[(idx)*13 + 1 + 6] = x12;
    tfile->tdata[(idx)*13 + 2 + 6] = y12;
    tfile->tdata[(idx)*13 + 4 + 6] = -tfile->tdata[(idx)*13 + 4 + 6];
    tfile->tdata[(idx)*13 + 6 + 6] = -tfile->tdata[(idx)*13 + 6 + 6];
    modified = true;
    this->refresh();
}

float Terrain::getScaleTexX(int idx){
    return fabs(tfile->tdata[(idx)*13 + 3 + 6] + tfile->tdata[(idx)*13 + 4 + 6])/0.062375;
}

float Terrain::getScaleTexY(int idx){
    return fabs(tfile->tdata[(idx)*13 + 5 + 6] + tfile->tdata[(idx)*13 + 6 + 6])/0.062375;
}

float Terrain::getScaleTex(int idx){
    return (getScaleTexX(idx) + getScaleTexY(idx)) / 2.0;
}

void Terrain::scaleTex(int idx, float val){
    float s = getScaleTexX(idx);
    float val1 = val/s;
    tfile->tdata[(idx)*13 + 3 + 6] *= val1;
    tfile->tdata[(idx)*13 + 4 + 6] *= val1;
    s = getScaleTexY(idx);
    val1 = val/s;
    tfile->tdata[(idx)*13 + 5 + 6] *= val1;
    tfile->tdata[(idx)*13 + 6 + 6] *= val1;
    modified = true;
    this->refresh();
}

void Terrain::scaleTexX(int idx, float val){
    float s = getScaleTexX(idx);
    val = val/s;
    tfile->tdata[(idx)*13 + 3 + 6] *= val;
    tfile->tdata[(idx)*13 + 4 + 6] *= val;
    modified = true;
    this->refresh();
}

void Terrain::scaleTexY(int idx, float val){
    float s = getScaleTexY(idx);
    val = val/s;
    tfile->tdata[(idx)*13 + 5 + 6] *= val;
    tfile->tdata[(idx)*13 + 6 + 6] *= val;
    modified = true;
    this->refresh();
}

void Terrain::setTileBlob(){
    if(this->showBlob){
        this->showBlob = false;
        return;
    } 
    int X, Y;
    getLowCornerTileXY(X, Y);
    int hash = (X*10000+Y);
    qDebug() << hash;
    if(MapWindow::mapTileImages[hash] != NULL)
        this->showBlob = true;
    else {
        if(MapWindow::LoadMapFromDisk(X, Y)){
            this->showBlob = true;
        } else {
            qDebug() << "load map first!";
        }
    }
}

void Terrain::makeTextureFromMap(){
    int X, Y;
    getLowCornerTileXY(X, Y);
    int hash = (X*10000+Y);
    qDebug() << hash;
    if(MapWindow::mapTileImages[hash] == NULL){
        qDebug() << "mat tex not found";
        return;
    }
    QString path = QString::number(hash)+".:maptex";
    QString tname = name + "_map.ace";
    int mapTexid = TexLib::addTex(path);
    if(!TexLib::mtex[mapTexid]->loaded){
        qDebug() << "mat tex not loaded";
        return;
    }
    
    int newMat = tfile->getMatByTexture(tname);
    if(newMat >= 0){
        qDebug() << "material already exist";
    } else {
        newMat = tfile->newMat();
    }
    *tfile->materials[newMat].tex[0] = tname;
    *tfile->amaterials[newMat].tex[0] = tname;
    float *texmult = (float*)&tfile->materials[newMat].itex[1][3];
    *texmult = 32*16;

    int newTexture = TexLib::cloneTex(mapTexid);
    TexLib::mtex[newTexture]->pathid = tname;
    TexLib::save("ace", texturepath + tname, newTexture);
    int patches = tfile->patchsetNpatches;
    float texstep = 1.0/patches;
    for(int i = 0; i < patches; i++)
        for(int j = 0; j < patches; j++){
            texid[j * patches + i] = newTexture;
            tfile->tdata[(j * patches + i)*13 + 0 + 6] = newMat;
            tfile->tdata[(j * patches + i)*13 + 1 + 6] = texstep*i;
            tfile->tdata[(j * patches + i)*13 + 2 + 6] = texstep*j;
            tfile->tdata[(j * patches + i)*13 + 3 + 6] = texstep/patches;
            tfile->tdata[(j * patches + i)*13 + 4 + 6] = 0;
            tfile->tdata[(j * patches + i)*13 + 5 + 6] = 0;
            tfile->tdata[(j * patches + i)*13 + 6 + 6] = texstep/patches;
            //TexLib::mtex[texid[j * 16 + i]]->pathid = name;
        }

    refresh();
    this->modified = true;
}

void Terrain::removeTextureFromMap(){
    QString name = this->getTileName(mojex, -mojez) + "_map.ace";
    int newMat = tfile->getMatByTexture(name);
    if(newMat <= 0){
        return;
    }
    tfile->removeMat(newMat);
    for (int i = 0; i < 256; i++) {
        texid[i] = -1;
    }    
    refresh();
    this->modified = true;
}

void Terrain::setWaterDraw() {
    int patches = tfile->patchsetNpatches;
    for (int uu = 0; uu < patches*patches; uu++) {
        if(selectedPatchs[uu]){
            tfile->flags[uu] = tfile->flags[uu] | 0x10000c0;
            this->setModified(true);
        }
    }
}

void Terrain::toggleWaterDraw() {
    int patches = tfile->patchsetNpatches;
    for (int uu = 0; uu < patches*patches; uu++) {
        if(selectedPatchs[uu]){
            tfile->flags[uu] = tfile->flags[uu] ^ 0x10000c0;
            this->setModified(true);
        }
    }
}

void Terrain::hideWaterDraw() {
    int patches = tfile->patchsetNpatches;
    for (int uu = 0; uu < patches*patches; uu++) {
        if(selectedPatchs[uu]){
            tfile->flags[uu] = tfile->flags[uu] & ~0x10000c0;
            this->setModified(true);
        }
    }
}

void Terrain::toggleWaterDraw(int x, int z, float posx, float posz, float direction) {
    getPatchCoords(x, z, posx, posz);
    int patches = tfile->patchsetNpatches;
    if(direction == 0)
        tfile->flags[z * patches + x] = tfile->flags[z * patches + x] ^ 0x10000c0;
    if(direction == 1)
        tfile->flags[z * patches + x] = tfile->flags[z * patches + x] | 0x10000c0;
    if(direction == -1)
        tfile->flags[z * patches + x] = tfile->flags[z * patches + x] & ~0x10000c0;
    this->setModified(true);
}

void Terrain::setDrawAdjacent(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            int u = uu / 16;
            int y = uu - u*16;
            for(int i = u - 1; i <= u+1; i++)
                for(int j = y - 1; j <= y+1; j++){
                    if(i < 0 || j < 0 || i > 15 || j > 15)
                        continue;
                    tfile->flags[i*16+j] = tfile->flags[i*16+j] & ~(0x1);
                }
                this->setModified(true);
        }
    }
}

void Terrain::rotatePatchTexture(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            rotateTex(uu);
        }
    }
}

void Terrain::mirrorXPatchTexture(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            mirrorXTex(uu);
        }
    }
}

void Terrain::mirrorYPatchTexture(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            mirrorYTex(uu);
        }
    }
}

float Terrain::getPatchScaleTex(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return getScaleTex(uu);
        }
    }
    return 0;
}

float Terrain::getPatchScaleTexX(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return getScaleTexX(uu);
        }
    }
    return 0;
}

float Terrain::getPatchScaleTexY(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return getScaleTexY(uu);
        }
    }
    return 0;
}

QString Terrain::getPatchRotationName(){
    for (int idx = 0; idx < 256; idx++)
        if(selectedPatchs[idx]){
            float x11 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
            float y11 = (0) * tfile->tdata[(idx)*13 + 5 + 6] + (0) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
            float x21 = (16) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
            float y21 = (16) * tfile->tdata[(idx)*13 + 5 + 6] + (0) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
            float x12 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (16) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
            float y12 = (0) * tfile->tdata[(idx)*13 + 5 + 6] + (16) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
            float x22 = (16) * tfile->tdata[(idx)*13 + 3 + 6] + (16) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
            float y22 = (16) * tfile->tdata[(idx)*13 + 5 + 6] + (16) * tfile->tdata[(idx)*13 + 6 + 6] + tfile->tdata[(idx)*13 + 2 + 6];
            if ((x11 < x21) && (y11 == y21)) {
                return QString("0°");
            } else if ((x11 == x21) && (y11 < y21)) {
                return QString("270°");
            } else if ((x11 > x21) && (y11 == y21)) {
                return QString("180°");
            } else if ((x11 == x21) && (y11 > y21)) {
                return QString("90°");
            } else {
                return QString("UNDEFINED");
            }
        }
    return QString("UNDEFINED");
}

void Terrain::scalePatchTexCoords(float val){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            scaleTex(uu, val);
        }
    }
}

void Terrain::scalePatchTexCoordsX(float val){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            scaleTexX(uu, val);
        }
    }
}

void Terrain::scalePatchTexCoordsY(float val){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            scaleTexY(uu, val);
        }
    }
}

QString Terrain::getPatchTexTransformString(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return QString::number(tfile->tdata[(uu)*13 + 1 + 6]) + " "+
                   QString::number(tfile->tdata[(uu)*13 + 2 + 6]) + " "+
                   QString::number(tfile->tdata[(uu)*13 + 3 + 6]) + " "+
                   QString::number(tfile->tdata[(uu)*13 + 4 + 6]) + " "+
                   QString::number(tfile->tdata[(uu)*13 + 5 + 6]) + " "+
                   QString::number(tfile->tdata[(uu)*13 + 6 + 6]);
        }
    }
    return "";
}

void Terrain::setPatchTexTransform(QString val){
    QStringList list = val.split(" ");
    if(list.size() != 6)
        return;
    
    float t[6];
    bool ok;
    for (int i = 0; i < 6; i++){
        t[i] = list[i].toFloat(&ok);
        if(!ok)
            return;
    }
    
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            for (int i = 0; i < 6; i++)
                tfile->tdata[(uu)*13 + i + 1 + 6] = t[i];
        }
    }
    modified = true;
    this->refresh();
}
    
void Terrain::removeAllGaps(){
    if(!jestF)
        return;
    int patches = tfile->patchsetNpatches;
    for (int uu = 0; uu < patches*patches; uu++) {
        if(selectedPatchs[uu]){
            int u = uu / patches;
            int y = uu - u*patches;
            // to do qt
            for(int i = 0; i < 16; i++)
                for(int j = 0; j < 16; j++)
                    fData[u*16+i][y*16+j] &= ~(0x04);
            modifiedF = true;
            modified = true;
        }
    }
    refresh();
}
    
void Terrain::setDraw() {
    int patches = tfile->patchsetNpatches;
    for (int uu = 0; uu < patches*patches; uu++) {
        if(selectedPatchs[uu]){
            tfile->flags[uu] = tfile->flags[uu] & ~0x1;
            this->setModified(true);
        }
    }
}

void Terrain::toggleDraw() {
    int patches = tfile->patchsetNpatches;
    for (int uu = 0; uu < patches*patches; uu++) {
        if(selectedPatchs[uu]){
            tfile->flags[uu] = tfile->flags[uu] ^ 0x1;
            this->setModified(true);
        }
    }
}

void Terrain::hideDraw() {
    int patches = tfile->patchsetNpatches;
    for (int uu = 0; uu < patches*patches; uu++) {
        if(selectedPatchs[uu]){
            tfile->flags[uu] = tfile->flags[uu] | 0x1;
            this->setModified(true);
        }
    }
}

void Terrain::toggleDraw(int x, int z, float posx, float posz) {
    getPatchCoords(x, z, posx, posz);
    int patches = tfile->patchsetNpatches;
    tfile->flags[z * patches + x] = tfile->flags[z * patches + x] ^ 0x1;
    this->setModified(true);
}

float Terrain::getErrorBias(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return tfile->erroeBias[uu];
        }
    }
    return -1;
}

void Terrain::getWTileIds(QSet<int> &ids){
    ids.insert((int)mojex*(int)10000+(int)mojez);
}

void Terrain::setErrorBias(float val){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            tfile->erroeBias[uu] = val;
            this->setModified(true);
        }
    }
}

void Terrain::setErrorBias(int x, int z, float posx, float posz, float val){
    this->getPatchCoords(x, z, posx, posz);
    int patches = tfile->patchsetNpatches;
    int u = x;
    int y = z;
    tfile->erroeBias[y * patches + u] = val;
}

float Terrain::getAvgVaterLevel(){
    return (tfile->WNE + tfile->WSE + tfile->WNW + tfile->WSW)/4.0;
}

void Terrain::getWaterLevels(float* w){
    w[0] = tfile->WNW;
    w[1] = tfile->WNE;
    w[2] = tfile->WSW;
    w[3] = tfile->WSE;
}

float Terrain::getWaterLevelNW(){
    return tfile->WNW;
}

float Terrain::getWaterLevelNE(){
    return tfile->WNE;
}

float Terrain::getWaterLevelSW(){
    return tfile->WSW;
}

float Terrain::getWaterLevelSE(){
    return tfile->WSE;
}

void Terrain::setWaterLevelNW(float val){
    tfile->WNW = val;
}

void Terrain::setWaterLevelNE(float val){
    tfile->WNE = val;
}

void Terrain::setWaterLevelSW(float val){
    tfile->WSW = val;
}

void Terrain::setWaterLevelSE(float val){
    tfile->WSE = val;
}

void Terrain::setAvgWaterLevel(float val){
    tfile->WNE = val;
    tfile->WSE = val;
    tfile->WNW = val;
    tfile->WSW = val;    
    refreshWaterShapes();
}

void Terrain::getAdjacentWaterLevels(float* w){
    Game::terrainLib->fillWaterLevels(w, mojex, mojez);
}

void Terrain::setAdjacentWaterLevels(float* w){
    Game::terrainLib->setWaterLevels(w, mojex, mojez);
}

void Terrain::setWaterLevelGui(){
    TerrainWaterWindow waterWindow;
    waterWindow.setWindowTitle("Water Level");
    waterWindow.WNE = tfile->WNE;
    waterWindow.WSE = tfile->WSE;
    waterWindow.WNW = tfile->WNW;
    waterWindow.WSW = tfile->WSW;    
    waterWindow.setWater();
    waterWindow.exec();
    //qDebug() << waterWindow->changed;
    if(waterWindow.changed){
        tfile->waterLevel = true;
        tfile->WNE = waterWindow.WNE;
        tfile->WSE = waterWindow.WSE;
        tfile->WNW = waterWindow.WNW;
        tfile->WSW = waterWindow.WSW;
        refreshWaterShapes();
    }
}

void Terrain::setWaterLevel(float nw, float ne, float sw, float se){
    tfile->waterLevel = true;
    tfile->WNE = ne;
    tfile->WSE = se;
    tfile->WNW = nw;
    tfile->WSW = sw;
    refreshWaterShapes();
}

void Terrain::getRotation(float* rot, int x, int z, int posx, int posz){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;

    posx -= 2048 * (mojex-x) - 1024;
    posz -= 2048 * (mojez-z) + 1024;
    posz = tileSize + posz;
    
    float tx = terrainData[(int) (posz ) / sampleSize + 1][(int) (posx) / sampleSize]
        - terrainData[(int) (posz ) / sampleSize][(int) (posx) / sampleSize];
    float tz = terrainData[(int) (posz ) / sampleSize][(int) (posx) / sampleSize + 1]
        - terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize];
    
    rot[0] = atan(tx/sampleSize);
    rot[1] = atan(tz/sampleSize);
}

float Terrain::getHeight(int x, int z, float posx, float posz, bool addR){
    //if ((posx + 1024) / 8 + 1 > 256 || (posz + 1024) / 8 + 1 > 256)
    //    return terrainData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8];
    
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;

    posx -= 2048 * (mojex-x) - 1024;
    posz -= 2048 * (mojez-z) + 1024;
    posz = tileSize + posz;

    float tx = (posx / sampleSize) - (float) floor(posx / sampleSize);
    float tz = (posz / sampleSize) - (float) floor(posz / sampleSize);
    
    float roznica = 0;
    //return 0;
    if((int)(posz / sampleSize) >= samples )
        posz = posz - 1;
    if((int)(posx / sampleSize) >= samples )
        posx = posx - 1;

    if (addR) {
        roznica = 0.25 * (terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize] +
                terrainData[(int) (posz) / sampleSize + 1][(int) (posx) / sampleSize + 1] +
                terrainData[(int) (posz) / sampleSize + 1][(int) (posx) / sampleSize] +
                terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize + 1]) -
                0.5f * (terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize] +
                terrainData[(int) (posz) / sampleSize + 1][(int) (posx) / sampleSize + 1]);
    }
    return (
            terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize]*(1.0 - tx)*(1.0 - tz) +
            terrainData[(int) (posz) / sampleSize][(int) (posx) / sampleSize + 1]*(tx)*(1.0 - tz) +
            terrainData[(int) (posz) / sampleSize + 1][(int) (posx) / sampleSize]*(1.0 - tx)*(tz) +
            terrainData[(int) (posz) / sampleSize + 1][(int) (posx) / sampleSize + 1]*(tx)*(tz)
            + fabs(roznica));
}

void Terrain::refreshWaterShapes(){
    for (WaterTile* wt : water){
        if(wt == NULL)
            continue;
        for (int uu = 0; uu < 16; uu++) {
            for (int yy = 0; yy < 16; yy++) {
                wt->w[uu * 16 + yy].loaded = false;
            }
        }
    }
    setModified(true);
}

bool Terrain::isXYinside(int x, int y){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;
    int patches = tfile->patchsetNpatches;
    int patchSize = tileSize/patches;
    float posx = 0, posz = 0;
    posx -= 2048 * (mojex-x) - 1024;
    posz -= 2048 * (mojez-y) + 1024;
    posz = tileSize + posz;

    if(posx < 0 || posz < 0 || posx > tileSize || posz > tileSize )
        return false;
    return true;
}

int Terrain::getPatchSize(){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;
    int patches = tfile->patchsetNpatches;
    return tileSize/patches;
}

int Terrain::getSampleSize(){
    return *tfile->sampleSize;
}

void Terrain::getLocalCoords(int x, int z, float &posx, float &posz){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;
    int patches = tfile->patchsetNpatches;
    int patchSize = tileSize/patches;
    posx -= 2048 * (mojex-x) - 1024;
    posz -= 2048 * (mojez-z) + 1024;
    posz = tileSize + posz;
}

void Terrain::getPatchCoords(int &x, int &z, float &posx, float &posz){
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;
    int patches = tfile->patchsetNpatches;
    int patchSize = tileSize/patches;
    posx -= 2048 * (mojex-x) - 1024;
    posz -= 2048 * (mojez-z) + 1024;
    posz = tileSize + posz;
    
    x = posx / patchSize;
    z = posz / patchSize;
}

void Terrain::setTexture(Brush* brush, int x, int z, float posx, float posz) {
    getPatchCoords(x, z, posx, posz);

    int patches = tfile->patchsetNpatches;
    setTexture(brush, z*patches + x);
}

void Terrain::setTexture(Brush* brush, int u) {
    if (brush->texId < 0)
        return;
    bool autoRot = false;
    if (brush->texId == texid[u]) {
        qDebug() << "same tex";
        //this->getPatchRotationName()
        //rotateTex(u);
        autoRot = true;
    } else {
        QString path = texturepath + brush->tex->pathid.section("/", -1);
        qDebug() << path;
        if (!QFile::exists(path)){
            int newTidx = TexLib::getTex(path);
            if(newTidx >= 0){
                qDebug() << "ref tex";
                texid[u] = newTidx;
            } else {
                qDebug() << "new tex";
                texid[u] = TexLib::cloneTex(brush->texId);
                path = path.section(".", 0, -2) + ".ace";
                TexLib::mtex[texid[u]]->pathid = path;
                texModified[u] = true;
            }
            brush->texId = texid[u];
            brush->tex = TexLib::mtex[texid[u]];
        }
        texid[u] = brush->texId;
        uniqueTex[u] = false;
        QString tname = TexLib::mtex[brush->texId]->pathid.section("/", -1);
        qDebug() << TexLib::mtex[brush->texId]->pathid;
        qDebug() << tname;
        int mid = tfile->getMatByTexture(tname);
        if(mid < 0){
            tfile->tdata[(u)*13 + 0 + 6] = tfile->cloneMat(tfile->tdata[(u)*13 + 0 + 6]);
            *tfile->materials[(int) tfile->tdata[(u)*13 + 0 + 6]].tex[0] = tname;
            *tfile->amaterials[(int) tfile->tdata[(u)*13 + 0 + 6]].tex[0] = tname;
            qDebug() << *tfile->materials[(int) tfile->tdata[(u)*13 + 0 + 6]].tex[0];
            qDebug() << "new material";
        } else {
            tfile->tdata[(u)*13 + 0 + 6] = mid;
            qDebug() << "existed material";
        }
        reloadLines();
    }
    if(brush->texTransformation == brush->RANDOM && autoRot){
        rotateTex(u);
    } else if(brush->texTransformation == brush->PRESENT){
        
    } else {
        int count = 0;
        resetPatchTexCoords(u);
        if(brush->texTransformation == brush->ROT90)
            count = 1;
        if(brush->texTransformation == brush->ROT180)
            count = 2;
        if(brush->texTransformation == brush->ROT270)
            count = 3;
        qDebug() << count;
        for(int i = 0; i < count; i++)
            rotateTex(u);
    }    
    
    /*QString name = this->getTileName(mojex, -mojez)+"_"+QString::number(y)+"_"+QString::number(u)+".ace";
    
    if(name != *tfile->materials[(int)tfile->tdata[(y * 16 + u)*13+0+6]].tex[0]){
        tfile->tdata[(y * 16 + u)*13+0+6] = tfile->cloneMat(tfile->tdata[(y * 16 + u)*13+0+6]);
     *tfile->materials[(int)tfile->tdata[(y * 16 + u)*13+0+6]].tex[0] = name;
        qDebug() << *tfile->materials[(int)tfile->tdata[(y * 16 + u)*13+0+6]].tex[0];
        texid[y * 16 + u] = TexLib::cloneTex(texid[y * 16 + u]);
        
        TexLib::save("ace", texturepath+name, texid[y * 16 + u]);
        //TexLib::mtex[texid[y * 16 + u]]->GLTextures();
    }*/

    this->modified = true;
}

void Terrain::paintTexture(Brush* brush, int x, int z, float posx, float posz) {
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    float tileSize = sampleSize*samples;
    int patches = tfile->patchsetNpatches;
    int patchSize = tileSize/patches;
    getPatchCoords(x, z, posx, posz);
    
    int u = x;
    int y = z;


    float size = (float) (brush->size) / (512);
    //qDebug() << "size " << size << " " << tx / 128 << " " << tz / 128;

    for (int i = u - 1; i < u + 2; i++)
        for (int j = y - 1; j < y + 2; j++) {
            float tx = posx - i * patchSize;
            float tz = posz - j * patchSize;
            tx /= patchSize;
            tz /= patchSize;
            //qDebug() << tx << " " << tz;
            if ((tx < 0.0 - size) || (tx > 1.0 + size) || (tz < 0.0 - size) || (tz > 1.0 + size))
                continue;
            if(!texLocked[j * patches + i])
                this->paintTextureOnTile(brush, j, i, tx, tz);
        }

}

void Terrain::lockTexture(Brush* brush, int x, int z, float posx, float posz) {
    getPatchCoords(x, z, posx, posz);
    int patches = tfile->patchsetNpatches;
    texLocked[z * patches + x] = !texLocked[z * patches + x];
    reloadLines();
}

void Terrain::paintTextureOnTile(Brush* brush, int y, int u, float x, float z) {
    //qDebug() << "painttile " << x << " " << z;
    int patches = tfile->patchsetNpatches;
    if (y > patches-1 || u > patches-1 || y < 0 || u < 0) return;
    //qDebug() << "painttile " << y << " " << u;
    QString name = this->getTileName(mojex, -mojez) + "_" + QString::number(y) + "_" + QString::number(u) + ".ace";
    if (texid[y * patches + u] < 0)
        return;
    
    if (name != *tfile->materials[(int) tfile->tdata[(y * patches + u)*13 + 0 + 6]].tex[0]) {
        tfile->tdata[(y * patches + u)*13 + 0 + 6] = tfile->cloneMat(tfile->tdata[(y * patches + u)*13 + 0 + 6]);
        tfile->materials[tfile->tdata[(y * patches + u)*13 + 0 + 6]].itex[1][3] = 1107296256;
        *tfile->materials[(int) tfile->tdata[(y * patches + u)*13 + 0 + 6]].tex[0] = name;
        *tfile->amaterials[(int) tfile->tdata[(y * patches + u)*13 + 0 + 6]].tex[0] = name;
        qDebug() << *tfile->materials[(int) tfile->tdata[(y * patches + u)*13 + 0 + 6]].tex[0];
        texid[y * patches + u] = TexLib::cloneTex(texid[y * patches + u]);
        TexLib::mtex[texid[y * patches + u]]->pathid = texturepath + name;
        uniqueTex[y * patches + u] = true;
        reloadLines();
        //TexLib::save("ace", texturepath+name, texid[y * 16 + u]);
        //TexLib::mtex[texid[y * 16 + u]]->GLTextures();
    }
    convertTexToDefaultCoords(y * patches + u);

    TexLib::mtex[texid[y * patches + u]]->sendToUndo(texid[y * patches + u]);
    TexLib::mtex[texid[y * patches + u]]->paint(brush, z, x);
    TexLib::mtex[texid[y * patches + u]]->update();
    this->texModified[y * patches + u] = true;
    this->modified = true;
}

void Terrain::render(float lodx, float lodz, int tileX, int tileY, float* playerW, float* target, float fov, int selectionColor) {
    if (!loaded)
        return;
    if (!isOgl) {
        Game::terrainLib->fillRaw(this, (int) mojex, (int) mojez);
        vertexInit();
        normalInit();
        oglInit();
        isOgl = true;
    }

    GLUU* gluu = GLUU::get();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (!lines.loaded) {
        reloadLines();
    }
    
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int patches = tfile->patchsetNpatches;
    int patchRes = samples/patches;
    
    //qDebug() << 2048 * (mojex-tileX) << 2048 * (mojez-tileY);
    if(mojex-tileX != 0 || mojez-tileY != 0){
        //qDebug() << mojex << mojez;
        //qDebug() << mojex-tileX << mojez-tileY;
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * (mojex-tileX) , 0, 2048 * (mojez-tileY) );
    }
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, -1024, 0, 1024-sampleSize*samples);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->currentMsMatrinxHash = 0;//gluu->getMatrixHash(gluu->objStrMatrix);
    if(Game::viewWorldGrid && selectionColor == 0)
        lines.render();
    if(Game::viewTileGrid && selectionColor == 0){
        slines.render();
        ulines.render();
        lockedlines.render();
        selectedlines.render();
    }

    gluu->enableTextures();  
    gluu->enableNormals();
    
    float lod = 0;
    float size = 512;

    QOpenGLVertexArrayObject::Binder vaoBinder(VAO);
        
    if(Game::viewTerrainShape && (!(showBlob && MapWindow::isAlpha == 0) || selectionColor != 0)){
        float shaderSecondTexUV = 0;
        for (int uu = 0; uu < patches; uu++) {
            for (int yy = 0; yy < patches; yy++) {
                if (hidden[yy * patches + uu]) continue;
                if ((tfile->flags[yy * patches + uu] & 1) != 0) continue;
                /*float lodxx = lodx + uu * 128 - 1024;
                float lodzz = lodz + yy * 128 - 1024;
                lod = sqrt(lodxx * lodxx + lodzz * lodzz);
                //System.out.println("-- "+lodxx+" "+lodzz);
                if (lod > Game::objectLod) continue;

                if ((lod > size)) {
                    float v1[2];
                    v1[0] = playerW[0] - (target[0]);
                    v1[1] = playerW[2] - (target[2]);
                    float v2[2];
                    v2[0] = lodxx;
                    v2[1] = lodzz;
                    float iloczyn = v1[0] * v2[0] + v1[1] * v2[1];
                    float d1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
                    float d2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
                    float zz = iloczyn / (d1 * d2);
                    if (zz > 0) continue;

                    float ccos = cos(fov) + zz;
                    float xxx = sqrt(2 * d2 * d2 * (1 - ccos));
                    if ((ccos > 0) && (xxx > size)) continue;
                }*/

                if(selectionColor != 0){
                    int tselectionColor = selectionColor | (yy * patches + uu);
                    int wColor = (int)(tselectionColor/65536);
                    int sColor = (int)(tselectionColor - wColor*65536)/256;
                    int bColor = (int)(tselectionColor - wColor*65536 - sColor*256);
                    gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
                } else {
                    if (texid[yy * patches + uu] == -2) {
                    } else {
                        if (texid[yy * patches + uu] == -1) {
                            //texid[uu*16+yy] = TexLib.addTex(texturepath,"nasyp-k.ace", gl);
                            //qDebug() << texturepath << " "<<tfile->tdata[(yy * 16 + uu)*7+0] <<" "<< tfile->materials[(int)tfile->tdata[(yy * 16 + uu)*7+0]].tex[0];
                            if (tfile->materialsCount <= (int) tfile->tdata[(yy * patches + uu)*13 + 0 + 6]){
                                texid[yy * patches + uu] = -2;
                                return;
                            } else {
                                texid[yy * patches + uu] = TexLib::addTex(texturepath, *tfile->materials[(int) tfile->tdata[(yy * patches + uu)*13 + 0 + 6]].tex[0]);
                            }//System.out.println(tfile.materials[tfile.tdata[uu*16+yy]].tex[0]);
                            //texid = TexLib.addTex(texturepath,"nasyp-k.ace", gl);
                            //    gl.glDisable(GL2.GL_TEXTURE_2D);
                        }
                        if (TexLib::mtex[texid[yy * patches + uu]]->loaded) {
                            if (!TexLib::mtex[texid[yy * patches + uu]]->glLoaded)
                                TexLib::mtex[texid[yy * patches + uu]]->GLTextures();
                            f->glActiveTexture(GL_TEXTURE0);
                            //f->glBindTexture(GL_TEXTURE_2D, TexLib::mtex[texid[yy * 16 + uu]]->tex[0]);
                            gluu->bindTexture(f, TexLib::mtex[texid[yy * patches + uu]]->tex[0]);
                        } else {
                        }
                    }
                    if (texid2[yy * patches + uu] == -2) {
                    } else if (tfile->materials[(int) tfile->tdata[(yy * patches + uu)*13 + 0 + 6]].count153 < 2){
                            texid2[yy * patches + uu] = -2;
                    } else {
                        if (texid2[yy * patches + uu] == -1) {
                            if (tfile->materialsCount <= (int) tfile->tdata[(yy * patches + uu)*13 + 0 + 6])
                                texid[yy * patches + uu] = -2;
                            else
                                texid2[yy * patches + uu] = TexLib::addTex(texturepath, *tfile->materials[(int) tfile->tdata[(yy * patches + uu)*13 + 0 + 6]].tex[1]);
                        }
                        if (TexLib::mtex[texid2[yy * patches + uu]]->loaded) {
                            if (!TexLib::mtex[texid2[yy * patches + uu]]->glLoaded)
                                TexLib::mtex[texid2[yy * patches + uu]]->GLTextures(true);
                            f->glActiveTexture(GL_TEXTURE1);
                            f->glBindTexture(GL_TEXTURE_2D, TexLib::mtex[texid2[yy * patches + uu]]->tex[0]);
                            if(shaderSecondTexUV != *(float*)&tfile->materials[(int) tfile->tdata[(yy * patches + uu)*13 + 0 + 6]].itex[1][3]){
                                shaderSecondTexUV = *(float*)&tfile->materials[(int) tfile->tdata[(yy * patches + uu)*13 + 0 + 6]].itex[1][3];
                                gluu->currentShader->setUniformValue(gluu->currentShader->shaderSecondTexEnabled, shaderSecondTexUV);
                            }
                        } else {
                        }
                    }
                }
                
                f->glDrawArrays(GL_TRIANGLES, (uu * patches + yy) * patchRes * patchRes * 6, patchRes * patchRes * 6);
            }
        }
        f->glActiveTexture(GL_TEXTURE0);
        gluu->currentShader->setUniformValue(gluu->currentShader->shaderSecondTexEnabled, 0.0f);
    }
        
    if(Game::viewTerrainGrid || !Game::viewTerrainShape){
        gluu->disableTextures(0.7,0.7,0.7,1.0);
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0.05, 0);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        for (int uu = 0; uu < patches; uu++) {
            for (int yy = 0; yy < patches; yy++) {
                if (hidden[yy * patches + uu]) continue;
                if ((tfile->flags[yy * patches + uu] & 1) != 0) continue;
                float lodxx = lodx + uu * 128 - 1024;
                float lodzz = lodz + yy * 128 - 1024;
                lod = sqrt(lodxx * lodxx + lodzz * lodzz);
                if(Game::viewTerrainShape)
                    if (lod > 300) continue;
                f->glDrawArrays(GL_TRIANGLES, (uu * patches + yy) * patchRes * patchRes * 6, patchRes * patchRes * 6);
            }
        }
        gluu->mvPopMatrix();
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    
    if(showBlob && selectionColor == 0){
        if(MapWindow::isAlpha == 0){
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            terrainBlob.render();
        }else{
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0.35, 0);
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            terrainBlob.render();
            gluu->mvPopMatrix();
        }
    } 
    
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            
    //if(!showBlob && selectionColor == 0)
    //    renderWater(lodx, lodz, playerT, playerW, target, fov);
}

void Terrain::renderWater(float lodx, float lodz, float tileX, float tileY, float* playerW, float* target, float fov, int layer, int selectionColor) {
    float lod;
    if(showBlob)
        return;
    float alpha = 0;
    
    GLUU* gluu = GLUU::get();
    
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int patches = tfile->patchsetNpatches;
    int patchRes = samples/patches;
    int tileSize = (sampleSize*samples);
    int patchSize = tileSize/patches;
    
    if(mojex-tileX != 0 || mojez-tileY != 0){
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * (mojex-tileX) , 0, 2048 * (mojez-tileY) );
    }
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, -1024, 0, 1024-sampleSize*samples);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));

    
    if(water[layer] == NULL)
        water[layer] = new WaterTile();
    OglObj *w = water[layer]->w;
    
    int tselectionColor = 0;
    for (int uu = 0; uu < patches; uu++) {
        for (int yy = 0; yy < patches; yy++) {
            if (hidden[yy * patches + uu]) continue;
            /*float lodxx = lodx + uu * 128 - 1024;
            float lodzz = lodz + yy * 128 - 1024;
            lod = sqrt(lodxx * lodxx + lodzz * lodzz);
            //System.out.println("-- "+lodxx+" "+lodzz);
            if (lod > Game::objectLod) continue;*/
            
            if ((tfile->flags[yy * patches + uu] & 0xc0) != 0) {

                if (!w[uu * patches + yy].loaded) {

                    float x1 = (uu)*patchSize;
                    float x2 = (uu + 1)*patchSize;
                    float z1 = (yy)*patchSize;
                    float z2 = (yy + 1)*patchSize;
                    float x1z1 = (((x1)*(z1)) / (tileSize * tileSize)) * tfile->WSE +
                            (((tileSize - x1)*(z1)) / (tileSize * tileSize)) * tfile->WSW +
                            (((tileSize - x1)*(tileSize - z1)) / (tileSize * tileSize)) * tfile->WNW +
                            (((x1)*(tileSize - z1)) / (tileSize * tileSize)) * tfile->WNE;
                    float x2z1 = (((x2)*(z1 )) / (tileSize * tileSize)) * tfile->WSE +
                            (((tileSize - x2)*(z1 )) / (tileSize * tileSize)) * tfile->WSW +
                            (((tileSize - x2)*(tileSize - z1)) / (tileSize * tileSize)) * tfile->WNW +
                            (((x2)*(tileSize - z1)) / (tileSize * tileSize)) * tfile->WNE;
                    float x1z2 = (((x1 )*(z2 )) / (tileSize * tileSize)) * tfile->WSE +
                            (((tileSize - x1)*(z2)) / (tileSize * tileSize)) * tfile->WSW +
                            (((tileSize - x1)*(tileSize - z2)) / (tileSize * tileSize)) * tfile->WNW +
                            (((x1)*(tileSize - z2)) / (tileSize * tileSize)) * tfile->WNE;
                    float x2z2 = (((x2)*(z2)) / (tileSize * tileSize)) * tfile->WSE +
                            (((tileSize - x2)*(z2)) / (tileSize * tileSize)) * tfile->WSW +
                            (((tileSize - x2)*(tileSize - z2)) / (tileSize * tileSize)) * tfile->WNW +
                            (((x2)*(tileSize - z2)) / (tileSize * tileSize)) * tfile->WNE;

                    float *punkty = new float[54];
                    int ptr = 0;

                    punkty[ptr++] = x2;
                    punkty[ptr++] = x2z2;
                    punkty[ptr++] = z2;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = alpha;

                    punkty[ptr++] = x2;
                    punkty[ptr++] = x2z1;
                    punkty[ptr++] = z1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = alpha;

                    punkty[ptr++] = x1;
                    punkty[ptr++] = x1z1;
                    punkty[ptr++] = z1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = alpha;

                    punkty[ptr++] = x1;
                    punkty[ptr++] = x1z2;
                    punkty[ptr++] = z2;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = alpha;

                    punkty[ptr++] = x2;
                    punkty[ptr++] = x2z2;
                    punkty[ptr++] = z2;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = alpha;

                    punkty[ptr++] = x1;
                    punkty[ptr++] = x1z1;
                    punkty[ptr++] = z1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = alpha;
                    //QString *texturePath = new QString("resources/woda.ace");
                    //water[uu * 16 + yy].setMaterial(texturePath);
                    w[uu * patches + yy].setMaterial(&Game::currentRoute->env->water[layer].tex);
                    w[uu * patches + yy].init(punkty, ptr, w[uu * patches + yy].VNT, GL_TRIANGLES);
                    delete punkty;
                }
                if(selectionColor != 0)
                    tselectionColor = selectionColor | (yy * patches + uu);
                w[uu * patches + yy].render(tselectionColor);
            }
        }
    }
}

void Terrain::reloadLines() {
    // tile lines
    float *punkty = new float[256 * 6 * 4];
    int ptr = 0;
    int i = 0;

    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    int patches = tfile->patchsetNpatches;
    int patchSize = (samples*sampleSize)/patches;
    int min = 0;
    int max = sampleSize * samples;
    
    for (i = 0; i < samples; i++) {
        punkty[ptr++] = min;
        punkty[ptr++] = 1 + terrainData[i][0];
        punkty[ptr++] = min + i *sampleSize;
        punkty[ptr++] = min;
        punkty[ptr++] = 1 + terrainData[i + 1][0];
        punkty[ptr++] = min + i *sampleSize +sampleSize;
    }

    for (i = 0; i < samples; i++) {
        punkty[ptr++] = min + i *sampleSize;
        punkty[ptr++] = 1 + terrainData[0][i];
        punkty[ptr++] = min;
        punkty[ptr++] = min + i *sampleSize +sampleSize;
        punkty[ptr++] = 1 + terrainData[0][i + 1];
        punkty[ptr++] = min;
    }

    for (i = 0; i < samples; i++) {
        punkty[ptr++] = max;
        punkty[ptr++] = 1 + terrainData[i][samples];
        punkty[ptr++] = min + i *sampleSize;
        punkty[ptr++] = max;
        punkty[ptr++] = 1 + terrainData[i + 1][samples];
        punkty[ptr++] = min + i *sampleSize +sampleSize;
    }

    for (i = 0; i < samples; i++) {
        punkty[ptr++] = min + i *sampleSize;
        punkty[ptr++] = 1 + terrainData[samples][i];
        punkty[ptr++] = max;
        punkty[ptr++] = min + i *sampleSize +sampleSize;
        punkty[ptr++] = 1 + terrainData[samples][i + 1];
        punkty[ptr++] = max;
    }

    lines.setMaterial(1.0, 0.0, 0.0);
    lines.init(punkty, ptr, lines.V, GL_LINES);
    delete[] punkty;
    //s tile lines
    punkty = new float[samples * 32 * 6];
    ptr = 0;
    i = 0;

    for (int j = 0; j < samples; j += 16) {
        for (i = 0; i < samples; i++) {
            punkty[ptr++] = min + j *sampleSize;
            punkty[ptr++] = 0.9 + terrainData[i][j];
            punkty[ptr++] = min + i *sampleSize;
            punkty[ptr++] = min + j *sampleSize;
            punkty[ptr++] = 0.9 + terrainData[i + 1][j];
            punkty[ptr++] = min + i *sampleSize +sampleSize;
        }

        for (i = 0; i < samples; i++) {
            punkty[ptr++] = min + i *sampleSize;
            punkty[ptr++] = 0.9 + terrainData[j][i];
            punkty[ptr++] = min + j *sampleSize;
            punkty[ptr++] = min + i *sampleSize +sampleSize;
            punkty[ptr++] = 0.9 + terrainData[j][i + 1];
            punkty[ptr++] = min + j *sampleSize;
        }
    }
    slines.setMaterial(0.5, 0.5, 0.5);
    slines.init(punkty, ptr, lines.V, GL_LINES);
    delete[] punkty;
    
    //////////////////////
    
    int ui = 0;
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++)
            if(this->uniqueTex[uu*16+yy]) ui++;
    
    punkty = new float[samples * 128 * 6];
    ptr = 0;
    i = 0;
    
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++){
            if(!this->uniqueTex[yy*16+uu]) continue;
            
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+i][uu*16+0];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize;
                 punkty[ptr++] = min + uu*patchSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+ i + 1][uu*16+0];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize +sampleSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+0][uu*16+i];
                 punkty[ptr++] = min + yy*patchSize;
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize +sampleSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+0][uu*16+i + 1];
                 punkty[ptr++] = min + yy*patchSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize+patchSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+i][uu*16+16];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize;
                 punkty[ptr++] = min + uu*patchSize+patchSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+ i + 1][uu*16+16];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize +sampleSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+16][uu*16+i];
                 punkty[ptr++] = min + yy*patchSize+patchSize;
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize +sampleSize;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+16][uu*16+i + 1];
                 punkty[ptr++] = min + yy*patchSize+patchSize;
            }
        }
    
    ulines.setMaterial(0.8, 0.8, 0.8);
    ulines.init(punkty, ptr, lines.V, GL_LINES);
    delete[] punkty;

    //////////////////////
    
    ui = 0;
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++)
            if(this->texLocked[uu*16+yy]) ui++;
    
    punkty = new float[samples * 128 * 6];
    ptr = 0;
    i = 0;
    
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++){
            if(!this->texLocked[yy*16+uu]) continue;
            
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+i][uu*16+0];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize;
                 punkty[ptr++] = min + uu*patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+ i + 1][uu*16+0];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize +sampleSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+0][uu*16+i];
                 punkty[ptr++] = min + yy*patchSize;
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize +sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+0][uu*16+i + 1];
                 punkty[ptr++] = min + yy*patchSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize+patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+i][uu*16+16];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize;
                 punkty[ptr++] = min + uu*patchSize+patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+ i + 1][uu*16+16];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize +sampleSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+16][uu*16+i];
                 punkty[ptr++] = min + yy*patchSize+patchSize;
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize +sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+16][uu*16+i + 1];
                 punkty[ptr++] = min + yy*patchSize+patchSize;
            }
        }
    
    lockedlines.setMaterial(0.1, 0.1, 0.1);
    lockedlines.init(punkty, ptr, lines.V, GL_LINES);
    delete[] punkty;
    
    //////////////////////
    
    ui = 0;
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++)
            if(this->selectedPatchs[uu*16+yy]) ui++;
    
    punkty = new float[samples * 128 * 6];
    ptr = 0;
    i = 0;
    
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++){
            if(!this->selectedPatchs[yy*16+uu]) continue;
            
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+i][uu*16+0];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize;
                 punkty[ptr++] = min + uu*patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+ i + 1][uu*16+0];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize +sampleSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+0][uu*16+i];
                 punkty[ptr++] = min + yy*patchSize;
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize +sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+0][uu*16+i + 1];
                 punkty[ptr++] = min + yy*patchSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize+patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+i][uu*16+16];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize;
                 punkty[ptr++] = min + uu*patchSize+patchSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+ i + 1][uu*16+16];
                 punkty[ptr++] = min + yy*patchSize + i *sampleSize +sampleSize;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+16][uu*16+i];
                 punkty[ptr++] = min + yy*patchSize+patchSize;
                 punkty[ptr++] = min + uu*patchSize + i *sampleSize +sampleSize;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+16][uu*16+i + 1];
                 punkty[ptr++] = min + yy*patchSize+patchSize;
            }
        }
    
    selectedlines.setMaterial(0.0, 0.0, 0.8);
    selectedlines.init(punkty, ptr, lines.V, GL_LINES);
    delete[] punkty;
}

void Terrain::vertexInit() {
    // this.vertexData[0] = new Array();
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;

    vertexData = new Vector3f*[samples+1];
    for (int i = 0; i < samples+1; i++)
        vertexData[i] = new Vector3f[samples+1];

    qDebug() << "min" << sampleSize << samples;
    for (int j = 0, jj = 0; jj < samples; j += sampleSize, jj++) {
        for (int i = 0, ii = 0; ii < samples; i += sampleSize, ii++) {
            vertexData[jj][ii].set(j, terrainData[ii][jj], i);
            vertexData[jj][ii + 1].set(j, terrainData[ii + 1][jj], (i + sampleSize));
            vertexData[(jj + 1)][ii + 1].set((j + sampleSize), terrainData[(ii + 1)][jj + 1], (i + sampleSize));
            vertexData[(jj + 1)][ii].set((j + sampleSize), terrainData[(ii)][jj + 1], i);
        }
    }
}

void Terrain::normalInit() {
    int samples = *tfile->nsamples;
    int sampleSize = *tfile->sampleSize;
    normalData = new Vector3f*[samples+1];
    for (int i = 0; i < samples+1; i++)
        normalData[i] = new Vector3f[samples+1];
    Vector3f U, V, O;
    for (int jj = 0; jj < samples; jj++) {
        for (int ii = 0; ii < samples; ii++) {
            U.setFromSub( vertexData[jj][ii], vertexData[jj + 1][ii]);
            V.setFromSub( vertexData[jj][ii], vertexData[jj][ii + 1]);
            O.setFromCross(V, U);
            normalData[jj][ii].add( O );
            normalData[jj + 1][ii].add( O );
            normalData[jj][ii + 1].add( O );
            U.setFromSub( vertexData[jj + 1][ii + 1], vertexData[jj + 1][ii]);
            V.setFromSub( vertexData[jj + 1][ii + 1], vertexData[jj][ii + 1]);
            O.setFromCross(U, V);
            normalData[jj + 1][ii + 1].add( O );
            normalData[jj + 1][ii].add( O );
            normalData[jj][ii + 1].add( O );
        }
    }

    for (int jj = 0; jj < samples+1; jj++) {
        for (int ii = 0; ii < samples+1; ii++) {
            normalData[jj][ii].normalize();
        }
    }
};
/*
void Terrain::oglInit() {
    if(!VAO->isCreated()){
       VAO->create();
       VBO->create();
    }
    QOpenGLVertexArrayObject::Binder vaoBinder(VAO);
    VBO->bind();
    VBO->allocate(256 * 16 * 16 * 6 * 5 * sizeof (GLfloat));
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
    
    int ilosc = 16 * 16;
    int suma;
    float * punkty = new float[16 * 16 * 30];
    //  var punkty = Terrain.punkty;
    for (int uu = 0; uu < 16; uu++) {
        for (int yy = 0; yy < 16; yy++) {
            int ptr = 0;

            for (int ii = 0; ii < 16; ii++) {
                for (int jj = 0; jj < 16; jj++) {
                    if (jestF) {
                        suma = ((fData[yy * 16 + ii][uu * 16 + jj]) & 0x04);
                        suma += ((fData[yy * 16 + ii][uu * 16 + jj + 1]) & 0x04);
                        suma += ((fData[yy * 16 + ii + 1][uu * 16 + jj + 1]) & 0x04);
                        suma += ((fData[yy * 16 + ii + 1][uu * 16 + jj]) & 0x04);
                    }

                    if (!jestF || (suma < 4)) {
                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].x;
                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].y;
                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].z;
                        punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                        punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                        punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                        punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                        punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                        punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].x;
                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].y;
                        punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].z;
                        punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                        punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                        punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                        punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                        punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                        punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                        punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];
                    }
                }
            }
            VBO->write((uu * 16 + yy) * 16 * 16 * 6 * 5 * sizeof (GLfloat), punkty, 16 * 16 * 6 * 5 * sizeof (GLfloat));
        }
    }

    VBO->release();
    delete[] punkty;

    initBlob();
    //for (int i = 0; i < 257; i++)
    //    delete normalData[i];
    //delete normalData;
    for (int i = 0; i < 257; i++)
        delete[] vertexData[i];
    delete[] vertexData;
}*/

void Terrain::oglInit() {
    if(!VAO->isCreated()){
       VAO->create();
       VBO->create();
    }
    QOpenGLVertexArrayObject::Binder vaoBinder(VAO);
    VBO->bind();
    VBO->allocate(256 * 16 * 16 * 6 * 8 * sizeof (GLfloat));
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (6 * sizeof (GLfloat)));
    
    //int ilosc = 16 * 16;
    //int suma;
    float * punkty = new float[16 * 16 * 48];
    //  var punkty = Terrain.punkty;
    int samples = *tfile->nsamples;
    int patches = tfile->patchsetNpatches;
    int patchRes = samples/patches;
    float texRes = 1.0;// (float)16.0/patchRes;
    
    for (int uu = 0; uu < patches; uu++) {
        for (int yy = 0; yy < patches; yy++) {
            int ptr = 0;
            bool fi0j0 = true, fi1j0 = true, fi0j1 = true, fi1j1 = true;

            for (int ii = 0; ii < patchRes; ii++) {
                for (int jj = 0; jj < patchRes; jj++) {
                    if (jestF) {
                        fi0j0 = !(((fData[yy * patchRes + ii][uu * patchRes + jj]) & 0x04) >> 2);
                        fi0j1 = !(((fData[yy * patchRes + ii][uu * patchRes + jj + 1]) & 0x04) >> 2);
                        fi1j1 = !(((fData[yy * patchRes + ii + 1][uu * patchRes + jj + 1]) &0x04) >> 2);
                        fi1j0 = !(((fData[yy * patchRes + ii + 1][uu * patchRes + jj]) & 0x04) >> 2);
                    }
                    /*if (jestF) {
                        suma = ((fData[yy * 16 + ii][uu * 16 + jj]) & 0x04);
                        suma += ((fData[yy * 16 + ii][uu * 16 + jj + 1]) & 0x04);
                        suma += ((fData[yy * 16 + ii + 1][uu * 16 + jj + 1]) & 0x04);
                        suma += ((fData[yy * 16 + ii + 1][uu * 16 + jj]) & 0x04);
                    }*/

                    //if (!jestF || (suma < 4)) {
                        if(((ii+jj) % 2 == 0)){
                            if(fi0j0 && fi1j0 && fi1j1){
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].z;
                            punkty[ptr++] = texRes * (jj * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * (jj * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = texRes * ((jj) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];
                            }
                            ///////////////////////////////////////////////////////////
                            if(fi0j0 && fi0j1 && fi1j1){
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].z;
                            punkty[ptr++] = texRes * (jj * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * (jj * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].z;
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];
                            }
                        }
                        if(((ii+jj) % 2 == 1)){
                            if(fi0j1 && fi1j1 && fi1j0){
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = texRes * ((jj) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];
                            
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].z;
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];
                            }
                            ///////////////////////////////////////////////////////////
                            if(fi0j0 && fi0j1 && fi1j0){
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii].z;
                            punkty[ptr++] = texRes * (jj * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * (jj * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj)][yy * patchRes + ii + 1].z;
                            punkty[ptr++] = texRes * ((jj) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].x;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].y;
                            punkty[ptr++] = vertexData[(uu * patchRes + jj + 1)][yy * patchRes + ii].z;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].x;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].y;
                            punkty[ptr++] = normalData[(uu * patchRes + jj + 1)][yy * patchRes + ii].z;
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 3 + 6] + (ii) * tfile->tdata[(yy * patches + uu)*13 + 4 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 1 + 6];
                            punkty[ptr++] = texRes * ((jj + 1) * tfile->tdata[(yy * patches + uu)*13 + 5 + 6] + (ii) * tfile->tdata[(yy * patches + uu)*13 + 6 + 6]) + tfile->tdata[(yy * patches + uu)*13 + 2 + 6];
                            }
                        }
                    //}
                }
            }
            /*for(var jj = 0; jj<16; jj++){
                for(var ii = 0; ii<16; ii++){
                    punkty.put(normalData[uu*16+jj][yy*16+ii].toFloat());
                    punkty.put(normalData[uu*16+jj][yy*16+ii+1].toFloat());
                    punkty.put(normalData[uu*16+jj+1][yy*16+ii+1].toFloat());

                    punkty.put(normalData[uu*16+jj][yy*16+ii].toFloat());
                    punkty.put(normalData[uu*16+jj+1][yy*16+ii+1].toFloat());
                    punkty.put(normalData[uu*16+jj+1][yy*16+ii].toFloat());
                }
            }*/

            //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
            //if(!VBO[0]->isCreated()){
                //VAO[uu * 16 + yy]->create();
           //     VBO[0]->create();
            //}
            
            //QOpenGLVertexArrayObject::Binder vaoBinder(VAO[uu * 16 + yy]);
            
            //VBO[0]->bind();
            //VBO[0]->
            VBO->write((uu * patches + yy) * patchRes * patchRes * 6 * 8 * sizeof (GLfloat), punkty, patchRes * patchRes * 6 * 8 * sizeof (GLfloat));
            //VBO[0]->allocate(punkty, 16 * 16 * 6 * 5 * sizeof (GLfloat));
            //f->glEnableVertexAttribArray(0);
            //f->glEnableVertexAttribArray(1);
            //f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
            //f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
            //VBO[0]->release();
        }
    }

    VBO->release();
    delete[] punkty;

    initBlob();
    //for (int i = 0; i < 257; i++)
    //    delete normalData[i];
    //delete normalData;

    for (int i = 0; i < samples+1; i++){
        delete[] vertexData[i];
        delete[] normalData[i];
    }
    delete[] vertexData;
    delete[] normalData;
}

void Terrain::initBlob(){
    
    GLUU* gluu = GLUU::get();
    float alpha = -0.01;
    int samples = *tfile->nsamples;
    int patches = tfile->patchsetNpatches;
    int patchRes = samples/patches;
    float *punkty = new float[65536 * 54];
    int ptr = 0;
    float step = 1.0/samples;
    for (int jj = 0; jj < samples; jj++) {
        for (int ii = 0; ii < samples; ii++) {
            punkty[ptr++] = vertexData[jj][ii].x;
            punkty[ptr++] = vertexData[jj][ii].y+0.00;
            punkty[ptr++] = vertexData[jj][ii].z;
            punkty[ptr++] = normalData[jj][ii].x;
            punkty[ptr++] = normalData[jj][ii].y;
            punkty[ptr++] = normalData[jj][ii].z;
            punkty[ptr++] = (jj)*step;
            punkty[ptr++] = (ii)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj][ii+1].x;
            punkty[ptr++] = vertexData[jj][ii+1].y+0.00;
            punkty[ptr++] = vertexData[jj][ii+1].z;
            punkty[ptr++] = normalData[jj][ii+1].x;
            punkty[ptr++] = normalData[jj][ii+1].y;
            punkty[ptr++] = normalData[jj][ii+1].z;
            punkty[ptr++] = (jj)*step;
            punkty[ptr++] = (ii+1)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj+1][ii+1].x;
            punkty[ptr++] = vertexData[jj+1][ii+1].y+0.00;
            punkty[ptr++] = vertexData[jj+1][ii+1].z;
            punkty[ptr++] = normalData[jj+1][ii+1].x;
            punkty[ptr++] = normalData[jj+1][ii+1].y;
            punkty[ptr++] = normalData[jj+1][ii+1].z;
            punkty[ptr++] = (jj+1)*step;
            punkty[ptr++] = (ii+1)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj][ii].x;
            punkty[ptr++] = vertexData[jj][ii].y+0.00;
            punkty[ptr++] = vertexData[jj][ii].z;
            punkty[ptr++] = normalData[jj][ii].x;
            punkty[ptr++] = normalData[jj][ii].y;
            punkty[ptr++] = normalData[jj][ii].z;
            punkty[ptr++] = (jj)*step;
            punkty[ptr++] = (ii)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj+1][ii+1].x;
            punkty[ptr++] = vertexData[jj+1][ii+1].y+0.00;
            punkty[ptr++] = vertexData[jj+1][ii+1].z;
            punkty[ptr++] = normalData[jj+1][ii+1].x;
            punkty[ptr++] = normalData[jj+1][ii+1].y;
            punkty[ptr++] = normalData[jj+1][ii+1].z;
            punkty[ptr++] = (jj+1)*step;
            punkty[ptr++] = (ii+1)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj+1][ii].x;
            punkty[ptr++] = vertexData[jj+1][ii].y+0.00;
            punkty[ptr++] = vertexData[jj+1][ii].z;
            punkty[ptr++] = normalData[jj+1][ii].x;
            punkty[ptr++] = normalData[jj+1][ii].y;
            punkty[ptr++] = normalData[jj+1][ii].z;
            punkty[ptr++] = (jj+1)*step;
            punkty[ptr++] = (ii)*step;
            punkty[ptr++] = alpha;
        }
    }
    QString* path = new QString;
    int X, Y;
    this->getLowCornerTileXY(X, Y);
    *path += QString::number((int)(X)*10000+(int)(Y))+".:maptex";
    //qDebug() << *path;
    terrainBlob.setMaterial(path);
    terrainBlob.init(punkty, ptr, terrainBlob.VNT, GL_TRIANGLES);
    delete[] punkty;
}

bool Terrain::readRAW(QString fSfile) {
    fSfile.replace("//", "/");
    //qDebug() << fSfile;
    //qDebug() << "Wczytam teren RAW: " << fSfile;
    QFile file(fSfile);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    FileBuffer* data = ReadFile::readRAW(&file);

    int samples = *tfile->nsamples;
    //qDebug() << data->length;
    terrainData = new float*[samples+1];
    //int u = 0;
    for (int i = 0; i < samples+1; i++) {
        terrainData[i] = new float[samples+1];
        for (int j = 0; j < samples+1; j++) {
            if (i == samples && j == samples) {
                terrainData[i][j] = terrainData[(i - 1)][j - 1];
            } else if (i == samples) {
                terrainData[i][j] = terrainData[(i - 1)][j];
            } else if (j == samples) {
                terrainData[i][j] = terrainData[i][j - 1];
            } else {
                terrainData[i][j] = tfile->floor + tfile->scale * (data->get() + 256 * data->get());
                //terrainData[i][j] = tfile->floor + tfile->scale * (data->data[u++] + 256*data->data[u++]);
            }
        }
    }
    delete data;
    return true;
}

void Terrain::fillHeightMap(float* data){
    int samples = *tfile->nsamples + 1;
    for (int i = 0; i < samples; i++)
        for (int j = 0; j < samples; j++) {
            terrainData[i][j] = data[i*samples+j];
        }
    this->refresh();
}

void Terrain::save() {
    QString path = Game::root + "/routes/" + Game::route + "/" + TileDir[(int)lowTile] + "/";
    QString filename = name;
    if(this->tfile->sampleYbuffer == NULL)
        this->tfile->sampleYbuffer = new QString(filename + "_y.raw");
    saveRAW(path + *this->tfile->sampleYbuffer );
    if(jestF && modifiedF){
        if(this->tfile->sampleFbuffer == NULL)
            this->tfile->sampleFbuffer = new QString(filename + "_f.raw");
        saveF(path + *this->tfile->sampleFbuffer);
    }
    qDebug() << "writing t start";
    this->tfile->save(path + filename + ".t");
    qDebug() << "writing t end";
    int patches = tfile->patchsetNpatches;
    for (int u = 0; u < patches; u++)
        for (int y = 0; y < patches; y++) {
            if (this->texModified[y * patches + u] == false) continue;
            //QString name = this->getTileName(mojex, -mojez) + "_" + QString::number(y) + "_" + QString::number(u) + ".ace";
            TexLib::save("ace", TexLib::mtex[texid[y * patches + u]]->pathid, texid[y * patches + u]);
            this->texModified[y * patches + u] = false;
        }
}

void Terrain::saveRAW(QString name) {
    name.replace("//", "/");
    QFile *file = new QFile(name);
    qDebug() << "zapis " << name;
    if (!file->open(QIODevice::WriteOnly))
        return;
    qDebug() << "w";
    QDataStream write(file);
    write.setByteOrder(QDataStream::LittleEndian);

    int samples = *tfile->nsamples;
    float min = 999999, max = -999999;
    for (int i = 0; i < samples; i++) {
        for (int j = 0; j < samples; j++) {
            if(terrainData[i][j] < min) min = terrainData[i][j];
            if(terrainData[i][j] > max) max = terrainData[i][j];
            //terrainData[i][j] = tfile->floor + tfile->scale * (data->get() + 256*data->get());
        }
    }
    min -= 10;
    max += 10;
    // new tfile floor/scale
    tfile->floor = min;
    tfile->scale = (max - min)/65535;
    
    float fvalue;
    unsigned short value;
    for (int i = 0; i < samples; i++) {
        for (int j = 0; j < samples; j++) {
            fvalue = (terrainData[i][j] - tfile->floor) / tfile->scale;
            if (fvalue > 65535) value = 65535;
            else if(fvalue < 0) value = 0;
            else value = fvalue;
            write << value;
            //terrainData[i][j] = tfile->floor + tfile->scale * (data->get() + 256*data->get());
        }
    }
    file->close();
    return;
}

void Terrain::newF(){
    fData = new unsigned char*[257];
    for (int j = 0; j < 257; j++) {
        fData[j] = new unsigned char[257];
        for (int i = 0; i < 257; i++) {
            fData[j][i] = 0;
        }
    }
    jestF = true;
    modifiedF = true;
    modified = true;
}

bool Terrain::readF(QString fSfile) {
    fSfile.replace("//", "/");
    QFile file(fSfile);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    FileBuffer* data = ReadFile::readRAW(&file);

    //qDebug() << "Wczytam teren F: " << fSfile << data->length;

    int samples = *tfile->nsamples;
    int u = 0;
    fData = new unsigned char*[samples+1];
    for (int j = 0; j < samples+1; j++) {
        fData[j] = new unsigned char[samples+1];
        for (int i = 0; i < samples+1; i++) {
            if (i == samples || j == samples)
                fData[j][i] = 0;
            else
                fData[j][i] = data->data[u++];
        }
    }
    delete data;
    return true;
} 

void Terrain::saveF(QString name) {
    name.replace("//", "/");
    QFile *file = new QFile(name);
    qDebug() << "zapis " << name;
    if (!file->open(QIODevice::WriteOnly))
        return;
    qDebug() << "w";
    QDataStream write(file);
    write.setByteOrder(QDataStream::LittleEndian);

    int samples = *tfile->nsamples;
    for (int i = 0; i < samples; i++) {
        for (int j = 0; j < samples; j++) {
            write << fData[i][j];
        }
    }
    modifiedF = false;
    file->close();
    return;
}

int Terrain::getSelectedPathId(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return uu;
        }
    }
    return -1;
}

int Terrain::getSelectedShaderId(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return tfile->tdata[(uu)*13 + 0 + 6];
        }
    }
    return -1;
}

QString Terrain::getPatchMainTextureName(){
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            return *tfile->materials[(int) tfile->tdata[(uu)*13 + 0 + 6]].tex[0];
        }
    }
    return "UNDEFINED";
}

bool Terrain::select(int value, bool oneMore){
    if(oneMore){
        selected = true;
        selectedPatchs[value] = !selectedPatchs[value];
        reloadLines();
        return true;
    }
    return select(value);
}

bool Terrain::select(int value){
    if(selected){
        int selectedId = 0;
        for (int uu = 0; uu < 256; uu++) {
            if(selectedPatchs[uu]){
                selectedId = uu;
                break;
            }
        }
        int u1 = selectedId / 16;
        int y1 = selectedId - u1*16;
        int u2 = value / 16;
        int y2 = value - u2*16;
        if(u1 > u2){
            int temp = u2;
            u2 = u1;
            u1 = temp;
        }
        if(y1 > y2){
            int temp = y2;
            y2 = y1;
            y1 = temp;
        }
        for(int i = u1; i <= u2; i++)
            for(int j = y1; j <= y2; j++){
                if(i < 0 || j < 0 || i > 15 || j > 15)
                    continue;
                selectedPatchs[i*16+j] = true;
            }
    } else {
        selected = true;
        selectedPatchs[value] = true;
    }
    reloadLines();
    return true;
}

bool Terrain::unselect(){
    for (int i = 0; i < 256; i++) {
        selectedPatchs[i] = false;
    }
    reloadLines();
    this->selected = false;
    return false;
}

void Terrain::pushContextMenuActions(QMenu *menu){
    if(contextMenuActions["togglewater"] == NULL){
        contextMenuActions["togglewater"] = new QAction(tr("&Toggle Water")); 
        QObject::connect(contextMenuActions["togglewater"], SIGNAL(triggered()), this, SLOT(menuToggleWater()));
    }
    if(contextMenuActions["puttexture"] == NULL){
        contextMenuActions["puttexture"] = new QAction(tr("&Put Texture")); 
        QObject::connect(contextMenuActions["puttexture"], SIGNAL(triggered()), this, SLOT(menuPutTexture()));
    }
    if(contextMenuActions["toggledraw"] == NULL){
        contextMenuActions["toggledraw"] = new QAction(tr("&Toggle Draw")); 
        QObject::connect(contextMenuActions["toggledraw"], SIGNAL(triggered()), this, SLOT(menuToggleDraw()));
    }
    menu->addAction(contextMenuActions["puttexture"]);
    menu->addAction(contextMenuActions["togglewater"]);
    menu->addAction(contextMenuActions["toggledraw"]);
}

void Terrain::menuToggleWater(){
    toggleWaterDraw();
}

void Terrain::menuToggleDraw(){
    toggleDraw();
}

void Terrain::menuPutTexture(){
    if(DefaultBrush == NULL)
        return;
    for (int uu = 0; uu < 256; uu++) {
        if(selectedPatchs[uu]){
            setTexture(DefaultBrush, uu);
        }
    }
}