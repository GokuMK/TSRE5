#include "Flex.h"
#include <QtWidgets>
#include "Game.h"
#include <QDebug>
#include "Vector2f.h"
#include "Vector3f.h"
#include "Game.h"
#include "TDB.h"
#include "Intersections.h"
#include "GLMatrix.h"

int Flex::FlexStage = 0;
float Flex::FlexP0[3];
float Flex::FlexQ0[4];
int Flex::FlexX;
int Flex::FlexZ;
QWidget* Flex::window;
int Flex::windowInit = 0;
int Flex::offx = 0;
int Flex::offy = 0;
QPainter* Flex::painter;
QImage* Flex::img;
QLabel* Flex::myLabel;

bool Flex::AutoFlex(int x1, int z1, float* p1, int x2, int z2, float* p2, float* dyntrackSections){
    float qe[4];
    qe[0] = 0;
    qe[1] = 0;
    qe[2] = 0;
    qe[3] = 1;
    TDB* tdb = Game::trackDB;
    bool success;
    qDebug() <<"flex "<< x1 << " " << z1 << " " << p1[0] << " " << p1[1] << " " << p1[2];
    qDebug() <<"flex "<< x2 << " " << z2 << " " << p2[0] << " " << p2[1] << " " << p2[2];
    
    tdb->findNearestNode(x1, z1, p1,(float*) &qe);
    success = Flex::NewFlex(x1, z1, p1, (float*)qe, dyntrackSections);
    qe[0] = 0;
    qe[1] = 0;
    qe[2] = 0;
    qe[3] = 1;
    tdb->findNearestNode(x2, z2, p2,(float*) &qe);
    success = Flex::NewFlex(x2, z2, p2, (float*)qe, dyntrackSections);
    
    return success;
}

