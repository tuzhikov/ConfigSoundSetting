#ifndef INTERFACEFORMS_H
#define INTERFACEFORMS_H

#include <QQueue>
class QWidget;
class QByteArray;

/**
 * @brief The InterfaceForms class
 */
class InterfaceForms : public QWidget
{
public:
    explicit InterfaceForms(QWidget *parent = 0):QWidget(parent){}
    virtual void ReadData(QQueue<QByteArray> &)=0;
    virtual void WriteData(QQueue<QByteArray> &)=0;
    virtual const int retMaxNumberElements()=0;
    virtual const int retMaxValue()=0;
};

#endif // INTERFACEFORMS_H
