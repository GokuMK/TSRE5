/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TrainNetworkEng.h"
#include <QTcpSocket> 
#include <QTcpServer> 

TrainNetworkEng::TrainNetworkEng() {
    socket = new QTcpSocket(this);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption,1);
    connect(socket, SIGNAL(readyRead()), SLOT(readData()), Qt::UniqueConnection);

    if(!(QAbstractSocket::ConnectedState == socket->state())){
        socket->connectToHost(QHostAddress::LocalHost, 7755, QIODevice::ReadWrite);
    }
}

void TrainNetworkEng::readData() {
    QByteArray data = socket->readAll();
    speed = *(float*)data.data();

}

void TrainNetworkEng::writeData(float *data) {
    socket->write((const char*)data, 16);
}

TrainNetworkEng::~TrainNetworkEng() {
}

float TrainNetworkEng::getSpeed(){
    return speed;
}

