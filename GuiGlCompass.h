/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GUIGLCOMPASS_H
#define	GUIGLCOMPASS_H

#include "OglObj.h"

class GuiGlCompass : public OglObj { 
public:
    GuiGlCompass();
    virtual ~GuiGlCompass();
    void render();
    void render(float a);

private:
    float angle = 0;
};

#endif	/* GUIGLCOMPASS_H */

