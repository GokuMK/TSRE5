/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYSERVICEWINDOW_H
#define	ACTIVITYSERVICEWINDOW_H

#include <QWidget>
#include <QtWidgets>

class Route;
class Activity;
class Service;
class ActivityServiceProperties;

class ActivityServiceWindow : public QWidget {
    Q_OBJECT
public:
    ActivityServiceWindow(QWidget* parent);
    virtual ~ActivityServiceWindow();
    
    ActivityServiceProperties *serviceProperties = NULL;
    
public slots:
    void showServices(Route* route);
    void serviceListSelected(QTreeWidgetItem *item, int column);
    void serviceNameChanged(int id);
    void bNewServiceSelected();
    void bDeleteServiceSelected();
    
private:
    QTreeWidget serviceList;
    
    Activity *activity;
    Route *route;
};

#endif	/* ACTIVITYSERVICEWINDOW_H */

