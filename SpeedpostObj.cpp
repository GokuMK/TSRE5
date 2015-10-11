#include "SpeedpostObj.h"
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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SpeedpostObj::SpeedpostObj() {
    this->shape = -1;
    this->loaded = false;
}

SpeedpostObj::SpeedpostObj(const SpeedpostObj& orig) {
}

SpeedpostObj::~SpeedpostObj() {
}

void SpeedpostObj::load(int x, int y) {
    this->shape = ShapeLib::addShape(resPath, fileName);
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 1;
    this->modified = false;
    this->drawPosition = NULL;
    this->pointer3d = NULL;
    setMartix();
}

void SpeedpostObj::set(QString sh, QString val){
    if (sh == ("filename")) {
        fileName = val;
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void SpeedpostObj::set(int sh, FileBuffer* data) {
    if (sh == TS::FileName) {
        data->off++;
        int slen = data->getShort()*2;
        fileName = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    if (sh == TS::Speed_Digit_Tex) {
        data->off++;
        int slen = data->getShort()*2;
        speedDigitTex = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    if (sh == TS::Speed_Sign_Shape) {
        data->off++;
        int len = data->getUint();
        speedSignShape = new float[len*4+1];
        speedSignShape[0] = len;
        for(int i = 0; i<speedSignShape[0]*4; i++)
            speedSignShape[i+1] = data->getFloat();
        return;
    }
    if (sh == TS::Speed_Text_Size) {
        data->off++;
        speedTextSize[0] = data->getFloat();
        speedTextSize[1] = data->getFloat();
        speedTextSize[2] = data->getFloat();
        return;
    }
    if (sh == TS::TrItemId) {
        data->off++;
        this->trItemId = new int[2];
        trItemId[0] = data->getUint();
        trItemId[1] = data->getUint();
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SpeedpostObj::set(QString sh, FileBuffer* data) {
    if (sh == ("filename")) {
        fileName = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("speed_digit_tex")) {
        speedDigitTex = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("speed_sign_shape")) {
        int len = ParserX::parsujr(data);
        speedSignShape = new float[len*4+1];
        speedSignShape[0] = len;
        for(int i = 0; i<speedSignShape[0]*4; i++)
            speedSignShape[i+1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("speed_text_size")) {
        speedTextSize[0] = ParserX::parsujr(data);
        speedTextSize[1] = ParserX::parsujr(data);
        speedTextSize[2] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemid")) {
        this->trItemId = new int[2];
        this->trItemId[0] = ParserX::parsujUint(data);
        this->trItemId[1] = ParserX::parsujUint(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SpeedpostObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;

    if(Game::viewInteractives) 
        this->renderTritems(gluu, selectionColor);
};

void SpeedpostObj::renderTritems(GLUU* gluu, int selectionColor){
    
    ///////////////////////////////
    TDB* tdb = Game::trackDB;
    if(drawPosition == NULL){
        if(this->trItemId == NULL){
            qDebug() << "speedpost: fail trItemId";
            loaded = false;
            return;
        }
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 0) {
            qDebug() << "speedpost: fail id";
            loaded = false;
            return;
        }
                //qDebug() << "id: "<< this->trItemId[i*2+1] << " "<< id;
        drawPosition = new float[7];
        //qDebug() << this->trItemId[1];
        bool ok = tdb->getDrawPositionOnTrNode(drawPosition, id, tdb->trackItems[this->trItemId[1]]->trItemSData1);
        if(!ok){
            this->loaded = false;
            return;
        }
        drawPosition[0] += 2048 * (drawPosition[5] - this->x);
        drawPosition[2] -= 2048 * (-drawPosition[6] - this->y);
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.7,0.7,0.7);
        }
    }
    
    //int aaa = drawPosition[0];
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

bool SpeedpostObj::getBorder(float* border){
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

void SpeedpostObj::save(QTextStream* out){
    if (!loaded) return;
*(out) << "	Speedpost (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		Speed_Digit_Tex ( "<<this->speedDigitTex<<" )\n";
*(out) << "		Speed_Sign_Shape ( "<<this->speedSignShape[0];
for(int i = 0; i < this->speedSignShape[0]*4; i++)
    *(out) <<" "<<this->speedSignShape[i+1];
*(out) <<" )\n";
//<<" "<<this->speedSignShape[1]<<" "<<this->speedSignShape[2]<<" "<<this->speedSignShape[3]<<" "<<this->speedSignShape[4]<<" "<<this->speedSignShape[5]<<" "<<this->speedSignShape[6]<<" "<<this->speedSignShape[7]<<" "<<this->speedSignShape[8]<<" )\n";
*(out) << "		Speed_Text_Size ( "<<this->speedTextSize[0]<<" "<<this->speedTextSize[1]<<" "<<this->speedTextSize[2]<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[0]<<" "<<this->trItemId[1]<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
*(out) << "	)\n";
}