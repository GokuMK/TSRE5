/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ERRORMESSAGESLIB_H
#define ERRORMESSAGESLIB_H

#include <QHash>
#include <QString>

class QWidget;
class ErrorMessage;
class ErrorMessagesWindow;

class ErrorMessagesLib {
public:
    //static QHash<int, ErrorMessage*> ErrorMessages;
    static QVector<ErrorMessage*> ErrorMessages;
    static ErrorMessagesWindow* GetWindow(QWidget *w);
    static QString PushErrorMessage(ErrorMessage* e);
    
    ErrorMessagesLib();
    virtual ~ErrorMessagesLib();
private:
    static ErrorMessagesWindow* Window;
};

#endif /* ERRORMESSAGESLIB_H */

