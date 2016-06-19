#ifndef TRKWINDOW_H
#define	TRKWINDOW_H

#include <QtWidgets>

class Trk;

class TrkWindow : public QDialog {
    Q_OBJECT
public:
    TrkWindow();
    virtual ~TrkWindow();
    Trk *trk;
    
    int exec();
    
private:
    QLabel* imageGraphic;
    QLabel* imageLoad;
};

#endif	/* TRKWINDOW_H */

