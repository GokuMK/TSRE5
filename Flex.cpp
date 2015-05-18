
#include "Flex.h"
#include <QtWidgets>
#include "Game.h"
#include <QDebug>
#include "Vector2f.h"
#include "Vector3f.h"

int Flex::FlexStage = 0;
float Flex::FlexP0[3];
float Flex::FlexQ0[3];
int Flex::FlexX;
int Flex::FlexZ;
QWidget* Flex::window;
int Flex::windowInit = 0;
QPainter* Flex::painter;
QImage* Flex::img;
QLabel* Flex::myLabel;

void Flex::NewFlex(int x, int z, float* p, float* q){
    if(FlexStage == 0){
        FlexP0[0] = p[0];
        FlexP0[1] = p[1];
        FlexP0[2] = p[2];
        FlexQ0[0] = q[0];
        FlexQ0[1] = q[1];
        FlexQ0[2] = q[2];
        FlexX = x;
        FlexZ = z;
        FlexStage = 1;
        return;
    }

    if(windowInit == 0){
        window = new QWidget();
        windowInit = 1;
        window->setFixedSize(600, 600);
        window->show();
        img = new QImage(600, 600, QImage::Format_RGBA8888);
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
    
    v1.x = sin(FlexQ0[1])*10;
    v1.y = cos(FlexQ0[1])*10;
    v2.x = sin(q[1])*10;
    v2.y = cos(q[1])*10;
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
    
    qDebug() << p1.x << " = "<< p1.y;
    qDebug() << p2.x << " = "<< p2.y;
    qDebug() << v1.x << " = "<< v1.y;
    qDebug() << v2.x << " = "<< v2.y;

    drawLine(niebieski, p1.x, p1.y, p1.x+v1.x*100,p1.y+v1.y*100);
    drawLine(niebieski, p2.x, p2.y, p2.x+v2.x*100,p2.y+v2.y*100);
    
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
    
    drawLine(czerwony, p1.x, p1.y, srodek1.x,srodek1.y);
    //drawLine(czerwony, p2.x, p2.y, srodek2.x,srodek2.y);
    //if(t1 < 0 || t2 < 0){
    //    qDebug() << "flex fail";
    //    FlexStage = 0;
    //    return;
    //}
    
    float angle = (q[1] - FlexQ0[1]);
    qDebug() << "angle " << angle*180/M_PI; 
    
    myLabel->setPixmap(QPixmap::fromImage(*img));
    FlexStage = 0;
}

void Flex::drawLine(QPen niebieski, int x1, int y1, int x2, int y2){
        int off = 300;
        int start = 600;
        painter->begin(img);
        painter->setRenderHint(QPainter::RenderHint::Antialiasing, false);
        painter->setPen(niebieski); 
        painter->drawLine((x1+off),start-(y1+off),(x2+off),start-(y2+off));
        painter->end();
}