/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRAINNETWORKENG_H
#define	TRAINNETWORKENG_H

#include <QObject>

class QTcpSocket;

class TrainNetworkEng : QObject {
    Q_OBJECT
public:
    TrainNetworkEng();
    virtual ~TrainNetworkEng();
    float getSpeed();
    void writeData(float *data);
    
public slots:
    void readData();
private:
    float speed = 1;
    QTcpSocket *socket = NULL;
    void processDatagram(QByteArray data);
};

#endif	/* TRAINNETWORKENG_H */

