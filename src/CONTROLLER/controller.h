#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "src/ACCESSDATE/accessdataproject.h"

class Controller : public QObject
{
    Q_OBJECT

private:
    AccessDataProject dataproject;
public:
    explicit Controller( QObject *parent = 0 );
    ~Controller(){}
    inline AccessDataProject &retDataProject(){ return dataproject;}
};

#endif // CONTROLLER_H
