/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ENGINFOWIDGET_H
#define	ENGINFOWIDGET_H

#include <QWidget>
#include <QtWidgets>

class Eng;

class EngInfoWidget : public QWidget {
    Q_OBJECT
public:
    EngInfoWidget(QWidget* parent);
    virtual ~EngInfoWidget();
    void setInfo(Eng *eng);
    
public slots:
    
signals:
    
private:
    QLineEdit eName;
    QLineEdit eFileName;
    QLineEdit eDirName;
    QLineEdit eType;
    QLineEdit eBrakes;
    QLineEdit eCouplings;
    QLineEdit eMass;
    QLineEdit eMaxSpeed;
    QLineEdit eMaxForce;
    QLineEdit eMaxPower;
    QLineEdit eShape;
    QLineEdit eSize;
};

#endif	/* ENGINFOWIDGET_H */

