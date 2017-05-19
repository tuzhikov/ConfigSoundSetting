#ifndef ACCESSDATAPROJECT_H
#define ACCESSDATAPROJECT_H

#include <QVector>
#include <QDebug>
#include <QDataStream>
#include "accessdata.h"

using namespace ACCESS_DATE;

class AccessDataProject
{
private:
    AccessData dataProject;
    QVector <QByteArray> tracks;

public:
    AccessDataProject();
    ~AccessDataProject();
    void readFileToProject( QDataStream &stream );
    void writeProjectToFile( QDataStream &stream );
    void addNewTack( QByteArray &track ) {tracks<<track;}
    void clearTack() { tracks.clear(); }
    inline AccessData &retDataProject( void ) {return dataProject;}
};

#endif // ACCESSDATAPROJECT_H
