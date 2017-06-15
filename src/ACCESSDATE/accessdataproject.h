#ifndef ACCESSDATAPROJECT_H
#define ACCESSDATAPROJECT_H

#include <QVector>
#include <QDebug>
#include <QMap>
#include <QDataStream>
#include "accessdata.h"

using namespace ACCESS_DATE;

class AccessDataProject
{
private:
    AccessData dataProject;
    QMap <QString, QByteArray>listPlay;

public:
    AccessDataProject();
    ~AccessDataProject();
    void readFileToProject( QDataStream &stream );
    void writeProjectToFile( QDataStream &stream );
    void clearPlayList() {listPlay.clear();}
    inline AccessData &retDataProject( void ) {return dataProject;}
    inline QMap <QString, QByteArray> &retPlayList( void ) {return listPlay;}
};

#endif // ACCESSDATAPROJECT_H
