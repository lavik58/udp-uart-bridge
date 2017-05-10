#ifndef USBPORT_H
#define USBPORT_H

#include <QObject>

class UsbPort: public QObject
{
    Q_OBJECT
public:
    explicit UsbPort(QObject *parent = 0);
    bool open( qint32 vid, qint32 pid );
    bool close( void );

signals:

public slots:

private:


};

#endif // USBPORT_H
