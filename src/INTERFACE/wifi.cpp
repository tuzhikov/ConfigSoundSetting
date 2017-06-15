#include "wifi.h"

/**
 * @brief LinkWIFI::LinkWIFI
 */
LinkWIFI::LinkWIFI()
{
    timeDelay = 10;//ms
}
/**
 * @brief LinkWIFI::~LinkWIFI
 */
LinkWIFI::~LinkWIFI()
{

}
/**
 * @brief LinkWIFI::sendDate
 * @param date
 * @return
 */
RET_ANSWER LinkWIFI::sendDate(QByteArray &)
{
    RET_ANSWER ret(RET_ANSWER::ERROR_ANSWER);

    return ret;
}
/**
 * @brief LinkWIFI::readDate
 * @param date
 * @return
 */
RET_ANSWER LinkWIFI::readDate(QByteArray &)
{
    RET_ANSWER ret(RET_ANSWER::ERROR_ANSWER);

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
bool LinkWIFI::connections(const QStringList &)
{
    bool returnStatus = false;
    return returnStatus;
}

bool LinkWIFI::close()
{
    return true;
}

bool LinkWIFI::open()
{
    bool returnStatus = false;
    return returnStatus;
}
