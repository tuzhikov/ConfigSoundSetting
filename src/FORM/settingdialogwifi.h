#ifndef SETTINGDIALOGWIFI_H
#define SETTINGDIALOGWIFI_H

#include <QDialog>
#include <QDebug>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

class QLabel;
class QStringList;
class QValidator;

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
    class SettingDialogWifi;
}

QT_END_NAMESPACE


class SettingDialogWifi : public QDialog
{
    Q_OBJECT

    public:
        explicit SettingDialogWifi(QWidget *parent = nullptr);
        ~SettingDialogWifi();
        void setParemetr( const QStringList prmlist);
        QStringList getParemetr( void ) const;
        QString getPORT( void ) const;
        QString getIP( void ) const;
        void setPORT( const QString port );
        void setIP( const QString ip );

    private:
        Ui::SettingDialogWifi *ui;
        QList<QNetworkConfiguration> listWifi;
        QStringList paremetrIP;
        enum TYPE_PARAMETR{
            IP_ETHERNET,
            PORT_ETHERNET,
            END_ETHERNET};

        void setMaskLabelIP( void );
        void setMaskLabelPORT( void );
    signals:
        void signalListParametr(const QStringList &);
    public slots:
        void updateListWifi( void );
        void checkSession( void );
        void updateParametrs( void );
};


#endif // SETTINGDIALOGWIFI_H
