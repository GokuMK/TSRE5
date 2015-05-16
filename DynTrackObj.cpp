#include "DynTrackObj.h"
#include "ParserX.h"
#include <QDebug>
#include "GLMatrix.h"
#include "TexLib.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include <QOpenGLShaderProgram>
#include "GLUU.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

DynTrackObj::DynTrackObj() {
}

DynTrackObj::DynTrackObj(const DynTrackObj& orig) {
}

DynTrackObj::~DynTrackObj() {
}

void DynTrackObj::load(int x, int y) {
    this->tex1 = -1;
    this->init = false;
    //this->shape = ShapeLib::addShape(path, fileName);
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    Mat4::fromRotationTranslation(this->matrix, qDirection, position);
    Mat4::rotate(this->matrix, this->matrix, M_PI, 0, -1, 0);
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 3;
    this->modified = false;
}

void DynTrackObj::set(QString sh, FileBuffer* data) {
    if (sh == ("sectionidx")) {
        //qDebug() << ParserX::parsujr(data);
        sectionIdx = ParserX::parsujr(data);
        return;
    }
    if (sh == ("elevation")) {
        elevation = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tracksections")) {
        sections = new Section[5];
        for (int iii = 0; iii < 5; iii++) {
            sections[iii].type = ParserX::parsujr(data);
            sections[iii].val1 = ParserX::parsujUint(data);
            sections[iii].a = ParserX::parsujr(data);
            sections[iii].r = ParserX::parsujr(data);
            ParserX::pominsekcje(data);
        }
        return;
    }
    if (sh == ("jnodeposn")) {
        jNodePosn = new float[5];
        jNodePosn[0] = ParserX::parsujr(data);
        jNodePosn[1] = ParserX::parsujr(data);
        jNodePosn[2] = ParserX::parsujr(data);
        jNodePosn[3] = ParserX::parsujr(data);
        jNodePosn[4] = ParserX::parsujr(data);
        return;
    }    
    //qDebug() <<"A";
    WorldObj::set(sh, data);
    return;
}

void DynTrackObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;

    Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);

    Mat4::identity(gluu->objStrMatrix);    
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->m_program->setUniformValue(gluu->shaderAlpha, 0.0f);
    
    if(selected){
        drawBox();
    }
    
    if(selectionColor != 0){
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
    } else {
        gluu->enableTextures();
    }
    
    drawShape();
};

void DynTrackObj::drawShape() {
    if (tex1 == -2) {
        glDisable(GL_TEXTURE_2D);
    } else {
        glEnable(GL_TEXTURE_2D);
        if (tex1 == -1) {
            //tex1 = 0;
            tex1 = TexLib::addTex(resPath, "ACleanTrack1.ace");
            tex2 = TexLib::addTex(resPath, "ACleanTrack2.ace");
            glDisable(GL_TEXTURE_2D);
        }
    }

    if (!init) {
        genShape();
        init = true;
    } else {
        
        if(TexLib::mtex[tex1]->loaded){
            if(!TexLib::mtex[tex1]->glLoaded) TexLib::mtex[tex1]->GLTextures();
            glBindTexture(GL_TEXTURE_2D, TexLib::mtex[tex1]->tex[0]);
        }
        
        QOpenGLVertexArrayObject::Binder vaoBinder1(&shape[0].VAO);
        glDrawArrays(GL_TRIANGLES, 0, shape[0].iloscv);
        
        if(TexLib::mtex[tex2]->loaded){
            if(!TexLib::mtex[tex2]->glLoaded) TexLib::mtex[tex2]->GLTextures();
            glBindTexture(GL_TEXTURE_2D, TexLib::mtex[tex2]->tex[0]);
        }
        
        QOpenGLVertexArrayObject::Binder vaoBinder2(&shape[1].VAO);
        glDrawArrays(GL_TRIANGLES, 0, shape[1].iloscv);
    }
}

