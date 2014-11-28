#ifndef FORESTOBJ_H
#define	FORESTOBJ_H

#include "WorldObj.h"
#include <QString>

class ForestObj : WorldObj{
public:
    struct Shape{
        int iloscv;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };
    Shape shape;
    QString treeTexture;
    float scaleRangeX;
    float scaleRangeZ;
    float areaX;
    float areaZ;
    float treeSizeX;
    float treeSizeZ;
    float population;
    ForestObj();
    ForestObj(const ForestObj& orig);
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
    
    virtual ~ForestObj();
private:
    void drawShape();
    int tex;
    bool init;
};

#endif	/* FORESTOBJ_H */

