#include "GLUU.h"
#include "zconf.h"
#include "GLMatrix.h"
#include <QOpenGLShaderProgram>
#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLUU* GLUU::get() {
    static GLUU* gluu = new GLUU();
    return gluu;
}

GLUU::GLUU() {
    imvMatrixStack = 0;
    pMatrix = new float[16];
    mvMatrix = new float[16];
    objStrMatrix = new float[16];
}

GLUU::~GLUU() {

}
static const char *vertexShaderSource =
        "attribute vec4 vertex;\n"
        "attribute vec2 aTextureCoord;\n"
        "uniform mat4 uPMatrix;\n"
        "uniform mat4 uMVMatrix;\n"
        "uniform mat4 uMSMatrix;\n"
        "varying vec2 vTextureCoord;\n"
        "void main() {\n"
        "   vTextureCoord = aTextureCoord;\n"
        "   gl_Position = uPMatrix * uMVMatrix * uMSMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying vec2 vTextureCoord;\n"
        "uniform sampler2D uSampler;\n"
        "void main() {\n"
        "   gl_FragColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));\n"
        "//gl_FragColor  =vec4(1.0,1.0,1.0,1.0);\n"
        "if(gl_FragColor.a < 0.3) discard;\n"
        "}\n";

unsigned int GLUU::getShader(const char *shaderScript, QString type) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    unsigned int shader;
    if (type == "fs") {
        shader = f->glCreateShader(GL_FRAGMENT_SHADER);
    } else if (type == "vs") {
        shader = f->glCreateShader(GL_VERTEX_SHADER);
    } else {
        return -1;
    }

    f->glShaderSource(shader, 1, &shaderScript, NULL);
    f->glCompileShader(shader);

    GLint isCompiled = 0;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        f->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* errorLog = new char[maxLength];
        f->glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
        qDebug() << errorLog;

        f->glDeleteShader(shader); //Don't leak the shader.
        return -1;
    }

    return shader;
};

void GLUU::initShader() {
    /*QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    
    unsigned int fragmentShader = getShader(fragmentShaderSource,"fs");
    unsigned int vertexShader = getShader(vertexShaderSource,"vs");
        
    f->glAttachShader(shaderProgram, vertexShader);
    f->glAttachShader(shaderProgram, fragmentShader);
    f->glLinkProgram(shaderProgram);

    //if (!gl.getProgramParameter(this.shaderProgram, gl.LINK_STATUS)) {
    //    alert("Could not initialise shaders");
    //}

    f->glUseProgram(shaderProgram);

    vertexPositionAttribute = f->glGetAttribLocation(shaderProgram, "vertex");
    f->glEnableVertexAttribArray(vertexPositionAttribute);

    textureCoordAttribute = f->glGetAttribLocation(shaderProgram, "aTextureCoord");
    f->glEnableVertexAttribArray(textureCoordAttribute);

    pMatrixUniform = f->glGetUniformLocation(shaderProgram, "uPMatrix");
    mvMatrixUniform = f->glGetUniformLocation(shaderProgram, "uMVMatrix");
    msMatrixUniform = f->glGetUniformLocation(shaderProgram, "uMSMatrix");
    samplerUniform = f->glGetUniformLocation(shaderProgram, "uSampler");*/

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("aTextureCoord", 1);
    m_program->link();

    m_program->bind();
    pMatrixUniform = m_program->uniformLocation("uPMatrix");
    mvMatrixUniform = m_program->uniformLocation("uMVMatrix");
    msMatrixUniform = m_program->uniformLocation("uMSMatrix");
    //m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    //m_lightPosLoc = m_program->uniformLocation("lightPos");
    // Light position is fixed.
    //m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->release();
}

void GLUU::mvPushMatrix() {
    float* copy = Mat4::clone(mvMatrix);
    mvMatrixStack[imvMatrixStack++] = copy;
}

void GLUU::mvPopMatrix() {
    imvMatrixStack--;
    if (imvMatrixStack < 0) {
        return;
    }
    delete mvMatrix;
    mvMatrix = mvMatrixStack[imvMatrixStack];
}

void GLUU::setMatrixUniforms() {
    m_program->setUniformValue(pMatrixUniform, *reinterpret_cast<float(*)[4][4]> (pMatrix));
    m_program->setUniformValue(mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (mvMatrix));
    m_program->setUniformValue(msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (objStrMatrix));
};

float GLUU::degToRad(float degrees) {
    return degrees * M_PI / 180.0;
}