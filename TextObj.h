#include "OglObj.h"
#include <QPainter>

#ifndef TEXTOBJ_H
#define	TEXTOBJ_H

class TextObj : public OglObj{
public:
    bool inUse = false;
    float pos[3];
    TextObj();
    TextObj(int val);
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
};

#endif	/* TEXTOBJ_H */

