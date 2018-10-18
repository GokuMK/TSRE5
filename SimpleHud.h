/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIMPLEHUD_H
#define	SIMPLEHUD_H

#include <QString>
#include <QVector>
#include <QMap>

class TextObj;
class OglObj;

class SimpleHud { 
public:
    SimpleHud();
    SimpleHud(const SimpleHud& orig);
    virtual ~SimpleHud();
    void render();
    void setAttribute(QString name, float val);
    
private:
    TextObj *speedText = NULL;
    TextObj *distanceText = NULL;
    TextObj *kmText = NULL;
    TextObj *kmsText = NULL;
    QVector<TextObj*> numberText;
    
    OglObj *gradientObj = NULL;
    
    QMap<QString, float> attributes;
};

#endif	/* SIMPLEHUD_H */

