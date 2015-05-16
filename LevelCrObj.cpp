#include "LevelCrObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>

LevelCrObj::LevelCrObj() {
    this->shape = -1;
    this->loaded = false;
}

LevelCrObj::LevelCrObj(const LevelCrObj& orig) {
}

LevelCrObj::~LevelCrObj() {
}

void LevelCrObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->skipLevel = 1;
    this->modified = false;
    
    setMartix();
}

void LevelCrObj::set(QString sh, FileBuffer* data) {
    if (sh == ("levelcrparameters")) {
        levelCrParameters[0] = ParserX::parsujr(data);
        levelCrParameters[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("Crashprobability")) {
        crashProbability = ParserX::parsujr(data);
        return;
    }
    if (sh == ("levelcrdata")) {
        levelCrData[0] = ParserX::parsuj16(data);
        levelCrData[1] = ParserX::parsujr(data);
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

};

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
*(out) << "		LevelCrData ( "<<flags2<<" "<<this->levelCrData[2]<<" )\n";
*(out) << "		LevelCrTiming ( "<<this->levelCrTiming[0]<<" "<<this->levelCrTiming[1]<<" "<<this->levelCrTiming[2]<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[0]<<" "<<this->trItemId[1]<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[2]<<" "<<this->trItemId[3]<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}