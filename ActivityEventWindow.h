/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYEVENTWINDOW_H
#define	ACTIVITYEVENTWINDOW_H

#include <QtWidgets>

class ActivityEventActionProperties;
class ActivityEventLocationProperties;
class ActivityEventTimeProperties;
class Activity;

class ActivityEventWindow : public QWidget {
    Q_OBJECT
public:
    ActivityEventWindow(QWidget* parent);
    virtual ~ActivityEventWindow();
    
public slots:
    void showEvents(Activity* act);
    void actionListSelected(QListWidgetItem * item);
        
signals:
    
        
private:
    QWidget actionWidget;
    QWidget locationWidget;
    QWidget timeWidget;
    
    QListWidget actionList;
    QListWidget locationList;
    QListWidget timeList;
    
    ActivityEventActionProperties *eventActionProperties;
    ActivityEventLocationProperties *eventLocationProperties;
    ActivityEventTimeProperties *eventTimeProperties;
    
    Activity *activity;
};

#endif	/* ACTIVITYEVENTWINDOW_H */

