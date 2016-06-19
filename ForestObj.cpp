#include <QFile>
#include "ForestObj.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <cstdlib>
#include "TexLib.h"
#include "Vector2f.h"
#include "TerrainLib.h"
#include "TS.h"
#include "Game.h"
#include "FileFunctions.h"
#include "ReadFile.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::vector<ForestObj::ForestList> ForestObj::forestList;

ForestObj::ForestObj() {
    this->loaded = false;
}

ForestObj::ForestObj(const ForestObj& orig) {
}

ForestObj::~ForestObj() {
}

void ForestObj::loadForestList(){
    QString path = Game::root + "/routes/" + Game::route + "/forests.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    ParserX::NextLine(data);

    QString sh = "";
    //ParserX::szukajsekcji1(sh, data);
    //ParserX::GetNumber(data);
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("forest")) {
            ForestObj::forestList.emplace_back();
            ForestObj::forestList.back().name = ParserX::GetString(data);
            ForestObj::forestList.back().texture = ParserX::GetString(data);
            ForestObj::forestList.back().treeSizeX = ParserX::GetNumber(data);
            ForestObj::forestList.back().treeSizeZ = ParserX::GetNumber(data);
            ForestObj::forestList.back().scaleRangeX = ParserX::GetNumber(data);
            ForestObj::forestList.back().scaleRangeZ = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }
}

void ForestObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->loaded = true;
    this->size = -1;
    this->tex = -1;
    this->init = false;
    this->skipLevel = 3;
    this->modified = false;
}

void ForestObj::set(QString sh, int val) {
    if (sh == ("ref_value")) {
        treeTexture = ForestObj::forestList[val].texture;
        scaleRangeX = ForestObj::forestList[val].scaleRangeX;
        scaleRangeZ = ForestObj::forestList[val].scaleRangeZ;
        areaX = 100;
        areaZ = 100;
        treeSizeX = ForestObj::forestList[val].treeSizeX;
        treeSizeZ = ForestObj::forestList[val].treeSizeZ;
        population = 10;
        return;
    }
    WorldObj::set(sh, val);
    this->modified = true;
}


