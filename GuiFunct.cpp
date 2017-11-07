/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GuiFunct.h"
#include <QtWidgets>
#include "Game.h"

QLabel* GuiFunct::newQLabel(QString text, int width){
    QLabel* label = new QLabel(text);
    label->setFixedWidth(width);
    return label;
}

QLabel* GuiFunct::newTQLabel(QString text, int width){
    QLabel *l = new QLabel(text);
    l->setContentsMargins(3,0,0,0);
    l->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    if(width >=0)
        l->setMinimumWidth(width);
    return l;
}

QLineEdit* GuiFunct::newQLineEdit(int width, int length){
    QLineEdit* edit = new QLineEdit;
    edit->setFixedWidth(width);
    edit->setMaxLength(length);
    return edit;
}

QAction* GuiFunct::newMenuCheckAction(QString desc, QWidget* window, bool checked){
    QAction *action = new QAction(desc, window);
    action->setCheckable(true);
    action->setChecked(checked);
    return action;
}