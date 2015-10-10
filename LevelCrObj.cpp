#include "LevelCrObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include "TDB.h"
#include "Game.h"
#include "TrackItemObj.h"
#include "TS.h"
#include <QDebug>

LevelCrObj::LevelCrObj() {
    this->shape = -1;
    this->loaded = false;
    this->trItemId;
}

LevelCrObj::LevelCrObj(const LevelCrObj& orig) {
}

LevelCrObj::~LevelCrObj() {
}

void LevelCrObj::load(int x, int y) {
    this->shape = ShapeLib::addShape(resPath, fileName);
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 1;
    this->modified = false;
    setMartix();
}

void LevelCrObj::set(int sh, FileBuffer* data) {
    if (sh == TS::LevelCrParameters) {
        data->off++;
        levelCrParameters[0] = data->getFloat();
        levelCrParameters[1] = data->getFloat();
        return;
    }
    if (sh == TS::CrashProbability) {
        data->off++;
        crashProbability = data->getFloat();
        return;
    }
    if (sh == TS::LevelCrData) {
        data->off++;
        levelCrData[0] = data->getUint();
        levelCrData[1] = data->getUint();
        trItemId = new int[levelCrData[1]*4];
        return;
    }
    if (sh == TS::LevelCrTiming) {
        data->off++;
        levelCrTiming[0] = data->getFloat();
        levelCrTiming[1] = data->getFloat();
        levelCrTiming[2] = data->getFloat();
        return;
    }
    if (sh == TS::TrItemId) {
        data->off++;
        trItemId[trItemIdCount++] = data->getUint();
        trItemId[trItemIdCount++] = data->getUint();
        return;
    }
    if (sh == TS::FileName) {
        data->off++;
        int slen = data->getShort()*2;
        fileName = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void LevelCrObj::set(QString sh, FileBuffer* data) {
    if (sh == ("levelcrparameters")) {
        levelCrParameters[0] = ParserX::parsujr(data);
        levelCrParameters[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("crashprobability")) {
        crashProbability = ParserX::parsujr(data);
        return;
    }
    if (sh == ("levelcrdata")) {
        levelCrData[0] = ParserX::parsuj16(data);
        levelCrData[1] = ParserX::parsujr(data);
        trItemId = new int[levelCrData[1]*4];
        return;
    }
    if (sh == ("levelcrtiming")) {
        levelCrTiming[0] = ParserX::parsujr(data);
        levelCrTiming[1] = ParserX::parsujr(data);
        levelCrTiming[2] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("filename")) {
        fileName = ParserX::odczytajtc(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void LevelCrObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;
    if (shape < 0) return;
    if (jestPQ < 2) return;
    //GLUU* gluu = GLUU::get();
    //if((this.position===undefined)||this.qDirection===undefined) return;
    //
    if (size > 0) {
        if ((lod > size + 150)) {
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
            if ((ccos > 0) && (xxx > size) && (skipLevel == 1)) return;
        }
    } else {
        if (ShapeLib::shape[shape]->loaded)
            size = ShapeLib::shape[shape]->size;
    }
    
    gluu->mvPushMatrix();
    Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    if(selectionColor != 0){
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
    } else {
        gluu->enableTextures();
    }
        
    ShapeLib::shape[shape]->render();
    
    if(selected){
        drawBox();
    }
    gluu->mvPopMatrix();
    
    if(Game::viewInteractives) 
        this->renderTritems(gluu, selectionColor);
};

void LevelCrObj::renderTritems(GLUU* gluu, int selectionColor){
    
    ///////////////////////////////
    TDB* tdb = Game::trackDB;
    if(drawPosition == NULL){
        if(this->trItemId == NULL){
            qDebug() << "LevelCrObj: fail trItemId";
            loaded = false;
            return;
        }
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 0) {
            qDebug() << "LevelCrObj: fail id";
            loaded = false;
            return;
        }
                //qDebug() << "id: "<< this->trItemId[i*2+1] << " "<< id;
        drawPosition = new float[7];
        tdb->getDrawPositionOnTrNode(drawPosition, id, tdb->trackItems[this->trItemId[1]]->trItemSData1);
        drawPosition[0] += 2048 * (drawPosition[5] - this->x);
        drawPosition[2] -= 2048 * (-drawPosition[6] - this->y);
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.9,0.5,0.0);
        }
    }

    //if(pos == NULL) return;
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();
    int useSC;

    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 0 * (drawPosition[4] - this->x), drawPosition[1] + 1, -drawPosition[2] + 0 * (-drawPosition[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3]);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    pointer3d->render(selectionColor + (1)*65536*25*useSC);
    gluu->mvPopMatrix();

};

bool LevelCrObj::getBorder(float* border){
    if (shape < 0) return false;
    if (!ShapeLib::shape[shape]->loaded)
        return false;
    float* bound = ShapeLib::shape[shape]->bound;
    border[0] = bound[0];
    border[1] = bound[1];
    border[2] = bound[2];
    border[3] = bound[3];
    border[4] = bound[4];
    border[5] = bound[5];
    return true;
}

void LevelCrObj::save(QTextStream* out){
    if (!loaded) return;
    int l;
    
    QString flags2;
    flags2 = QString::number(this->levelCrData[0], 16);
    l = flags2.length();
    for(int i=0; i<8-l; i++)
        flags2 = "0"+flags2; 
    
*(out) << "	LevelCr (\n";
    
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		LevelCrParameters ( "<<this->levelCrParameters[0]<<" "<<this->levelCrParameters[1]<<" )\n";
*(out) << "		CrashProbability ( "<<this->crashProbability<<" )\n";
*(out) << "		LevelCrData ( "<<flags2<<" "<<this->levelCrData[1]<<" )\n";
*(out) << "		LevelCrTiming ( "<<this->levelCrTiming[0]<<" "<<this->levelCrTiming[1]<<" "<<this->levelCrTiming[2]<<" )\n";
for(int i = 0; i < trItemIdCount; i+=2){
*(out) << "		TrItemId ( "<<this->trItemId[i]<<" "<<this->trItemId[i+1]<<" )\n";
}
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}