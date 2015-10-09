#ifndef GUIFUNCTIONS_H
#define	GUIFUNCTIONS_H

#include <QString>

class QLabel;
class QLineEdit;
class QAction;
class Window;

class GuiFunct {
public:
    static QLabel* newQLabel(QString text, int width);
    static QLineEdit* newQLineEdit(int width, int length);
    static QAction* newMenuCheckAction(QString desc, Window* window);
private:

};

#endif	/* GUIFUNCTIONS_H */

