#ifndef TOOLBOX_H
#define	TOOLBOX_H

#include <QtWidgets>
#include "Route.h"
#include "Ref.h"

class ToolBox : public QWidget{
    Q_OBJECT

public:
    ToolBox(QString name);
    virtual ~ToolBox();
    
public slots:
    void routeLoaded(Route * a);
    void refClassSelected(const QString & text);
    void refListSelected(QListWidgetItem * item);
    void trackListSelected(QListWidgetItem * item);
    void selectToolEnabled();
    void placeToolEnabled();
signals:
    void enableTool(QString name);
    
private:
    Route* route;
    QListWidget refList;
    QListWidget trackList;
    QComboBox refClass;
    Ref::RefItem trackRef;
};

#endif	/* TOOLBOX_H */

