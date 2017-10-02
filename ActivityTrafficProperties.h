/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYTRAFFICPROPERTIES_H
#define	ACTIVITYTRAFFICPROPERTIES_H

#include <QtWidgets>
#include <QString>

class Traffic;

class ActivityTrafficProperties : public QWidget {
    Q_OBJECT
public:
    ActivityTrafficProperties(QWidget* parent);
    virtual ~ActivityTrafficProperties();
    void showTraffic(Traffic* t);
public slots:
    void lServciesSelected(QTreeWidgetItem* item, int column);
    void bAddServiceSelected();
    void bRemoveServiceSelected();
    
signals:
    
private:
    Traffic *traffic = NULL;
    
    QLineEdit eFileName;
    QLineEdit eDisplayName;
    QTreeWidget lServcies;
    QComboBox cServiceList;
    QTimeEdit eTime;
    
    
};

#endif	/* ACTIVITYTRAFFICPROPERTIES_H */

