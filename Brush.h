/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef BRUSH_H
#define	BRUSH_H

class Texture;
class QImage;

class Brush {
public:
    enum Transformation {
        RANDOM = 0,
        ROT0 = 1,
        ROT90 = 2,
        ROT180 = 3,
        ROT270 = 4,
        PRESENT = 5
    };
    
    Brush();
    // texture
    Texture* tex = 0;
    Transformation texTransformation = RANDOM;
    bool useTexture = false;
    int color[3];
    int texId = -1;
    // universal
    int size = 10;
    float scale = 1;
    float alpha = 1;
    float direction = 1;
    // height
    float hFixed = 0;
    int hType = 0;
    // embarkment
    int eSize = 2;
    int eEmb = 5;
    int eCut = 5;
    int eRadius = 20;
    QImage* brushshape = 0; 
    float getAlpha(int x, int y, int size );
private:

};

#endif	/* BRUSH_H */

