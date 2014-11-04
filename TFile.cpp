#include "TFile.h"
#include <QDebug>
#include "SFileC.h"
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
        SFileC::szukajsekcjic(136, data);
        //qDebug() << "znaleziono sekcje 136 na " << data->off << " ";
        data->off += 5;
        for (int i = 0; i < 6; i++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);

            switch (pozycja) {
                case 137:
                    break;
                case 138:
                    break;
                case 139:
                    get139(data);
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
                    i--;
                    break;
            }
            if(pozycja==157) break;
            data->off = akto + offset;
        }
        delete data;
        return true;
    }

void TFile::get139(FileBuffer* data) {
        int pozycja, offset, akto;
        data->off++;
        for (int j = 0; j < 2;) {

            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("=znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);

            switch (pozycja) {
                case 142:
                    data->off++;
                    floor = data->getFloat();
                    //qDebug() << floor;
                    j++;
                    break;
                case 143:
                    data->off++;
                    scale = data->getFloat();
                    //qDebug() << scale;
                    j++;
                    break;
                default:
                    break;
            }
            data->off = akto + offset;
        }
    }

void TFile::get151(FileBuffer* data) {
        data->off++;
        int pozycja, offset, akto;
        int ttilosc = data->getInt();
        materials = new mat[ttilosc];
        //System.out.println("ilosc " + ttilosc);
        for (int j = 0; j < ttilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("=znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);

            int tttpozycja, tttoffset, tttakto;
            data->off += 3;

            QString tname = "";
            char bbb;
            while ((bbb = data->get()) != 0) {
                tname += bbb;
                data->off++;
            }
            tname = tname.trimmed();

            //System.out.println(tname);
            data->off -= 3;
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
        data->off++;
        int ilosc = data->getInt();
        //System.out.println("i to " + ilosc);
        for (int j = 0; j < ilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("===znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);     
            data->off += 3;

            m->tex[j] = "";
            char bbb;
            while ((bbb = data->get()) > 31) {
                m->tex[j] += bbb;
                data->off++;
            }
            m->tex[j] = m->tex[j].trimmed();
            
            //qDebug() << j << " "<< m->tex[j];
            data->off -= 3;
            m->atex[0] = data->getInt();
            m->atex[1] = data->getInt();
            
            data->off = akto + offset;
        }
    }
    
void TFile::get156(FileBuffer* data, TFile::mat* m) {
        int pozycja, offset, akto;
        data->off++;
        int ilosc = data->getInt();
        //System.out.println("i to " + ilosc);
        for (int j = 0; j < ilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("===znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);     
            data->off++;
            
            m->itex[j][0] = data->getInt()&0xff;
            m->itex[j][1] = data->getInt()&0xff;
            m->itex[j][2] = data->getInt()&0xff;
            m->itex[j][3] = data->getInt()&0xff;

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
            int patch = 0;
            for (int jj = 0; jj < 3;) {
                ttpozycja = data->getInt();
                ttoffset = data->getInt();
                ttakto = data->off;
                //System.out.println("===znaleziono sekcje " + ttpozycja + " na " + data.position() + " " + ttoffset);

                switch (ttpozycja) {
                    case 160:
                        data->off++;
                        //System.out.println("= " + data.getInt());
                        jj++;
                        break;
                    case 161:
                        data->off++;
                        patch = data->getInt();
                        jj++;
                        break;
                    case 163:
                        get163(data, patch);
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
        tdata = new float[n*n*7];
        flags = new int[n*n];
        for (int j = 0; j < n*n; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //System.out.println("===znaleziono sekcje " + pozycja + " na " + data.position() + " " + offset);     
            data->off++;
            
            flags[j] = data->getInt();//&0xFFFF;
            data->off += 4*6;
            //System.out.println((data.getInt()&0xff));
            //System.out.println((data.getInt()&0xff));
            //System.out.println((data.getInt()&0xff));
            //System.out.println((data.getInt()&0xff));
            tdata[j*7+0] = (float)data->getInt();
            tdata[j*7+1] = data->getFloat();
            tdata[j*7+2] = data->getFloat();
            tdata[j*7+3] = data->getFloat();
            tdata[j*7+4] = data->getFloat();
            tdata[j*7+5] = data->getFloat();
            tdata[j*7+6] = data->getFloat();
            //qDebug() << tdata[j*7+1] << tdata[j*7+2] << tdata[j*7+3] << tdata[j*7+4] << tdata[j*7+5] << tdata[j*7+6];
            
            data->off = akto + offset;
        }
    }

void TFile::get251(FileBuffer* data) {
        data->off++;

        WSW = data->getFloat();
        WSE = data->getFloat();
        WNE = data->getFloat();
        WNW = data->getFloat();
        //tdata = new float[n*n][7];
        //flags = new int[n*n];
    }
