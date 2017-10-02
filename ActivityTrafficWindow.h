/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYTRAFFICWINDOW_H
#define	ACTIVITYTRAFFICWINDOW_H

#include <QWidget>
#include <QtWidgets>

class Route;
class Traffic;
class Service;
class ActivityTrafficProperties;

class ActivityTrafficWindow : public QWidget {
    Q_OBJECT
public:
    ActivityTrafficWindow(QWidget* parent);
    virtual ~ActivityTrafficWindow();
    ActivityTrafficProperties* trafficProperties = NULL;
    
public slots:
    void lTrafficSelected(QTreeWidgetItem * item, int column);
    void showTraffic(Route* r);
    void bNewTrafficSelected();
    
private:
    Route *route = NULL;
    QTreeWidget lTraffic;
};

#endif	/* ACTIVITYTRAFFICWINDOW_H */

