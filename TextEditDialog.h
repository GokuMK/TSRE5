/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TEXTEDITDIALOG_H
#define	TEXTEDITDIALOG_H

#include <QtWidgets>

class TextEditDialog : public QDialog {
    Q_OBJECT
public:
    TextEditDialog();
    virtual ~TextEditDialog();
    int changed = 0;
    QPlainTextEdit textBox;
            
public slots:
    void ok();
    void cancel();
    
private:
    
};

#endif	/* TEXTEDITDIALOG_H */