void ForestObj::set(int sh, FileBuffer* data) {
    if (sh == TS::TreeTexture) {
        data->off++;
        int slen = data->getShort()*2;
        treeTexture = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    if (sh == TS::ScaleRange) {
        data->off++;
        scaleRangeX = data->getFloat();
        scaleRangeZ = data->getFloat();
        return;
    }
    if (sh == TS::Area) {
        data->off++;
        areaX = data->getFloat();
        areaZ = data->getFloat();
        return;
    }
    if (sh == TS::TreeSize) {
        data->off++;
        treeSizeX = data->getFloat();
        treeSizeZ = data->getFloat();
        return;
    }
    if (sh == TS::Population) {
        data->off++;
        population = data->getUint();
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void ForestObj::set(QString sh, FileBuffer* data) {
    if (sh == ("treetexture")) {
        treeTexture = ParserX::GetString(data);
        return;
    }
    if (sh == ("scalerange")) {
        scaleRangeX = ParserX::GetNumber(data);
        scaleRangeZ = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("area")) {
        areaX = ParserX::GetNumber(data);
        areaZ = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("treesize")) {
        treeSizeX = ParserX::GetNumber(data);
        treeSizeZ = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("population")) {
        population = ParserX::GetNumber(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void ForestObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;
    //if (jestPQ < 2) return;
    //GLUU* gluu = GLUU::get();
    //if((this.position===undefined)||this.qDirection===undefined) return;

    /*if (size > 0) {
        if ((lod > size)) {
            float v1[2];
            v1[0] = pos[0] - (target[0]);
            v1[1] = pos[2] - (target[2]);
            float v2[2];
            v2[0] = posx;
            v2[1] = posz;
            float iloczyn = v1[0] * v2[0] + v1[1] * v2[1];
            float d1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
            float d2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
            float zz = iloczyn / (d1 * d2);
            if (zz > 0) return;

            float ccos = cos(fov) + zz;
            float xxx = sqrt(2 * d2 * d2 * (1 - ccos));
            //if((ccos > 0) && (xxx > 200+50)) return;
            if ((ccos > 0) && (xxx > size + 150) && (skipLevel == 1)) return;
        }
    } else {
        if (ShapeLib::shape[shape]->loaded)
            size = ShapeLib::shape[shape]->size;
    }
*/
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, position[0], 0, position[2]);
    //float scale = sqrt(qDirection[0] * qDirection[0] + qDirection[1] * qDirection[1] + qDirection[2] * qDirection[2]);
    //float angle = ((acos(qDirection[3])*360) / M_PI);
    //Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, gluu->degToRad(-angle), -qDirection[0] * scale, -qDirection[1] * scale, qDirection[2] * scale);
    //Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, gluu->degToRad(180), 0, -1, 0);

    //if(selected){
    //    selected = !selected;
    //    selectionColor = 155;
    //}
    //gluu.setMatrixUniforms();

    //
    //var z = this.position[0]*mmm[9] + this.position[1]*mmm[7] + this.position[2]*mmm[9];

    Mat4::identity(gluu->objStrMatrix);    
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->m_program->setUniformValue(gluu->shaderAlpha, 0.0f);
    
    if(selectionColor != 0){
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
    } else {
        gluu->enableTextures();
    }
    
    drawShape();
};

void ForestObj::drawShape(){
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (tex == -2) {
        f->glDisable(GL_TEXTURE_2D);
    } else {
        f->glEnable(GL_TEXTURE_2D);
        if (tex == -1) {
            tex = TexLib::addTex(resPath, treeTexture);
            f->glDisable(GL_TEXTURE_2D);
        }
    }

    if (!init) {
            shape.iloscv = population*24;
            float* punkty = new float[shape.iloscv*8];
            int ptr = 0;

            int seed = (int)(position[0] + position[1] + position[2]);
            //Random random = new Random(seed);
            std::srand(seed);
            float treeSizeXt = treeSizeX*0.7;
            for(int uu = 0; uu < population; uu++){

                float tposx = ((float)((std::rand()%100))/100)*areaX-areaX/2.0;
                float tposz = ((float)((std::rand()%100))/100)*areaZ-areaZ/2.0;
                Vector2f uuu(tposx,tposz);
                uuu.rotate(((qDirection[1]+0.00001f)/fabs(qDirection[1]+0.00001f))*(float)-acos(qDirection[3])*2.0, 0);
                tposx = uuu.x;
                tposz = uuu.y;

                float wysokosc = TerrainLib::getHeight(x, y, tposx+position[0], tposz+position[2]);
                
                for(int j = -1; j < 2; j+=2){
                    for(int i = -1; i<2; i+=2){
                            punkty[ptr++] = -treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc+treeSizeZ;
                            punkty[ptr++] = -treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 0; punkty[ptr++] = 0;
                            
                            punkty[ptr++] = treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc+treeSizeZ;
                            punkty[ptr++] = treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 1; punkty[ptr++] = 0;
                            
                            punkty[ptr++] = treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc;
                            punkty[ptr++] = treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 1; punkty[ptr++] = 1;
                            
                            punkty[ptr++] = -treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc;
                            punkty[ptr++] = -treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 0; punkty[ptr++] = 1;
                            
                            punkty[ptr++] = -treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc+treeSizeZ;
                            punkty[ptr++] = -treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 0; punkty[ptr++] = 0;
                            
                            punkty[ptr++] = treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc;
                            punkty[ptr++] = treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 1; punkty[ptr++] = 1;
                    }
                }
            }
            
        shape.VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&shape.VAO);

        shape.VBO.create();
        shape.VBO.bind();
        shape.VBO.allocate(punkty, ptr * sizeof (GLfloat));
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (6 * sizeof (GLfloat)));
        shape.VBO.release();
        //shape.iloscv = ptr/8;

        delete[] punkty;
        init = true;
    } else {
        
        if(TexLib::mtex[tex]->loaded){
            if(!TexLib::mtex[tex]->glLoaded) TexLib::mtex[tex]->GLTextures();
            f->glBindTexture(GL_TEXTURE_2D, TexLib::mtex[tex]->tex[0]);
        }
        
        QOpenGLVertexArrayObject::Binder vaoBinder1(&shape.VAO);
        f->glDrawArrays(GL_TRIANGLES, 0, shape.iloscv);
    }
}

void ForestObj::save(QTextStream* out){
    if (!loaded) return;
    
*(out) << "	Forest (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
if(this->treeTexture != "")
*(out) << "		TreeTexture ( "<<this->treeTexture<<" )\n";
*(out) << "		ScaleRange ( "<<this->scaleRangeX<<" "<<this->scaleRangeZ<<" )\n";
*(out) << "		Area ( "<<this->areaX<<" "<<this->areaZ<<" )\n";
*(out) << "		Population ( "<<this->population<<" )\n";
*(out) << "		TreeSize ( "<<this->treeSizeX<<" "<<this->treeSizeZ<<" )\n";
*(out) << "		StaticFlags ( "<<ParserX::MakeFlagsString(this->staticFlags)<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}

bool ForestObj::allowNew(){
    return true;
}