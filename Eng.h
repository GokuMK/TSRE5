/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ENG_H
#define	ENG_H
#include <QString>
#include <QVector>

class OglObj;

class Eng {
public:
    struct Coupling {
        QString type;
	float r0[2];
	float velocity;
    };
    
    struct EngShape {
        QString name;
        float x = 0;
        float y = 0;
        float z = 0;
        int id = -2;
    };
    
    std::vector<Coupling> coupling;
    QString name;
    QString path;
    QString pathid;
    QString orpathid;
    QString orpath;
    QVector<QString> filePaths;
    
    EngShape shape;
    std::vector<EngShape> freightanimShape;

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
    float maxForce = 0;
    float maxPower = 0;
    float maxCurrent = 0;
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
    void addToFileList(QString val);
};

#endif	/* ENG_H */