void DynTrackObj::genShape() {
    float* pd = new float[50000];
    float* sk = new float[50000];

    int ptr = 0;
    int str = 0;
    
    Vector2f tx;
    Vector2f ty;
    Vector2f offpos(0.0, 0.0);
    Vector2f b1;
    Vector2f a1;
    Vector2f a;
    Vector2f b;
    Vector2f dl;
    
    float offrot = 0;

    for (int i = 0; i < 5; i++) {
        if (sections[i].val1 > 100000000) continue;
        //prosta
        if (sections[i].type == 0) {
            //podklady

            b.set(2.5, 0.0);
            b.rotate(offrot, 0);
            a1.set(0.0, sections[i].a);
            a1.rotate(offrot, 0);

            pd[ptr++] = offpos.x + b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0;

            pd[ptr++] = offpos.x - b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0;

            pd[ptr++] = offpos.x - b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].a;

            pd[ptr++] = offpos.x + b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].a;

            pd[ptr++] = offpos.x + b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0;

            pd[ptr++] = offpos.x - b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].a;
            //szyny

            tx.set(-0.717500, 0.0);
            ty.set(-0.867500, 0.0);

            for (int jj = 0; jj < 2; jj++) {
                tx.rotate(offrot, 0);
                ty.rotate(offrot, 0);

                sk[str++] = offpos.x + tx.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2270;

                sk[str++] = offpos.x + ty.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.1330;

                sk[str++] = offpos.x + ty.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.1330;

                sk[str++] = offpos.x + tx.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.2270;

                sk[str++] = offpos.x + tx.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2270;

                sk[str++] = offpos.x + ty.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.1330;
                
                tx.set(0.867500, 0.0);
                ty.set(0.717500, 0.0);
            }
            ///
            tx.set(0.717500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            
            tx.set(-0.717500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            
            tx.set(-0.867500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;

            tx.set(0.867500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            
            offpos.x += a1.x;
            offpos.y += a1.y;
        }
        //krzywa
        if(sections[i].type==1){
            float kierunek = 1;
            if(sections[i].a > 0){
                kierunek = -1;
            }
            float aa = 0;
            float angle;
            float angle2;
            for(angle = 0, angle2 = 0; angle2>sections[i].a*kierunek; angle-=0.03*kierunek,angle2-=0.03){
                if(sections[i].a*kierunek-angle2<-0.03) 
                    aa = -0.03*kierunek;
                else 
                    aa = (sections[i].a*kierunek-angle2)*kierunek;
                //podklady
                b1.set(-2.5, 0.0);
                a1.set(2.5, 0.0);
                a.set(-2.5, 0.0);
                b.set(2.5, 0.0);
                a1.rotate(angle, sections[i].r*kierunek);
                b1.rotate(angle, sections[i].r*kierunek);           
                a.rotate(angle+aa, sections[i].r*kierunek);
                b.rotate(angle+aa, sections[i].r*kierunek);
                a1.rotate(offrot, 0);
                b1.rotate(offrot, 0);
                a.rotate(offrot, 0);
                b.rotate(offrot, 0);
                dl.set(0.0, 0.0);
                dl.rotate(aa, sections[i].r); 
                float dlugosc = dl.getDlugosc();
                    
                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                    
                pd[ptr++] = offpos.x+b1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                  
                pd[ptr++] = offpos.x+b.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0 + 0.2*dlugosc;

                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                
                //szyny
                   
                tx.set(-0.717500, 0.0);
                ty.set(-0.867500, 0.0);
              
                for( int jj = 0; jj < 2; jj++){
                    a.set(tx.x, 0.0);
                    b.set(ty.x, 0.0);
                    tx.rotate(angle, sections[i].r*kierunek);
                    ty.rotate(angle, sections[i].r*kierunek);
                    a.rotate(angle+aa, sections[i].r*kierunek);
                    b.rotate(angle+aa, sections[i].r*kierunek);
                    a.rotate(offrot, 0);
                    b.rotate(offrot, 0);      
                    tx.rotate(offrot, 0); 
                    ty.rotate(offrot, 0); 
                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        
                        sk[str++] = offpos.x+ty.x; sk[str++] = 0.325000; sk[str++] = offpos.y+ty.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.1330;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                        
                        sk[str++] = offpos.x+a.x; sk[str++] =  0.325000; sk[str++] =  offpos.y+a.y;
                        sk[str++] = 0.0; sk[str++] =  1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] =  0.2270;

                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                    ty.set(0.717500, 0.0);
                    tx.set(0.867500, 0.0);
                }
                ///
                tx.set(0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                tx.set(-0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;

                tx.set(-0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;

                tx.set(0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r*kierunek);
                a.rotate(angle+aa, sections[i].r*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
            }
            a.set(0.0, 0.0);
            a.rotate(sections[i].a, sections[i].r*kierunek);
            a.rotate(offrot, 0);
            offpos.x+=a.x; offpos.y+=a.y;
            offrot+=sections[i].a;
        }
        /*if(sections[i].type==1){
            //if(sections[i].a>0) continue;
            float kierunek = 1;
            float aaaaaaaa = 0;
            if(sections[i].a > 0){
                aaaaaaaa = 1;
                kierunek = -1;
            }
            float aa = 0;
            float angle;
            float angle2;
            for(angle = 0, angle2 = 0; angle2>sections[i].a*kierunek; angle-=0.03*kierunek,angle2-=0.03){
                //if(sections[i].a*kierunek-angle2<-0.03) 
                    aa = -0.03*kierunek;
                //else 
                //    aa = sections[i].a-angle;

                //podklady
                b1.set(-2.5, 0.0);
                a1.set(2.5, 0.0);
                a.set(-2.5, 0.0);
                b.set(2.5, 0.0);
                a1.rotate(angle, sections[i].r);
                a1.rotate(M_PI*aaaaaaaa, 0);
                b1.rotate(angle, sections[i].r);           
                b1.rotate(M_PI*aaaaaaaa, 0);
                a.rotate(angle+aa, sections[i].r);
                a.rotate(M_PI*aaaaaaaa, 0);
                b.rotate(angle+aa, sections[i].r);
                b.rotate(M_PI*aaaaaaaa, 0);
                a1.rotate(offrot, 0);
                b1.rotate(offrot, 0);
                a.rotate(offrot, 0);
                b.rotate(offrot, 0);
                dl.set(0.0, 0.0);
                dl.rotate(aa, sections[i].r); 
                float dlugosc = dl.getDlugosc();
                    
                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                    
                pd[ptr++] = offpos.x+b1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                  
                pd[ptr++] = offpos.x+b.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0 + 0.2*dlugosc;

                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                
                //szyny
                   
                tx.set(-0.717500, 0.0);
                ty.set(-0.867500, 0.0);
              
                for( int jj = 0; jj < 2; jj++){
                    a.set(tx.x, 0.0);
                    b.set(ty.x, 0.0);
                    tx.rotate(angle, sections[i].r);
                    ty.rotate(angle, sections[i].r);
                    a.rotate(angle+aa, sections[i].r);
                    b.rotate(angle+aa, sections[i].r);
                    a.rotate(offrot, 0);
                    b.rotate(offrot, 0);      
                    tx.rotate(offrot, 0); 
                    ty.rotate(offrot, 0); 
                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        
                        sk[str++] = offpos.x+ty.x; sk[str++] = 0.325000; sk[str++] = offpos.y+ty.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.1330;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                        
                        sk[str++] = offpos.x+a.x; sk[str++] =  0.325000; sk[str++] =  offpos.y+a.y;
                        sk[str++] = 0.0; sk[str++] =  1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] =  0.2270;

                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                    ty.set(0.717500, 0.0);
                    tx.set(0.867500, 0.0);
                }
                ///
                tx.set(0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r);
                a.rotate(angle+aa, sections[i].r);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                tx.set(-0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r);
                a.rotate(angle+aa, sections[i].r);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;

                tx.set(-0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r);
                a.rotate(angle+aa, sections[i].r);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;

                tx.set(0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].r);
                a.rotate(angle+aa, sections[i].r);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
            }
            a.set(0.0, 0.0);
            a.rotate(sections[i].a, sections[i].r);
            a.rotate(M_PI*aaaaaaaa, 0);
            a.rotate(offrot, 0);
            offpos.x+=a.x; offpos.y+=a.y;
            offrot+=sections[i].a;
        }*/
    }
    //qDebug() << ptr << "" << str;
    
        bound[0] = -9999;
        bound[1] = 9999;
        bound[2] = -9999;
        bound[3] = 9999;
        bound[4] = -9999;
        bound[5] = 9999;
        for (int i = 0; i < ptr ; i+=8) {
            if(pd[i+0] < bound[1]) bound[1] = pd[i+0];
            if(pd[i+1] < bound[3]) bound[3] = pd[i+1];
            if(pd[i+2] < bound[5]) bound[5] = pd[i+2];
            if(pd[i+0] > bound[0]) bound[0] = pd[i+0];
            if(pd[i+1] > bound[2]) bound[2] = pd[i+1];
            if(pd[i+2] > bound[4]) bound[4] = pd[i+2];
        }
        for (int i = 0; i < str ; i+=8) {
            if(sk[i+0] < bound[1]) bound[1] = sk[i+0];
            if(sk[i+1] < bound[3]) bound[3] = sk[i+1];
            if(sk[i+2] < bound[5]) bound[5] = sk[i+2];
            if(sk[i+0] > bound[0]) bound[0] = sk[i+0];
            if(sk[i+1] > bound[2]) bound[2] = sk[i+1];
            if(sk[i+2] > bound[4]) bound[4] = sk[i+2];
        }
    
    createVBO(&shape[0], ptr, pd);
    createVBO(&shape[1], str, sk);
}

void DynTrackObj::createVBO(Shape* shape, int ptr, float * data) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    shape->VAO.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&shape->VAO);

    shape->VBO.create();
    shape->VBO.bind();
    shape->VBO.allocate(data, ptr * sizeof (GLfloat));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (6 * sizeof (GLfloat)));
    shape->VBO.release();
    shape->iloscv = ptr/8;
    
    delete data;
}

bool DynTrackObj::getBorder(float* border){
    border[0] = bound[0];
    border[1] = bound[1];
    border[2] = bound[2];
    border[3] = bound[3];
    border[4] = bound[4];
    border[5] = bound[5];
    return true;
}

void DynTrackObj::save(QTextStream* out){
    if (!loaded) return;
    int l;
    QString flags;
    flags = QString::number(this->staticFlags, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++)
        flags = "0"+flags;
    
*(out) << "	Dyntrack (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		TrackSections (\n";
for(int i = 0; i < 5; i++){
*(out) << "			TrackSection (\n";
*(out) << "				SectionCurve ( "<<this->sections[i].type<<" ) "<<this->sections[i].val1<<" "<<this->sections[i].a<<" "<<this->sections[i].r<<"\n";
*(out) << "			)\n";
}
*(out) << "		)\n";
*(out) << "		SectionIdx ( "<<this->sectionIdx<<" )\n";
*(out) << "		Elevation ( "<<this->elevation<<" )\n";
if(this->jNodePosn!=NULL)
*(out) << "		JNodePosn ( "<<this->jNodePosn[0]<<" "<<this->jNodePosn[1]<<" "<<this->jNodePosn[2]<<" "<<this->jNodePosn[3]<<" "<<this->jNodePosn[4]<<" )\n";
*(out) << "		CollideFlags ( "<<this->collideFlags<<" )\n";
*(out) << "		StaticFlags ( "<<flags<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}
