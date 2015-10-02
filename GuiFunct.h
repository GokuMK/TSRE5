#ifndef GUIFUNCTIONS_H
#define	GUIFUNCTIONS_H

#include <QString>

class QLabel;
class QLineEdit;

class GuiFunct {
public:
    static QLabel* newQLabel(QString text, int width);
    static QLineEdit* newQLineEdit(int width, int length);
private:

};

#endif	/* GUIFUNCTIONS_H */

