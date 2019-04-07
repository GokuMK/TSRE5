/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPEINFOWIDGET_H
#define	SHAPEINFOWIDGET_H

#include <QWidget>
#include <QtWidgets>

class ShapeInfoWidget : public QWidget {
    Q_OBJECT
public:
    ShapeInfoWidget(QWidget* parent);
    virtual ~ShapeInfoWidget();
private:
    QLineEdit sName;
    QLineEdit sType;
    QLineEdit sSdFile;
    QLineEdit sPolyCount;
    QLineEdit sDimensionsSd;
    QLineEdit sDimensionsCalc;
};

#endif	/* SHAPEINFOWIDGET_H */

