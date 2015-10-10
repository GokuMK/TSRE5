#include "PickupObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include "Game.h"
#include "TDB.h"
#include "TrackItemObj.h"
#include "TS.h"

PickupObj::PickupObj() {
    this->shape = -1;
    this->loaded = false;
    pointer3d = NULL;
}

PickupObj::PickupObj(const PickupObj& orig) {
}

PickupObj::~PickupObj() {
}

void PickupObj::load(int x, int y) {
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

bool PickupObj::allowNew(){
    return true;
}

bool PickupObj::isTrackItem(){
    return true;
}

void PickupObj::initTrItems(float* tpos){
    if(tpos == NULL)
        return;
    int trNodeId = tpos[0];
    float metry = tpos[1];
    
    TDB* tdb = Game::trackDB;
    qDebug() <<"new pickup  "<<this->fileName;

    tdb->newPickupObject(trItemId, trNodeId, metry, this->typeID);
    drawPosition = NULL;
    pickupType[0] = 5;
    pickupCapacity1 = 20000;
    setPickupContent(20000);
    pickupCapacity2 = 2000;
    speedRange[0] = 0;
    speedRange[1] = 0;
    pickupAnimData1 = 3;
    pickupAnimData2 = 2;
    pickupType[1] = 0;
}

void PickupObj::setPickupContent(float val){
    TDB* tdb = Game::trackDB;
    if(tdb->trackItems[this->trItemId[1]] == NULL)
        return;
    tdb->trackItems[this->trItemId[1]]->setPickupContent(val);
}

void PickupObj::set(QString sh, QString val){
    if (sh == ("filename")) {
        fileName = val;
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void PickupObj::set(int sh, FileBuffer* data) {
    if (sh == TS::SpeedRange) {
        data->off++;
        speedRange[0] = data->getFloat();
        speedRange[1] = data->getFloat();
        return;
    }
    if (sh == TS::PickupType) {
        data->off++;
        pickupType[0] = data->getUint();
        pickupType[1] = data->getUint();
        return;
    }
    if (sh == TS::PickupAnimData) {
        data->off++;
        pickupAnimData1 = data->getUint();
        pickupAnimData2 = data->getFloat();
        return;
    }
    if (sh == TS::PickupCapacity) {
        data->off++;
        pickupCapacity1 = data->getFloat();
        pickupCapacity2 = data->getFloat();
        return;
    }
    if (sh == TS::TrItemId) {
        data->off++;
        this->trItemId = new int[2];
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

void PickupObj::set(QString sh, FileBuffer* data) {
    if (sh == ("speedrange")) {
        speedRange[0] = ParserX::parsujr(data);
        speedRange[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("pickuptype")) {
        pickupType[0] = ParserX::parsujr(data);
        pickupType[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("pickupanimdata")) {
        pickupAnimData1 = ParserX::parsujr(data);
        pickupAnimData2 = ParserX::parsujr(data);
        return;
    }
    if (sh == ("pickupcapacity")) {
        pickupCapacity1 = ParserX::parsujr(data);
        pickupCapacity2 = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId = new int[2];
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

void PickupObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;
    if (shape < 0) return;
    if (jestPQ < 2) return;

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
void PickupObj::renderTritems(GLUU* gluu, int selectionColor){
    
    ///////////////////////////////
    TDB* tdb = Game::trackDB;
    if(drawPosition == NULL){
        if(this->trItemId == NULL){
            qDebug() << "PickupObj: fail trItemId";
            loaded = false;
            return;
        }
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 0) {
            qDebug() << "PickupObj: fail id";
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
            pointer3d->setMaterial(0.8,0.2,0.8);
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

bool PickupObj::getBorder(float* border){
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

float PickupObj::getPickupContent(){
    TRitem* trit = Game::trackDB->trackItems[this->trItemId[1]];
    if(trit == NULL) return 0;
    return trit->pickupTrItemData1;
    return 0;
}

void PickupObj::save(QTextStream* out){
    if (!loaded) return;

*(out) << "	Pickup (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		SpeedRange ( "<<this->speedRange[0]<<" "<<this->speedRange[1]<<" )\n";
*(out) << "		PickupType ( "<<this->pickupType[0]<<" "<<this->pickupType[1]<<" )\n";
*(out) << "		PickupAnimData ( "<<this->pickupAnimData1<<" "<<this->pickupAnimData2<<" )\n";
*(out) << "		PickupCapacity ( "<<this->pickupCapacity1<<" "<<this->pickupCapacity2<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[0]<<" "<<this->trItemId[1]<<" )\n";
*(out) << "		CollideFlags ( "<<this->collideFlags<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}