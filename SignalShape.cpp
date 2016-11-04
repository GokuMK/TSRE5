/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
        int fidx = 0;
        int bidx = 0;
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
                    subObj[idx].sigSubTypeId = SigSubTypeStringToId[subObj[idx].sigSubType];
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "signalflags") {
                    QString sflags;
                    while((sflags = ParserX::GetStringInside(data)) != "" ){
                        if(sflags == "JN_LINK"){
                            //qDebug() << "JN_LINK";
                            //this->isJnLink = true;
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
            if(subObj[idx].backFacing){
                subObj[idx].faceidx = bidx++;
            }else{
                subObj[idx].faceidx = fidx++;
            }
        }
        return;
    }
    qDebug() << "-- " << sh;
    return;
}

QMap< QString, SignalShape::SigSubType > SignalShape::SigSubTypeStringToId = {
    { "UNDEFINED" , SignalShape::UNDEFINED },
    { "SIGNAL_HEAD" , SignalShape::SIGNAL_HEAD },
    { "NUMBER_PLATE" , SignalShape::NUMBER_PLATE },
    { "GRADIENT_PLATE" , SignalShape::GRADIENT_PLATE },
    { "USER1" , SignalShape::USER1 },
    { "USER2" , SignalShape::USER2 },
    { "USER3" , SignalShape::USER3 },
    { "USER4" , SignalShape::USER4 },
    { "DECOR" , SignalShape::DECOR }
};