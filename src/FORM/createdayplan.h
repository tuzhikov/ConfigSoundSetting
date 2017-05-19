#ifndef CREATEDAYPLAN_H
#define CREATEDAYPLAN_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimeEdit>
#include "InterfaceForms.h"

namespace UnitDeyPlan{
    class CreateDayPlan : public InterfaceForms
    {
        Q_OBJECT
        QLabel *lbText;
        QTimeEdit *timeEdit;
        QSpinBox *value1;
        QSpinBox *value2;
        QSpacerItem *horizontalSpacer;

    public:
        explicit CreateDayPlan(QWidget *parent = 0,
                               const int index = 1,
                               const QTime &time = QTime::currentTime(),
                               const int val1 = 30,
                               const int val2 = 70);
        void setNumberItem( const int number );
        int getNumberItem( void ) const;
        void setTime(const QTime time );
        QTime getTime( void ) const;
        void setVolume( const int val1, const int val2 );
        void getVolume( int *const val1, int *const val2 );
    signals:
    public slots:
    };
}

#endif // CREATEDAYPLAN_H
