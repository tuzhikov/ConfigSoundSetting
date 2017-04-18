#ifndef INTERFACEFORMS_H
#define INTERFACEFORMS_H

#include <QQueue>
#include <QWidget>
//class QWidget;
//class QByteArray;

/**
 * @brief The InterfaceForms class
 */
class InterfaceForms : public QWidget
{
public:
    explicit InterfaceForms(QWidget *parent = 0):QWidget(parent){}
    //virtual ~InterfaceForms(){}
    //virtual void ReadData(QQueue<QByteArray> &)=0;
    //virtual void WriteData(QQueue<QByteArray> &)=0;
    //virtual const int retMaxNumberElements()=0;
    //virtual const int retMaxValue()=0;
    virtual void setNumberItem( const int number ) = 0;
};

#endif // INTERFACEFORMS_H
