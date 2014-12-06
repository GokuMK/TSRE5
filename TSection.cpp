#include "TSection.h"
#include <math.h>
#include "GLMatrix.h"
#include <QDebug>
#include "Vector2f.h"

TSection::TSection(int index) {
    id = index;
    type = 0;
}

TSection::TSection(const TSection& orig) {
}

TSection::~TSection() {
}

float TSection::getDlugosc(){
        if(type == 0){
            return size;
        }
        else if(type==1){
            return fabs(radius*angle);
        }
        return 0;
    }
    
int TSection::getLineBufferSize(){
    float kierunek;
    if(type==0){
        return 6;
    } else if(type==1){
        kierunek = 1;
        int bufSize = 0;
        if(angle > 0) kierunek = -1;
        float aa = -0.05f*kierunek;
        for(float angle2 = angle*kierunek; angle2<0; angle2+=0.05){
            if(angle2 > -0.05f) aa = angle2*kierunek;
               bufSize+=6;
        }
        return bufSize;
    }
    return 0;
}

void TSection::drawSection(float* &ptr, float* matrix) {

            float kierunek;
            if(type==0){

                float point1[3]; point1[0] = 0; point1[1] = 2; point1[2] = 0;
                float point2[3]; point2[0] = 0; point2[1] = 2; point2[2] = size;
    
                Vec3::transformMat4(point1, point1, matrix);
                Vec3::transformMat4(point2, point2, matrix);

                *ptr++ = point1[0];
                *ptr++ = point1[1];
                *ptr++ = point1[2];
                *ptr++ = point2[0];
                *ptr++ = point2[1];
                *ptr++ = point2[2];
                //podklady
                //gl.glBegin(GL2.GL_LINES);
                //gl.glVertex3f(0, 2f, 0); 
                //gl.glVertex3f(0, 2f, size);
                //gl.glEnd();
            }
            //krzywa
            else if(type==1){
                //if(angle==0||radius==0||angle>6.28) {
                    //System.out.println("fail"+this.id);
                    //return;
                //}
                
                kierunek = 1;
                if(angle > 0) kierunek = -1;
                float aa = -0.05f*kierunek;
                float point1[3]; 
                float point2[3]; 
                for(float angle2 = angle*kierunek; angle2<0; angle2+=0.05){
                    if(angle2 > -0.05f) aa = angle2*kierunek;

                    Vector2f a(0, 0);
                    a.rotate(aa, radius);

                    point1[0] = 0; point1[1] = 2; point1[2] = 0;
                    point2[0] = kierunek*a.x; point2[1] = 2; point2[2] = kierunek*a.y;
    
                    Vec3::transformMat4(point1, point1, matrix);
                    Vec3::transformMat4(point2, point2, matrix);
                    *ptr++ = point1[0];
                    *ptr++ = point1[1];
                    *ptr++ = point1[2];
                    *ptr++ = point2[0];
                    *ptr++ = point2[1];
                    *ptr++ = point2[2];
                    Mat4::translate(matrix, matrix, kierunek*a.x,0,kierunek*a.y);
                    Mat4::rotateY(matrix, matrix, -aa);
                }
            }
    }
    
void TSection::setDrawPosition(float metry) {
            //prosta
       /*     if(type==0){
                gl.glTranslatef(0, 0, metry);
            }
            //krzywa
            else if(type==1){
                float kierunek = 1;
                if(angle > 0) kierunek = -1;

                float aa = -metry/radius*kierunek;
                
                Vector2f a = new Vector2f(0.0f, 0.0f);
                a.rotate(aa, radius);

                float dlugosc = a.getDlugosc();

                gl.glTranslatef(kierunek*a.x,0,kierunek*a.y);
                //gl.glRotatef((float)(-aa*180f/Math.PI),0,1,0);
            }*/
    }
    
Vector3f *TSection::getDrawPosition(float metry) {
            //prosta
            if(type==0){
                return(new Vector3f(0, 0, metry));
            }
            //krzywa
            else if(type==1){
                float kierunek = 1;
                if(angle > 0) kierunek = -1;

                float aa = -metry/radius*kierunek;
                
                Vector2f a;
                a.rotate(aa, radius);

                return(new Vector3f(kierunek*a.x, 0, kierunek*a.y));
            }
            return new Vector3f();
    }
    
void TSection::setCamPosition(float metry, float ob) {
         /*   if(type==0){
                gl.glTranslatef(0, 0, metry*ob);
            }
            //krzywa
            else if(type==1){
                float kierunek = 1;
                if(angle > 0) 
                    kierunek = -1;

                float aa = -metry*ob/radius*kierunek;
                
                Vector2f a = new Vector2f(0.0f, 0.0f);
                a.rotate(aa, radius);

                float dlugosc = a.getDlugosc();

                if(ob==-1){
                    gl.glTranslatef(kierunek*a.x,0,kierunek*a.y);
                    gl.glRotatef((float)(-aa*180f/Math.PI),0,1,0);
                } else {
                    aa = -aa;
                    gl.glRotatef((float)(-aa*180f/Math.PI),0,1,0);       
                    gl.glTranslatef(kierunek*a.x,0,kierunek*a.y);
                }
            }*/
    }

void TSection::setCamRotation(float metry, float ob) {
           /* if(type==1){
                float kierunek = 1;
                if(angle > 0) 
                    kierunek = -1;

                float aa = -metry*ob/radius*kierunek;
                
                Vector2f a = new Vector2f(0.0f, 0.0f);
                a.rotate(aa, radius);

                if(ob==-1){
                    gl.glRotatef((float)(-aa*180f/Math.PI),0,1,0);
                } else {
                    aa = -aa;
                    gl.glRotatef((float)(-aa*180f/Math.PI),0,1,0);       
                }
            }*/
    }

