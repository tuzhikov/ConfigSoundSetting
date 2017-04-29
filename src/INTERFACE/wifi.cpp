#include "wifi.h"

/**
 * @brief LinkWIFI::LinkWIFI
 */
LinkWIFI::LinkWIFI()
{

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
RET_ANSWER LinkWIFI::sendDate(QByteArray &date)
{
    RET_ANSWER ret(RET_ANSWER::ERROR_ANSWER);

    return ret;
}
/**
 * @brief LinkWIFI::readDate
 * @param date
 * @return
 */
RET_ANSWER LinkWIFI::readDate(QByteArray &date)
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

bool LinkWIFI::closePort()
{

}
