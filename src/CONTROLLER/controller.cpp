#include <time.h>
#include "controller.h"
#include "crc.h"

#define DEBUG_ANSWER

/**
 * @brief Controller::commandWriteAll
 */
void Controller::commandWriteAll()
{
    // data project
    QByteArray cmd;
    const quint16 lengh(sizeof(TYPEPROJECT)+(uint8_t)3);
    cmd.append((char*)&lengh,sizeof(uint16_t));
    cmd.append(TYPE_UZTVOP);    // type
    cmd.append(CMD_TR_WRITE);   // cmd
    cmd.append(CMD_PRJ);        // type cmd
    cmd.append(dataproject.retDataProject().getDataStructure());
    QQueue<QByteArray> list;
    list.enqueue( cmd );
    //
    // extract sound files
    QMap <QString, QByteArray>::iterator it = dataproject.retPlayList().begin();
    quint8 count_sound = dataproject.retPlayList().size();
    bool first_package = true;

    for (;it!= dataproject.retPlayList().end(); ++it)
    {
        QByteArray name;name.append(it.key());
        QByteArray data(it.value());
        const quint16 max_pask = (data.size()/max_lengh_data_wav) +1;
        //first wav
        QByteArray cmd;
        const quint16 lengh = max_lengh_name_file+max_lengh_hander_wav+2+1+3;
        cmd.append((char*)&lengh,sizeof(quint16));
        cmd.append(TYPE_UZTVOP);    // type
        cmd.append(CMD_TR_WRITE);   // cmd
        cmd.append(CMD_WAV);        // type cmd
        cmd.append(name.data(),max_lengh_name_file); // name
        cmd.append(data.data(),max_lengh_hander_wav); //
        data.remove(0, max_lengh_hander_wav);
        cmd.append((char*)&max_pask, 2);
        count_sound--;
        if ( first_package )
        {
            cmd.append( COD_CLEAR_WAV );
            first_package = false;
        }
        else
        {
            cmd.append( count_sound );
        }
        list.enqueue(cmd);
        // data wav
        for (int i = max_pask;--i>=0;)
        {
            QByteArray cmd;
            const quint16 len = max_lengh_data_wav+5; // CRC16(2) + CMD_WAV(1)+index_psk(2)
            cmd.append((char*)&len,sizeof(quint16));
            cmd.append(TYPE_UZTVOP);    // type
            cmd.append(CMD_TR_WRITE);   // cmd
            cmd.append(CMD_WAV);        // type cmd

            const int lengh(data.size());
            const char *const pBegin(data.data());
            if ( lengh<max_lengh_data_wav )
            {
                cmd.append(pBegin,lengh);
                const int lengh_null(max_lengh_data_wav-lengh);
                cmd.append(lengh_null, (char)0);
            }
            else
            {
                cmd.append(pBegin, max_lengh_data_wav);
                data.remove(0, max_lengh_data_wav);
            }
            // add number paskage
            cmd.append((char*)&i,sizeof(quint16));
            list.enqueue(cmd);
        }
    }
    collectTransportLevel(list);
    sendMessage(list);
    qDebug()<<"command Write";
}

void Controller::collectCommandRead(const QByteArray codefile, const quint16 index)
{
    QByteArray cmd;
    cmd.append(codefile.size()+(uint8_t)5);  // lengh low CMD_WAV(1)+crc(2)+index(2)
    cmd.append((char)0);     // lengh hi
    cmd.append(TYPE_UZTVOP); // type
    cmd.append(CMD_TR_READ); // cmd
    cmd.append(CMD_WAV);     // type cmd
    cmd.append(codefile);    // code file 4 byte
    cmd.append((char*)&index,sizeof(quint16)); // Package index
    collectTransportLevel( cmd );
    sendMessage( cmd );
    qDebug()<<"Command Read wav file:"<<cmd;

}
/**
 * @brief Controller::checkCommandReadAll
 * @return
 */
