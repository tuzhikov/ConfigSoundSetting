#ifndef SETTINGDIALOGWIFI_H
#define SETTINGDIALOGWIFI_H

#include <QDialog>
#include <QDebug>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

class QLabel;
class QStringList;

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

    private:
        Ui::SettingDialogWifi *ui;
        QList<QNetworkConfiguration> list;
        QNetworkSession *session;
        //QList<QNetworkConfiguration> updateListWifi(void);
    signals:
    public slots:
        void updateListWifi( void );
        void applyButton( void );
};

#endif // SETTINGDIALOGWIFI_H
