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
    void naviInfo(int x, int z, int all, int hidden);

signals:
    void jumpTo(int x, int y);

private:
    QLineEdit xBox;
    QLineEdit yBox;
    QLabel tileInfo;
    
    int lastX = 0; 
    int lastZ = 0;
    int objCount = 0;
    int objHidden = 0;
};

#endif	/* NAVIBOX_H */

