/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYTIMETABLEWINDOW_H
#define	ACTIVITYTIMETABLEWINDOW_H

#include <QWidget>
#include <QtWidgets>

class Route;
class ActivityTimetable;
class Service;
class ActivityServiceDefinition;
class ActivityTimetableProperties;

class ActivityTimetableWindow : public QWidget {
    Q_OBJECT
public:
    ActivityTimetableWindow(QWidget* parent);
    virtual ~ActivityTimetableWindow();
    ActivityTimetableProperties *timetableProperties;
    
public slots:
    void showTimetable(QVector<ActivityServiceDefinition*> s);
    void listSelected(QListWidgetItem *item);
    
signals:
    
private:
    QListWidget list;
    //Route *route = NULL;
    QVector<ActivityServiceDefinition*> services;
};

#endif	/* ACTIVITYTIMETABLEWINDOW_H */

