#include "GuiFunct.h"
#include "AboutWindow.h"
#include <QtWidgets>
#include "window.h"

QLabel* GuiFunct::newQLabel(QString text, int width){
    QLabel* label = new QLabel(text);
    label->setFixedWidth(width);
    return label;
}

QLabel* GuiFunct::newTQLabel(QString text, int width){
    QLabel *l = new QLabel(text);
    l->setContentsMargins(3,0,0,0);
    l->setStyleSheet("QLabel { color : #999999; }");
    if(width >=0)
        l->setMinimumWidth(width);
    return l;
}

QLineEdit* GuiFunct::newQLineEdit(int width, int length){
    QLineEdit* edit = new QLineEdit;
    edit->setFixedWidth(width);
    edit->setMaxLength(length);
    return edit;
}

QAction* GuiFunct::newMenuCheckAction(QString desc, QMainWindow* window, bool checked){
    QAction *action = new QAction(desc, window);
    action->setCheckable(true);
    action->setChecked(checked);
    return action;
}