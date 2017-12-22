/****************************************************************************************
//    myudp.cpp
//
//    Copyright (C) 2017  Zach (Zhengyu) Peng, https://zpeng.me
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************************/

#include "myudp.h"

MyUDP::MyUDP(QObject *parent) : QUdpSocket(parent)
{
    socket = new QUdpSocket();

    for (quint16 i=0; i<8192;i++)
    {
        timeStamp.append(i/250.0);
    }
}

bool MyUDP::bindPort(QHostAddress addr, qint16 port)
{
    socket->abort();
    bool isBinded = socket->bind(addr, port);
    if (isBinded)
    {
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    return isBinded;
}

void MyUDP::sendMessage(QHostAddress sender, quint16 senderPort, QString string)
{
    QByteArray Data;
    Data.append(string);

    // Sends the datagram datagram
    // to the host address and at port.
    socket->writeDatagram(Data, sender, senderPort);
}

void MyUDP::readyRead()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).
    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    if (buffer.left(7) == "ADCDATA")
    {
        acceptingADCData = true;
        array.clear();
    }

    if (acceptingADCData)
    {
        array.append(buffer);
    }
    else
    {
        emit newMessage(sender.toString(), buffer);
    }

    if (array.right(7) == "ADCSTOP")
    {
        acceptingADCData = false;
        array = array.mid(7, array.size() - 14);

        for (qint32 i = 0; i < 1024; i++)
        {
            //timeStamp.append(((((quint32)array.at(i)) << 16) + (((quint32)array.at(i + 1024)) << 8) + ((quint32)array.at(i + 2048))) >> 6);
            // Ping
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 0)) << 8) + ((quint16)array.at(i + 1024 * 1))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 2)) << 8) + ((quint16)array.at(i + 1024 * 3))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 4)) << 8) + ((quint16)array.at(i + 1024 * 5))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 6)) << 8) + ((quint16)array.at(i + 1024 * 7))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);

            // Pong
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 8)) << 8) + ((quint16)array.at(i + 1024 * 9))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 10)) << 8) + ((quint16)array.at(i + 1024 * 11))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 12)) << 8) + ((quint16)array.at(i + 1024 * 13))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);
            adcData.append(((float)((((((quint16)array.at(i + 1024 * 14)) << 8) + ((quint16)array.at(i + 1024 * 15))) >> 2) & 0x0FFF)) / pow(2, 12) * 1.48);
        }
        emit newMessage(sender.toString(), array.toHex());
        for (quint16 i=0; i<8192;i++)
        {
            plotData.append(QPointF(timeStamp.at(i), adcData.at(i)));
        }

        //! TODO
        //! QChart has a very poor performance, switch to QML
        //emit newData(plotData);

        qDebug() << adcData.size();
        qDebug() << adcData.mid(0,1024);
        qDebug() << timeStamp.mid(0,1024);
        //qDebug() << adcData.mid(1024,1024);
        //qDebug() << adcData.mid(1024*2,1024);
        //qDebug() << adcData.mid(1024*3,1024);
        //qDebug() << adcData.mid(1024*4,1024);
        //qDebug() << adcData.mid(1024*5,1024);
        //qDebug() << adcData.mid(1024*6,1024);
        //qDebug() << adcData.mid(1024*7,1024);

        array.clear();
        adcData.clear();
    }
}

void MyUDP::unbindPort()
{
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    socket->close();
}
