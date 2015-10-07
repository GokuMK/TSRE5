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
        
    qDebug() << "-- " << sh;
    return;
}
