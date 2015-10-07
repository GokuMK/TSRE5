#ifndef SIGNALTYPE_H
#define	SIGNALTYPE_H

#include <QString>

class FileBuffer;

class SignalType {
public:
    QString type;
    SignalType();
    virtual ~SignalType();
    void set(QString sh, FileBuffer* data);
private:

};

#endif	/* SIGNALTYPE_H */

