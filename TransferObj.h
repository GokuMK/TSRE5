#ifndef TRANSFEROBJ_H
#define	TRANSFEROBJ_H

#include "WorldObj.h"
#include <QString>

class TransferObj : WorldObj {
public:
    struct Shape{
        int iloscv;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };
    Shape shape;
    QString texture;
    
    TransferObj();
    TransferObj(const TransferObj& orig);
    virtual ~TransferObj();
    
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    void drawShape();
    int tex;
    bool init;
    float width;
    float height;
};

#endif	/* TRANSFEROBJ_H */

