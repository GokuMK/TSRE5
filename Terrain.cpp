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

Terrain::Terrain(float x, float y) {
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
    }
    VBO = new QOpenGLBuffer();
    VAO = new QOpenGLVertexArrayObject();
    mojex = x;
    mojez = y;
    texturepath = Game::root + "/routes/" + Game::route + "/terrtex/";
    QString path = Game::root + "/routes/" + Game::route + "/tiles/";
    tfile = new TFile();

    QString filename = getTileName((int) x, (int) -y);
    //qDebug() << filename << x << -y;
    if (!tfile->readT((path + filename + ".t"))) {
        //qDebug() << " t fail";
        return;
    }
    if(tfile->sampleYbuffer == NULL)
        return;
    if (!readRAW((path + *tfile->sampleYbuffer/* + "_y.raw"*/))) {
        //qDebug() << " y fail";
        return;
    }
    if(tfile->sampleFbuffer != NULL)
        jestF = readF(path + *tfile->sampleFbuffer/* + "_f.raw"*/);
    modifiedF = false;
    //qDebug() << " ok";
    
    QString name = this->getTileName(mojex, -mojez);
    QString name2;
    for (int u = 0; u < 16; u++)
        for (int y = 0; y < 16; y++) {
            name2 = name + "_" + QString::number(y) + "_" + QString::number(u) + ".ace";
            if (name2 == *tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0])
                this->uniqueTex[y*16+u] = true;
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

void Terrain::saveEmpty(int x, int y) {
    QString name = getTileName(x, y);
    qDebug() << "New terrain tile";
    QFile file(Game::root + "/routes/" + Game::route + "/tiles/" + name + "_y.raw");
    if (!file.exists())
        QFile::copy("resources/templateRoute/tiles/template_y.raw", Game::root + "/routes/" + Game::route + "/tiles/" + name + "_y.raw");
    file.setFileName(Game::root + "/routes/" + Game::route + "/tiles/" + name + ".t");
    if (!file.exists())
        QFile::copy("resources/templateRoute/tiles/template.t", Game::root + "/routes/" + Game::route + "/tiles/" + name + ".t");
    TFile *tfile = new TFile();
    tfile->readT(Game::root + "/routes/" + Game::route + "/tiles/" + name + ".t");
    tfile->setBufferNames(name);
    tfile->save(Game::root + "/routes/" + Game::route + "/tiles/" + name + ".t");
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
    reloadLines();
}

void Terrain::toggleGaps(int x, int z, float posx, float posz){
    if(!jestF)
        newF();
    fData[(int) (posz + 1024) / 8][(int) (posx + 1024) / 8] ^= 0x04;
    modifiedF = true;
    modified = true;
    refresh();
}

int Terrain::getTexture(int x, int z, float posx, float posz) {

    int u = (posx + 1024) / 128;
    int y = (posz + 1024) / 128;
    //hidden[y*16 + u] = true;
    float tx = posx + 1024 - u * 128;
    float tz = posz + 1024 - y * 128;
    tx /= 128;
    tz /= 128;
    qDebug() << tx << " " << tz;

    return texid[y * 16 + u];
}

void Terrain::convertTexToDefaultCoords(int idx) {
    float x11 = (0) * tfile->tdata[(idx)*13 + 3 + 6] + (0) * tfile->tdata[(idx)*13 + 4 + 6] + tfile->tdata[(idx)*13 + 1 + 6];
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
    float t;
    if ((x11 < x21) && (y11 == y21)) {
        qDebug() << "rot1 - ok";
    } else {
        qDebug() << "rot";
        TexLib::mtex[texid[idx]]->crop(x11, y11, x22, y22);
    }

    tfile->tdata[(idx)*13 + 1 + 6] = 0.001;
    tfile->tdata[(idx)*13 + 2 + 6] = 0.001;

    tfile->tdata[(idx)*13 + 3 + 6] = 0.062375;
    tfile->tdata[(idx)*13 + 4 + 6] = 0.0;
    tfile->tdata[(idx)*13 + 5 + 6] = 0.0;
    tfile->tdata[(idx)*13 + 6 + 6] = 0.062375;
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
        tfile->tdata[(idx)*13 + 1 + 6] = x21;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = -tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = t;
    } else if ((x11 == x21) && (y11 < y21)) {
        qDebug() << "rot2";
        tfile->tdata[(idx)*13 + 2 + 6] = y21;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = -tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = -t;
    } else if ((x11 > x21) && (y11 == y21)) {
        qDebug() << "rot3";
        tfile->tdata[(idx)*13 + 1 + 6] = x21;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = -tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = t;
    } else if ((x11 == x21) && (y11 > y21)) {
        qDebug() << "rot4";
        tfile->tdata[(idx)*13 + 2 + 6] = y21;
        t = tfile->tdata[(idx)*13 + 4 + 6];
        tfile->tdata[(idx)*13 + 4 + 6] = tfile->tdata[(idx)*13 + 3 + 6];
        tfile->tdata[(idx)*13 + 3 + 6] = t;
        t = tfile->tdata[(idx)*13 + 5 + 6];
        tfile->tdata[(idx)*13 + 5 + 6] = -tfile->tdata[(idx)*13 + 6 + 6];
        tfile->tdata[(idx)*13 + 6 + 6] = -t;
    }
    this->refresh();
}

