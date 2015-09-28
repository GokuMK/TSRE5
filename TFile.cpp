#include "TFile.h"
#include <QDebug>
#include <QFile>
#include "ReadFile.h"

TFile::TFile() {
    loaded = false;
    used = false;
}

TFile::TFile(const TFile& orig) {
}

TFile::~TFile() {
}

bool TFile::readT(QString fSfile) {
        fSfile.replace("//","/");
        //qDebug() << fSfile;
        QFile *file = new QFile(fSfile);
        if (!file->open(QIODevice::ReadOnly))
            return false;
        FileBuffer* data = ReadFile::read(file);
        //qDebug() << "Date:" << data->length;
        data->off = 16;
        
        int pozycja, offset, akto;
        data->findToken(136);
        //qDebug() << "znaleziono sekcje 136 na " << data->off << " ";
        data->off += 5;
        for (int i = 0; i < 6; i++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //qDebug() << "znaleziono sekcje " << pozycja <<" na " << data->off << " " << offset;

            switch (pozycja) {
                case 137:
                    data->off++;
                    errthresholdScale = new float;
                    *errthresholdScale = data->getFloat();
                    break;
                case 138:
                    data->off++;
                    alwaysselectMaxdist = new float;
                    *alwaysselectMaxdist = data->getFloat();
                    break;
                case 139:
                    get139(data, akto + offset);
                    break;
                case 251:
                    get251(data);
                    break;
                case 151:
                    get151(data);
                    break;
                case 157:
                    get157(data);
                    break;
                default:
                    qDebug() << "unknown section";
                    i--;
                    break;
            }
            //if(pozycja==157) qDebug() << " ok";
            data->off = akto + offset;
            if(data->off >= data->length) break;
        }
        delete data;

        return true;
    }

void TFile::get139(FileBuffer* data, int length) {
        int pozycja, offset, akto;
        int slen;
        data->off++;
        for (int j = 0; j < 2;) {

            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //qDebug() << "139 znaleziono sekcje " << pozycja <<" na " << data->off << " " << offset;

            switch (pozycja) {
                case 140:
                    data->off++;
                    nsamples = new int;
                    *nsamples = data->getInt();
                    break;
                case 141:
                    data->off++;
                    sampleRotation = new float;
                    *sampleRotation = data->getFloat();
                    break;    
                case 142:
                    data->off++;
                    floor = data->getFloat();
                    break;
                case 143:
                    data->off++;
                    scale = data->getFloat();
                    break;
                case 144:
                    data->off++;
                    sampleSize = new float;
                    *sampleSize = data->getFloat();
                    break;
                case 145:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleFbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 146:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleYbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 147:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleEbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 148:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleNbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 281:
                    data->off++;
                    sampleASbuffer = new char[257*257/8+1];
                    for(int i = 0; i <= 257*257/8; i++)
                        sampleASbuffer[i] = data->get();
                    break;
                default:
                    qDebug() << "unknown section";
                    break;
            }
            data->off = akto + offset;
            if(data->off >= length) break;
        }
    }

