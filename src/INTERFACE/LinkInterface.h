#ifndef LINKINREFACE_H
#define LINKINREFACE_H

#include <stdint.h>
#include <QByteArray>

enum RET_ANSWER{
    SUCCESSFULLY    = 0x00000001,
    NO_ANSWER       = 0x00000002,
    ERROR_ANSWER    = 0x00000004,
    DELAY_ANSWER    = 0x00000008,
    END_ANS         = 0x80000000
};
enum RET_CONNECT{
    CONNECT_OK      = 0x00000001,
    CONNECT_NO      = 0x00000002,
    CONNECT_DELAY   = 0x00000004,
    END_CMD         = 0x80000000
};
/**
* Interface link
*/
class LinkInreface
{
    public:
        virtual ~LinkInreface(){}
        virtual RET_ANSWER sendDate( QByteArray &date ) = 0;
        virtual RET_ANSWER readDate(QByteArray &date) = 0;
        virtual RET_CONNECT connectionInformation() = 0;
        virtual bool closePort() = 0;
};

#endif // LINKINREFACE_H
