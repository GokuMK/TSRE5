#include <unordered_map>
#include <QString>
#include "Texture.h"

#ifndef TEXLIB_H
#define	TEXLIB_H

class TexLib {
public:
    TexLib();
    TexLib(const TexLib& orig);
    virtual ~TexLib();
    static int jesttextur;
    static std::unordered_map<int, Texture*> mtex;
    static void reset();
    static void delRef(int texx);
    static void addRef(int texx);
    static int addTex(QString path, QString name);
    static int addTex(QString pathid);
private:

};

#endif	/* TEXLIB_H */

