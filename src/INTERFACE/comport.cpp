#include "comport.h"

/**
 * @brief ComPort::ComPort
 */
ComPort::ComPort()
{

}
/**
 * @brief ComPort::~ComPort
 */
ComPort::~ComPort()
{

}
/**
 * @brief ComPort::sendDate
 * @param date
 * @return
 */
RET_ANSWER ComPort::sendDate(QByteArray &date)
{
    RET_ANSWER ret(RET_ANSWER::ERROR_ANSWER);

    return ret;
}
/**
 * @brief ComPort::readDate
 * @param date
 * @return
 */
RET_ANSWER ComPort::readDate(QByteArray &date)
{
    RET_ANSWER ret(RET_ANSWER::ERROR_ANSWER);

    return ret;
}
/**
 * @brief ComPort::connectionInformation
 * @return
 */
RET_CONNECT ComPort::connectionInformation()
{
    RET_CONNECT ret(RET_CONNECT::CONNECT_NO);

    return ret;
}
/**
 * @brief ComPort::closePort
 * @return
 */
bool ComPort::closePort()
{
    if (serial->isOpen())
        serial->close();
    return true;
}

bool ComPort::openPort()
{
    bool returnStatus = false;
    //    SettingsDialog::Settings p = settings->settings();
//    serial->setPortName(p.name);
//    serial->setBaudRate(p.baudRate);
//    serial->setDataBits(p.dataBits);
//    serial->setParity(p.parity);
//    serial->setStopBits(p.stopBits);
//    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite))
    {

//        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
//                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
//                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        returnStatus = true;
    } else {
        //QMessageBox::critical(0, QObject::tr("Error"), serial->errorString());
//        showStatusMessage(tr("Open error"));
    }
    return returnStatus;
}
