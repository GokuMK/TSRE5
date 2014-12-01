#ifndef TOOLBOX_H
#define	TOOLBOX_H

#include <QtWidgets>
#include "Ref.h"

class ToolBox : public QWidget{
    Q_OBJECT

public:
    ToolBox(QString name);
    virtual ~ToolBox();
    
public slots:
    void routeLoaded(Ref * a);
    void refClassSelected(const QString & text);
    void refListSelected(QListWidgetItem * item);
    void selectToolEnabled();
    void placeToolEnabled();
signals:
    void enableTool(QString name);
    
private:
    Ref * ref;
    QListWidget refList;
    QComboBox refClass;
};

#endif	/* TOOLBOX_H */

