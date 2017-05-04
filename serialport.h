/*
    Copyright 2012 Benjamin Vedder	benjamin@vedder.se

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

#ifndef SERIALPORT_H
#define SERIALPORT_H

#if 0
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>


class SerialPort : public QThread
{
    Q_OBJECT

public:
    SerialPort(QObject *parent = 0);
    ~SerialPort();

    enum SerialParity {PARITY_NONE, PARITY_EVEN, PARITY_ODD};
    enum SerialDataBits {DATA_5, DATA_6, DATA_7, DATA_8};
    enum SerialStopBits {STOP_1, STOP_2};

    struct SerialSettings {
        SerialParity parity;
        int baudrate;
        SerialDataBits dataBits;
        SerialStopBits stopBits;
    };

    int openPort(
        const QString& port,
        int baudrate = 115200,
        SerialDataBits dataBits = DATA_8,
        SerialStopBits stopBits = STOP_1,
        SerialParity parity = PARITY_NONE);

    void closePort();

    bool setParity(SerialParity parity);
    bool setStopBits(SerialStopBits stopBits);
    bool setDataBits(SerialDataBits dataBits);

    int readBytes(char* buffer, int bytes);

    QByteArray readAll();
    int writeData(const char* data, int length, bool block = true);

    bool writeString(const QString& string, bool block = true);
    int bytesAvailable();
    bool isOpen();
    int captureBytes(char* buffer, int num, int timeoutMs = 0, const QString& preTransmit = "");
    int captureBytes(char* buffer, int num, int timeoutMs = 0, const char* preTransmit = 0, int preTransLen = 0);

Q_SIGNALS:
    void serial_data_available();
    void serial_port_error(int e);

public Q_SLOTS:

protected:
    void run();

private:


    QMutex mMutex;
    QWaitCondition mCondition;
    bool mAbort;
    bool mIsOpen;

    SerialSettings mSettings;
    int mFd;

    int mBufferSize;
    char* mReadBuffer;
    int mBufferRead;
    int mBufferWrite;

    int mCaptureBytes;
    char* mCaptureBuffer;
    int mCaptureWrite;
};
#endif

#endif // SERIALPORT_H