bool Flex::NewFlex(int x, int z, float* p, float* q, float * dyntrackSections){
    
    if(FlexStage == 0){
        FlexP0[0] = p[0];
        FlexP0[1] = p[1];
        FlexP0[2] = p[2];
        FlexQ0[0] = q[0];
        FlexQ0[1] = q[1];
        FlexQ0[2] = q[2];
        FlexQ0[3] = q[3];
        FlexX = x;
        FlexZ = z;
        FlexStage = 1;
        return false;
    }

    if(windowInit == 0){
        window = new QWidget();
        windowInit = 1;
        window->setFixedSize(800, 800);
        window->show();
        img = new QImage(800, 800, QImage::Format_RGBA8888);
        painter = new QPainter();
        
        //QImage* myImage = new QImage();
        //myImage->load("resources/load.png");
        myLabel = new QLabel("");
        //myLabel->setContentsMargins(0,0,0,0);
        myLabel->setPixmap(QPixmap::fromImage(*img));
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(myLabel);
        window->setLayout(mainLayout);
        window->show();
        //drawLine(0, 0, 200,200);
    }
    img->fill(0);
    QPen niebieski(QColor(50,50,255));
    QPen czerwony(QColor(255,50,50));
    
    
    qDebug() << "point 1: "<< FlexP0[0] << " " << FlexP0[1] << " " << FlexP0[2] << "=" << FlexQ0[0] << " " << FlexQ0[1] << " " << FlexQ0[2];
    qDebug() << "point 2: "<< p[0] << " " << p[1] << " " << p[2] << "="<< q[0] << " " << q[1] << " " << q[2];
    
    Vector2f v1;
    Vector2f v2;
    Vector2f p1;
    Vector2f p2;

    p[2] = -p[2];
    FlexP0[2] = -FlexP0[2];
    
    v1.x = sin(FlexQ0[1])*1;
    v1.y = cos(FlexQ0[1])*1;
    v2.x = sin(q[1])*1;
    v2.y = cos(q[1])*1;
    p1.x = FlexP0[0];
    p1.y = FlexP0[2];
    p2.x = p[0];
    p2.y = p[2];
    
    //p2.x -= p1.x;
    //p2.y -= p1.y;
    //p1.x = 0; p1.y = 0;
    //float distance = p2.getDlugosc();
    //p2.rotate(FlexQ0[1], 0);
    //v2.rotate(FlexQ0[1], 0);
    //v1.x = 0;
    //v1.y = 1;
    offx = (p1.x + p2.x)/2;
    offy = (p1.y + p2.y)/2;
    qDebug() << offx <<" "<<offy;
    
    qDebug() << p1.x << " = "<< p1.y;
    qDebug() << p2.x << " = "<< p2.y;
    qDebug() << v1.x << " = "<< v1.y;
    qDebug() << v2.x << " = "<< v2.y;

    drawLine(niebieski, p1.x, p1.y, p1.x+v1.x*1000,p1.y+v1.y*1000);
    drawLine(niebieski, p2.x, p2.y, p2.x+v2.x*1000,p2.y+v2.y*1000);
    
    // if prosta
    float dp1[3], dp2[3];
    Vec3::set((float*)dp1, p1.x, 0, p1.y);
    Vec3::set((float*)dp2, p1.x+v1.x*1000, 0, p1.y+v1.y*1000);
    
    int distance = Intersections::pointSegmentDistance((float*)dp1,(float*)dp2, p, NULL);
    if(distance < 0.001){
        qDebug() <<"prosta";
        for(int i = 0; i < 10; i++){
            dyntrackSections[i] = 0;
        }
        dyntrackSections[0] = Vec3::len(Vec3::sub(FlexP0, FlexP0, p));
        FlexStage = 0;
        return true;
    }
    //Vector2f line(p2);
    //line.subv(p1);
    
    //float s, t;
    float t1 = ( v1.y * (p1.x - p2.x) + v1.x * (p1.y - p2.y)) / (-v2.x * v1.y + v1.x * v2.y);
    float t2 = ( v2.x * (p1.y - p2.y) - v2.y * (p1.x - p2.x)) / (-v2.x * v1.y + v1.x * v2.y);
    //float t1 = line.cross(line,v2) / v1.cross(v1, v2);
    qDebug() << t1;
    //float t2 = line.cross(line,v1) / v1.cross(v1, v2);
    qDebug() << t2;
    
    Vector2f srodek1;
    Vector2f srodek2;
    srodek1.x = p1.x + v1.x*t2;
    srodek1.y = p1.y + v1.y*t2;
    srodek2.x = p2.x + v2.x*t1;
    srodek2.y = p2.y + v2.y*t1;
    
    qDebug() << srodek1.x << " = "<< srodek1.y;
    qDebug() << srodek2.x << " = "<< srodek2.y;
    
    //if(t1 < 0 || t2 < 0){
    //    qDebug() << "flex fail";
    //    FlexStage = 0;
    //    return;
    //}
    Vector2f line1(p1.x, p1.y);
    line1.x-=srodek1.x;
    line1.y-=srodek1.y;
    Vector2f line2(p2.x, p2.y);
    line2.x-=srodek1.x;
    line2.y-=srodek1.y;
    float dlugosc1 = line1.getDlugosc();
    float dlugosc2 = line2.getDlugosc();
    float dlugosc = dlugosc1;
    if(dlugosc2 < dlugosc1)
        dlugosc = dlugosc2;
    
    //Vector punkt
    Vector2f kp1, kp2;
    kp1.x = srodek1.x - v1.x*dlugosc;
    kp1.y = srodek1.y - v1.y*dlugosc;
    kp2.x = srodek1.x - v2.x*dlugosc;
    kp2.y = srodek1.y - v2.y*dlugosc;
    Vector2f kc;
    kc.x = kp2.x - kp1.x;
    kc.y = kp2.y - kp1.y;
    float length = kc.getDlugosc();
    //Vector2f ks;
    //ks.x = (kp1.x + kc.x/2) - srodek1.x;
    //ks.y = (kp1.y + kc.y/2) - srodek1.y;
    //float height = ks.getDlugosc();
    

    //float radius = height/2 + (length*length)/(8*height);
    //drawLine(czerwony, p2.x, p2.y, srodek1.x,srodek1.y);
    //drawLine(czerwony, p1.x, p1.y, srodek1.x,srodek1.y);
    //drawLine(czerwony, kp1.x, kp1.y, srodek1.x,srodek1.y);
    //drawLine(czerwony, kp2.x, kp2.y, srodek1.x,srodek1.y);
    drawLine(czerwony, kp1.x, kp1.y, kp2.x, kp2.y);
    
    float angle = (q[1] - FlexQ0[1]);
    int sign = -1;
    if(angle < 0) sign = 1;
    angle = M_PI - fabs(angle);
    //if(angle > M_PI) angle -= M_PI;
    angle*=sign;
    
    float radius = (length/2.0)/sin(fabs(angle/2.0));
    
    qDebug() << "angle " << angle*180/M_PI; 
    qDebug() << "dlugosc " << dlugosc; 
    qDebug() << "length " << length; 
    qDebug() << "radius " << radius; 
    
    ////////////////////////////////////////////////////////////////////////
    dlugosc1 = fabs(dlugosc1 - dlugosc);
    dlugosc2 = fabs(dlugosc2 - dlugosc);
    
    if(dlugosc1 > 0.01 && dlugosc1 < 1000){
        dyntrackSections[0] = dlugosc1;
        dyntrackSections[1] = 0;
    } else {
        dyntrackSections[0] = 0;
    }
    dyntrackSections[2] = angle;
    dyntrackSections[3] = radius;
    if(dlugosc2 > 0.01 && dlugosc2 < 1000){
        dyntrackSections[4] = dlugosc2;
        dyntrackSections[5] = 0;
    } else {
        dyntrackSections[4] = 0;
        dyntrackSections[5] = 0;
    }
    dyntrackSections[6] = 0;
    dyntrackSections[8] = 0;
    myLabel->setPixmap(QPixmap::fromImage(*img));
    FlexStage = 0;
    
    p[0] = FlexP0[0];
    p[1] = FlexP0[1];
    p[2] = -FlexP0[2];
    q[0] = FlexQ0[0];
    q[1] = FlexQ0[1];
    q[2] = FlexQ0[2];
    q[3] = FlexQ0[3];
    return true;
}

void Flex::drawLine(QPen niebieski, int x1, int y1, int x2, int y2){
        int off = 400;
        int start = 800;
        //x1 /= 4;
        //x2 /= 4;
        //y1 /= 4;
        //y2 /= 4;
        
        painter->begin(img);
        painter->setRenderHint(QPainter::RenderHint::Antialiasing, false);
        painter->setPen(niebieski); 
        painter->drawLine((x1-offx+off),start-(y1-offy+off),(x2-offx+off),start-(y2-offy+off));
        painter->end();
}