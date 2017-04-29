#ifndef COMPORT_H
#define COMPORT_H

#include <QtSerialPort/QSerialPort>
#include "LinkInterface.h"

QT_USE_NAMESPACE

class QByteArray;
class QMessageBox;

class ComPort : LinkInreface
{
    public:
        ComPort();
        ~ComPort();
        RET_ANSWER sendDate( QByteArray &date );
        RET_ANSWER readDate(QByteArray &date);
        RET_CONNECT connectionInformation();
        bool closePort();
        bool openPort();
    private:
        QSerialPort *serial;
};

Q_DECLARE_METATYPE( ComPort )

#endif // COMPORT_H
