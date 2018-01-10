/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYACTIVITYSELECTWINDOW_H
#define	PLAYACTIVITYSELECTWINDOW_H

#include <QtWidgets>

class Route;

class PlayActivitySelectWindow : public QDialog {
    Q_OBJECT
public:
    PlayActivitySelectWindow();
    virtual ~PlayActivitySelectWindow();
    int actId = -1;
    void setRoute(Route *r);
    
public slots:
    void okButtonEnabled();
    void activitySelected(QString val);
    
private:
    QPlainTextEdit eDescription;
    QComboBox actList;
    Route *route = NULL;
};

#endif	/* PLAYACTIVITYSELECTWINDOW_H */