QByteArray Controller::checkCommandReadAll()
{
    if ( codeNameFile.isEmpty() )
        return QByteArray();
    const QByteArray codefile(iteratorNameFile.value());
    return codefile;
}
/**
 * @brief Controller::commandRead wav
 */
void Controller::commandReadAll()
{
    const QByteArray codefile(checkCommandReadAll());
    if ( !codefile.isEmpty() )
    {
        //
        startCommandWavFile();
        // collect command
        collectCommandRead(codefile, firstPackageIndex);
        return;
    }
    qDebug()<<"Error command read wav file!";
}
/**
 * @brief Controller::commandReadWAV
 */
void Controller::commandReadFileWAV()
{
    const QByteArray codefile(checkCommandReadAll());
    if ( !codefile.isEmpty() )
    {
        collectCommandRead(codefile, packageIndex);
        return;
    }
    qDebug()<<"Error Controller::commandReadAll()!";
}

/**
 * @brief Controller::commandWriteSetting
 */
void Controller::commandWriteSetting()
{

    QByteArray cmd;
    const quint16 lengh(sizeof(TYPEPROJECT)+(uint8_t)3);
    cmd.append((char*)&lengh,sizeof(uint16_t));
    cmd.append(TYPE_UZTVOP);    // type
    cmd.append(CMD_TR_WRITE);   // cmd
    cmd.append(CMD_PRJ);       // type cmd
    cmd.append(dataproject.retDataProject().getDataStructure());

    collectTransportLevel(cmd);
    sendMessage(cmd);
    qDebug()<<"command Write Setting";
}
/**
 * @brief Controller::commandReadSetting
 */
void Controller::commandReadSetting()
{

    QByteArray cmd;
    cmd.append((uint8_t)3);     // lengh low
    cmd.append((char)0);        // lengh hi
    cmd.append(TYPE_UZTVOP);    // type
    cmd.append(CMD_TR_READ);    // cmd
    cmd.append(CMD_PRJ);        // type cmd

    collectTransportLevel(cmd);
    sendMessage(cmd);
    qDebug()<<"command Read Setting";
}
/**
 * @brief Controller::commandSetTime
 */
void Controller::commandSetTime()
{
    QByteArray cmd;
    cmd.append(sizeof(timeDevice)+(uint8_t)3);// lengh time +crc +cmd
    cmd.append((char)0);
    cmd.append(TYPE_UZTVOP);    // type
    cmd.append(CMD_TR_WRITE);   // cmd
    cmd.append(CMD_TIME);       // type cmd
    cmd.append((const char*)&timeDevice,sizeof(timeDevice)); //data

    collectTransportLevel(cmd);
    sendMessage(cmd);
    struct tm * ptm;
    ptm = gmtime( (time_t*)&timeDevice.time );
    qDebug()<<"command Set Time: " <<timeDevice.time<<"GTM: "<<timeDevice.gmt<<"TIME hour: "<<ptm->tm_hour;
}
/**
 * @brief Controller::commandGetTime
 */
void Controller::commandGetTime()
{
    QByteArray cmd;
    cmd.append((uint8_t)3);     // lengh low
    cmd.append((char)0);        // lengh hi
    cmd.append(TYPE_UZTVOP);    // type
    cmd.append(CMD_TR_READ);    // cmd
    cmd.append(CMD_TIME);       // type cmd

    collectTransportLevel(cmd);
    sendMessage(cmd);
    qDebug()<<"command Get Time";
}
/**
 * @brief Controller::commandListen
 */
void Controller::commandListen()
{
    QByteArray cmd;
    cmd.append(sizeof(TYPE_TEST_TRACK)+(uint8_t)4);
    cmd.append((char)0);
    cmd.append(TYPE_UZTVOP);      // type
    cmd.append(CMD_TR_WRITE);     // cmd
    cmd.append(CMD_TEST);         // type cmd test first
    cmd.append(CMD_TEST_LISTEN);  // type cmd test second
    cmd.append((const char*)&listen,sizeof(TYPE_TEST_TRACK)); //data

    collectTransportLevel(cmd);
    sendMessage(cmd);
    qDebug()<<"command Listen";
}
/**
 * @brief Controller::commandTest
 */
