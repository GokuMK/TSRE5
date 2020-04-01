/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ErrorMessagesLib.h"
#include "ErrorMessage.h"
#include "ErrorMessagesWindow.h"

QVector<ErrorMessage*> ErrorMessagesLib::ErrorMessages;
ErrorMessagesWindow* ErrorMessagesLib::Window = NULL;

ErrorMessagesWindow* ErrorMessagesLib::GetWindow(QWidget *w){
    if(Window == NULL)
        Window = new ErrorMessagesWindow(w);
    return Window;
}

QString ErrorMessagesLib::PushErrorMessage(ErrorMessage* e){
    QString reply = "";
    ErrorMessages.push_back(e);
    
    if(Window != NULL)
        if(Window->isVisible())
            Window->refreshErrorList();
    
    return reply;
}

ErrorMessagesLib::ErrorMessagesLib() {
}

ErrorMessagesLib::~ErrorMessagesLib() {
}

