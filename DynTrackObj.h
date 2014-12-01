#ifndef DYNTRACKOBJ_H
#define	DYNTRACKOBJ_H

#include "WorldObj.h"
#include <QString>

class DynTrackObj : WorldObj{
public:
    struct Section{
        int type;
        unsigned int val1;
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
    
    DynTrackObj();
    DynTrackObj(const DynTrackObj& orig);
    virtual ~DynTrackObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    bool getBorder(float* border);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    int tex1;
    int tex2;
    bool init;
    int sectionIdx;
    float elevation;
    float* jNodePosn = NULL;
    float bound[6];
    void drawShape();
    void genShape();
    void createVBO(Shape* shape, int ptr, float * data);
};

#endif	/* DYNTRACKOBJ_H */

