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
    float width = 0;
    float height = 0;
    
    TransferObj();
    TransferObj(const TransferObj& orig);
    virtual ~TransferObj();
    bool allowNew();
    void load(int x, int y);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, float val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void deleteVBO();
    int getTexId();
    virtual Ref::RefItem* getRefInfo();
    void translate(float px, float py, float pz);
    void rotate(float x, float y, float z);
    void resize(float x, float y, float z);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    void drawShape();
    int tex;
    bool init;
    float bound[6];
    QString *texturePath;
};

#endif	/* TRANSFEROBJ_H */

