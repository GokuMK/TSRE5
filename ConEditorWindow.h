#ifndef CONEDITORWINDOW_H
#define	CONEDITORWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>

class EngListWidget;
class ConListWidget;
class GlShapeWidget;
class QScrollBar;
class Camera;
class Consist;
class EngLib;
class Eng;

class ConEditorWindow : public QMainWindow {
    Q_OBJECT

public:
    ConEditorWindow();
    virtual ~ConEditorWindow();
    
public slots:
    void engListSelected(int id);
    void conListSelected(int id);
    void conSliderValueChanged(int val);
        
signals:
    void showEng(QString path, QString name);
    void showCon(int id);

private:
    EngListWidget *eng1;
    ConListWidget *con1;
    GlShapeWidget *glShapeWidget;
    GlShapeWidget *glConWidget;
    QScrollBar *conSlider;
    
    QMenu *fileMenu;
    Consist *currentCon = NULL;
    Eng *currentEng = NULL;
    Camera *conCamera;
    EngLib* englib;
    //EngLib* conEngLib;
    
    QLineEdit eName;
    QLineEdit eType;
    QLineEdit eBrakes;
    QLineEdit eCouplings;
    QLineEdit eMass;
    QLineEdit eMaxSpeed;
    QLineEdit eShape;
    QLineEdit eSize;
            
    QLineEdit cFileName;
    QLineEdit cDisplayName;
    QLineEdit cMass;
    QLineEdit cLength;
    QLineEdit cUnits;
    QLineEdit cDurability;
};

#endif	/* CONEDITORWINDOW_H */

