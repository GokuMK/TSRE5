/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESSOUNDSOURCE_H
#define	PROPERTIESSOUNDSOURCE_H

#include "PropertiesAbstract.h"

class SoundSourceObj;

class PropertiesSoundSource : public PropertiesAbstract{
    Q_OBJECT 
public:
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    PropertiesSoundSource();
    virtual ~PropertiesSoundSource();
private:
    SoundSourceObj *sobj = NULL;
    QComboBox sources;
    QLineEdit sName;
};

#endif	/* PROPERTIESSOUNDSOURCE_H */

