#include "Terrain.h"
#include <QDebug>
#include "Game.h"
#include <QFile>
#include "ReadFile.h"
#include "TexLib.h"
#include "TerrainLib.h"
#include "GLMatrix.h"

Terrain::Terrain(float x, float y) {
    loaded = false;
    isOgl = false;
    wTexid = -1;
    for (int i = 0; i < 256; i++) {
        texid[i] = -1;
    }
    mojex = x;
    mojez = y;
    texturepath = Game::root + "/routes/" + Game::route + "/terrtex/";
    QString path = Game::root + "/routes/" + Game::route + "/tiles/";
    tfile = new TFile();

    QString filename = getTileName((int) x, (int) -y);
    //qDebug() << filename;
    if (!tfile->readT((path + filename + ".t"))) {
        //qDebug() << " t fail";
        return;
    }
    if (!readRAW((path + filename + "_y.raw"))) {
        //qDebug() << " y fail";
        return;
    }
    jestF = readF(path + filename + "_f.raw");
    loaded = true;
}

Terrain::Terrain(const Terrain& orig) {
}

Terrain::~Terrain() {
    qDebug() << "= release terrain";

    if(this->loaded){
        for (int i = 0; i < 257; i++){
            delete terrainData[i];
            if(this->jestF)
                delete fData[i];
        }

        delete terrainData;
        if(this->jestF)
            delete fData;
    }
}

void Terrain::saveEmpty(int x, int y) {
    QString name = getTileName(x, y);
    qDebug() << "Aa";
    QFile file(Game::root + "/routes/" + Game::route + "/tiles/" + name + "_y.raw");
    if(!file.exists())
        QFile::copy("resources/templateRoute/tiles/template_y.raw", Game::root + "/routes/" + Game::route + "/tiles/" + name + "_y.raw");
    file.setFileName(Game::root + "/routes/" + Game::route + "/tiles/" + name + ".t");
    if(!file.exists())
        QFile::copy("resources/templateRoute/tiles/template.t", Game::root + "/routes/" + Game::route + "/tiles/" + name + ".t");
}

QString Terrain::getTileName(int x, int y) {
    int offset = 16384;
    int xs = offset;
    int ys = offset;
    x += offset;
    y += offset;
    unsigned int numer = 0;
    
    for (int i = offset / 2, j = 30; j > 0; i /= 2, j-=2) {
        if (x < xs && y < ys) {
            numer = numer | ( 3 << j);
            xs -= i;
            ys -= i;
        } else if (x < xs) {
            xs -= i;
            ys += i;
        } else if (y < ys) {
            numer = numer | ( 2 << j);
            xs += i;
            ys -= i;
        } else {
            numer = numer | ( 1 << j);
            xs += i;
            ys += i;
        }
    }

    QString name = "-"+QString::number(numer, 16);
    //qDebug() << name;
    return name;
}

