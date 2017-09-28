/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYTIMETABLEPROPERTIES_H
#define	ACTIVITYTIMETABLEPROPERTIES_H

#include <QtWidgets>
#include <QString>

class ActivityTimetableProperties : public QWidget {
    Q_OBJECT
public:
    ActivityTimetableProperties(QWidget* parent);
    virtual ~ActivityTimetableProperties();
private:
    QTreeWidget lTimetable;
};

#endif	/* ACTIVITYTIMETABLEPROPERTIES_H */

