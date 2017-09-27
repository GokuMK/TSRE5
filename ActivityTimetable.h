/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYTIMETABLE_H
#define	ACTIVITYTIMETABLE_H

#include <QString>
#include <QVector>

class FileBuffer;
class QTextStream;

class ActivityTimetable {
public:
    ActivityTimetable();
    ActivityTimetable(const ActivityTimetable& orig);
    virtual ~ActivityTimetable();
    int time = 0;
    QString name;
    bool actTimetable = false;
    QVector<int> arrivalTime;
    QVector<int> departTime;
    QVector<int> skipCount;
    QVector<float> distanceDownPath;
    QVector<int> platformStartID;
    void load(FileBuffer* data);
    void save(QTextStream* out, QString off = "");
private:

};

#endif	/* ACTIVITYTIMETABLE_H */

