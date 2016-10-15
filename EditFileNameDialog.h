/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef EDITFILENAMEDIALOG_H
#define	EDITFILENAMEDIALOG_H

#include <QtWidgets>

class EditFileNameDialog : public QDialog {
    Q_OBJECT
public:
    QLineEdit name;
    bool isOk = false;
    EditFileNameDialog();
    virtual ~EditFileNameDialog();
    
public slots:
    void cancel();
    void ok();
private:

};

#endif	/* EDITFILENAMEDIALOG_H */