void Terrain::setTileBlob(){
    if(this->showBlob){
        this->showBlob = false;
        return;
    } 
    int hash = (int)(this->mojex)*10000+(int)(this->mojez);
    qDebug() << hash;
    if(MapWindow::mapTileImages[hash] != NULL)
        this->showBlob = true;
    else
        qDebug() << "load map first!";
}

void Terrain::setWaterDraw(int x, int z, float posx, float posz) {

    int u = (posx + 1024) / 128;
    int y = (posz + 1024) / 128;
    //hidden[y*16 + u] = true;
    float tx = posx + 1024 - u * 128;
    float tz = posz + 1024 - y * 128;
    tx /= 128;
    tz /= 128;
    qDebug() << tx << " " << tz;
    //qDebug() << tfile->erroeBias[y * 16 + u];
    tfile->flags[y * 16 + u] = tfile->flags[y * 16 + u] ^ 0x10000c0;
    this->setModified(true);
}

void Terrain::setDraw(int x, int z, float posx, float posz) {

    int u = (posx + 1024) / 128;
    int y = (posz + 1024) / 128;
    //hidden[y*16 + u] = true;
    float tx = posx + 1024 - u * 128;
    float tz = posz + 1024 - y * 128;
    tx /= 128;
    tz /= 128;
    qDebug() << tx << " " << tz;
    qDebug() << tfile->flags[y * 16 + u];
    tfile->flags[y * 16 + u] = tfile->flags[y * 16 + u] ^ 0x1;
    this->setModified(true);
}

void Terrain::setErrorBias(int x, int z, float val){
    int u = (x + 1024) / 128;
    int y = (z + 1024) / 128;
    tfile->erroeBias[y * 16 + u] = val;
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
        tfile->WNE = waterWindow.WNE;
        tfile->WSE = waterWindow.WSE;
        tfile->WNW = waterWindow.WNW;
        tfile->WSW = waterWindow.WSW;    
        for (int uu = 0; uu < 16; uu++) {
            for (int yy = 0; yy < 16; yy++) {
                water[uu * 16 + yy].loaded = false;
            }
        }
        this->setModified(true);
    }
}

void Terrain::setTexture(Brush* brush, int x, int z, float posx, float posz) {

    int u = (posx + 1024) / 128;
    int y = (posz + 1024) / 128;
    //hidden[y*16 + u] = true;
    float tx = posx + 1024 - u * 128;
    float tz = posz + 1024 - y * 128;
    tx /= 128;
    tz /= 128;
    qDebug() << tx << " " << tz;

    if (brush->texId == texid[y * 16 + u]) {
        qDebug() << "same tex";
        this->rotateTex(y * 16 + u);
    } else {
        texid[y * 16 + u] = brush->texId;
        uniqueTex[y * 16 + u] = false;
        QStringList tpath = TexLib::mtex[brush->texId]->pathid.split("/");
        qDebug() << TexLib::mtex[brush->texId]->pathid;
        qDebug() << tpath.last();
        QString tname = tpath.last();
        int mid = tfile->getMatByTexture(tname);
        if(mid < 0){
            tfile->tdata[(y * 16 + u)*13 + 0 + 6] = tfile->cloneMat(tfile->tdata[(y * 16 + u)*13 + 0 + 6]);
            *tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0] = tname;
            *tfile->amaterials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0] = tname;
            qDebug() << *tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0];
            qDebug() << "new material";
        } else {
            tfile->tdata[(y * 16 + u)*13 + 0 + 6] = mid;
            qDebug() << "existed material";
        }
        reloadLines();
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

    int u = (posx + 1024) / 128;
    int y = (posz + 1024) / 128;
    //hidden[y*16 + u] = true;
    float tx = posx + 1024 - u * 128;
    float tz = posz + 1024 - y * 128;
    //tx/= 128;
    //tz/= 128;

    float size = (float) (brush->size) / (512);
    qDebug() << "size " << size << " " << tx / 128 << " " << tz / 128;

    for (int i = u - 1; i < u + 2; i++)
        for (int j = y - 1; j < y + 2; j++) {
            float tx = posx + 1024 - i * 128;
            float tz = posz + 1024 - j * 128;
            tx /= 128;
            tz /= 128;
            qDebug() << tx << " " << tz;
            if ((tx < 0.0 - size) || (tx > 1.0 + size) || (tz < 0.0 - size) || (tz > 1.0 + size))
                continue;
            if(!texLocked[j * 16 + i])
                this->paintTextureOnTile(brush, j, i, tx, tz);
        }

}

