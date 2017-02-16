/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */
#include "ShapeViewWindow.h"
#include <QDebug>
#include "Game.h"
#include "glShapeWidget.h"
#include "CameraRot.h"

ShapeViewWindow::ShapeViewWindow(QWidget* parent) : QWidget(parent) {
    this->setWindowFlags(Qt::WindowType::Tool);
    //this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle(tr("Shape Preview"));
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->resize(384,256);
    glShapeWidget = new GlShapeWidget(parent);
    if(Game::colorShapeView != NULL)
        glShapeWidget->setBackgroundGlColor(Game::colorShapeView->redF(), Game::colorShapeView->greenF(), Game::colorShapeView->blueF());
    camera = new CameraRot();
    camera->setPos(0,0,0);
    camera->setPlayerRot(M_PI/2.0,0);
    glShapeWidget->setCamera(camera);
    glShapeWidget->setMode("rot");
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glShapeWidget);

    //glShapeWidget->setMinimumSize(100, 100);

    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
}

ShapeViewWindow::~ShapeViewWindow() {
}

void ShapeViewWindow::msg(QString text){
}

void ShapeViewWindow::msg(QString text, bool val){
}

void ShapeViewWindow::msg(QString text, int val){
}

void ShapeViewWindow::msg(QString text, float val){
}

void ShapeViewWindow::msg(QString text, QString val){
    //qDebug() << text;
    if(text == "showShape"){
        qDebug() << val;
        if(val.length() < 1) return;
        if(!this->isVisible()) return;
        QStringList values = val.split("|");
        if(values.length() == 1)
            glShapeWidget->showShape((QString)values[0], "");
        if(values.length() == 2)
            glShapeWidget->showShape((QString)values[0], (QString)values[1]);
        return;
    }
}

void ShapeViewWindow::hideEvent(QHideEvent *e){
    emit windowClosed();
}
