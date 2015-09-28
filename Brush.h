#ifndef BRUSH_H
#define	BRUSH_H

class Texture;

class Brush {
public:
    Brush();
    //int color[3];
    Texture* tex = 0;
    int color[3];
    int texId = -1;
    int size = 10;
    float scale = 1;
    float alpha = 1;
private:

};

#endif	/* BRUSH_H */

