#include "TSection.h"
#include <math.h>

TSection::TSection(int index) {
    id = index;
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
    
void TSection::drawSection() {
       /*     float kierunek;
            if(type==0){
                //podklady
                gl.glBegin(GL2.GL_LINES);
                gl.glVertex3f(0, 2f, 0); 
                gl.glVertex3f(0, 2f, size);
                gl.glEnd();
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
                for(float angle2 = angle*kierunek; angle2<0; angle2+=0.05){
                    if(angle2 > -0.05f) aa = angle2*kierunek;

                    //podklady
                    Vector2f a = new Vector2f(0.0f, 0.0f);
                    a.rotate(aa, radius);

                    float dlugosc = a.getDlugosc();

                    gl.glBegin(GL2.GL_LINES);
                    gl.glVertex3f(0, 2f, 0); 
                    gl.glVertex3f(kierunek*a.x, 2f, kierunek*a.y);
                    gl.glEnd();

                    gl.glTranslatef(kierunek*a.x,0,kierunek*a.y);
                    gl.glRotatef((float)(-aa*180f/Math.PI),0,1,0);
                }
            }*/
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
        /*    if(type==0){
                return(new Vector3f(0, 0, metry));
            }
            //krzywa
            else if(type==1){
                float kierunek = 1;
                if(angle > 0) kierunek = -1;

                float aa = -metry/radius*kierunek;
                
                Vector2f a = new Vector2f(0.0f, 0.0f);
                a.rotate(aa, radius);

                return(new Vector3f(kierunek*a.x, 0, kierunek*a.y));
            }*/
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

