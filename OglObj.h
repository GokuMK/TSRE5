#ifndef OGLOBJ_H
#define	OGLOBJ_H

#include "GLUU.h"

class OglObj {
public:
    enum VertexAttr {V = 3, VT = 5, VNT = 8};
    bool loaded = false;
    
    OglObj();
    OglObj(const OglObj& orig);
    virtual ~OglObj();
    void init(float* punkty, int ptr, enum VertexAttr v, int type);
    void render();
private:
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
    int length; 
    int shapeType;
};

#endif	/* OGLOBJ_H */

