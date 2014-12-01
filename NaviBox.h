#ifndef NAVIBOX_H
#define	NAVIBOX_H

#include <QtWidgets>

class NaviBox : public QWidget {
    Q_OBJECT   
public:
    NaviBox();
    virtual ~NaviBox();
    
public slots:
    void jumpTileSelected();  

signals:
    void jumpTo(int x, int y);
private:
    QLineEdit xBox;
    QLineEdit yBox;

};

#endif	/* NAVIBOX_H */

