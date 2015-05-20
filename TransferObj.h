#ifndef TRANSFEROBJ_H
#define	TRANSFEROBJ_H

#include "WorldObj.h"
#include "OglObj.h"
#include <QString>

class TransferObj : public WorldObj {
public:
    /*struct Shape{
        int iloscv;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };*/
    //Shape shape;
    OglObj shape;
    QString texture;
    
    TransferObj();
    TransferObj(const TransferObj& orig);
    virtual ~TransferObj();
    bool allowNew();
    void load(int x, int y);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void deleteVBO();
    void translate(float px, float py, float pz);
    void rotate(float x, float y, float z);
    void resize(float x, float y, float z);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    void drawShape();
    int tex;
    bool init;
    float width = 0;
    float height = 0;
    float bound[6];
    QString *texturePath;
};

#endif	/* TRANSFEROBJ_H */

