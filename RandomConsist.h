#ifndef RANDOMCONSIST_H
#define	RANDOMCONSIST_H

#include <QtWidgets>

class RandomConsist : public QWidget {
    Q_OBJECT
public:
    RandomConsist();
    virtual ~RandomConsist();
    QListWidget items;
    int changed = 0;
    
public slots:
    void ok();
    void cancel();
    
signals:
    void addToConSelected(int id, int pos, int count);
    
private:
    QLineEdit number;
};

#endif	/* RANDOMCONSIST_H */

