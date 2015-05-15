#include "OglObj.h"
#include "TexLib.h"

OglObj::OglObj() {
    loaded = false;
    texId = -1;
    materialType = NONE;
}

OglObj::OglObj(const OglObj& orig) {
}

OglObj::~OglObj() {

}

void OglObj::setMaterial(float r, float g, float b) {
    materialType = COLOR;
    color = new Vector4f(r, g, b, 1.0);
}

void OglObj::setMaterial(QString* path) {
    materialType = TEXTURE;
    res = path;
}

void OglObj::deleteVBO(){
    if(loaded){
        VBO.destroy();
        VAO.destroy();
    }
    loaded = false;
}

void OglObj::init(float* punkty, int ptr, enum VertexAttr v, int type) {
    if(loaded){
        VBO.destroy();
        VAO.destroy();
    }
    
    shapeType = type;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    VAO.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);

    VBO.create();
    VBO.bind();
    VBO.allocate(punkty, ptr * sizeof (GLfloat));

    if (v == V) {
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
    } else if (v == VT) {
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), reinterpret_cast<void *> (3 * sizeof (GLfloat)));
    } else if (v == VNT) {
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (6 * sizeof (GLfloat)));
    }

    VBO.release();
    length = ptr / v;
    loaded = true;
}

void OglObj::render() {
    GLUU *gluu = GLUU::get();
    if(materialType == TEXTURE){
        gluu->enableTextures();
        if (texId == -2) {
            return;
        } else {
            if (texId == -1) {
                texId = TexLib::addTex(*res);
            }
            if (TexLib::mtex[texId]->loaded) {
                if (!TexLib::mtex[texId]->glLoaded)
                    TexLib::mtex[texId]->GLTextures();
                glBindTexture(GL_TEXTURE_2D, TexLib::mtex[texId]->tex[0]);
                gluu->alpha = 0;
                gluu->m_program->setUniformValue(gluu->shaderAlpha, gluu->alpha);
            } else {
            }
        }
    } else if(materialType == COLOR){
        gluu->disableTextures(color);
    } else if(materialType == NONE){
        return;
    }

    QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);
    glDrawArrays(shapeType, 0, length); /**/
}

