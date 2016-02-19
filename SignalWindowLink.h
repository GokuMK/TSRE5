#ifndef SIGNALWINDOWLINK_H
#define	SIGNALWINDOWLINK_H

#include <QtWidgets>

class SignalObj;

class SignalWindowLink : public QDialog {
    Q_OBJECT

public:
    QLineEdit from;
    QLineEdit to;
    bool changed = false;
    SignalWindowLink();
    virtual ~SignalWindowLink();

    
public slots:
    void cancel();
    void ok();
    
private:

};

#endif	/* SIGNALWINDOW_H */