void Terrain::lockTexture(Brush* brush, int x, int z, float posx, float posz) {

    int u = (posx + 1024) / 128;
    int y = (posz + 1024) / 128;
    //hidden[y*16 + u] = true;
    float tx = posx + 1024 - u * 128;
    float tz = posz + 1024 - y * 128;
    //tx/= 128;
    //tz/= 128;
    texLocked[y * 16 + u] = !texLocked[y * 16 + u];
    reloadLines();
}

void Terrain::paintTextureOnTile(Brush* brush, int y, int u, float x, float z) {
    qDebug() << "painttile " << x << " " << z;
    if (y > 15 || u > 15 || y < 0 || u < 0) return;

    QString name = this->getTileName(mojex, -mojez) + "_" + QString::number(y) + "_" + QString::number(u) + ".ace";

    if (name != *tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0]) {
        tfile->tdata[(y * 16 + u)*13 + 0 + 6] = tfile->cloneMat(tfile->tdata[(y * 16 + u)*13 + 0 + 6]);
        *tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0] = name;
        *tfile->amaterials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0] = name;
        qDebug() << *tfile->materials[(int) tfile->tdata[(y * 16 + u)*13 + 0 + 6]].tex[0];
        texid[y * 16 + u] = TexLib::cloneTex(texid[y * 16 + u]);
        convertTexToDefaultCoords(y * 16 + u);
        uniqueTex[y * 16 + u] = true;
        reloadLines();
        //TexLib::save("ace", texturepath+name, texid[y * 16 + u]);
        //TexLib::mtex[texid[y * 16 + u]]->GLTextures();
    }

    TexLib::mtex[texid[y * 16 + u]]->paint(brush, z, x);
    TexLib::mtex[texid[y * 16 + u]]->update();
    this->texModified[y * 16 + u] = true;
    this->modified = true;
}

