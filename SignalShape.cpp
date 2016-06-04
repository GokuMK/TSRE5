#include "SignalShape.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include <QDebug>

SignalShape::SignalShape() {
}

SignalShape::~SignalShape() {
}

void SignalShape::set(QString sh, FileBuffer* data) {
    //if (sh == ("tritemid")) {
    //    trItemId = ParserX::GetUInt(data);
    //    return;
    //}
    if (sh == "signalsubobjs") {
        iSubObj = ParserX::GetNumber(data);
        //qDebug() << iSubObj;
        subObj = new SubObj[iSubObj];
        int idx;
        for(int i = 0; i < iSubObj; i++){
            idx = ParserX::GetNumber(data);
            subObj[idx].iLink = 0;
            subObj[idx].type = ParserX::GetString(data);
            subObj[idx].desc = ParserX::GetString(data);
            //qDebug() <<subObj[idx].type;
            //qDebug() <<subObj[idx].desc;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //qDebug() << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "sigsubtype") {
                    subObj[idx].sigSubType = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "signalflags") {
                    QString sflags;
                    while((sflags = ParserX::GetStringInside(data)) != "" ){
                        if(sflags == "JN_LINK"){
                            //qDebug() << "JN_LINK";
                            this->isJnLink = true;
                            subObj[idx].isJnLink = true;
                            continue;
                        }
                        if(sflags == "DEFAULT"){
                            subObj[idx].defaultt = true;
                            continue;
                        }
                        if(sflags == "OPTIONAL"){
                            subObj[idx].optional = true;
                            continue;
                        }
                        if(sflags == "BACK_FACING"){
                            subObj[idx].backFacing = true;
                            continue;
                        }
                        qDebug() << "signalFlags" << sflags;
                        //subObj[idx].signalFlags = sflags;
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "sigsubstype") {
                    subObj[idx].sigSubSType = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "sigsubjnlinkif") {
                    subObj[idx].iLink = ParserX::GetNumber(data);
                    subObj[idx].sigSubJnLinkIf = new int[subObj[idx].iLink];
                    for(int j = 0; j < subObj[idx].iLink; j++)
                        subObj[idx].sigSubJnLinkIf[j] = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "--- " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
        }
        return;
    }
    qDebug() << "-- " << sh;
    return;
}
