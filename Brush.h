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
    int eSize = 2;
    int eEmb = 5;
    int eCut = 5;
    int eRadius = 20;
private:

};

#endif	/* BRUSH_H */

