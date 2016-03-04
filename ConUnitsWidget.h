#ifndef CONUNITSWIDGET_H
#define	CONUNITSWIDGET_H

#include <QWidget>
#include <QtWidgets>

class Consist;
class EngLib;
class ConUnitsWidget : public QWidget {
    Q_OBJECT
public:
    ConUnitsWidget();
    virtual ~ConUnitsWidget();
    void setCon(Consist * c);
    Consist* con;
    EngLib* englib;

public slots:
    void itemsSelected(QListWidgetItem* it);
    
signals:
    void selected(int id);
        
private:
    QListWidget items;
    QLineEdit count;
};
#endif	/* CONUNITSWIDGET_H */

