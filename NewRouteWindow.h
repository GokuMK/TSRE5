/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef NEWROUTEWINDOW_H
#define	NEWROUTEWINDOW_H

#include <QtWidgets>

class NewRouteWindow  : public QDialog {
    Q_OBJECT
public:
    NewRouteWindow();
    virtual ~NewRouteWindow();
    QLineEdit name;
    QLineEdit lat;
    QLineEdit lon;
    bool changed = false;
    
public slots:
    void cancel();
    void ok();

};

#endif	/* NEWROUTEWINDOW_H */

