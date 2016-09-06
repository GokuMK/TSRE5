/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CHOOSEFILEDIALOG_H
#define	CHOOSEFILEDIALOG_H

#include <QtWidgets>

class ChooseFileDialog : public QDialog {
    Q_OBJECT
public:
    ChooseFileDialog();
    virtual ~ChooseFileDialog();
    QListWidget items;
    void setMsg(QString msg);
    int changed = 0;
    
public slots:
    void ok();
    void cancel();
    
private:
    QLabel infoLabel;
};

#endif	/* CHOOSEFILEDIALOG_H */

