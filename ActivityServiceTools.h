/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYSERVICETOOLS_H
#define	ACTIVITYSERVICETOOLS_H

#include <QtWidgets>
#include <QString>

class Service;
class Path;

class ActivityServiceTools : public QDialog {
    Q_OBJECT
public:
    bool changed = false;
    ActivityServiceTools();
    virtual ~ActivityServiceTools();
    
    void setData(Service *s, QVector<Path*> &path);
public slots:
    void bOkEnabled();
    void bCancelEnabled();
    
signals:

private:
    QLineEdit eFileName;
    QLineEdit eDisplayName;
    QLineEdit ePlayerPerformance;
    QLineEdit eStartSpeed;
    QLineEdit eEndSpeed;
    QComboBox cConFiles;
    QComboBox cPath;
    
    Service *service;
};

#endif	/* ACTIVITYSERVICETOOLS_H */

