#ifndef TERRAINWATERWINDOW_H
#define	TERRAINWATERWINDOW_H

#include <QtWidgets>

class TerrainWaterWindow  : public QDialog {
    Q_OBJECT
public:
    float WSW;
    float WSE;
    float WNE;
    float WNW;
    QLineEdit eWSW;
    QLineEdit eWSE;
    QLineEdit eWNE;
    QLineEdit eWNW;
    bool changed = false;
    void setWater();
    TerrainWaterWindow();
    virtual ~TerrainWaterWindow();
    
public slots:
    void cancel();
    void ok();
    
private:

};

#endif	/* TERRAINWATERWINDOW_H */