void TFile::get151(FileBuffer* data) {
        data->off++;
        int pozycja, offset, akto;
        int slen;
        
        int ttilosc = data->getInt();
        //materials = new mat[ttilosc];
        materialsCount = ttilosc;
        //System.out.println("ilosc " + ttilosc);
        for (int j = 0; j < ttilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("=znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);

            int tttpozycja, tttoffset, tttakto;
            data->off++;

            slen = data->getShort()*2;
            materials[j].name = data->getString(data->off, data->off + slen);
            //*tname = tname->trimmed();
            //qDebug() << *tname;
            data->off += slen;
            
            for (int jj = 0; jj < 2; ) {

                tttpozycja = data->getInt();
                tttoffset = data->getInt();
                tttakto = data->off;
                //System.out.println("==znaleziono sekcje " + tttpozycja + " na " + data.position() + " " + tttoffset);

                switch (tttpozycja) {
                    case 153:
                        get153(data, &materials[j]);
                        jj++;
                        break;
                    case 155:
                        get156(data, &materials[j]);
                        jj++;
                        break;
                    default:
                        break;
                }
                data->off = tttakto + tttoffset;
            }

           data->off = akto + offset;
        }
    }

void TFile::get153(FileBuffer* data, TFile::mat* m) {
        int pozycja, offset, akto;
        int slen;
        
        data->off++;
        int ilosc = data->getInt();
        m->count153 = ilosc;
        //System.out.println("i to " + ilosc);
        for (int j = 0; j < ilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("===znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);     
            data->off ++;

            //m->tex[j] = "";
            //char bbb;
            //while ((bbb = data->get()) > 31) {
            //    m->tex[j] += bbb;
            //    data->off++;
            //}
            //m->tex[j] = m->tex[j].trimmed();

            slen = data->getShort()*2;
            m->tex[j] = data->getString(data->off, data->off + slen);
            //*tname = tname->trimmed();
            //qDebug() << *tname;
            data->off += slen;
            
            
            //qDebug() << j << " "<< m->tex[j];
            //data->off -= 3;
            m->atex[j][0] = data->getInt();
            m->atex[j][1] = data->getInt();
            //qDebug() << m->atex[j][0] << " " << m->atex[j][1];
            data->off = akto + offset;
        }
    }
    
void TFile::get156(FileBuffer* data, TFile::mat* m) {
        int pozycja, offset, akto;
        data->off++;
        int ilosc = data->getInt();
        m->count155 = ilosc;
        //System.out.println("i to " + ilosc);
        for (int j = 0; j < ilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("===znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);     
            data->off++;
            
            m->itex[j][0] = data->getInt();//&0xff;
            m->itex[j][1] = data->getInt();//&0xff;
            m->itex[j][2] = data->getInt();//&0xff;
            m->itex[j][3] = data->getInt();//&0xff;
            //qDebug() << m->itex[j][0] << " " << m->itex[j][1] << " " << m->itex[j][2] << " " << m->itex[j][3];

            data->off = akto + offset;
        }
    }
    
void TFile::get157(FileBuffer* data) {
        data->off++;
        int pozycja, offset, akto;
        
        pozycja = data->getInt();
        offset = data->getInt();
        akto = data->off;
        //System.out.println("=znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);
        data->off++;
        int ttilosc = data->getInt();
        //qDebug() << "ttilosc " << ttilosc;
        //System.out.println("ilosc " + ttilosc);
        int tttpozycja, tttoffset, tttakto;
        //
        
        for (int j = 0; j < ttilosc; j++) {
            tttpozycja = data->getInt();
            tttoffset = data->getInt();
            tttakto = data->off;
            //System.out.println("==znaleziono sekcje " + tttpozycja + " na " + data.position() + " " + tttoffset);
            data->off++;

            int ttpozycja, ttoffset, ttakto;

            for (int jj = 0; jj < 3;) {
                ttpozycja = data->getInt();
                ttoffset = data->getInt();
                ttakto = data->off;
                //System.out.println("===znaleziono sekcje " + ttpozycja + " na " + data.position() + " " + ttoffset);

                switch (ttpozycja) {
                    case 160:
                        data->off++;
                        patchsetDistance = data->getInt();
                        //System.out.println("= " + data.getInt());
                        jj++;
                        break;
                    case 161:
                        data->off++;
                        patchsetNpatches = data->getInt();
                        jj++;
                        break;
                    case 163:
                        get163(data, patchsetNpatches);
                        jj++;
                        break;                        
                    default:
                        break;
                }
                data->off = ttakto + ttoffset;
            }
            data->off = tttakto + tttoffset;
        }
    }
    
void TFile::get163(FileBuffer* data, int n) {
        int pozycja, offset, akto;
        data->off++;
        //int ilosc = data.getInt();
        //qDebug() << "i to " << n;
        tdata = new float[n*n*13];
        erroeBias = new float[n*n];
        flags = new int[n*n];
        for (int j = 0; j < n*n; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("===znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);     
            data->off++;
            
            flags[j] = data->getInt();//&0xFFFF;
            //data->off += 4*6;
            tdata[j*13+0] = data->getFloat();
            tdata[j*13+1] = data->getFloat();
            tdata[j*13+2] = data->getFloat();
            tdata[j*13+3] = data->getFloat();
            tdata[j*13+4] = data->getFloat();
            tdata[j*13+5] = data->getFloat();
            tdata[j*13+6] = (float)data->getInt();
            tdata[j*13+7] = data->getFloat();
            tdata[j*13+8] = data->getFloat();
            tdata[j*13+9] = data->getFloat();
            tdata[j*13+10] = data->getFloat();
            tdata[j*13+11] = data->getFloat();
            tdata[j*13+12] = data->getFloat();
            erroeBias[j] = data->getFloat();
            //qDebug() << tdata[j*7+1] << tdata[j*7+2] << tdata[j*7+3] << tdata[j*7+4] << tdata[j*7+5] << tdata[j*7+6];
            
            //data->off = akto + offset;
        }
    }

void TFile::get251(FileBuffer* data) {
        data->off++;
        
        waterLevel = true;
        WSW = data->getFloat();
        WSE = data->getFloat();
        WNE = data->getFloat();
        WNW = data->getFloat();
        //tdata = new float[n*n][7];
        //flags = new int[n*n];
    }

int TFile::cloneMat(int id){
    QString* name = new QString();
    *name += *materials[id].name;
    materials[materialsCount].name = name;
    
    materials[materialsCount].count153 = materials[id].count153;
    for(int i = 0; i < materials[id].count153; i++){
        name = new QString();
        *name += *materials[id].tex[i];
        materials[materialsCount].tex[i] = name;
        materials[materialsCount].atex[i][0] = materials[id].atex[i][0];
        materials[materialsCount].atex[i][1] = materials[id].atex[i][1];
    }
    
    materials[materialsCount].count155 = materials[id].count155;
    for(int i = 0; i < materials[id].count155; i++){
        materials[materialsCount].itex[i][0] = materials[id].itex[i][0];
        materials[materialsCount].itex[i][1] = materials[id].itex[i][1];
        materials[materialsCount].itex[i][2] = materials[id].itex[i][2];
        materials[materialsCount].itex[i][3] = materials[id].itex[i][3];
    }
        
    return this->materialsCount++;
}

void TFile::save(QString name){
    name.replace("//", "/");
    QFile *file = new QFile(name);
    qDebug() << "zapis .t "<<name;
    if (!file->open(QIODevice::WriteOnly))
        return;
    QDataStream write(file);
    write.setByteOrder(QDataStream::LittleEndian);
    write.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //calculate size
    
    int t137 = 0;
    if(errthresholdScale != NULL)
        t137 = 13;
   
    int t251 = 0;
    if(waterLevel)
        t251 = 25;
    
    int t138 = 0;
    if(alwaysselectMaxdist != NULL)
        t138 = 13;

    // 139
    int t139 = 1;
    // 140
    if(nsamples != NULL)
        t139+=13;
    // 141 
    if(sampleRotation != NULL)
        t139+=13;
    // 142
    t139+=13;
    // 143 
    t139+=13;
    // 144 
    if(sampleSize != NULL)
        t139+=13;
    // 281
    if(sampleASbuffer != NULL)
        t139+=257*257/8+2+8;
    // 145
    if(sampleFbuffer != NULL)
        t139+=sampleFbuffer->length()*2+3+8;
    // 146
    if(sampleYbuffer != NULL)
        t139+=sampleYbuffer->length()*2+3+8;
    // 147
    if(sampleEbuffer != NULL)
        t139+=sampleEbuffer->length()*2+3+8;
    // 148
    if(sampleNbuffer != NULL)
        t139+=sampleNbuffer->length()*2+3+8;
    
    // 151 
    int t151 = 0;
    t151+=5;
    int* t152 = new int[materialsCount];
    int* t153 = new int[materialsCount];
    for(int j = 0; j < materialsCount; j++){
        t152[j] = 1;
        t152[j] += materials[j].name->length()*2+2;
        t152[j] += 13;
        t153[j] = 5;
        for(int i = 0; i < materials[j].count153; i++){
            t152[j] += 11;
            t152[j] += materials[j].tex[i]->length()*2;
            t152[j] += 8;
            t153[j] += 11;
            t153[j] += materials[j].tex[i]->length()*2;
            t153[j] += 8;
        }
        t152[j] += 13;
        t152[j] += 25*materials[j].count155;
        t151 += t152[j] + 8;
    }

    // 157
    int t157 = 0;
    t157 = 69*patchsetNpatches*patchsetNpatches+9+24+3+8+4+1+9;
    
    int t136 = 1 + t137 + t251 + t138 + t139 + 8 + t151 + 8 + t157 + 8;
    
    //write
    const char header[] = {
        0x53,0x49,0x4D,0x49,0x53,0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
        0x4A,0x49,0x4E,0x58,0x30,0x74,0x36,0x62,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x0D,0x0A
    };
    write.writeRawData(header, 32);
    write << (qint32)136;
    write << (qint32)t136;
    write << (qint8)0;
    
    if(errthresholdScale != NULL){
        write << (qint32)137;
        write << (qint32)5;
        write << (qint8)0;
        write << *errthresholdScale;
    }
    
    if(waterLevel){
        write << (qint32)251;
        write << (qint32)17;
        write << (qint8)0;
        write << WSW;
        write << WSE;
        write << WNE;
        write << WNW;
    }
    
    if(alwaysselectMaxdist != NULL){
        write << (qint32)138;
        write << (qint32)5;
        write << (qint8)0;
        write << *alwaysselectMaxdist;
    }
    
    write << (qint32)139;
    write << (qint32)t139;
    write << (qint8)0;
    // 140
    if(nsamples != NULL){
        write << (qint32)140;
        write << (qint32)5;
        write << (qint8)0;
        write << *nsamples;
    }
    // 141 
    if(sampleRotation != NULL){
        write << (qint32)141;
        write << (qint32)5;
        write << (qint8)0;
        write << *sampleRotation;
    }
    // 142
    write << (qint32)142;
    write << (qint32)5;
    write << (qint8)0;
    write << floor;
    // 143 
    write << (qint32)143;
    write << (qint32)5;
    write << (qint8)0;
    write << scale;
    // 144 
    if(sampleSize != NULL){
        write << (qint32)144;
        write << (qint32)5;
        write << (qint8)0;
        write << *sampleSize;
    }
    // 281
    if(sampleASbuffer != NULL){
        write << (qint32)281;
        write << (qint32)257*257/8+2;
        write << (qint8)0;
        write.writeRawData(sampleASbuffer, 257*257/8+1);
    }
    // 145
    if(sampleFbuffer != NULL){
        write << (qint32)145;
        write << (qint32)sampleFbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleFbuffer->length();
        for(int i = 0; i < sampleFbuffer->length(); i++){
            write << sampleFbuffer->at(i).unicode();
        }
    }
    // 146
    if(sampleYbuffer != NULL){
        write << (qint32)146;
        write << (qint32)sampleYbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleYbuffer->length();
        for(int i = 0; i < sampleYbuffer->length(); i++){
            write << sampleYbuffer->at(i).unicode();
        }
    }
    // 147
    if(sampleEbuffer != NULL){
        write << (qint32)147;
        write << (qint32)sampleEbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleEbuffer->length();
        for(int i = 0; i < sampleEbuffer->length(); i++){
            write << sampleEbuffer->at(i).unicode();
        }
    }
    // 148
    if(sampleNbuffer != NULL){
        write << (qint32)148;
        write << (qint32)sampleNbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleNbuffer->length();
        for(int i = 0; i < sampleNbuffer->length(); i++){
            write << sampleNbuffer->at(i).unicode();
        }
    }

    // 151 
    write << (qint32)151;
    write << (qint32)t151;
    write << (qint8)0;
    write << (qint32)materialsCount;
    
    for(int j = 0; j < materialsCount; j++){
        write << (qint32)152;
        write << (qint32)t152[j];
        write << (qint8)0;
        write << (qint16)materials[j].name->length();
        for(int i = 0; i < materials[j].name->length(); i++){
            write << materials[j].name->at(i).unicode();
        }
        write << (qint32)153;
        write << (qint32)t153[j];
        write << (qint8)0;
        write << (qint32)materials[j].count153;
        for(int i = 0; i < materials[j].count153; i++){
            write << (qint32)154;
            write << (qint32)(materials[j].tex[i]->length()*2+3+8);
            write << (qint8)0;
            write << (qint16)materials[j].tex[i]->length();
            for(int ii = 0; ii < materials[j].tex[i]->length(); ii++){
                write << materials[j].tex[i]->at(ii).unicode();
            }
            write << (qint32)materials[j].atex[i][0];
            write << (qint32)materials[j].atex[i][1];
        }
        write << (qint32)155;
        write << (qint32)(25*materials[j].count155+5);
        write << (qint8)0;
        write << (qint32)materials[j].count155;
        for(int i = 0; i < materials[j].count155; i++){
            write << (qint32)156;
            write << (qint32)17;
            write << (qint8)0;
            write << (qint32)materials[j].itex[i][0];
            write << (qint32)materials[j].itex[i][1];
            write << (qint32)materials[j].itex[i][2];
            write << (qint32)materials[j].itex[i][3];
        }
    }
    
    // 157
    write << (qint32)157;
    write << (qint32)t157;
    write << (qint8)0;
    
    write << (qint32)158;
    write << (qint32)69*patchsetNpatches*patchsetNpatches+9+24+3+8+4+1;
    write << (qint8)0;
    write << (qint32)1;
    
    write << (qint32)159;
    write << (qint32)69*patchsetNpatches*patchsetNpatches+9+24+3;
    write << (qint8)0;

    write << (qint32)160;
    write << (qint32)5;
    write << (qint8)0;
    write << (qint32)patchsetDistance;
    
    write << (qint32)161;
    write << (qint32)5;
    write << (qint8)0;
    write << (qint32)patchsetNpatches;
    
    write << (qint32)163;
    write << (qint32)69*patchsetNpatches*patchsetNpatches+1;
    write << (qint8)0;

    for(int j = 0; j < patchsetNpatches*patchsetNpatches; j++){
        write << (qint32)164;
        write << (qint32)61;
        write << (qint8)0;
        write << (qint32)flags[j];
        write << (float)tdata[j*13+0];
        write << (float)tdata[j*13+1];
        write << (float)tdata[j*13+2];
        write << (float)tdata[j*13+3];
        write << (float)tdata[j*13+4];
        write << (float)tdata[j*13+5];
        write << (qint32)tdata[j*13+6];
        write << (float)tdata[j*13+7];
        write << (float)tdata[j*13+8];
        write << (float)tdata[j*13+9];
        write << (float)tdata[j*13+10];
        write << (float)tdata[j*13+11];
        write << (float)tdata[j*13+12];
        write << (float)erroeBias[j];
    }
    
    write.unsetDevice();
    file->close();
}