void Terrain::render(float lodx, float lodz, float * playerT, float* playerW, float* target, float fov) {
    if (!loaded)
        return;
    if (!isOgl) {
        TerrainLib::fillRAW(terrainData, (int) mojex, (int) mojez);
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

    gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->currentMsMatrinxHash = 0;//gluu->getMatrixHash(gluu->objStrMatrix);
    if(Game::viewWorldGrid)
        lines.render();
    if(Game::viewTileGrid){
        slines.render();
        ulines.render();
        lockedlines.render();
    }

    gluu->enableTextures();  
    gluu->enableNormals();
    if(this->showBlob){
            terrainBlob.render();
    } else {
        //gluu->currentShader->setUniformValue(gluu->currentShader->shaderSecondTexEnabled, 1.0f);
        int off = 0;
        float lod = 0;
        float size = 512;

        QOpenGLVertexArrayObject::Binder vaoBinder(VAO);

        for (int uu = 0; uu < 16; uu++) {
            for (int yy = 0; yy < 16; yy++) {
                if (hidden[yy * 16 + uu]) continue;
                if ((tfile->flags[yy * 16 + uu] & 1) != 0) continue;
                float lodxx = lodx + uu * 128 - 1024;
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
                }

                if (texid[yy * 16 + uu] == -2) {
                    //gl.glColor3f(0.5f, 0.5f, 0f);
                    //gl.glDisable(GL2.GL_TEXTURE_2D);
                } else {
                    if (texid[yy * 16 + uu] == -1) {
                        //texid[uu*16+yy] = TexLib.addTex(texturepath,"nasyp-k.ace", gl);
                        //qDebug() << texturepath << " "<<tfile->tdata[(yy * 16 + uu)*7+0] <<" "<< tfile->materials[(int)tfile->tdata[(yy * 16 + uu)*7+0]].tex[0];
                        texid[yy * 16 + uu] = TexLib::addTex(texturepath, *tfile->materials[(int) tfile->tdata[(yy * 16 + uu)*13 + 0 + 6]].tex[0]);
                        //System.out.println(tfile.materials[tfile.tdata[uu*16+yy]].tex[0]);
                        //texid = TexLib.addTex(texturepath,"nasyp-k.ace", gl);
                        //    gl.glDisable(GL2.GL_TEXTURE_2D);
                    }
                    if (TexLib::mtex[texid[yy * 16 + uu]]->loaded) {
                        if (!TexLib::mtex[texid[yy * 16 + uu]]->glLoaded)
                            TexLib::mtex[texid[yy * 16 + uu]]->GLTextures();
                        gluu->bindTexture(f, TexLib::mtex[texid[yy * 16 + uu]]->tex[0]);
                        //f->glBindTexture(GL_TEXTURE_2D, TexLib::mtex[texid[yy * 16 + uu]]->tex[0]);
                        //System.out.println(tfile.materials[tfile.tdata[uu*16+yy]].tex[0]);
                        //  gl.glEnable(GL2.GL_TEXTURE_2D);
                        //gl.glDisable(GL2.GL_ALPHA_TEST);
                        //gl.glDisable(GL2.GL_BLEND);
                    } else {
                        // gl.glDisable(GL2.GL_TEXTURE_2D);
                    }
                }
                /*if (texid2[yy * 16 + uu] == -2) {
                } else if (tfile->materials[(int) tfile->tdata[(yy * 16 + uu)*13 + 0 + 6]].count153 < 2){
                        texid2[yy * 16 + uu] = -2;
                } else {
                    if (texid2[yy * 16 + uu] == -1) {
                        texid2[yy * 16 + uu] = TexLib::addTex(texturepath, *tfile->materials[(int) tfile->tdata[(yy * 16 + uu)*13 + 0 + 6]].tex[1]);
                    }
                    if (TexLib::mtex[texid2[yy * 16 + uu]]->loaded) {
                        if (!TexLib::mtex[texid2[yy * 16 + uu]]->glLoaded)
                            TexLib::mtex[texid2[yy * 16 + uu]]->GLTextures();
                        //f->glActiveTexture(GL_TEXTURE1);
                        //f->glBindTexture(GL_TEXTURE_2D, TexLib::mtex[texid2[yy * 16 + uu]]->tex[0]);
                        //f->glActiveTexture(GL_TEXTURE0);
                        //System.out.println(tfile.materials[tfile.tdata[uu*16+yy]].tex[0]);
                        //  gl.glEnable(GL2.GL_TEXTURE_2D);
                        //gl.glDisable(GL2.GL_ALPHA_TEST);
                        //gl.glDisable(GL2.GL_BLEND);
                    } else {
                        // gl.glDisable(GL2.GL_TEXTURE_2D);
                    }
                }*/
                //QOpenGLVertexArrayObject::Binder vaoBinder(VAO[uu * 16 + yy]);
                

                f->glDrawArrays(GL_TRIANGLES, (uu * 16 + yy) * 16 * 16 * 6, 16 * 16 * 6);
                

            }
        }
        
        if(Game::viewTerrainGrid){
            gluu->disableTextures(0.7,0.7,0.7,1.0);
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 0.1, 0);
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            for (int uu = 0; uu < 16; uu++) {
                for (int yy = 0; yy < 16; yy++) {
                    if (hidden[yy * 16 + uu]) continue;
                    if ((tfile->flags[yy * 16 + uu] & 1) != 0) continue;
                    float lodxx = lodx + uu * 128 - 1024;
                    float lodzz = lodz + yy * 128 - 1024;
                    lod = sqrt(lodxx * lodxx + lodzz * lodzz);
                    if (lod > 300) continue;
                    f->glDrawArrays(GL_TRIANGLES, (uu * 16 + yy) * 16 * 16 * 6, 16 * 16 * 6);
                }
            }
            gluu->mvPopMatrix();
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        gluu->currentShader->setUniformValue(gluu->currentShader->shaderSecondTexEnabled, 0.0f);
        renderWater(lodx, lodz, playerT, playerW, target, fov);
    }
}

