#include "ReadFile.h"
#include "zlib.h"

//unsigned char* ReadFile::out = new unsigned char[10000000];

FileBuffer* ReadFile::read(QFile* file) {
    
    QByteArray fileData = file->readAll();
    unsigned char* in = (unsigned char*)fileData.data();
    unsigned char* out;
    unsigned char *data;
    int nLength;
    //for (int i = 0; i < 100; i++)
    //   qDebug() << ":" << (char)in[i];

    if (in[7] == 'F') {
        out = new unsigned char[15000000];
        //System.out.println("plik skompresowany");
        int aLength =  fileData.length() - 16;
        int ret;
        z_stream strm;
        //unsigned char* in = (unsigned char*)buffer.data() + 7;
        /* allocate inflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = aLength;
        strm.next_in = in + 16;
        ret = inflateInit(&strm);
        if (ret != Z_OK) {
            qDebug() << "fail2";
        } else {
            //qDebug() << "inflate";
            strm.avail_out = 10000000;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            //cout << 1000000 - strm.avail_out << endl;

            (void) inflateEnd(&strm);

            nLength = 10000000 - strm.avail_out;
            data = new unsigned char[nLength];
            //for(int i = 0; i < chunkData->length; i++)
            //    chunkData->data[i] = Global::out[i];
            std::copy(out, out + nLength, data);
            //for (int i = 0; i < 128; i++)
            //    std::cout << ":" << (int)data[i];
        }
    } else {
        data = new unsigned char[fileData.length() - 16];
        nLength = fileData.length() - 16;
        std::copy(in + 16, in + nLength, data);
    }
    delete out;
    return new FileBuffer(data,nLength);
}
