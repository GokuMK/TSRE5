#ifndef OGLOBJ_H
#define	OGLOBJ_H

#include <QString>
#include "GLUU.h"
#include "Vector4f.h"

class OglObj {
public:
    enum VertexAttr {V = 3, VT = 5, VNT = 8};
    enum MaterialType {NONE, TEXTURE, COLOR};
    bool loaded;
    
    OglObj();
    OglObj(const OglObj& orig);
    virtual ~OglObj();
    void init(float* punkty, int ptr, enum VertexAttr v, int type);
    virtual void render();
    virtual void render(int selectionColor);
    void deleteVBO();
    void setMaterial(float r, float g, float b);
    void setMaterial(QString* path);
    int getTexId();
private:
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
    int length; 
    int shapeType;
    int texId;
    int materialType;
    QString *res;
    Vector4f *color = NULL;
};

#endif	/* OGLOBJ_H */

