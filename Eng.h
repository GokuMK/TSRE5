#ifndef ENG_H
#define	ENG_H
#include <QString>

class OglObj;

class Eng {
public:
    struct Coupling {
        QString type;
	float r0[2];
	float velocity;
    };
    std::vector<Coupling> coupling;
    QString name;
    QString path;
    QString pathid;
    int sfile[2];
    QString sNames[2];
    QString engName;
    QString displayName;
    QString engType;
    QString typeHash;
    QString type;
    QString brakeSystemType;
    int wagonTypeId = 0;
    float mass;
    float sizex;
    float sizey;
    float sizez;
    float maxSpeed = 0;
    int flip = flip;
    int loaded = -1;
    bool kierunek = false;
    int ref = 0;
    Eng();
    virtual ~Eng();
    Eng(QString p, QString n);
    Eng(QString src, QString p, QString n);
    void load();
    float getFullWidth();
    QString getCouplingsName();
    void select();
    void unselect();
    bool isSelected();
    void drawBorder();
    bool engFilter(QString q);
    bool couplingFilter(QString q);
    bool searchFilter(QString q);
    void render(int selectionColor = 0);
    void render(int aktwx, int aktwz, int selectionColor);
private:
    bool selected = false;
    OglObj *borderObj = NULL;
};

#endif	/* ENG_H */