void Terrain::renderWater(float lodx, float lodz, float * playerT, float* playerW, float* target, float fov) {
    float lod;
    
    GLUU *gluu = GLUU::get();
    float alpha = -gluu->alphaTest;
    
    for (int uu = 0; uu < 16; uu++) {
        for (int yy = 0; yy < 16; yy++) {
            if (hidden[yy * 16 + uu]) continue;
            float lodxx = lodx + uu * 128 - 1024;
            float lodzz = lodz + yy * 128 - 1024;
            lod = sqrt(lodxx * lodxx + lodzz * lodzz);
            //System.out.println("-- "+lodxx+" "+lodzz);
            if (lod > Game::objectLod) continue;
            
            if ((tfile->flags[yy * 16 + uu] & 0xc0) != 0) {

                if (!water[uu * 16 + yy].loaded) {

                    float x1 = (uu)*128 - 1024;
                    float x2 = (uu + 1)*128 - 1024;
                    float z1 = (yy)*128 - 1024;
                    float z2 = (yy + 1)*128 - 1024;
                    float x1z1 = (((x1 + 1024)*(z1 + 1024)) / (2048 * 2048)) * tfile->WSE +
                            (((1024 - x1)*(z1 + 1024)) / (2048 * 2048)) * tfile->WSW +
                            (((1024 - x1)*(1024 - z1)) / (2048 * 2048)) * tfile->WNW +
                            (((x1 + 1024)*(1024 - z1)) / (2048 * 2048)) * tfile->WNE;
                    float x2z1 = (((x2 + 1024)*(z1 + 1024)) / (2048 * 2048)) * tfile->WSE +
                            (((1024 - x2)*(z1 + 1024)) / (2048 * 2048)) * tfile->WSW +
                            (((1024 - x2)*(1024 - z1)) / (2048 * 2048)) * tfile->WNW +
                            (((x2 + 1024)*(1024 - z1)) / (2048 * 2048)) * tfile->WNE;
                    float x1z2 = (((x1 + 1024)*(z2 + 1024)) / (2048 * 2048)) * tfile->WSE +
                            (((1024 - x1)*(z2 + 1024)) / (2048 * 2048)) * tfile->WSW +
                            (((1024 - x1)*(1024 - z2)) / (2048 * 2048)) * tfile->WNW +
                            (((x1 + 1024)*(1024 - z2)) / (2048 * 2048)) * tfile->WNE;
                    float x2z2 = (((x2 + 1024)*(z2 + 1024)) / (2048 * 2048)) * tfile->WSE +
                            (((1024 - x2)*(z2 + 1024)) / (2048 * 2048)) * tfile->WSW +
                            (((1024 - x2)*(1024 - z2)) / (2048 * 2048)) * tfile->WNW +
                            (((x2 + 1024)*(1024 - z2)) / (2048 * 2048)) * tfile->WNE;

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

                    QString *texturePath = new QString("resources/woda.ace");
                    water[uu * 16 + yy].setMaterial(texturePath);
                    water[uu * 16 + yy].init(punkty, ptr, water[uu * 16 + yy].VNT, GL_TRIANGLES);
                    delete punkty;
                }
                water[uu * 16 + yy].render();
            }
        }
    }
}

