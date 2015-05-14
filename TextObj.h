/* 
 * File:   TextObj.h
 * Author: Goku
 *
 * Created on 14 maja 2015, 21:43
 */
#include "OglObj.h"

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
private:
    QString text;
    void init();
};

#endif	/* TEXTOBJ_H */

