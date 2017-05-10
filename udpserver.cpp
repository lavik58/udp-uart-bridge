/*
    Copyright 2014 Benjamin Vedder	benjamin@vedder.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "udpserver.h"
#include <QDebug>

UdpServer::UdpServer(QObject *parent) :
    QObject(parent)
{
    mUdpSocket = new QUdpSocket(this);
    mSerialPort = new QSerialPort(this);
    mPacketInterface = new PacketInterface(this);
    mTimer = new QTimer(this);
    mTimer->setInterval(10);
    mTimer->start();

    mPortPath = "COM3";
    mUdpPort = 10000;
    mBaudrate = 115200;
    mReconnectTimer = 0;

    connect(mUdpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
    connect(mSerialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
            this, SLOT(serialPortError(QSerialPort::SerialPortError)));
    connect(mSerialPort, SIGNAL(readyRead()),
            this, SLOT(serialDataAvilable()));
    connect(mPacketInterface, SIGNAL(packetReceived(QByteArray)),
            this, SLOT(processPacket(QByteArray)));
    connect(mPacketInterface, SIGNAL(dataToSend(QByteArray&)),
            this, SLOT(packetDataToSend(QByteArray&)));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

bool UdpServer::openSerialWithDefaults(void)
{
    // port settings:
    mSerialPort->setPortName( "COM3" );
    mSerialPort->setBaudRate( QSerialPort::BaudRate::Baud115200 );
    mSerialPort->setDataBits(QSerialPort::DataBits::Data8 );
    mSerialPort->setParity(QSerialPort::Parity::NoParity );
    mSerialPort->setStopBits(QSerialPort::StopBits::OneStop );
    mSerialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl );

    if ( mSerialPort->open(QIODevice::ReadOnly) == false ) {
        qWarning() << "Unable to open serial port with default settings";
        return false;
    }
    return true;
}

bool UdpServer::startServer(const QString &serialPort, int baudrate, quint16 udpPort)
{
    mPortPath = serialPort;
    mUdpPort = udpPort;
    mBaudrate = baudrate;

    if ( openSerialWithDefaults() == false )
    {
        return false;
    }

    return mUdpSocket->bind(QHostAddress::LocalHost, mUdpPort);
}

void UdpServer::readPendingDatagrams()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(mUdpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        mUdpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        mPacketInterface->sendPacket(datagram);

        mLastHostAddress = sender;
    }
}

void UdpServer::serialDataAvilable()
{
    while (mSerialPort->bytesAvailable() > 0) {
        QByteArray data = mSerialPort->readAll();
        //mPacketInterface->processData(data);
        mPacketInterface->bypassRawData(data);
    }
}

void UdpServer::serialPortError( QSerialPort::SerialPortError error )
{
    if ( error != QSerialPort::SerialPortError::NoError )
    {
        qWarning() << "Serial port error: " << error;
        mReconnectTimer = 50;
    }
}

void UdpServer::timerSlot()
{
    mPacketInterface->timerSlot();

    if (mReconnectTimer) {
        mReconnectTimer--;
        if (!mReconnectTimer) {
            if ( openSerialWithDefaults() == false )
            {
                qWarning() << "Retrying...";
                mReconnectTimer = 50;
            }
        }
    }
}

void UdpServer::processPacket(QByteArray data)
{
    if (QString::compare(mLastHostAddress.toString(), "0.0.0.0") == 0) {
        qDebug() << "No send destination...";
        return;
    }

    mUdpSocket->writeDatagram(data, QHostAddress::LocalHost, mUdpPort );
}

void UdpServer::packetDataToSend(QByteArray &data)
{
    qWarning() << "Writing to serial is not supported!";
}

QString UdpServer::getSerialPortPath()
{
    return mPortPath;
}

quint16 UdpServer::getUdpPort()
{
    return mUdpPort;
}
