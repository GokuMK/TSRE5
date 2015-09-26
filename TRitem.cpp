/* 
 * Author: Goku
 */

#include "TRitem.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include "GLMatrix.h"
#include "TDB.h"
#include "Vector3f.h"
#include <QString>
#include <QDebug>

TRitem::TRitem() {
}

TRitem::TRitem(const TRitem& orig) {
}

TRitem::~TRitem() {
}

bool TRitem::init(QString sh){
    type = sh;
    trItemPData = NULL;
    trItemRData = NULL;
    crossoverTrItemData = NULL;
    platformTrItemData = NULL;
    trItemSRData = NULL;
    speedpostTrItemData = NULL;
    trSignalDir = NULL;
    
    pointer3d = new Pointer3d();
    
    if(sh == "crossoveritem") return true;
    if(sh == "signalitem") return true;
    if(sh == "soundregionitem") return true;
    if(sh == "levelcritem") return true;
    if(sh == "speedpostitem") return true;
    if(sh == "platformitem") return true;
    if(sh == "sidingitem") return true;
    if(sh == "carspawneritem") return true;    
    
    return false;
}

void TRitem::set(QString sh, FileBuffer* data) {
    if (sh == ("tritemid")) {
        trItemId = ParserX::parsujUint(data);
        return;
    }
    if (sh == ("tritemsdata")) {
        trItemSData1 = ParserX::parsujr(data);
        trItemSData2 = ParserX::parsuj16(data);
        return;
    }
    if (sh == ("tritempdata")) {
        trItemPData = new float[4];
        trItemPData[0] = ParserX::parsujr(data);
        trItemPData[1] = ParserX::parsujr(data);
        trItemPData[2] = ParserX::parsujr(data);
        trItemPData[3] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemrdata")) {
        trItemRData = new float[5];
        trItemRData[0] = ParserX::parsujr(data);
        trItemRData[1] = ParserX::parsujr(data);
        trItemRData[2] = ParserX::parsujr(data);
        trItemRData[3] = ParserX::parsujr(data);
        trItemRData[4] = ParserX::parsujr(data);
        return;
    }
    
    // crossover
    if (sh == ("crossovertritemdata")) {
        crossoverTrItemData = new int[2];
        crossoverTrItemData[0] = (int) ParserX::parsujr(data);
        crossoverTrItemData[1] = (int) ParserX::parsujr(data);
        return;
    }
    
    // platform / siding
    if (sh == ("platformtritemdata") || sh == ("sidingtritemdata")) {
        platformTrItemData = new unsigned int[2];
        platformTrItemData[0] = ParserX::parsuj16(data);
        platformTrItemData[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("platformname") || sh == ("sidingname")) {
        platformName = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("station")) {
        station = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("platformminwaitingtime")) {
        platformMinWaitingTime = ParserX::parsujr(data);
        return;
    }
    if (sh == ("platformnumpassengerswaiting")) {
        platformNumPassengersWaiting = ParserX::parsujr(data);
        return;
    }
    
    //soundregion
    if (sh == ("tritemsrdata")) {
        trItemSRData = new float[3];
        trItemSRData[0] = ParserX::parsujr(data);
        trItemSRData[1] = ParserX::parsujr(data);
        trItemSRData[2] = ParserX::parsujr(data);
        return;
    }
    
    //speed
    if (sh == ("speedposttritemdata")){
        speedpostTrItemData = new float[3];
        speedpostTrItemData[0] = ParserX::parsujr(data);
        speedpostTrItemData[1] = ParserX::parsujr(data);
        speedpostTrItemData[2] = ParserX::parsujr(data);
        return;
    }
    
    // signal
    
    if (sh == ("trsignaltype")) {
        trSignalType1 = ParserX::parsuj16(data);
        trSignalType2 = ParserX::parsujr(data);
        trSignalType3 = ParserX::parsujr(data);
        trSignalType4 = ParserX::odczytajtc(data);
        return;
    }
    
    if (sh == ("trsignaldirs")) {
        trSignalDirs = ParserX::parsujr(data);
        trSignalDir = new int[trSignalDirs*4];
        for(int i = 0; i < trSignalDirs; i++){
            trSignalDir[i] = ParserX::parsujr(data);
        }
        ParserX::pominsekcje(data);
        return;
    }
    
    qDebug() << "=" << sh;
    return;
}

void TRitem::render(TDB *tdb, GLUU *gluu, float* playerT, float playerRot){
    if(type == "platformitem" || type == "sidingitem"){
        gluu->mvPushMatrix();
        
        //Vector3f *pos = tdb->getDrawPositionOnTrNode(playerT, id, this->trItemSData1);
        if(drawPosition == NULL){
            int id = tdb->findTrItemNodeId(this->trItemId);
            if(id < 0) {
                qDebug() << "fail id";
                return;
            }
            drawPosition = new float[6];
            tdb->getDrawPositionOnTrNode(drawPosition, id, this->trItemSData1);
        }
        
        //if(pos == NULL) return;
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 2048*(drawPosition[4] - playerT[0]), drawPosition[1]+1, -drawPosition[2] + 2048*(-drawPosition[5] - playerT[1]));
        Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3]);
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
        Mat4::identity(gluu->objStrMatrix);
        gluu->setMatrixUniforms();
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        pointer3d->render();
        gluu->mvPopMatrix();
    }
}

