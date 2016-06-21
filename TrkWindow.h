#ifndef TRKWINDOW_H
#define	TRKWINDOW_H

#include <QtWidgets>

class Trk;

class TrkWindow : public QDialog {
    Q_OBJECT
public:
    TrkWindow();
    virtual ~TrkWindow();
    Trk *trk = NULL;
    
    int exec();
    
signals:    
    
public slots:
    void bokEnabled();
    void bcancelEnabled();
    
private:
    QLabel imageGraphic;
    QLabel imageLoad;
    
    QLineEdit idName;
    QLineEdit displayName;
    QPlainTextEdit description;
    QLineEdit routeName;
    QComboBox electrified;
    //d int mountains;
    QDoubleSpinBox overheadWireHeight;
    QDoubleSpinBox speedLimit;
    QDoubleSpinBox terrainErrorScale;
    QLineEdit startTileX;
    QLineEdit startTileZ;
    QLineEdit startpX;
    QLineEdit startpZ;
    QComboBox milepostUnitsKilometers;
    QDoubleSpinBox maxLineVoltage;
    QDoubleSpinBox tempRestrictedSpeed;
    QComboBox envName;
    QLineEdit envValue;
    QComboBox iList;
    QPushButton iCopy;
    QPushButton iPaste;
};

#endif	/* TRKWINDOW_H */

