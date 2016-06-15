#ifndef ACTIVITYTOOLS_H
#define	ACTIVITYTOOLS_H

#include <QtWidgets>
#include <QString>

class Route;

class ActivityTools : public QWidget{
    Q_OBJECT
public:
    ActivityTools(QString name);
    virtual ~ActivityTools();

public slots:
    void loadActFiles();
    void fillConListAct(QString n);
    void routeLoaded(Route* r);
    
signals:    

private:
    Route *route = NULL;
    QListWidget consists;
    QComboBox actShow;
};

#endif	/* ACTIVITYTOOLS_H */

