#include "wifi.h"
#include <QQueue>
#include <windows.h>

/**
 * @brief LinkWIFI::LinkWIFI
 */
LinkWIFI::LinkWIFI()
{
    pSocket = new QTcpSocket;
    // default parametr
    setting.ip = "192.168.1.1";
    setting.port = 10001;
    timeDelay = 5;//ms
}
/**
 * @brief LinkWIFI::~LinkWIFI
 */
LinkWIFI::~LinkWIFI()
{
    delete pSocket;
}
/**
 * @brief LinkWIFI::sendDate
 * @param date
 * @return
 */
RET_ANSWER LinkWIFI::sendDate(QByteArray &data)
{
    RET_ANSWER ret(RET_ANSWER::ERROR_ANSWER);
    const quint16 lengh(512);
    QQueue<QByteArray> list;
    QByteArray sourse(data);

    if ( data.size()<lengh )
        list.enqueue( data );
    else
    {
        do{
            const int lg = sourse.size()<lengh?sourse.size():lengh;
            QByteArray cmd(sourse.constData(),lg);
            sourse.remove(0,lg);
            list.enqueue( cmd );
        }while(sourse.size());
    }
    // send
    while ( !list.isEmpty() )
    {
        QByteArray item(list.dequeue());
        pSocket->write(item);
        pSocket->flush();
        Sleep(1);
    }

    return ret;
}
/**
 * @brief LinkWIFI::readDate
 * @param date
 * @return
 */
RET_ANSWER LinkWIFI::readDate(QByteArray &data)
{
    RET_ANSWER ret(RET_ANSWER::ERROR_ANSWER);
    const quint64 lengRead = pSocket->bytesAvailable();
    if (lengRead)
    {
        data.append(pSocket->readAll());
        ret = RET_ANSWER::SUCCESSFULLY;
    }
    return ret;
}
/**
 * @brief LinkWIFI::connectionInformation
 * @return
 */
RET_CONNECT LinkWIFI::connectionInformation()
{
    RET_CONNECT ret(RET_CONNECT::CONNECT_NO);

    return ret;
}
/**
 * @brief LinkWIFI::connections
 * @return
 */
bool LinkWIFI::connections(const QStringList &list)
{
    bool returnStatus = false;
    if ( list.count() >= static_cast<int>(END_WIFI) )
    {
        setting.ip = list.at(WIFI_IP);
        setting.port = static_cast<int>(list.at(WIFI_PORT).toInt());
        returnStatus = true;
    }
    return returnStatus;
}

bool LinkWIFI::close()
{
    if ( pSocket->isOpen() )
            pSocket->close();
    return true;
}

bool LinkWIFI::open()
{
    bool returnStatus = false;
    pSocket->connectToHost(setting.ip,setting.port);

    if (pSocket->waitForConnected(500))
    {
        returnStatus = true;
    }
    return returnStatus;
}
