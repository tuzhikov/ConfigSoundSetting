#ifndef WIFI_H
#define WIFI_H

#include "LinkInterface.h"
#include <QTcpSocket>
class QByteArray;


class LinkWIFI : public LinkInreface
{
    public:
        LinkWIFI();
        ~LinkWIFI();
        RET_ANSWER sendDate( QByteArray &date );
        RET_ANSWER readDate(QByteArray &date);
        RET_CONNECT connectionInformation();
        bool connections(const QStringList &);
        bool close();
        bool open();
        int retTimeDelay() {return timeDelay;}
    private:
        QTcpSocket *pSocket;
        int timeDelay;
        enum TYPE_PARAMETR_WIFI{
            WIFI_IP,
            WIFI_PORT,
            END_WIFI
        };
        struct Settings
        {
            QString ip;
            int port;
        };
        Settings setting;
};

#endif // WIFI_H
