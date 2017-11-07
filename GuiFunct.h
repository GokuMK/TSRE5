/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GUIFUNCTIONS_H
#define	GUIFUNCTIONS_H

#include <QString>

class QLabel;
class QLineEdit;
class QAction;
class QWidget;

class GuiFunct {
public:
    static QLabel* newQLabel(QString text, int width);
    static QLabel* newTQLabel(QString text, int width = -1);
    static QLineEdit* newQLineEdit(int width, int length);
    static QAction* newMenuCheckAction(QString desc, QWidget* window, bool checked = true);
private:

};

#endif	/* GUIFUNCTIONS_H */

