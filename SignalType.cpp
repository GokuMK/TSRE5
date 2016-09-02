/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SignalType.h"
#include "FileBuffer.h"
#include <QDebug>

SignalType::SignalType() {
}

SignalType::~SignalType() {
}

void SignalType::set(QString sh, FileBuffer* data) {
        if (sh == "signalflags") {   
            return;
        }
        if (sh == "signalfntype") {   
            return;
        }
        if (sh == "signallighttex") {   
            return;
        }
        if (sh == "semaphoreinfo") {   
            return;
        }
        if (sh == "signallights") {   
            return;
        }
        if (sh == "signaldrawstates") {   
            return;
        }
        if (sh == "signalaspects") {   
            return;
        }
        if (sh == "signalnumclearahead") {   
            return;
        }
        if (sh == "sigflashduration") {   
            return;
        }
        
    qDebug() << "-- " << sh;
    return;
}
