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

float TSection::getDlugosc() {
    if (type == 0) {
        return size;
    } else if (type == 1) {
        return fabs(radius * angle);
    }
    return 0;
}

float TSection::getAngle() {
    if (type == 0) {
        return 0;
    } else if (type == 1) {
        return angle;
    }
    return 0;
}

int TSection::getLineBufferSize(int pointSize, int step) {
    float kierunek;
    if (type == 0) {
        if (step == 0) {
            return pointSize * 2;
        } else {
            int len = 0;
            for (int i = 0; i < size; i += step) {
                len += pointSize * 2;
            }
            return len;
        }
    } else if (type == 1) {
        kierunek = 1;
        if (angle > 0) kierunek = -1;
        float astep = -0.05;
        if (step != 0)
            astep = -step / radius;
        float aa = astep*kierunek;
        int bufSize = 0;
        for (float angle2 = angle * kierunek; angle2 < 0; angle2 -= astep) {
            if (angle2 > astep) aa = angle2 * kierunek;
            bufSize += pointSize * 2;
        }
        return bufSize;
    }
    return 0;
}

void TSection::drawSection(float* &ptr, float* matrix, float height) {
    drawSection(ptr, matrix, height, -1, 0, 0, 0);
}

void TSection::drawSection(float* &ptr, float* matrix, float height, int idx, int vidx) {
    drawSection(ptr, matrix, height, idx, vidx, 0, 0);
}

void TSection::drawSection(float* &ptr, float* matrix, float height, int idx, int vidx, float offset, int step) {

    float kierunek;
    if (type == 0) {

        float point1[3];
        point1[0] = 0;
        point1[1] = height;
        point1[2] = 0;
        float point2[3];
        point2[0] = 0;
        point2[1] = height;
        point2[2] = size;

        Vec3::transformMat4(point1, point1, matrix);
        Vec3::transformMat4(point2, point2, matrix);
        if (step == 0) {
            *ptr++ = point1[0];
            *ptr++ = point1[1];
            *ptr++ = point1[2];
            if (idx >= 0) {
                *ptr++ = idx;
                *ptr++ = vidx;
                *ptr++ = 0 + offset;
            }
            *ptr++ = point2[0];
            *ptr++ = point2[1];
            *ptr++ = point2[2];
            if (idx >= 0) {
                *ptr++ = idx;
                *ptr++ = vidx;
                *ptr++ = size + offset;
            }
        } else {
            float step2 = 0;
            float point[3];
            for (int i = 0; i < size; i += step) {
                step2 = i + step;
                if (step2 > size) step2 = size;
                point[0] = 0;
                point[1] = height;
                point[2] = i;
                Vec3::transformMat4(point, point, matrix);
                *ptr++ = point[0];
                *ptr++ = point[1];
                *ptr++ = point[2];
                if (idx >= 0) {
                    *ptr++ = idx;
                    *ptr++ = vidx;
                    *ptr++ = i + offset;
                }
                point[0] = 0;
                point[1] = height;
                point[2] = step2;
                Vec3::transformMat4(point, point, matrix);
                *ptr++ = point[0];
                *ptr++ = point[1];
                *ptr++ = point[2];
                if (idx >= 0) {
                    *ptr++ = idx;
                    *ptr++ = vidx;
                    *ptr++ = step2 + offset;
                }
            }
        }
    }        //krzywa
    else if (type == 1) {
        kierunek = 1;
        if (angle > 0) kierunek = -1;
        float astep = -0.05;
        if (step != 0)
            astep = -step / radius;
        float aa = astep*kierunek;
        float point1[3];
        float point2[3];
        for (float angle2 = angle * kierunek; angle2 < 0; angle2 -= astep) {
            if (angle2 > astep) aa = angle2 * kierunek;

            Vector2f a(0, 0);
            a.rotate(aa, radius);

            point1[0] = 0;
            point1[1] = height;
            point1[2] = 0;
            point2[0] = kierunek * a.x;
            point2[1] = height;
            point2[2] = kierunek * a.y;

            Vec3::transformMat4(point1, point1, matrix);
            Vec3::transformMat4(point2, point2, matrix);
            *ptr++ = point1[0];
            *ptr++ = point1[1];
            *ptr++ = point1[2];
            if (idx >= 0) {
                *ptr++ = idx;
                *ptr++ = vidx;
                *ptr++ = fabs(radius * (angle2 - angle * kierunek)) + offset;
            }
            *ptr++ = point2[0];
            *ptr++ = point2[1];
            *ptr++ = point2[2];
            if (idx >= 0) {
                float d = ((angle2 - astep) - angle * kierunek);
                if (d > -angle * kierunek) d = -angle * kierunek;
                *ptr++ = idx;
                *ptr++ = vidx;
                *ptr++ = fabs(radius * d) + offset;
            }
            Mat4::translate(matrix, matrix, kierunek * a.x, 0, kierunek * a.y);
            Mat4::rotateY(matrix, matrix, -aa);
        }
    }
}

void TSection::getPoints(float* &ptr, float* matrix) {
    float kierunek;
    if (type == 0) {

        //float dlugosc = getDlugosc();

        float point1[3];
        for (int i = 0; i < size; i += 4) {
            point1[0] = 0;
            point1[1] = 0;
            point1[2] = i;
            Vec3::transformMat4(point1, point1, matrix);
            *ptr++ = point1[0];
            *ptr++ = point1[1];
            *ptr++ = point1[2];
        }
    }        //krzywa
    else if (type == 1) {
        kierunek = 1;
        if (angle > 0) kierunek = -1;
        float point1[3];
        float dlugosc = getDlugosc() / 4;
        float step = fabs(angle) / dlugosc;

        float aa = -step*kierunek;
        for (float angle2 = angle * kierunek; angle2 < 0; angle2 += step) {

            Vector2f a(0, 0);
            a.rotate(aa, radius);

            point1[0] = 0;
            point1[1] = 0;
            point1[2] = 0;
            //point2[0] = kierunek*a.x; point2[1] = 2; point2[2] = kierunek*a.y;

            Vec3::transformMat4(point1, point1, matrix);
            *ptr++ = point1[0];
            *ptr++ = point1[1];
            *ptr++ = point1[2];
            Mat4::translate(matrix, matrix, kierunek * a.x, 0, kierunek * a.y);
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

    if (metry > this->getDlugosc())
        metry = this->getDlugosc();
    //prosta
    if (type == 0) {
        return (new Vector3f(0, 0, metry));
    }        //krzywa
    else if (type == 1) {
        float kierunek = 1;
        if (angle > 0) kierunek = -1;

        float aa = -metry / radius*kierunek;

        Vector2f a;
        a.rotate(aa, radius);

        return (new Vector3f(kierunek * a.x, 0, kierunek * a.y));
    }
    return new Vector3f();
}

float TSection::getDrawAngle(float metry) {

    if (metry > this->getDlugosc())
        metry = this->getDlugosc();
    //prosta
    if (type == 0) {
        return 0;
    }        //krzywa
    else if (type == 1) {
        float kierunek = 1;
        if (angle > 0) kierunek = -1;
        return -metry / radius*kierunek;
    }
    return 0;
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

