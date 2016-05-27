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
    void addBegButtonSelected();
    void addCurButtonSelected();
    void addEndButtonSelected();
    void addRndButtonSelected();
    void fs(QString n);
    bool eventFilter(QObject *obj, QEvent *event);
    
signals:
    void engListSelected(int id);
    void addToConSelected(int id, int pos, int count);
    
private:
    void addBegButtonSelected(int count);
    void addCurButtonSelected(int count);
    void addEndButtonSelected(int count);

    QListWidget items;
    QComboBox engType;
    QComboBox couplingType;
    QPushButton addBegButton;
    QPushButton addEndButton;
    QPushButton addCurButton;
    QPushButton addRandButton;
    QLineEdit totalVal;
    QLineEdit addNum;
    QLineEdit searchBox;
};

#endif	/* ENGLISTWIDGET_H */

