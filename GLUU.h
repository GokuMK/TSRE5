#ifndef GLUU_H
#define	GLUU_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
		
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLUU {
public:
    QOpenGLShaderProgram *m_program;
    unsigned int shaderProgram;
    
    unsigned int vertexPositionAttribute;
    unsigned int textureCoordAttribute;
    unsigned int pMatrixUniform;
    unsigned int mvMatrixUniform;
    unsigned int msMatrixUniform;
    unsigned int samplerUniform;
    
    float* mvMatrixStack[1000];
    int imvMatrixStack;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    //QMatrix4x4 m_proj;
    //QMatrix4x4 m_camera;
    //QMatrix4x4 m_world;
    float* pMatrix;
    float* mvMatrix;
    float* objStrMatrix;
    static GLUU *get();
    GLUU();
    virtual ~GLUU();
    void initShader();
    void setMatrixUniforms();
    void mvPushMatrix();
    void mvPopMatrix();
    float degToRad(float degrees);
private:
    unsigned int getShader(const char *shaderScript, QString type);
};

#endif	/* GLUU_H */

