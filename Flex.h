/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
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

    static bool NewFlex(int x, int z, float *p, float *q, float * dyntrackSections);
    static bool AutoFlex(int x1, int z1, float* p1, int x2, int z2, float* p2, float* dyntrackSections, float &elev);
private:
    static int FlexStage;
    static float FlexP0[3];
    static float FlexQ0[4];
    static int FlexX;
    static int FlexZ;
    static QWidget* window;
    static QPainter* painter;
    static QImage* img;
    static QLabel* myLabel;
    static int windowInit;
    static int offx, offy;
    static void drawLine(QPen niebieski, int x1, int y1, int x2, int y2);
};

#endif	/* FLEX_H */

