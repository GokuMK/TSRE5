#include "SignalObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include "TrackItemObj.h"
#include "TDB.h"
#include "TRitem.h"
#include "SigCfg.h"
#include "SignalShape.h"
#include "Game.h"
#include "TS.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SignalObj::SignalObj() {
    this->shape = -1;
    this->loaded = false;
    //pointer3d = new TrackItemObj();
    //pointer3d->setMaterial(1,0,0);
    //pointer3dSelected = new TrackItemObj();
    //pointer3dSelected->setMaterial(1,0.5,0.5);
}

SignalObj::SignalObj(const SignalObj& orig) {
}

SignalObj::~SignalObj() {
}

void SignalObj::load(int x, int y) {
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

bool SignalObj::allowNew(){
    return true;
}

bool SignalObj::isTrackItem(){
    return true;
}

void SignalObj::initTrItems(float* tpos){
    if(tpos == NULL)
        return;
    int trNodeId = tpos[0];
    float metry = tpos[1];
    
    TDB* tdb = Game::trackDB;
    qDebug() <<"new signal  "<<this->fileName;

    tdb->newSignalObject(this->fileName, trItemId, this->signalUnits, trNodeId, metry, this->typeID);
    
    this->signalSubObj = 0;
    qDebug() <<"signalUnits  "<<this->signalUnits;
    for(int i = 0; i < this->signalUnits; i++)
        this->signalSubObj = this->signalSubObj | (1 << i);
   // this->trItemIdCount = 4;
  //  this->trItemId[0] = isRoad;
   // this->trItemId[1] = trItemId[0];
   // this->trItemId[2] = isRoad;
  //  this->trItemId[3] = trItemId[1];
    this->drawPositions = NULL;
}

void SignalObj::set(QString sh, int val) {
    if (sh == ("_refvalue")) {
        SignalShape * signal = Game::trackDB->sigCfg->signalShapeById[val];
        fileName = signal->name;
        return;
    }
    WorldObj::set(sh, val);
    this->modified = true;
}

void SignalObj::set(QString sh, QString val){
    if (sh == ("filename")) {
        fileName = val;
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void SignalObj::set(int sh, FileBuffer* data) {
    if (sh == TS::FileName) {
        data->off++;
        int slen = data->getShort()*2;
        fileName = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    if (sh == TS::SignalSubObj) {
        data->off++;
        signalSubObj = data->getUint();
        return;
    }
    if (sh == TS::SignalUnits) {
        data->off++;
        signalUnits = data->getUint();
        trItemId = new int[signalUnits*2];
        for(int i=0; i<signalUnits; i++){
            data->getToken();
            data->getInt();
            data->off++;
            data->getInt();
            data->getToken();
            data->getInt();
            data->off++;
            trItemId[i*2+0] = data->getUint();
            trItemId[i*2+1] = data->getUint();
        }
        return;
    }
    
    WorldObj::set(sh, data);
    return;
}

void SignalObj::set(QString sh, FileBuffer* data) {
    if (sh == ("filename")) {
        fileName = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("signalsubobj")) {
        signalSubObj = ParserX::parsuj16(data);
        return;
    }
    if (sh == ("signalunits")) {
        signalUnits = ParserX::parsujr(data);
        trItemId = new int[signalUnits*2];
        for(int i=0; i<signalUnits; i++){
            ParserX::parsujr(data);
            trItemId[i*2+0] = ParserX::parsujr(data);
            trItemId[i*2+1] = ParserX::parsujr(data);
        }
        return;
    }
    WorldObj::set(sh, data);
    return;
}

int SignalObj::getLinkedJunctionValue(int i){
    if(i >= this->signalUnits) return -1;
    TDB* tdb = Game::trackDB;
    int tritId = this->trItemId[i*2+1];
    TRitem* trit = tdb->trackItems[tritId];
    if(trit == NULL) return -1;
    if(trit->trSignalDirs < 1) return 0;
    return trit->trSignalDir[0];
}

bool SignalObj::isJunctionAvailable(int i){
    if(i >= this->signalUnits) return false;
    TDB* tdb = Game::trackDB;
    int tritId = this->trItemId[i*2+1];
    TRitem* trit = tdb->trackItems[tritId];
    if(trit == NULL) return false;
    int nid = tdb->findTrItemNodeId(tritId);
    if(nid < 0) return false;
    int direction = fabs(trit->trSignalType2 - 1);
    int endPointType = tdb->getEndpointType(nid, direction);
       
    if(endPointType == 2) return true;
    return false;
}

bool SignalObj::isSubObjEnabled(int i){
    return ((this->signalSubObj >> i) & 1) == 1;
}

void SignalObj::enableSubObj(int i){
    if(isSubObjEnabled(i)) return;
    
    this->signalSubObj = this->signalSubObj | (1 << i);
    TDB* tdb = Game::trackDB;
    for(int j = 0; j < this->signalUnits; j++){
        if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
            continue;
        tdb->trackItems[this->trItemId[j*2+1]]->enableSignalSubObj(i);
    }
    this->modified = true;
    return;
}

void SignalObj::disableSubObj(int i){
    if(!isSubObjEnabled(i)) return;
    
    SignalShape* signalShape = Game::trackDB->sigCfg->signalShape[fileName.toStdString()];
    if(signalShape == NULL)
        return;
    
    if(!signalShape->subObj[i].optional)
        return;

    TDB* tdb = Game::trackDB;
    for(int j = 0; j < this->signalUnits; j++){
        if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
            continue;
        tdb->trackItems[this->trItemId[j*2+1]]->disableSignalSubObj(i);
    }
    this->signalSubObj = this->signalSubObj ^ (1 << i);
    this->modified = true; 
}

void SignalObj::flip(bool flipShape){
    if(flipShape){
        Quat::rotateY(this->qDirection, this->qDirection, M_PI);
        this->setMartix();
    }

    TDB* tdb = Game::trackDB;
    for(int j = 0; j < this->signalUnits; j++){
        if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
            continue;
        tdb->trackItems[this->trItemId[j*2+1]]->flipSignal();
    }
    this->modified = true; 
    delete[] drawPositions;
    drawPositions = NULL;
}

void SignalObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
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

void SignalObj::renderTritems(GLUU* gluu, int selectionColor){
    
    ///////////////////////////////
    if (drawPositions == NULL) {
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj();
            pointer3d->setMaterial(1,0,0);
        }
        if(pointer3dSelected == NULL){
            pointer3dSelected = new TrackItemObj();
            pointer3dSelected->setMaterial(1,0.5,0.5);
        }
        drawPositions = new float*[this->signalUnits];
        TDB* tdb = Game::trackDB;
        for(int i = 0; i < this->signalUnits; i++){
            int id = tdb->findTrItemNodeId(this->trItemId[i*2+1]);
            if (id < 0) {
                qDebug() << "fail id";
                return;
            }
            //qDebug() << "id: "<< this->trItemId[i*2+1] << " "<< id;
            drawPositions[i] = new float[8];
            bool ok = tdb->getDrawPositionOnTrNode(drawPositions[i], id, tdb->trackItems[this->trItemId[i*2+1]]->trItemSData1);
            if(!ok){
                this->loaded = false;
                return;
            }
            drawPositions[i][7] = tdb->trackItems[this->trItemId[i*2+1]]->trSignalType2;
            drawPositions[i][0] += 2048 * (drawPositions[i][5] - this->x);
            drawPositions[i][2] -= 2048 * (-drawPositions[i][6] - this->y);
        }
    }

    //if(pos == NULL) return;
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();
    int useSC;
    int w = 1;
    //w = this->signalUnits;
    for(int i = 0; i < w; i++){
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositions[i][0] + 0 * (drawPositions[i][4] - this->x), drawPositions[i][1] + i + 1, -drawPositions[i][2] + 0 * (-drawPositions[i][5] - this->y));
        Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositions[i][3] + drawPositions[i][7]*M_PI);
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        useSC = (float)selectionColor/(float)(selectionColor+0.000001);
        if(this->selected && this->selectionValue > 0) 
            pointer3dSelected->render(selectionColor + (i+1)*65536*25*useSC);
        else
            pointer3d->render(selectionColor + (i+1)*65536*25*useSC);
        gluu->mvPopMatrix();
    }
};

bool SignalObj::select(int value){
    this->selectionValue = value;
    this->selected = true;
}

bool SignalObj::getBorder(float* border){
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

void SignalObj::save(QTextStream* out){
    if (!loaded) return;
int l;
QString flags;
    flags = QString::number(this->signalSubObj, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++)
        flags = "0"+flags;
    
*(out) << "	Signal (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "		SignalSubObj ( "<<flags<<" )\n";
if(signalUnits > 0){
*(out) << "		SignalUnits ( "<<this->signalUnits<<"\n";
for(int i=0; i<signalUnits; i++){
*(out) << "			SignalUnit ( "<<i<<"\n";
*(out) << "				TrItemId ( "<<this->trItemId[i*2+0]<<" "<<this->trItemId[i*2+1]<<" )\n";
*(out) << "			)\n";
}
*(out) << "		)\n";
}
*(out) << "	)\n";
}