void Terrain::render(float lodx, float lodz, float * playerT, float* playerW, float* target, float fov) {
    if (!loaded) 
        return;
    if (!isOgl) {
        TerrainLib::fillRAW(terrainData, (int)mojex, (int)mojez);
        vertexInit();
        //normalInit();
        oglInit();
        isOgl = true;
    }
    
    GLUU* gluu = GLUU::get();
    if (!lines.loaded) {
        float *punkty = new float[256*6*4];
        int ptr = 0;
        int i = 0;
        
        for(i = 0; i < 256; i++){
            punkty[ptr++] = -1024;
            punkty[ptr++] = 1 + terrainData[i][0];
            punkty[ptr++] = -1024 + i*8;
            punkty[ptr++] = -1024;
            punkty[ptr++] = 1 + terrainData[i+1][0];
            punkty[ptr++] = -1024 + i*8 + 8;
        }

        for(i = 0; i < 256; i++){
            punkty[ptr++] = -1024 + i*8;
            punkty[ptr++] = 1 + terrainData[0][i];
            punkty[ptr++] = -1024;
            punkty[ptr++] = -1024 + i*8 + 8;
            punkty[ptr++] = 1 + terrainData[0][i+1];
            punkty[ptr++] = -1024;
        }
        
        for(i = 0; i < 256; i++){
            punkty[ptr++] = 1024;
            punkty[ptr++] = 1 + terrainData[i][256];
            punkty[ptr++] = -1024 + i*8;
            punkty[ptr++] = 1024;
            punkty[ptr++] = 1 + terrainData[i+1][256];
            punkty[ptr++] = -1024 + i*8 + 8;
        }

        for(i = 0; i < 256; i++){
            punkty[ptr++] = -1024 + i*8;
            punkty[ptr++] = 1 + terrainData[256][i];
            punkty[ptr++] = 1024;
            punkty[ptr++] = -1024 + i*8 + 8;
            punkty[ptr++] = 1 + terrainData[256][i+1];
            punkty[ptr++] = 1024;
        }
        
        lines.setMaterial(1.0,0.0,0.0);
        lines.init(punkty, ptr, lines.V, GL_LINES);
        delete punkty;
    }
    
    Mat4::identity(gluu->objStrMatrix);
    gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    lines.render();
    
    gluu->enableTextures();
    int off = 0;
    float lod = 0;
    float size = 512;
    for (int uu = 0; uu < 16; uu++) {
        for (int yy = 0; yy < 16; yy++) {
            float lodxx = lodx + uu * 128 - 1024;
            float lodzz = lodz + yy * 128 - 1024;
            lod = sqrt(lodxx * lodxx + lodzz * lodzz);
            //System.out.println("-- "+lodxx+" "+lodzz);
            if ( lod > Game::objectLod) continue;
            
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
                    texid[yy * 16 + uu] = TexLib::addTex(texturepath, tfile->materials[(int)tfile->tdata[(yy * 16 + uu)*7+0]].tex[0]);
                    //System.out.println(tfile.materials[tfile.tdata[uu*16+yy]].tex[0]);
                    //texid = TexLib.addTex(texturepath,"nasyp-k.ace", gl);
                //    gl.glDisable(GL2.GL_TEXTURE_2D);
                }
                if (TexLib::mtex[texid[yy * 16 + uu]]->loaded) {
                    if (!TexLib::mtex[texid[yy * 16 + uu]]->glLoaded)
                        TexLib::mtex[texid[yy * 16 + uu]]->GLTextures();
                    glBindTexture(GL_TEXTURE_2D, TexLib::mtex[texid[yy * 16 + uu]]->tex[0]);
                    //System.out.println(tfile.materials[tfile.tdata[uu*16+yy]].tex[0]);
                  //  gl.glEnable(GL2.GL_TEXTURE_2D);
                    //gl.glDisable(GL2.GL_ALPHA_TEST);
                    //gl.glDisable(GL2.GL_BLEND);
                } else {
                   // gl.glDisable(GL2.GL_TEXTURE_2D);
                }
            }
             
            QOpenGLVertexArrayObject::Binder vaoBinder(&VAO[uu * 16 + yy]);
            glDrawArrays(GL_TRIANGLES, 0, 16*16*6);

            if ((tfile->flags[yy * 16 + uu] & 0xc0) != 0) {
                
                if(!water[uu * 16 + yy].loaded){
                    
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

                    float *punkty = new float[48];
                    int ptr = 0;
                    
                    punkty[ptr++] = x2;
                    punkty[ptr++] = x2z2;
                    punkty[ptr++] = z2;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = 4;
                    
                    punkty[ptr++] = x2;
                    punkty[ptr++] = x2z1;
                    punkty[ptr++] = z1;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = 0;
                    
                    punkty[ptr++] = x1;
                    punkty[ptr++] = x1z1;
                    punkty[ptr++] = z1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 0;
                    
                    punkty[ptr++] = x1;
                    punkty[ptr++] = x1z2;
                    punkty[ptr++] = z2;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 4;
                    
                    punkty[ptr++] = x2;
                    punkty[ptr++] = x2z2;
                    punkty[ptr++] = z2;
                    punkty[ptr++] = 4;
                    punkty[ptr++] = 4;
                    
                    punkty[ptr++] = x1;
                    punkty[ptr++] = x1z1;
                    punkty[ptr++] = z1;
                    punkty[ptr++] = 0;
                    punkty[ptr++] = 0;
                    
                    QString *texturePath = new QString("resources/woda.ace");
                    water[uu * 16 + yy].setMaterial(texturePath);
                    water[uu * 16 + yy].init(punkty, ptr, water[uu * 16 + yy].VT, GL_TRIANGLES);
                    delete punkty;
                }
                water[uu * 16 + yy].render();
            }
        }
    }
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

            Vector3f::sub(U, vertexData[jj][ii], vertexData[jj + 1][ii]);
            Vector3f::sub(V, vertexData[jj][ii], vertexData[jj][ii + 1]);
            Vector3f::add(normalData[jj][ii], normalData[jj][ii], Vector3f::cross(O, U, V));
            Vector3f::add(normalData[jj + 1][ii], normalData[jj + 1][ii], Vector3f::cross(O, U, V));
            Vector3f::add(normalData[jj][ii + 1], normalData[jj][ii + 1], Vector3f::cross(O, U, V));

            Vector3f::sub(U, vertexData[jj + 1][ii + 1], vertexData[jj + 1][ii]);
            Vector3f::sub(V, vertexData[jj + 1][ii + 1], vertexData[jj][ii + 1]);
            Vector3f::add(normalData[jj + 1][ii + 1], normalData[jj + 1][ii + 1], Vector3f::cross(O, V, U));
            Vector3f::add(normalData[jj + 1][ii], normalData[jj + 1][ii], Vector3f::cross(O, V, U));
            Vector3f::add(normalData[jj][ii + 1], normalData[jj][ii + 1], Vector3f::cross(O, V, U));
        }
    }

    for (int jj = 0; jj < 257; jj++) {
        for (int ii = 0; ii < 257; ii++) {
            normalData[jj][ii].div(-1);
            normalData[jj][ii].normalize();
            //System.out.print(normal.x+" "+normal.y+" "+normal.z+";");
            //normalData[jj][ii] = normal;
        }
    }
};

