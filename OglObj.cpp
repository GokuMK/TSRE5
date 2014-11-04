#include "OglObj.h"

OglObj::OglObj() {
    loaded = false;
}

OglObj::OglObj(const OglObj& orig) {
}

OglObj::~OglObj() {
}

void OglObj::init(float* punkty, int ptr, enum VertexAttr v, int type){
    shapeType = type;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    VAO.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);

    VBO.create();
    VBO.bind();
    VBO.allocate(punkty, ptr * sizeof (GLfloat));
    
    if(v == V){
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
    } else if(v == VT){
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
    } else if(v == VNT){
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (5 * sizeof (GLfloat)));
    }
    
    VBO.release();
    length = ptr/v;
    loaded = true;
}

void OglObj::render(){
    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);
    glDrawArrays(shapeType, 0, length);/**/
}