void TRitem::save(QTextStream* out){

int l;
QString tritemsdata = QString::number(this->trItemSData2, 16);
l = tritemsdata.length();
for(int i=0; i<8-l; i++) tritemsdata = "0"+tritemsdata;

QString flags;
if(type == "platformitem" || type == "sidingitem"){
    flags = QString::number(this->platformTrItemData[0], 16);
    l = flags.length();
    for(int i=0; i<8-l; i++) flags = "0"+flags;
}
if(type == "signalitem"){
    flags = QString::number(this->trSignalType1, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++) flags = "0"+flags;
}

if(type == "crossoveritem")
*(out) << "		CrossoverItem (\n";
if(type == "signalitem")
*(out) << "		SignalItem (\n";
if(type == "soundregionitem")
*(out) << "		SoundRegionItem (\n";
if(type == "levelcritem")
*(out) << "		LevelCrItem (\n";
if(type == "speedpostitem")
*(out) << "		SpeedPostItem (\n";
if(type == "platformitem")
*(out) << "		PlatformItem (\n";
if(type == "sidingitem")
*(out) << "		SidingItem (\n";
if(type == "carspawneritem")
*(out) << "		CarSpawnerItem (\n";

*(out) << "			TrItemId ( "<<this->trItemId<<" )\n";
*(out) << "			TrItemSData ( "<<this->trItemSData1<<" "<< tritemsdata <<" )\n";
if(this->trItemPData != NULL)
*(out) << "			TrItemPData ( "<<this->trItemPData[0]<<" "<< this->trItemPData[1]<<" "<< this->trItemPData[2]<<" "<< this->trItemPData[3]<<" )\n";
if(this->trItemRData != NULL)
*(out) << "			TrItemRData ( "<<this->trItemRData[0]<<" "<< this->trItemRData[1]<<" "<< this->trItemRData[2]<<" "<< this->trItemRData[3]<<" "<< this->trItemRData[4] <<" )\n";

if(type == "crossoveritem" && this->crossoverTrItemData != NULL)
*(out) << "			CrossoverTrItemData ( "<<this->crossoverTrItemData[0]<<" "<< this->crossoverTrItemData[1] <<" )\n";

if(type == "soundregionitem" && this->trItemSRData != NULL)
*(out) << "			TrItemSRData ( "<<this->trItemSRData[0]<<" "<< this->trItemSRData[1] <<" "<< this->trItemSRData[2] <<" )\n";

if(type == "speedpostitem" && this->speedpostTrItemData != NULL)
*(out) << "			SpeedpostTrItemData ( "<<this->speedpostTrItemData[0]<<" "<< this->speedpostTrItemData[1] <<" "<< this->speedpostTrItemData[2] <<" )\n";

if(type == "sidingitem"){
if(this->platformTrItemData != NULL)
*(out) << "			SidingTrItemData ( "<<flags<<" "<< this->platformTrItemData[1]<<" )\n";
*(out) << "			SidingName ( \""<<this->platformName<<"\" )\n";
}

if(type == "platformitem"){
if(this->platformTrItemData != NULL)
*(out) << "			PlatformTrItemData ( "<<flags<<" "<< this->platformTrItemData[1]<<" )\n";
*(out) << "			PlatformName ( \""<<this->platformName<<"\" )\n";
*(out) << "			Station ( \""<<this->station<<"\" )\n";
*(out) << "			PlatformMinWaitingTime ( "<<this->platformMinWaitingTime<<" )\n";
*(out) << "			PlatformNumPassengersWaiting ( "<<this->platformNumPassengersWaiting<<" )\n";
}

if(type == "signalitem"){
*(out) << "			TrSignalType ( "<<flags<<" "<<this->trSignalType2<<" "<<this->trSignalType3<<" "<<this->trSignalType4<<" )\n";
if(this->trSignalDir != NULL){
    *(out) << "			TrSignalDirs ( "<<this->trSignalDirs<<"\n";
    for(int i = 0; i < this->trSignalDirs; i++)
        *(out) << "				TrSignalDir ( "<<this->trSignalDir[i+0]<<" "<< this->trSignalDir[i+1]<<" "<< this->trSignalDir[i+2]<<" "<< this->trSignalDir[i+3]<<" )\n";
    *(out) << "			)\n";
}
}

*(out) << "		)\n";
}