void Terrain::oglInit() {
    int ilosc = 16 * 16;
    int suma;
    float * punkty = new float[16 * 16 * 30];
    //  var punkty = Terrain.punkty;
        for(int uu = 0; uu<16; uu++){
            for(int yy = 0; yy<16; yy++){
            int ptr = 0;
            
            for (int ii = 0; ii < 16; ii++) {
                for (int jj = 0; jj < 16; jj++) {
                    if(jestF){
                        suma = ((fData[yy*16+ii][uu*16+jj]) & 0x04);
                        suma += ((fData[yy*16+ii][uu*16+jj+1]) & 0x04);
                        suma += ((fData[yy*16+ii+1][uu*16+jj+1]) & 0x04);
                        suma += ((fData[yy*16+ii+1][uu*16+jj]) & 0x04);
                    }
                    
                    if(!jestF || (suma < 4)){
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].x;
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].y;
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].z;
                    punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*7 + 3] + ii * tfile->tdata[(yy * 16 + uu)*7 + 4] + tfile->tdata[(yy * 16 + uu)*7 + 1];
                    punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*7 + 5] + ii * tfile->tdata[(yy * 16 + uu)*7 + 6] + tfile->tdata[(yy * 16 + uu)*7 + 2];
                    
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].x;
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].y;
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii + 1].z;
                    punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*7 + 3] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*7 + 4] + tfile->tdata[(yy * 16 + uu)*7 + 1];
                    punkty[ptr++] = (jj) * tfile->tdata[(yy * 16 + uu)*7 + 5] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*7 + 6] + tfile->tdata[(yy * 16 + uu)*7 + 2];
                    
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                    punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*7 + 3] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*7 + 4] + tfile->tdata[(yy * 16 + uu)*7 + 1];
                    punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*7 + 5] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*7 + 6] + tfile->tdata[(yy * 16 + uu)*7 + 2];
                    
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].x;
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].y;
                    punkty[ptr++] = vertexData[(uu * 16 + jj)][yy * 16 + ii].z;
                    punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*7 + 3] + ii * tfile->tdata[(yy * 16 + uu)*7 + 4] + tfile->tdata[(yy * 16 + uu)*7 + 1];
                    punkty[ptr++] = jj * tfile->tdata[(yy * 16 + uu)*7 + 5] + ii * tfile->tdata[(yy * 16 + uu)*7 + 6] + tfile->tdata[(yy * 16 + uu)*7 + 2];
                    
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].x;
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].y;
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii + 1].z;
                    punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*7 + 3] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*7 + 4] + tfile->tdata[(yy * 16 + uu)*7 + 1];
                    punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*7 + 5] + (ii + 1) * tfile->tdata[(yy * 16 + uu)*7 + 6] + tfile->tdata[(yy * 16 + uu)*7 + 2];
                    
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].x;
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].y;
                    punkty[ptr++] = vertexData[(uu * 16 + jj + 1)][yy * 16 + ii].z;
                    punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*7 + 3] + (ii) * tfile->tdata[(yy * 16 + uu)*7 + 4] + tfile->tdata[(yy * 16 + uu)*7 + 1];
                    punkty[ptr++] = (jj + 1) * tfile->tdata[(yy * 16 + uu)*7 + 5] + (ii) * tfile->tdata[(yy * 16 + uu)*7 + 6] + tfile->tdata[(yy * 16 + uu)*7 + 2];
                    }
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
           
            QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
            VAO[uu * 16 + yy].create();
            QOpenGLVertexArrayObject::Binder vaoBinder(&VAO[uu * 16 + yy]);

            VBO[uu * 16 + yy].create();
            VBO[uu * 16 + yy].bind();
            VBO[uu * 16 + yy].allocate(punkty, 16 * 16 * 6 * 5 * sizeof (GLfloat));
            f->glEnableVertexAttribArray(0);
            f->glEnableVertexAttribArray(1);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
            f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
            VBO[uu * 16 + yy].release();            
        }
    }

    delete punkty;
    //for (int i = 0; i < 257; i++)
    //    delete normalData[i];
    //delete normalData;
    for (int i = 0; i < 257; i++)
        delete vertexData[i];
    delete vertexData;
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
                terrainData[i][j] = tfile->floor + tfile->scale * (data->get() + 256*data->get());
                //terrainData[i][j] = tfile->floor + tfile->scale * (data->data[u++] + 256*data->data[u++]);
            }
        }
    }
    delete data;
    return true;
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
        for(int j = 0; j<257; j++){
            fData[j] = new unsigned char[257];
            for(int i = 0; i<257; i++){
                if(i==256||j==256)
                    fData[j][i] = 0;
                else
                    fData[j][i] = data->data[u++];
            }
        }
        delete data;
        return true;
    }    