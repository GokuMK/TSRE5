/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYSERVICEPROPERTIES_H
#define	ACTIVITYSERVICEPROPERTIES_H

#include <QtWidgets>
#include <QString>

class Service;
class Path;

class ActivityServiceProperties : public QWidget {
    Q_OBJECT
public:
    ActivityServiceProperties(QWidget* parent);
    virtual ~ActivityServiceProperties();
    
    void showService(Service *s);
    void setPaths(QVector<Path*> &path);
public slots:
    void cPathEnabled(int val);
    void cConFilesEnabled(int val);
    void eDisplayNameEnabled(QString val);
    void ePlayerPerformanceEnabled(QString val);
    void eStartSpeedEnabled(QString val);
    void eEndSpeedEnabled(QString val);
    void stationListSelected(QTreeWidgetItem* item, int column);
signals:

private:
    QLineEdit eFileName;
    QLineEdit eDisplayName;
    QLineEdit ePlayerPerformance;
    QLineEdit eStartSpeed;
    QLineEdit eEndSpeed;
    QComboBox cConFiles;
    QComboBox cPath;
    QTreeWidget stationList;
    QTreeWidget lUsedBy;
    
    Service *service;
    QVector<Path*> paths;
    
    void setStationList();
    void setUsedByList();
};

#endif	/* ACTIVITYSERVICEPROPERTIES_H */

