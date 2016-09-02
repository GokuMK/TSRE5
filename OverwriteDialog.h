/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef OVERWRITEDIALOG_H
#define	OVERWRITEDIALOG_H

#include <QtWidgets>

class OverwriteDialog : public QDialog {
    Q_OBJECT
public:
    int changed = 0;
    OverwriteDialog();
    virtual ~OverwriteDialog();
    QLineEdit name;
    
    
public slots:
    //void trynew();
    void cancel();
    void ok();
    
private:

};

#endif	/* OVERWRITEDIALOG_H */

