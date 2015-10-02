#ifndef BRUSH_H
#define	BRUSH_H

class Texture;

class Brush {
public:
    Brush();
    // texture
    Texture* tex = 0;
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
    int eSize = 3;
    int eEmb = 2;
    int eCut = 2;
    int eRadius = 2;
private:

};

#endif	/* BRUSH_H */

