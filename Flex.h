/* 
 * File:   Flex.h
 * Author: Goku
 *
 * Created on 19 maja 2015, 00:06
 */

#ifndef FLEX_H
#define	FLEX_H

class QWidget;
class QPainter;
class QImage;
class QLabel;
class QPen;

class Flex {
public:

    static void NewFlex(int x, int z, float *p, float *q);
private:
    static int FlexStage;
    static float FlexP0[3];
    static float FlexQ0[3];
    static int FlexX;
    static int FlexZ;
    static QWidget* window;
    static QPainter* painter;
    static QImage* img;
    static QLabel* myLabel;
    static int windowInit;
    static void drawLine(QPen niebieski, int x1, int y1, int x2, int y2);
};

#endif	/* FLEX_H */

