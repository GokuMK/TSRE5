#ifndef GLH_H
#define	GLH_H

class GLH {
public:
    GLH();
    GLH(const GLH& orig);
    virtual ~GLH();
    static int glhProjectf(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate);
    static int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate);
    
private:
    static void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2);
    static void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector);
    static int glhInvertMatrixf2(float *m, float *out);
};

#endif	/* GLH_H */

