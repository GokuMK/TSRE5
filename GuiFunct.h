#ifndef GUIFUNCTIONS_H
#define	GUIFUNCTIONS_H

#include <QString>

class QLabel;
class QLineEdit;
class QAction;
class QMainWindow;

class GuiFunct {
public:
    static QLabel* newQLabel(QString text, int width);
    static QLabel* newTQLabel(QString text, int width = -1);
    static QLineEdit* newQLineEdit(int width, int length);
    static QAction* newMenuCheckAction(QString desc, QMainWindow* window, bool checked = true);
private:

};

#endif	/* GUIFUNCTIONS_H */

