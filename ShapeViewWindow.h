/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */
#ifndef SHAPEVIEWWINDOW_H
#define	SHAPEVIEWWINDOW_H

#include <QtWidgets>

class GlShapeWidget;
class QScrollBar;
class Camera;
class WorldObj;
class Window;

class ShapeViewWindow : public QWidget {
    Q_OBJECT
public:
    ShapeViewWindow(QWidget *parent = 0);
    virtual ~ShapeViewWindow();
    
public slots:
    void msg(QString text);
    void msg(QString name, bool val);
    void msg(QString name, int val);
    void msg(QString name, float val);
    void msg(QString name, QString val);
    
signals:

private:
    GlShapeWidget *glShapeWidget;
    Camera *camera;
};

#endif	/* SHAPEVIEWWINDOW_H */

