#ifndef ENG_H
#define	ENG_H
#include <QString>

class Eng {
public:
    QString path;
    QString name;
    int sfile[2];
    float sizex;
    float sizey;
    float sizez;
    int flip = flip;
    int loaded = -1;
    bool kierunek = false;
    Eng();
    Eng(const Eng& orig);
    virtual ~Eng();
    Eng(QString src, QString n, int f);
    void load();
    void render();
    void render(int aktwx, int aktwz);
private:

};

#endif	/* ENG_H */

