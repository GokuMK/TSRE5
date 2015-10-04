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
    void refListSelected(QListWidgetItem * item);
    void trackListSelected(QListWidgetItem * item);
    void lastItemsListSelected(QListWidgetItem * item);
    void selectToolEnabled();
    void placeToolEnabled();
    void itemSelected(Ref::RefItem* item);
signals:
    void enableTool(QString name);
    
private:
    Route* route;
    QListWidget refList;
    QListWidget trackList;
    QListWidget lastItems;
    QComboBox refClass;
    QComboBox refTrack;
    QComboBox refRoad;
    Ref::RefItem itemRef;
    std::deque<Ref::RefItem*> lastItemsPtr;
};

#endif	/* TOOLBOX_H */

