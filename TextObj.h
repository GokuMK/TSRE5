#include "OglObj.h"
#include <QPainter>

#ifndef TEXTOBJ_H
#define	TEXTOBJ_H

#include <QString>

class TextObj : public OglObj{
public:
    bool inUse = false;
    float pos[3];
    TextObj(QString val, float s = 0, float sc = 0);
    TextObj();
    TextObj(int val, float s = 0, float sc = 0);
    TextObj(const TextObj& orig);
    virtual ~TextObj();
    void render();
    void render(float rot);
    void setColor(int r, int g, int b);
private:
    QString text;
    void init();
    QColor color;
    bool isInit = false;
    float size = 4;
    float scale = 1;
};

#endif	/* TEXTOBJ_H */

