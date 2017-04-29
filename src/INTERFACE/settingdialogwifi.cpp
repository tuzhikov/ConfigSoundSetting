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
    // connection
    connect(ui->pbFind, &QPushButton::clicked, this, &SettingDialogWifi::updateListWifi);
    connect(ui->pbConnect, &QPushButton::clicked, this, &SettingDialogWifi::applyButton);
}
/**
 * @brief SettingDialogWifi::~SettingDialogWifi
 */
SettingDialogWifi::~SettingDialogWifi()
{
    delete ui;
}
/**
 * @brief SettingDialogWifi::updateListWifi
 */
void SettingDialogWifi::updateListWifi(void)
{
    QListWidget *lwg(ui->listWifi);
    lwg->clear();
    QNetworkConfigurationManager conNetManager;
    list.clear();
    list<<conNetManager.allConfigurations();

    for (auto &con : list)
    {
        if (con.bearerType() != QNetworkConfiguration::BearerUnknown)
        {
            lwg->addItem(con.name());
        }
    }
}
/**
 * @brief SettingDialogWifi::applyButton
 */
void SettingDialogWifi::applyButton()
{
    QListWidget *const wd(ui->listWifi);
    QLabel *const lb(ui->lbCurrentAP);
    QList<QNetworkConfiguration> lst(list);
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
            lb->setText(nameAP);
        }
    }

    QNetworkSession networkSession (currentConfig, this);
    networkSession.open();
    networkSession.waitForOpened(1000);
    qDebug () << "Session info:";
    qDebug () << "- usage: " << (networkSession.isOpen () ? "Opened" : "Closed");
    qDebug () << "- state: " << (networkSession.state  () == QNetworkSession::Connected ? "Connected" : "Not connected");

    // create new sessoin
    //session = new QNetworkSession(currentConfig, this);
    //session->open();

    qDebug()<<"Class SettingDialogWifi functions applyButton.";
}
