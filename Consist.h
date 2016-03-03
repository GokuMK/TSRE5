#ifndef CONSIST_H
#define	CONSIST_H

#include <QString>

class Eng;

class Consist {
public:
    struct EngItem{
        bool flip = false;
        int uid = 0;
        int eng = -1;
        float pos = 0;
    };
    Consist();
    virtual ~Consist();
    QString name;
    QString path;
    QString pathid;
    QString conName;
    unsigned int serial = 0;
    float maxVelocity[2];
    int nextWagonUID = 0;
    float durability = 0;
    int loaded = -1;
    bool kierunek = false;
    int ref = 0;
    int posInit = false;
    std::vector<EngItem> engItems;
    Consist(QString p, QString n);
    Consist(QString src, QString p, QString n);
    void load();
    void render();
    void render(int aktwx, int aktwz);
private:
    void initPos();
};

#endif	/* CONSIST_H */

