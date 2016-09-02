/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TOOLBOX_H
#define	TOOLBOX_H

#include <QtWidgets>
#include "Route.h"
#include "Ref.h"
#include <deque>

class ObjTools : public QWidget{
    Q_OBJECT

public:
    ObjTools(QString name);
    virtual ~ObjTools();
    
public slots:
    void routeLoaded(Route * a);
    void refClassSelected(const QString & text);
    void refTrackSelected(const QString & text);
    void refOtherSelected(const QString & text);
    void refListSelected(QListWidgetItem * item);
    void trackListSelected(QListWidgetItem * item);
    void otherListSelected(QListWidgetItem * item);
    void lastItemsListSelected(QListWidgetItem * item);
    void selectToolEnabled();
    void placeToolEnabled();
    void itemSelected(Ref::RefItem* item);
    void stickToTDBEnabled(int state);
    void resetRotationButtonEnabled();
    
    void msg(QString name);
    void msg(QString name, bool val);
    void msg(QString name, int val);
    void msg(QString name, float val);
    void msg(QString name, QString val);
signals:
    void enableTool(QString name);

    void sendMsg(QString name);
    void sendMsg(QString name, bool val);
    void sendMsg(QString name, int val);
    void sendMsg(QString name, float val);
    void sendMsg(QString name, QString val);
    
private:
    Route* route;
    QListWidget refList;
    QListWidget trackList;
    QListWidget otherList;
    QListWidget lastItems;
    QComboBox refClass;
    QComboBox refTrack;
    QComboBox refRoad;
    QComboBox refOther;
    Ref::RefItem itemRef;
    std::deque<Ref::RefItem*> lastItemsPtr;
    QCheckBox stickToTDB;
    QCheckBox stickToRDB;
    void hideAllLists();
};

#endif	/* TOOLBOX_H */