void Terrain::reloadLines() {
    // tile lines
    float *punkty = new float[256 * 6 * 4];
    int ptr = 0;
    int i = 0;

    for (i = 0; i < 256; i++) {
        punkty[ptr++] = -1024;
        punkty[ptr++] = 1 + terrainData[i][0];
        punkty[ptr++] = -1024 + i * 8;
        punkty[ptr++] = -1024;
        punkty[ptr++] = 1 + terrainData[i + 1][0];
        punkty[ptr++] = -1024 + i * 8 + 8;
    }

    for (i = 0; i < 256; i++) {
        punkty[ptr++] = -1024 + i * 8;
        punkty[ptr++] = 1 + terrainData[0][i];
        punkty[ptr++] = -1024;
        punkty[ptr++] = -1024 + i * 8 + 8;
        punkty[ptr++] = 1 + terrainData[0][i + 1];
        punkty[ptr++] = -1024;
    }

    for (i = 0; i < 256; i++) {
        punkty[ptr++] = 1024;
        punkty[ptr++] = 1 + terrainData[i][256];
        punkty[ptr++] = -1024 + i * 8;
        punkty[ptr++] = 1024;
        punkty[ptr++] = 1 + terrainData[i + 1][256];
        punkty[ptr++] = -1024 + i * 8 + 8;
    }

    for (i = 0; i < 256; i++) {
        punkty[ptr++] = -1024 + i * 8;
        punkty[ptr++] = 1 + terrainData[256][i];
        punkty[ptr++] = 1024;
        punkty[ptr++] = -1024 + i * 8 + 8;
        punkty[ptr++] = 1 + terrainData[256][i + 1];
        punkty[ptr++] = 1024;
    }

    lines.setMaterial(1.0, 0.0, 0.0);
    lines.init(punkty, ptr, lines.V, GL_LINES);
    delete[] punkty;
    //s tile lines
    punkty = new float[256 * 32 * 6];
    ptr = 0;
    i = 0;

    for (int j = 0; j < 256; j += 16) {
        for (i = 0; i < 256; i++) {
            punkty[ptr++] = -1024 + j * 8;
            punkty[ptr++] = 0.9 + terrainData[i][j];
            punkty[ptr++] = -1024 + i * 8;
            punkty[ptr++] = -1024 + j * 8;
            punkty[ptr++] = 0.9 + terrainData[i + 1][j];
            punkty[ptr++] = -1024 + i * 8 + 8;
        }

        for (i = 0; i < 256; i++) {
            punkty[ptr++] = -1024 + i * 8;
            punkty[ptr++] = 0.9 + terrainData[j][i];
            punkty[ptr++] = -1024 + j * 8;
            punkty[ptr++] = -1024 + i * 8 + 8;
            punkty[ptr++] = 0.9 + terrainData[j][i + 1];
            punkty[ptr++] = -1024 + j * 8;
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
    
    punkty = new float[256 * 128 * 6];
    ptr = 0;
    i = 0;
    
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++){
            if(!this->uniqueTex[yy*16+uu]) continue;
            
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+i][uu*16+0];
                 punkty[ptr++] = -1024 + yy*128 + i * 8;
                 punkty[ptr++] = -1024 + uu*128;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+ i + 1][uu*16+0];
                 punkty[ptr++] = -1024 + yy*128 + i * 8 + 8;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128 + i * 8;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+0][uu*16+i];
                 punkty[ptr++] = -1024 + yy*128;
                 punkty[ptr++] = -1024 + uu*128 + i * 8 + 8;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+0][uu*16+i + 1];
                 punkty[ptr++] = -1024 + yy*128;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128+128;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+i][uu*16+16];
                 punkty[ptr++] = -1024 + yy*128 + i * 8;
                 punkty[ptr++] = -1024 + uu*128+128;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+ i + 1][uu*16+16];
                 punkty[ptr++] = -1024 + yy*128 + i * 8 + 8;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128 + i * 8;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+16][uu*16+i];
                 punkty[ptr++] = -1024 + yy*128+128;
                 punkty[ptr++] = -1024 + uu*128 + i * 8 + 8;
                 punkty[ptr++] = 0.95 + terrainData[yy*16+16][uu*16+i + 1];
                 punkty[ptr++] = -1024 + yy*128+128;
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
    
    punkty = new float[256 * 128 * 6];
    ptr = 0;
    i = 0;
    
    for (int uu = 0; uu < 16; uu++)
        for (int yy = 0; yy < 16; yy++){
            if(!this->texLocked[yy*16+uu]) continue;
            
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+i][uu*16+0];
                 punkty[ptr++] = -1024 + yy*128 + i * 8;
                 punkty[ptr++] = -1024 + uu*128;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+ i + 1][uu*16+0];
                 punkty[ptr++] = -1024 + yy*128 + i * 8 + 8;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128 + i * 8;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+0][uu*16+i];
                 punkty[ptr++] = -1024 + yy*128;
                 punkty[ptr++] = -1024 + uu*128 + i * 8 + 8;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+0][uu*16+i + 1];
                 punkty[ptr++] = -1024 + yy*128;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128+128;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+i][uu*16+16];
                 punkty[ptr++] = -1024 + yy*128 + i * 8;
                 punkty[ptr++] = -1024 + uu*128+128;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+ i + 1][uu*16+16];
                 punkty[ptr++] = -1024 + yy*128 + i * 8 + 8;
            }
            for (i = 0; i < 16; i++) {
                 punkty[ptr++] = -1024 + uu*128 + i * 8;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+16][uu*16+i];
                 punkty[ptr++] = -1024 + yy*128+128;
                 punkty[ptr++] = -1024 + uu*128 + i * 8 + 8;
                 punkty[ptr++] = 0.99 + terrainData[yy*16+16][uu*16+i + 1];
                 punkty[ptr++] = -1024 + yy*128+128;
            }
        }
    
    lockedlines.setMaterial(0.1, 0.1, 0.1);
    lockedlines.init(punkty, ptr, lines.V, GL_LINES);
    delete[] punkty;
}

