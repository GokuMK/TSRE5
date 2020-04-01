/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ErrorMessage.h"

ErrorMessage::ErrorMessage() {
}

ErrorMessage::ErrorMessage(QString type, QString source, QString description) {
    this->type = type;
    this->source = source;
    this->description = description;
}

ErrorMessage::ErrorMessage(const ErrorMessage& orig) {
    type = orig.type;
    source = orig.source;
    description = orig.description;
}

ErrorMessage::~ErrorMessage() {
}

