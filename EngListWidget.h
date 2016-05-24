#ifndef ENGLISTWIDGET_H
#define	ENGLISTWIDGET_H

#include <QWidget>
#include <QtWidgets>

class EngLib;

class EngListWidget : public QWidget {
    Q_OBJECT
public:
    EngListWidget();
    virtual ~EngListWidget();
    void fillEngList();
    void fillEngList(QString engFilter, QString couplingFilter, QString searchFilter);
    EngLib* englib;
    
public slots:
    void itemsSelected();
    void addButtonSelected();
    void addButtonSelected(int count);
    void filterSelected(QString n);
    bool eventFilter(QObject *obj, QEvent *event);
    
signals:
    void engListSelected(int id);
    void addToConSelected(int id, int count);
    
private:
    QListWidget items;
    QComboBox engType;
    QComboBox couplingType;
    QPushButton addButton;
    QLineEdit addNum;
    QLineEdit searchBox;
};

#endif	/* ENGLISTWIDGET_H */

