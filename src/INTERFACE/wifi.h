#ifndef WIFI_H
#define WIFI_H

#include "LinkInterface.h"
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
        int timeDelay;
};

#endif // WIFI_H
