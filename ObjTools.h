#ifndef TOOLBOX_H
#define	TOOLBOX_H

#include <QtWidgets>
#include "Route.h"
#include "Ref.h"

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
    void itemSelected(int pointer);
signals:
    void enableTool(QString name);
    
private:
    Route* route;
    QListWidget refList;
    QListWidget trackList;
    QListWidget lastItems;
    QComboBox refClass;
    QComboBox refTrack;
    Ref::RefItem itemRef;
};

#endif	/* TOOLBOX_H */

