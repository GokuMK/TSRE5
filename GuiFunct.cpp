#include "GuiFunct.h"
#include "AboutWindow.h"
#include <QtWidgets>

QLabel* GuiFunct::newQLabel(QString text, int width){
    QLabel* label = new QLabel(text);
    label->setFixedWidth(width);
    return label;
}
QLineEdit* GuiFunct::newQLineEdit(int width, int length){
    QLineEdit* edit = new QLineEdit;
    edit->setFixedWidth(width);
    edit->setMaxLength(length);
    return edit;
}