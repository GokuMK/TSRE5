#ifndef CONEDITORWINDOW_H
#define	CONEDITORWINDOW_H

#include <QWidget>
#include <QMainWindow>

class EngListWidget;
class ConListWidget;
class GlShapeWidget;
class QScrollBar;

class ConEditorWindow : public QMainWindow {
    Q_OBJECT

public:
    ConEditorWindow();
    virtual ~ConEditorWindow();
    
public slots:
    void engListSelected(int id);
    void conListSelected(int id);
        
signals:
    void showEng(int id);
    void showCon(int id);

private:
    EngListWidget *eng1;
    ConListWidget *con1;
    GlShapeWidget *glShapeWidget;
    GlShapeWidget *glConWidget;
    QScrollBar *conSlider;
    
    QMenu *fileMenu;
};

#endif	/* CONEDITORWINDOW_H */