void Terrain::vertexInit() {
    // this.vertexData[0] = new Array();
    vertexData = new Vector3f*[257];
    for (int i = 0; i < 257; i++)
        vertexData[i] = new Vector3f[257];

    for (int j = -1024, jj = 0; jj < 256; j += 8, jj++) {
        for (int i = -1024, ii = 0; ii < 256; i += 8, ii++) {
            vertexData[jj][ii].set(j, terrainData[ii][jj], i);
            vertexData[jj][ii + 1].set(j, terrainData[ii + 1][jj], (i + 8));
            vertexData[(jj + 1)][ii + 1].set((j + 8), terrainData[(ii + 1)][jj + 1], (i + 8));
            vertexData[(jj + 1)][ii].set((j + 8), terrainData[(ii)][jj + 1], i);
        }
    }
}

void Terrain::normalInit() {
    normalData = new Vector3f*[257];
    for (int i = 0; i < 257; i++)
        normalData[i] = new Vector3f[257];
    Vector3f U, V, O;
    for (int jj = 0; jj < 256; jj++) {
        for (int ii = 0; ii < 256; ii++) {
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

    for (int jj = 0; jj < 257; jj++) {
        for (int ii = 0; ii < 257; ii++) {
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
    
    int ilosc = 16 * 16;
    int suma;
    float * punkty = new float[16 * 16 * 48];
    //  var punkty = Terrain.punkty;
    for (int uu = 0; uu < 16; uu++) {
        for (int yy = 0; yy < 16; yy++) {
            int ptr = 0;
            bool fi0j0 = true, fi1j0 = true, fi0j1 = true, fi1j1 = true;

            for (int ii = 0; ii < 16; ii++) {
                for (int jj = 0; jj < 16; jj++) {
                    if (jestF) {
                        fi0j0 = !(((fData[yy * 16 + ii][uu * 16 + jj]) & 0x04) >> 2);
                        fi0j1 = !(((fData[yy * 16 + ii][uu * 16 + jj + 1]) & 0x04) >> 2);
                        fi1j1 = !(((fData[yy * 16 + ii + 1][uu * 16 + jj + 1]) &0x04) >> 2);
                        fi1j0 = !(((fData[yy * 16 + ii + 1][uu * 16 + jj]) & 0x04) >> 2);
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
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].z;
                            punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];
                            }
                            ///////////////////////////////////////////////////////////
                            if(fi0j0 && fi0j1 && fi1j1){
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].z;
                            punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];
                            }
                        }
                        if(((ii+jj) % 2 == 1)){
                            if(fi0j1 && fi1j1 && fi1j0){
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];
                            
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];
                            }
                            ///////////////////////////////////////////////////////////
                            if(fi0j0 && fi0j1 && fi1j0){
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii].z;
                            punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + ii * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                            punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];

                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                            punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                            punkty[ptr++] = normalData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 3 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 4 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 1 + 6];
                            punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*13 + 5 + 6] + (ii) * tfile->tdata[(yy * 16 + uu)*13 + 6 + 6] + tfile->tdata[(yy * 16 + uu)*13 + 2 + 6];
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
            VBO->write((uu * 16 + yy) * 16 * 16 * 6 * 8 * sizeof (GLfloat), punkty, 16 * 16 * 6 * 8 * sizeof (GLfloat));
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
    for (int i = 0; i < 257; i++){
        delete[] vertexData[i];
        delete[] normalData[i];
    }
    delete[] vertexData;
    delete[] normalData;
}

