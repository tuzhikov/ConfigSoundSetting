#include "accessdataproject.h"
/**
 * @brief AccessDataProject::AccessDataProject
 */
AccessDataProject::AccessDataProject()
{

}
/**
 * @brief AccessDataProject::~AccessDataProject
 */
AccessDataProject::~AccessDataProject()
{

}
/**
 * @brief AccessDataProject::readProject
 * @param stream
 */
void AccessDataProject::readFileToProject(QDataStream &stream)
{
    AccessData tmpProject;
    QVector <QByteArray> tmpTracks;
    stream>>tmpProject>>tmpTracks;

    if (tmpProject.checkCRC())
    {
        dataProject.clear();
        tracks.clear();
        tracks.append(tmpTracks);
        QByteArray data(tmpProject.getDataStructure());
        dataProject.setDateStructure(data);
    }
}

/**
 * @brief AccessDataProject::writeProject
 * @param stream
 */
void AccessDataProject::writeProjectToFile(QDataStream &stream)
{
    dataProject.checkCRC();
    stream<<dataProject<<tracks;
}

//quint16 AccessDataProject::checkSumm()
//{
//    tracks<<QByteArray("12345")<<QByteArray("67890")<<QByteArray("11111");
//    ///////////////////////////
//    //QByteArray tmpData;
//    QDataStream stream( &tmpData, QIODevice::WriteOnly );
//    stream<<dataProject<<tracks;
//    const char *pData = tmpData.constData();
//    const int lengh = tmpData.count();
//    const quint16 crc16 = qChecksum(pData,lengh);
//    /////////////
//    stream<<crc16;
//    tracks.clear();
//    dataProject.clear();
//    return crc16;
//}

//void AccessDataProject::tmpfunc()
//{
//    QDataStream stream( &tmpData, QIODevice::ReadOnly );
//    int crc16 = 0;
//    stream>>dataProject>>tracks>>crc16;
//    QByteArray tmpData1;
//    QDataStream stream1( &tmpData1, QIODevice::WriteOnly );
//    stream1<<dataProject<<tracks;
//    const char *pData = tmpData1.constData();
//    const int lengh = tmpData1.count();
//    const quint16 crc162 = qChecksum(pData,lengh);
//    if ( crc16 == crc162 )
//    {

//    }
//}
