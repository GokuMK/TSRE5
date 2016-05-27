#ifndef UNSAVEDDIALOG_H
#define	UNSAVEDDIALOG_H

#include <QtWidgets>

class UnsavedDialog : public QDialog {
    Q_OBJECT
public:
    UnsavedDialog();
    virtual ~UnsavedDialog();
    QListWidget items;
    int changed = 0;
    
public slots:
    void ok();
    void cancel();
    void exit();
    
private:

};

#endif	/* UNSAVEDDIALOG_H */

