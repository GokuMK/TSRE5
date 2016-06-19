#ifndef UNSAVEDDIALOG_H
#define	UNSAVEDDIALOG_H

#include <QtWidgets>

class UnsavedDialog : public QDialog {
    Q_OBJECT
public:
    UnsavedDialog();
    virtual ~UnsavedDialog();
    QListWidget items;
    void setMsg(QString msg);
    int changed = 0;
    
public slots:
    void ok();
    void cancel();
    void exit();
    
private:
    QLabel infoLabel;
};

#endif	/* UNSAVEDDIALOG_H */

