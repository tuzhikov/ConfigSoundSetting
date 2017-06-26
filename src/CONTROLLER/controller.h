#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QQueue>
#include <QByteArray>
#include <QDateTime>
#include <QColor>
#include <QTimeZone>
#include "cmd_device.h"
#include "src/ACCESSDATE/accessdataproject.h"
#include "src/INTERFACE/LinkInterface.h"
#include "src/INTERFACE/comport.h"
#include "src/INTERFACE/wifi.h"
#include "src/CONTROLLER/cmd_device.h"

class Controller : public QObject
{
    Q_OBJECT
    typedef QMap <QString, QByteArray> TYPE_PLAY;
    typedef QMap <int,QByteArray> TYPE_CODE_NAME;

public:
    enum TYPE_COMMAND{cmWriteAll,cmReadAll,cmWriteSetting,cmReadSetting,cmSetTime,cmGetTime,cmListen,cmTest,END_TYPE_COMMAND};
    explicit Controller( QObject *parent = 0 );
    ~Controller(){}
    inline AccessDataProject &retDataProject(){ return dataproject;}
private:
    enum TYPE_STATE{stOpen,stCounter,stRepeat,stWrite,stRead,stClose,stStopExchange};
    enum TYPE_STEP{STEP_ONE,STEP_TWO,STEP_THREE,STEP_ERROR};
    enum TYPE_INTERFACE{INTERFACE_COM,INTERFACE_WIFI,END_INTERFACE};

    TYPE_STATE stat = stOpen;
    AccessDataProject dataproject;
    QQueue<QByteArray> listCMD, templistCMD;
    QByteArray sendCmd;
    TYPE_PLAY tmpRaedDataWav;   // временно храним даные wav файла
    TYPE_CODE_NAME codeNameFile; // 4 байта код в имени файла "0001///"
    TYPE_CODE_NAME::const_iterator inNameFile;

    QTimer *timerRead;
    QVector <LinkInreface* > interface;
    LinkInreface* currentInterface = 0;
    int currentValueProgress = 0;
    TYPETIME timeDevice;
    TYPE_TEST_TRACK listen;
    int countSend = 0;
    const int max_repeat = 2;
    QString messageError,messageOk;
    const quint16 max_lengh_data_wav;
    const quint16 max_lengh_hander_wav;
    const quint16 max_lengh_name_file;
    bool progressVisible;

    // send command
    void commandWrite();
    void commandRead();
    void commandWriteSetting();
    void commandReadSetting();
    void commandSetTime();
    void commandGetTime();
    void commandListen();
    void commandTest();
    typedef void (Controller::*NUMBER_COMMAND)();
    const NUMBER_COMMAND command_send[END_TYPE_COMMAND] = {
        commandWrite,commandRead,commandWriteSetting,commandReadSetting,
        commandSetTime,commandGetTime,commandListen,commandTest};
    // read command
    RET_ANSWER commandWriteProject(const char *, const int);
    RET_ANSWER commandReadProject(const char *, const int);
    RET_ANSWER commandWriteWAV(const char *, const int);
    RET_ANSWER commandReadWAV(const char *, const int);
    RET_ANSWER commandWriteTime(const char *, const int);
    RET_ANSWER commandReadTime(const char *, const int);
    RET_ANSWER commandWriteTest(const char *, const int);
    RET_ANSWER commandReadTest(const char *, const int);
    RET_ANSWER commandWrite(const char *, const int );
    RET_ANSWER commandRead(const char *, const int );

    typedef RET_ANSWER (Controller::*NUMBER_COMMAND_READ)(const char *, const int);

    const NUMBER_COMMAND_READ command_set[END_TYPE_NUMBER_COMMAND] = {
        commandWriteTest, commandWriteProject, commandWriteWAV, commandWriteTime};

    const NUMBER_COMMAND_READ command_get[END_TYPE_NUMBER_COMMAND] = {
        commandReadTest, commandReadProject, commandReadWAV, commandReadTime};

    const NUMBER_COMMAND_READ command_read[END_TYPE_NUMBER_COMMAND_TRANSPORT] = { commandWrite, commandRead };

    void collectTransportLevel( QQueue<QByteArray> &list );
    void collectTransportLevel( QByteArray &cmd );
    RET_ANSWER checksumReceived(const QByteArray &);
    RET_ANSWER parserReceivedPacket(const QByteArray &);
    void setMessageError( const QString msg ){messageError = msg;messageOk.clear();}
    void setMessageOk( const QString msg ){messageOk = msg;messageError.clear();}
    void sendOutputMessage();
private slots:
    void on_Machine();
public slots:
    void checkInterface(const int number);
    void sendMessage(QQueue<QByteArray> &ba);
    void sendMessage(QByteArray &ba);
    void stopMessage();
    void commandRun(const int numberCmd);
    void setParametrWIFI(const QStringList &);
    void setParametrCOMPORT(const QStringList &);
    void setDateTime(const QDateTime &);
    void setDateTimeZone(const int);
    void setListenTest(const TYPE_TEST_TRACK &);
signals:
    void signalStart();
    void signalStop();
    void signalStep();
    void signalTimerDiagnosisDisabled();
    void signalSearchUsb(QStringList&,quint16,quint16);
    void signalSendMessage(const QByteArray&,const QColor&);
    //void signalStatus(const QString&,const QColor&);
    void signalMessageOk(const QString&);
    void signalMessageError(const QString&);
    void signalProgressRange(const int,const int);
    void signalProgressValue(const int);
    void signalProgressVisible(const bool);
    void signalTypeDevice(const int);
    void signalTime(const TYPETIME &);
    void signalTest(const TYPE_TEST &);
};

#endif // CONTROLLER_H
