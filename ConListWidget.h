#ifndef CONLISTWIDGET_H
#define	CONLISTWIDGET_H

#include <QWidget>
#include <QtWidgets>

class ConListWidget : public QWidget {
    Q_OBJECT
public:
    ConListWidget();
    virtual ~ConListWidget();
    void fillConList();
    void newConsist();
    
public slots:
    void itemsSelected(QListWidgetItem* it);
        
signals:
    void conListSelected(int id);
        
private:
    QListWidget items;
    QComboBox conType;
};
#endif	/* CONLISTWIDGET_H */