void Controller::commandTest()
{
    QByteArray cmd;
    cmd.append((uint8_t)4);
    cmd.append((char)0);
    cmd.append(TYPE_UZTVOP);    // type
    cmd.append(CMD_TR_READ);    // cmd
    cmd.append(CMD_TEST);       // type cmd test
    cmd.append(CMD_TEST_DEVICE);// type cmd test

    collectTransportLevel(cmd);
    sendMessage(cmd);
    qDebug()<<"command Test";
}
/**
 * @brief Controller::commandWriteProject
 * @return
 */
RET_ANSWER Controller::commandWriteProject(const char *, const int){return SUCCESSFULLY;}
/**
 * @brief Controller::commandReadProject
 * @param p
 * @param lengh
 * @return
 */
RET_ANSWER Controller::commandReadProject(const char *p, const int lengh)
{
    const int cound_data = dataproject.retDataProject().getMaxDate();

    if (cound_data == lengh)
    {
        QByteArray date( p,lengh );
        dataproject.retDataProject().setDateStructure( date );
        return SUCCESSFULLY;
    }
    setMessageError(tr("<CENTER><b>Data Project is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}
/**
 * @brief Controller::commandWriteWAV
 * @return
 */
RET_ANSWER Controller::commandWriteWAV(const char *, const int){ return SUCCESSFULLY; }
/**
 * @brief Controller::commandReadWAV
 * @param pDate
 * @param lengh
 * @return
 */
RET_ANSWER Controller::commandReadWAV(const char *pDate, const int lengh)
{
    const quint16 data_wav(max_lengh_data_wav+2);
    const quint16 prefix_wav_long(max_lengh_name_file+max_lengh_hander_wav+2+1);
    const quint16 prefix_wav_short(max_lengh_name_file+max_lengh_hander_wav+2);

    if ( lengh == data_wav ) // data wav file
    {
        if ( lookingEndWavFile() )
        {
            if ( finishCommandWavFile() )
            {
                return SUCCESSFULLY;
            }
        }
        tmpRaedDataWav[namefile].append(pDate,lengh); // save data
        commandReadFileWAV(); // send command
        return SUCCESSFULLY;
    }

    if ( (lengh == prefix_wav_long) || (lengh == prefix_wav_short) ) // prefix wav file
    {
        const char *pTotalPackages = pDate+max_lengh_name_file+max_lengh_hander_wav;
        const char *pBeginWavFiles = pDate+max_lengh_name_file;
        memcpy(&packageIndex,pTotalPackages,sizeof(quint16));
        packageIndex -= 1;
        QString name_file(pDate);
        namefile = name_file;
        tmpRaedDataWav[namefile].clear();
        tmpRaedDataWav[namefile].append(pBeginWavFiles,max_lengh_hander_wav);
        commandReadFileWAV(); // send command
        return SUCCESSFULLY;
    }

    if (lengh == 2) // command OK
    {
        if ( !finishCommandWavFile() )
        {
            commandReadFileWAV(); // send command
        }
        return SUCCESSFULLY;
    }

    setMessageError(tr("<CENTER><b>WAV file is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}
/**
 * @brief Controller::commandSetTime
 * @return
 */
RET_ANSWER Controller::commandWriteTime(const char *, const int ){ return SUCCESSFULLY; }
/**
 * @brief Controller::commandGetTime
 * @param pDate
 * @param lengh
 * @return
 */
RET_ANSWER Controller::commandReadTime(const char *pDate, const int lengh)
{
    TYPETIME time = {};
    if ( sizeof(time) == lengh)
    {
        memcpy(&time,pDate,lengh);
        emit signalTime(time);
        return SUCCESSFULLY;
    }
    setMessageError(tr("<CENTER><b>Data Time is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}
/**
 * @brief Controller::commandWriteTest
 * @return
 */
RET_ANSWER Controller::commandWriteTest(const char *, const int){ return SUCCESSFULLY; }
/**
 * @brief Controller::commandTest
 * @param pDate
 * @param lengh
 * @return
 */
RET_ANSWER Controller::commandReadTest(const char *pDate, const int lengh)
{
    TYPE_TEST test = {};
    const int lengh_test(sizeof(test));
    if (lengh_test==lengh)
    {
        memcpy(&test,pDate,sizeof(test));
        emit signalTest(test);
         return SUCCESSFULLY;
    }
    setMessageError(tr("<CENTER><b>Data Controller::commandReadTest is incorrect!</CENTER></b>"));
    // stop timer diagnosis
    emit signalTimerDiagnosisDisabled();
    return ERROR_ANSWER;
}
/**
 * @brief Controller::commandRead
 * @param p
 * @param lengh
 * @return
 */
RET_ANSWER Controller::commandRead(const char *p, const int lengh)
{
    const int number_cmd = *p;
    return ( number_cmd<END_TYPE_NUMBER_COMMAND )?(this->*command_get[number_cmd])(p+1,lengh-1):ERROR_ANSWER;
}
/**
 * @brief Controller::lookingEndWavFile
 */
bool Controller::lookingEndWavFile()
{
    if ( packageIndex == 0 )
    {
        packageIndex = firstPackageIndex;
        return true;
    }
    packageIndex -=1;
    return false;
}
/**
 * @brief Controller::nextCommandWavFile
 * @return
 */
bool Controller::finishCommandWavFile()
{
    bool answer = false;
    if ( iteratorNameFile!= codeNameFile.constEnd() )
    {
        iteratorNameFile++;
        // save one wav file
    }
    else
    {
        // save one wav file
        // start
        startCommandWavFile();
        answer = true;
    }
    packageIndex = firstPackageIndex;

    return answer;
}
/**
 * @brief Controller::startCommandWavFile
 */
void Controller::startCommandWavFile()
{
    iteratorNameFile = codeNameFile.constBegin();
    tmpRaedDataWav.clear();
}
/**
 * @brief Controller::commandWrite
 * @param p
 * @param lengh
 * @return
 */
RET_ANSWER Controller::commandWrite(const char *p, const int lengh)
{
    const int number_cmd = *p;
    return ( number_cmd<END_TYPE_NUMBER_COMMAND )?(this->*command_set[number_cmd])(p,lengh):ERROR_ANSWER;

}
/**
 * @brief Controller::collectTransportLevel
 * @param cmd
 * @return
 */
void Controller::collectTransportLevel(QQueue<QByteArray> &list)
{
    for (int i=0; i<list.count(); i++)
    {
        list[i].prepend(header.pHeader2,header.lengn2);
        quint16 crc16 = CRC::Bit16( list.at(i) );
        list[i].append((char*)&crc16,sizeof(crc16));
        list[i].prepend(header.pHeader1,header.lengn1);
    }
}
/**
 * @brief collectTransportLevel
 * @param cmd
 * @return
 */
void Controller::collectTransportLevel( QByteArray &cmd )
{
    cmd.prepend(header.pHeader2,header.lengn2);
    quint16 crc16 = CRC::Bit16( cmd );
    cmd.append((char*)&crc16,sizeof(crc16));
    cmd.prepend(header.pHeader1,header.lengn1);
}
/**
 * @brief Controller::checksumReceived
 * @param cmd
 * @return
 */
RET_ANSWER Controller::checksumReceived(const QByteArray &cmd)
{
    RET_ANSWER result = ERROR_ANSWER;

    if ( cmd.size()<10 )
    {
        return DELAY_ANSWER;
    }
    const int lehgn = cmd.length();
    const char *pLengh_p(cmd.data()+3); // 4 byte
    quint16 lengh_p = 0;
    memcpy(&lengh_p,pLengh_p,sizeof(quint16)); //
    const int max_date(lengh_p+7);
    if ( max_date>lehgn )// error packet length
    {
        qDebug()<<"Length error of the received packet: protocol.cpp";
        return DELAY_ANSWER;
    }
    // Prune the tail
    const QByteArray cmdl(cmd.left(max_date));

    if ( !CRC::Bit16(cmdl) ) // CRC ok
    {
        const quint8 answer = cmdl.at(7); //
        if ( answer != 0xFF )
        {
            const quint16 number_w_r = cmdl.at(6)-1;
            if ( number_w_r<END_TYPE_NUMBER_COMMAND_TRANSPORT )
            {
                const char *begin_data(cmdl.data()+8);   // begin pointer date without a reply byte
                const int lengh(cmdl.count()-10);         // lehgn date without a reply byte
                result= (this->*command_read[number_w_r])(begin_data,lengh);
            }
        }    // bite 7
        else // [answer][command][coderror]
        {
            const QString message(
                        QString("<CENTER><b>Code error = %1</CENTER></b>").arg(
                            QString::number(cmdl.at(9))));
            setMessageError(message);
        }
    }
    else
    {
        setMessageError(tr("<CENTER><b>Error CRC16!</CENTER></b>"));
    }

    return result;
}
/**
 * @brief Controller::parserReceivedPacket
 * @return
 */
RET_ANSWER Controller::parserReceivedPacket(const QByteArray &cmd)
{
    static QByteArray readBuffer;
    static TYPE_STEP step = STEP_ONE;
    static int counter = 0;
    const int max_repeat = (1000/currentInterface->retTimeDelay());
    RET_ANSWER result = ERROR_ANSWER;
    const QByteArray HEAD(header.pHeader2,header.lengn2);

    switch(step)
    {
        case STEP_ONE:
            counter = 0;
            readBuffer.clear();
            step = STEP_TWO;
        case STEP_TWO:
        {
            //time is over
            if ( ++counter>max_repeat )
            {
                step = STEP_ERROR;
            }
            else
            {
                readBuffer.append(cmd);
                const int index = readBuffer.indexOf(HEAD);
                if ( index !=-1 )
                {
                    result = checksumReceived(readBuffer.remove(0,index));
                    step = (result!=DELAY_ANSWER)?STEP_ONE:STEP_TWO;
                    return result;
                }
            }
        }
        return DELAY_ANSWER;

        case STEP_ERROR:
        default:
        {
            step = STEP_ONE;
            setMessageError(tr("<CENTER><b>Device does not answer!</CENTER></b>"));
        }
        return NO_ANSWER;
    }
}
/**
 * @brief Controller::sendOutputMessage
 */
void Controller::sendOutputMessage()
{
    if (!messageError.isEmpty())
    {
        emit signalMessageError(messageError);
        messageError.clear();
    }
    else if (!messageOk.isEmpty())
    {
        emit signalMessageOk(messageOk);
        messageOk.clear();
    }
}
/**
 * @brief Controller::nextAnswerWavTest
 * @return
 */
QByteArray Controller::nextAnswerWavTest()
{
    QByteArray retData;
#ifdef DEBUG_ANSWER
        //
        //0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x05,0x00,0x10,0x01,0x00,0x03,0x00,0x37,0x2F
        //
        //0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x05,0x00,0x10,0x01,0x00,0x00,0x00,0x37,0xDF
        //test
        //0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x1B,0x00,0x10,0x02,0x00,0x00,0x2D,0x5E,0x42,0x59,0x00,
        //0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x1E,0x1E,0x00,0x00,0x40,0xAC
//        const uint8_t BuffFistWav[] ={
//                // read
//                0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x73,0x00,0x10,0x02,0x00,0x02,0x32,0x30,0x30,0x30,0x5F,0x30,0x32,
//                0x30,0x30,0x5F,0xD0,0xA2,0xD0,0xB8,0xD0,0xBA,0x20,0xD1,0x82,0xD0,0xB0,0xD0,0xBA,0x20,0x31,0xD1,0x81,0x20,
//                0x32,0xD1,0x82,0xD0,0xB8,0xD0,0xBA,0xD0,0xB0,0x2E,0x77,0x61,0x76,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,
//                0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x06,0x01,0x00,0x00,0x03,0x00,0x00,0x00,0x52,0x49,0x46,0x46,0x24,0xAB,
//                0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6D,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x22,0x56,0x00,
//                0x00,0x44,0xAC,0x00,0x00,0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x00,0xAB,0x00,0x00,0x16,0x00,0x02,0x53,0xA4
//         };
    // extract sound files
    static QQueue<QByteArray> list;
    static TYPE_STEP step = STEP_ONE;
switch(step)
{
    case STEP_ONE:
    {
        list.clear();
        QMap <QString, QByteArray>::iterator it = dataproject.retPlayList().begin();
        quint8 count_sound = dataproject.retPlayList().size();
        bool first_package = true;

        for (;it!= dataproject.retPlayList().end(); ++it)
        {
            QByteArray name;name.append(it.key());
            QByteArray data(it.value());
            const quint16 max_pask = (data.size()/max_lengh_data_wav) +1;
            //first wav
            QByteArray cmd;
            const quint16 lengh = max_lengh_name_file+max_lengh_hander_wav+2+1+4;
            cmd.append((char*)&lengh,sizeof(quint16));
            cmd.append(TYPE_UZTVOP);    // type
            cmd.append(CMD_TR_READ);   // cmd
            cmd.append((char)0);
            cmd.append(CMD_WAV);        // type cmd
            cmd.append(name.data(),max_lengh_name_file); // name
            cmd.append(data.data(),max_lengh_hander_wav); //
            data.remove(0, max_lengh_hander_wav);
            cmd.append((char*)&max_pask, 2);
            count_sound--;
            if ( first_package )
            {
                cmd.append( COD_CLEAR_WAV );
                first_package = false;
            }
            else
            {
                cmd.append( count_sound );
            }
            list.enqueue(cmd);
            // data wav
            for (int i = max_pask;--i>=0;)
            {
                QByteArray cmd;
                const quint16 len = max_lengh_data_wav+6; // CRC16(2) + CMD_WAV(1)+index_psk(2)
                cmd.append((char*)&len,sizeof(quint16));
                cmd.append(TYPE_UZTVOP);    // type
                cmd.append(CMD_TR_READ);   // cmd
                cmd.append((char)0);
                cmd.append(CMD_WAV);        // type cmd

                const int lengh(data.size());
                const char *const pBegin(data.data());
                if ( lengh<max_lengh_data_wav )
                {
                    cmd.append(pBegin,lengh);
                    const int lengh_null(max_lengh_data_wav-lengh);
                    cmd.append(lengh_null, (char)0);
                }
                else
                {
                    cmd.append(pBegin, max_lengh_data_wav);
                    data.remove(0, max_lengh_data_wav);
                }
                // add number paskage
                cmd.append((char*)&i,sizeof(quint16));
                list.enqueue(cmd);
            }
        }
        collectTransportLevel(list);

        step = STEP_TWO;
    }
    case STEP_TWO:

        if (!list.isEmpty())
        {
            retData.append(list.dequeue());
        }
        else
        {
            step = STEP_THREE;
        }
    break;

    default:
        retData.clear();
    break;
}
#endif
    return retData;
}
/**
 * @brief Controller::Controller
 * @param parent
 */
Controller::Controller( QObject *parent ) :
                    QObject(parent),
                    timerRead(new QTimer(parent)),
                    max_lengh_data_wav(1024*2),
                    max_lengh_hander_wav(44),
                    max_lengh_name_file(64)
{
    connect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()),Qt::DirectConnection);
    connect(this,SIGNAL(signalStep()),this,SLOT(on_Machine()),Qt::DirectConnection);
    connect(timerRead,SIGNAL(timeout()),this,SLOT(on_Machine()),Qt::DirectConnection);
    interface<<new ComPort()<<new LinkWIFI();
    checkInterface(INTERFACE_COM);
    // test name file
    codeNameFile.insert(0,"2000");
    codeNameFile.insert(1,"2100");
    codeNameFile.insert(2,"0001");
    codeNameFile.insert(3,"0010");
    codeNameFile.insert(4,"0020");
    codeNameFile.insert(5,"0030");
    codeNameFile.insert(6,"0040");
    codeNameFile.insert(7,"0050");
    codeNameFile.insert(8,"0060");
    codeNameFile.insert(9,"0090");
    codeNameFile.insert(10,"0002");
    codeNameFile.insert(11,"0100");
    codeNameFile.insert(12,"0200");
    codeNameFile.insert(13,"0300");
    codeNameFile.insert(14,"0400");
    codeNameFile.insert(15,"0500");
    codeNameFile.insert(16,"0600");
    codeNameFile.insert(17,"0900");
    codeNameFile.insert(18,"3000");
    iteratorNameFile = codeNameFile.constBegin();
}
/**
 * @brief Controller::on_Machine
 */
void Controller::on_Machine()
{
    switch(stat)
    {
        case stOpen:
            if(!listCMD.isEmpty())
            {
                if(currentInterface->open())
                {
                    stat=stCounter;
                    disconnect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()));
                    timerRead->setInterval(currentInterface->retTimeDelay());
                    templistCMD = listCMD;
                    currentValueProgress = 0;
                    const int valueMax = 3*templistCMD.count();
                    emit signalProgressRange(0,valueMax);
                    emit signalProgressVisible(progressVisible);
                }else{
                    stat = stStopExchange;
                    setMessageError(tr("<CENTER><b>Interface open ERROR!</CENTER></b>"));
                }
            }
            listCMD.clear();
            emit signalStep();
        return;

        case stCounter:
            if ( !listCMD.isEmpty() )
            {
                templistCMD = listCMD;
                listCMD.clear();
            }
            if( !templistCMD.isEmpty() )
            {
                emit signalProgressValue(++currentValueProgress);
                sendCmd.clear();
                sendCmd.append(templistCMD.dequeue());
                countSend = 0;
                stat=stWrite;
            }else{
                stat=stClose;
            }
            emit signalStep();
        return;

        case stRepeat:
            if (++countSend<max_repeat)
            {
                stat = stWrite;
            }
            else
            {
                templistCMD.clear();
                stat = stStopExchange;
            }
            emit signalStep();
        return;

        case stWrite:
        {
            currentInterface->sendDate(sendCmd);
            emit signalSendMessage(sendCmd,Qt::green);
            emit signalProgressValue(++currentValueProgress);
            stat=stRead;
            timerRead->start();
        }
        return;

        case stRead:
        {
            QByteArray resBuff;
#ifdef DEBUG_ANSWER
            resBuff.append(nextAnswerWavTest());
#else
            currentInterface->readDate(resBuff);
#endif
            emit signalSendMessage(resBuff,Qt::darkYellow);
            const RET_ANSWER answer = parserReceivedPacket(resBuff);
            if ( answer == DELAY_ANSWER) // next package
            {
                return;
            }
            timerRead->stop();
            stat = (answer != SUCCESSFULLY)?stRepeat:stCounter;
            const int progress = (answer != SUCCESSFULLY)?currentValueProgress:++currentValueProgress;
            emit signalProgressValue(progress);
            emit signalStep();
        }
        return;

        case stStopExchange:
            emit signalTimerDiagnosisDisabled();
        case stClose:
            currentInterface->close();
        default:
            stat = stOpen;
            timerRead->stop();
            connect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()),Qt::DirectConnection);
            //connect(this,&Controller::signalProgressEnabled,this,)
            emit signalProgressValue(0);
            emit signalProgressVisible(false);
            emit signalStop();
            sendOutputMessage();
        return;
    }
}
//// test --------------------------------------------------------
//QByteArray cmd;
//QDateTime timeDevice(QDateTime::currentDateTime());
//const TYPETIME time{
//        .time = timeDevice.toTime_t(),
//        .gmt  = static_cast<uint8_t>(timeDevice.offsetFromUtc()/3600)
//};
//cmd.append(sizeof(time)+(uint8_t)4);// lengh time +crc +cmd
//cmd.append((char)0);
//cmd.append(TYPE_UZTVOP);    // type
//cmd.append(CMD_TR_READ);    // cmd
//cmd.append((char)0);        // answer
//cmd.append(CMD_TIME);       // type cmd
//cmd.append((const char*)&time,sizeof(time));
//collectTransportLevel(cmd);
//QByteArray cmd1(cmd.data(),3);cmd.remove(0,3);
//QByteArray cmd2(cmd.data(),7);cmd.remove(0,7);
//QByteArray cmd3(cmd.data(),3);cmd.remove(0,3);
//QByteArray cmd4(cmd);cmd.clear();
//QVector <QByteArray> list;
//list.append(cmd);
//list.append(cmd1);
//list.append(cmd);
//list.append(cmd2);
//list.append(cmd);
//list.append(cmd3);
//list.append(cmd);
//list.append(cmd4);
//QByteArray cmd_res(index<list.size()?list.at(index++):QByteArray());
//QByteArray resBuff;
//resBuff.append(cmd_res);
//              QByteArray cmd;
//              cmd.append((uint8_t)4);
//              cmd.append((char)0);
//              cmd.append(TYPE_UZTVOP);    // type
//              cmd.append(CMD_TR_WRITE);    // cmd
//              cmd.append((char)0);       // type cmd test
//              cmd.append(CMD_TIME);// type cmd test
//              collectTransportLevel(cmd);

//// test --------------------------------------------------------
/**
 * @brief Controller::checkInterface
 * @param number
 */
void Controller::checkInterface(const int number)
{
    if (number<interface.count())
    {
        currentInterface = interface.at(number);
    }else{
        emit signalMessageError(tr("<CENTER><b>Number interface!</CENTER></b>"));
    }

}
/**
 * @brief Controller::sendMessage
 * @param ba
 */
void Controller::sendMessage(QQueue<QByteArray> &ba)
{
    listCMD.clear();
    listCMD = ba;
    emit signalStart();
}
/**
 * @brief Controller::sendMessage
 * @param ba
 */
void Controller::sendMessage(QByteArray &ba)
{
    listCMD.enqueue(ba);
    emit signalStart();
}
/**
 * @brief Controller::stopMessage
 */
void Controller::stopMessage()
{
    listCMD.clear();
    connect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()),Qt::DirectConnection);
}
/**
 * @brief Controller::commandRun
 * @param numberCmd
 */
void Controller::commandRun(const int numberCmd)
{
    if (numberCmd<TYPE_COMMAND::END_TYPE_COMMAND)
    {
        // disabled progressbar
        progressVisible = (numberCmd == cmTest)?false:true;
        // send command
        (this->*command_send[numberCmd])();
    }
}
/**
 * @brief Controller::setParametrWIFI
 */
void Controller::setParametrWIFI(const QStringList &list)
{
    LinkInreface *const wifi(interface.at(INTERFACE_WIFI));
    wifi->connections(list);
    qDebug()<<list;
}
/**
 * @brief Controller::setParametrCOMPORT
 */
void Controller::setParametrCOMPORT(const QStringList &list)
{

    LinkInreface *const com(interface.at(INTERFACE_COM));
    com->connections(list);
    qDebug()<<list;
}
/**
 * @brief Controller::setDateTime
 */
void Controller::setDateTime(const QDateTime &datatime)
{
    QDateTime dt(datatime);
    dt.setTimeZone(QTimeZone::utc());
    timeDevice.time = dt.toTime_t();
    qDebug()<<dt;
}
/**
 * @brief Controller::setDateTimeZone
 * @param num_zone
 */
void Controller::setDateTimeZone(const int num_zone)
{
    const int indexUTC0 = 12;
    timeDevice.gmt = num_zone - indexUTC0;
}
/**
 * @brief Controller::setListenTest
 */
void Controller::setListenTest(const TYPE_TEST_TRACK &tmplisten)
{
    memcpy(&listen,&tmplisten,sizeof(listen));
}
