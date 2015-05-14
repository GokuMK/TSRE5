#include "SpeedpostObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
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
        for(int i = 0; i<9; i++)
            speedSignShape[i] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("speed_text_size")) {
        speedTextSize[0] = ParserX::parsujr(data);
        speedTextSize[1] = ParserX::parsujr(data);
        speedTextSize[2] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[0] = ParserX::parsujUint(data);
        trItemId[1] = ParserX::parsujUint(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SpeedpostObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;
    if (shape < 0) return;
    if (jestPQ < 2) return;
    //GLUU* gluu = GLUU::get();
    //if((this.position===undefined)||this.qDirection===undefined) return;

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
*(out) << "		Speed_Sign_Shape ( "<<this->speedSignShape[0]<<" "<<this->speedSignShape[1]<<" "<<this->speedSignShape[2]<<" "<<this->speedSignShape[3]<<" "<<this->speedSignShape[4]<<" "<<this->speedSignShape[5]<<" "<<this->speedSignShape[6]<<" "<<this->speedSignShape[7]<<" "<<this->speedSignShape[8]<<" )\n";
*(out) << "		Speed_Text_Size ( "<<this->speedTextSize[0]<<" "<<this->speedTextSize[1]<<" "<<this->speedTextSize[2]<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[0]<<" "<<this->trItemId[1]<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
*(out) << "	)\n";
}