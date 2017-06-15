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
    QMap <QString, QByteArray>tmpPlayList;
    stream>>tmpProject>>tmpPlayList;

    if (tmpProject.checkCRC())
    {
        // clear
        dataProject.clearProject();
        clearPlayList();
        // add
        listPlay = tmpPlayList;
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
    stream<<dataProject<<listPlay;
}