void Terrain::initBlob(){
    
    GLUU* gluu = GLUU::get();
    float alpha = -gluu->alphaTest;    
    
    float *punkty = new float[65536 * 54];
    int ptr = 0;
    float step = 1.0/256;
    for (int jj = 0; jj < 256; jj++) {
        for (int ii = 0; ii < 256; ii++) {
            punkty[ptr++] = vertexData[jj][ii].x;
            punkty[ptr++] = vertexData[jj][ii].y+0.02;
            punkty[ptr++] = vertexData[jj][ii].z;
            punkty[ptr++] = normalData[jj][ii].x;
            punkty[ptr++] = normalData[jj][ii].y;
            punkty[ptr++] = normalData[jj][ii].z;
            punkty[ptr++] = (jj)*step;
            punkty[ptr++] = (ii)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj][ii+1].x;
            punkty[ptr++] = vertexData[jj][ii+1].y+0.02;
            punkty[ptr++] = vertexData[jj][ii+1].z;
            punkty[ptr++] = normalData[jj][ii+1].x;
            punkty[ptr++] = normalData[jj][ii+1].y;
            punkty[ptr++] = normalData[jj][ii+1].z;
            punkty[ptr++] = (jj)*step;
            punkty[ptr++] = (ii+1)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj+1][ii+1].x;
            punkty[ptr++] = vertexData[jj+1][ii+1].y+0.02;
            punkty[ptr++] = vertexData[jj+1][ii+1].z;
            punkty[ptr++] = normalData[jj+1][ii+1].x;
            punkty[ptr++] = normalData[jj+1][ii+1].y;
            punkty[ptr++] = normalData[jj+1][ii+1].z;
            punkty[ptr++] = (jj+1)*step;
            punkty[ptr++] = (ii+1)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj][ii].x;
            punkty[ptr++] = vertexData[jj][ii].y+0.02;
            punkty[ptr++] = vertexData[jj][ii].z;
            punkty[ptr++] = normalData[jj][ii].x;
            punkty[ptr++] = normalData[jj][ii].y;
            punkty[ptr++] = normalData[jj][ii].z;
            punkty[ptr++] = (jj)*step;
            punkty[ptr++] = (ii)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj+1][ii+1].x;
            punkty[ptr++] = vertexData[jj+1][ii+1].y+0.02;
            punkty[ptr++] = vertexData[jj+1][ii+1].z;
            punkty[ptr++] = normalData[jj+1][ii+1].x;
            punkty[ptr++] = normalData[jj+1][ii+1].y;
            punkty[ptr++] = normalData[jj+1][ii+1].z;
            punkty[ptr++] = (jj+1)*step;
            punkty[ptr++] = (ii+1)*step;
            punkty[ptr++] = alpha;
            punkty[ptr++] = vertexData[jj+1][ii].x;
            punkty[ptr++] = vertexData[jj+1][ii].y+0.02;
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
    *path += QString::number((int)(this->mojex)*10000+(int)(this->mojez))+".:maptex";
    //qDebug() << *path;
    terrainBlob.setMaterial(path);
    terrainBlob.init(punkty, ptr, terrainBlob.VNT, GL_TRIANGLES);
    delete[] punkty;
}

bool Terrain::readRAW(QString fSfile) {
    fSfile.replace("//", "/");
    //qDebug() << fSfile;
    //qDebug() << "Wczytam teren RAW: " << fSfile;
    QFile *file = new QFile(fSfile);
    if (!file->open(QIODevice::ReadOnly))
        return false;
    FileBuffer* data = ReadFile::readRAW(file);

    //qDebug() << data->length;
    terrainData = new float*[257];
    //int u = 0;
    for (int i = 0; i < 257; i++) {
        terrainData[i] = new float[257];
        for (int j = 0; j < 257; j++) {
            if (i == 256 && j == 256) {
                terrainData[i][j] = terrainData[(i - 1)][j - 1];
            } else if (i == 256) {
                terrainData[i][j] = terrainData[(i - 1)][j];
            } else if (j == 256) {
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

void Terrain::save() {
    QString path = Game::root + "/routes/" + Game::route + "/tiles/";
    QString filename = getTileName((int) this->mojex, (int) - this->mojez);
    if(this->tfile->sampleYbuffer == NULL)
        this->tfile->sampleYbuffer = new QString(filename + "_y.raw");
    saveRAW(path + *this->tfile->sampleYbuffer );
    this->tfile->save(path + filename + ".t");
    if(jestF && modifiedF){
        if(this->tfile->sampleFbuffer == NULL)
            this->tfile->sampleFbuffer = new QString(filename + "_f.raw");
        saveF(path + *this->tfile->sampleFbuffer);
    }
    
    for (int u = 0; u < 16; u++)
        for (int y = 0; y < 16; y++) {
            if (this->texModified[y * 16 + u] == false) continue;
            QString name = this->getTileName(mojex, -mojez) + "_" + QString::number(y) + "_" + QString::number(u) + ".ace";
            TexLib::save("ace", texturepath + name, texid[y * 16 + u]);
            this->texModified[y * 16 + u] = false;
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

    float min = 999999, max = -999999;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
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
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
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
    QFile *file = new QFile(fSfile);
    if (!file->open(QIODevice::ReadOnly))
        return false;
    FileBuffer* data = ReadFile::readRAW(file);

    //qDebug() << "Wczytam teren F: " << fSfile << data->length;

    int u = 0;
    fData = new unsigned char*[257];
    for (int j = 0; j < 257; j++) {
        fData[j] = new unsigned char[257];
        for (int i = 0; i < 257; i++) {
            if (i == 256 || j == 256)
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

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            write << fData[i][j];
        }
    }
    modifiedF = false;
    file->close();
    return;
}