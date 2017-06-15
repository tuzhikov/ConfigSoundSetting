/*
 * Copyright (C)
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Contact us: at_alex@bk.ru
*/
#include "settingdialogwifi.h"
#include "ui_settingdialogwifi.h"

QT_USE_NAMESPACE
/**
 * @brief SettingDialogWifi::SettingDialogWifi
 * @param parent
 */
SettingDialogWifi::SettingDialogWifi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialogWifi)
{
    ui->setupUi(this);
    updateListWifi();
    setMaskLabelIP();
    setMaskLabelPORT();
    // connection
    connect(ui->pbFind, &QPushButton::clicked, this, &SettingDialogWifi::updateListWifi);
    connect(ui->pbConnect, &QPushButton::clicked, this, &SettingDialogWifi::checkSession);
    connect(ui->pbApply, &QPushButton::clicked, this, &SettingDialogWifi::updateParametrs);
    // default parametr
    paremetrIP << ui->leIP->text()<<ui->lePORT->text()<<QString();
}
/**
 * @brief SettingDialogWifi::~SettingDialogWifi
 */
SettingDialogWifi::~SettingDialogWifi()
{
    delete ui;
}
/**
 * @brief SettingDialogWifi::setMaskLabelIP
 */
void SettingDialogWifi::setMaskLabelIP()
{
    QLineEdit *const ip(ui->leIP);
    // Создаем строку для регулярного выражения
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    // Создаем регулярное выражение с применением строки, какповторяющегося элемента
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    // create validator
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ip->setValidator(ipValidator);
}
/**
 * @brief SettingDialogWifi::setMaskLabelPORT
 */
void SettingDialogWifi::setMaskLabelPORT()
{
    QLineEdit *const port(ui->lePORT);
    // set int validator on QLineEdit
   port->setValidator( new QIntValidator( 0, 65535));
}
/**
 * @brief SettingDialogWifi::updateListWifi
 */
void SettingDialogWifi::updateListWifi(void)
{
    QListWidget *lwg(ui->listWifi);
    lwg->clear();
    QNetworkConfigurationManager conNetManager;
    listWifi.clear();
    listWifi<<conNetManager.allConfigurations();

    for (auto &con : listWifi)
    {
        if (con.bearerType() != QNetworkConfiguration::BearerUnknown)
        {
            lwg->addItem(con.name());
        }
    }
}
/**
 * @brief SettingDialogWifi::checkSession
 */
void SettingDialogWifi::checkSession()
{
    QListWidget *const wd(ui->listWifi);
    QLabel *const lbconnect(ui->lbConnection);
    QLabel *const lbusage(ui->lbUsage);
    QLabel *const lbstate(ui->lbSate);
    QList<QNetworkConfiguration> lst(listWifi);

    //QListWidget is empty
    if(!wd->count())
        return;
    // select network
    QNetworkConfiguration currentConfig;
    const QString nameAP(wd->currentItem()->text());
    // select action configuration
    for (auto &con : lst)
    {
        if ( con.name() == nameAP )
        {
            currentConfig = con;
            QNetworkSession session (currentConfig, this);
            session.open ();
            session.waitForOpened (1000);
            lbconnect->setText(nameAP);
            lbusage->setText("- usage: " + static_cast<QString>(session.isOpen () ? "Opened" : "Closed"));
            lbstate->setText("- state: " + static_cast<QString>(session.state () == QNetworkSession::Connected ? "Connected" : "Not connected"));
        }
    }

}
/**
 * @brief SettingDialogWifi::setParemetrIP
 * @param ip
 * @param pr
 */
void SettingDialogWifi::setParemetr(const QStringList prmlist)
{
    paremetrIP = prmlist;
}
/**
 * @brief SettingDialogWifi::getParemetrIP
 * @param ip
 * @param pr
 */
QStringList SettingDialogWifi::getParemetr(void) const
{
    return paremetrIP;
}
/**
 * @brief SettingDialogWifi::getPORT
 * @return
 */
QString SettingDialogWifi::getPORT() const
{
    if (paremetrIP.count() > TYPE_PARAMETR::PORT_ETHERNET)
    {
        return paremetrIP.at(TYPE_PARAMETR::PORT_ETHERNET);
    }
    return QString();
}
/**
 * @brief SettingDialogWifi::getIP
 * @return
 */
QString SettingDialogWifi::getIP() const
{
    if (paremetrIP.count() > TYPE_PARAMETR::IP_ETHERNET)
    {
        return paremetrIP.at(TYPE_PARAMETR::IP_ETHERNET);
    }
    return QString();
}
/**
 * @brief SettingDialogWifi::setPORT
 * @param port
 */
void SettingDialogWifi::setPORT(const QString port)
{
    if (paremetrIP.count() > TYPE_PARAMETR::PORT_ETHERNET)
    {
        paremetrIP[TYPE_PARAMETR::PORT_ETHERNET] = port;
    }else{
        paremetrIP << port;
    }
    // wiev PORT
    QLineEdit *const leport(ui->lePORT);
    leport->setText(port);
}
/**
 * @brief SettingDialogWifi::setIP
 * @param ip
 */
void SettingDialogWifi::setIP(const QString ip)
{
    if (paremetrIP.count() > TYPE_PARAMETR::IP_ETHERNET)
    {
        paremetrIP[TYPE_PARAMETR::IP_ETHERNET] = ip;
    }else{
        paremetrIP << ip;
    }
    // wiev IP
    QLineEdit *const leip(ui->leIP);
    leip->setText(ip);
}
/**
 * @brief SettingDialogWifi::updateParametr
 */
void SettingDialogWifi::updateParametrs()
{
    QLineEdit *const ip(ui->leIP);
    QLineEdit *const port(ui->lePORT);
    QVector <QLineEdit *> lelist;
    lelist<<ip<<port;
    paremetrIP.clear();

    foreach( QLineEdit* ed, lelist )
    {
        if( ed->hasAcceptableInput() )
        {
            if( !lelist.isEmpty() )
            {

                paremetrIP << lelist.takeFirst()->text();
            }
        } else {
            qDebug() << "Invalid";
        }
   }
   qDebug()<< paremetrIP;
   emit signalListParametr(paremetrIP);
}
