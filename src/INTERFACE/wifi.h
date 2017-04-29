#ifndef WIFI_H
#define WIFI_H

#include "LinkInterface.h"
class QByteArray;


class LinkWIFI : LinkInreface
{
    public:
        LinkWIFI();
        ~LinkWIFI();
        RET_ANSWER sendDate( QByteArray &date );
        RET_ANSWER readDate(QByteArray &date);
        RET_CONNECT connectionInformation();
        bool closePort();
};

#endif // WIFI_H
