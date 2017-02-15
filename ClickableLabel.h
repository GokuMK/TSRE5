/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CLICKABLELABEL_H
#define	CLICKABLELABEL_H

#include <QtWidgets>

class ClickableLabel : public QLabel
{
   Q_OBJECT
public:
   ClickableLabel(QString text, QWidget *parent = 0) : QLabel(text, parent) {}
   ~ClickableLabel() {}

signals:
   void clicked();

protected:
   void mousePressEvent(QMouseEvent *event) { emit clicked(); }
};

#endif	/* CLICKABLELABEL_H */

