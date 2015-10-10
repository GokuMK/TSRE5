#ifndef DYNTRACKOBJ_H
#define	DYNTRACKOBJ_H

#include "WorldObj.h"
#include <QString>

class DynTrackObj : public WorldObj{
public:
    struct Section{
        int type;
        unsigned int sectIdx;
        float a;
        float r;
    };
    struct Shape{
        int iloscv;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };
    
    Section* sections;
    Shape shape[2];
    int sectionIdx;
    DynTrackObj();
    DynTrackObj(const DynTrackObj& orig);
    virtual ~DynTrackObj();
    bool allowNew();
    void load(int x, int y);
    void set(int sh, FileBuffer* val);
    void set(QString sh, FileBuffer* data);
    void set(QString sh, float* val);
    bool getBorder(float* border);
    void save(QTextStream* out);
    void resize(float x, float y, float z);
    virtual void deleteVBO();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    int tex1;
    int tex2;
    bool init;
    float elevation;
    float* jNodePosn = NULL;
    float bound[6];
    int sidxSelected = 0;
    void drawShape();
    void genShape();
    void createVBO(Shape* shape, int ptr, float * data);
};

#endif	/* DYNTRACKOBJ_H */

