#ifndef TERRAINWATERWINDOW_H
#define	TERRAINWATERWINDOW_H

#include <QtWidgets>

class OverwriteDialog : public QDialog {
    Q_OBJECT
public:
    int changed = 0;
    OverwriteDialog();
    virtual ~OverwriteDialog();
    QLineEdit name;
    
    
public slots:
    //void trynew();
    void cancel();
    void ok();
    
private:

};

#endif	/* TERRAINWATERWINDOW_H */

