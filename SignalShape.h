#ifndef SIGNALSHAPE_H
#define	SIGNALSHAPE_H

#include <QString>

class FileBuffer;

class SignalShape {
public:
    struct SubObj {
        QString type;
        QString desc;
        QString sigSubType;
        int* sigSubJnLinkIf;
        int iLink;
        QString sigSubSType;
        bool isJnLink = false;
        bool optional = false;
        bool backFacing = false;
    };
    int listId;
    int iSubObj;
    SubObj* subObj;
    QString name;
    QString desc;
    bool isJnLink = false;
    
    SignalShape();
    virtual ~SignalShape();
    void set(QString sh, FileBuffer* data);
private:

};

#endif	/* SIGNALSHAPE_H */

