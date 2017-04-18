#ifndef CREATEHOLIDAYS_H
#define CREATEHOLIDAYS_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateEdit>
#include "InterfaceForms.h"

class QLabel;
class QSpinBox;
class QPushButton;
class QHBoxLayout;

namespace UnitHolidays{

    class CreateHolidays : public InterfaceForms
    {
        Q_OBJECT

        QLabel *lbText;
        QDateEdit *dataEdit;
        QSpinBox *sbNumberPlan;
        QPushButton *btDelete;
        QSpacerItem *horizontalSpacer;

    public:
        explicit CreateHolidays(QWidget *parent     = 0,
                                const int index     = 1,
                                const QDate date = QDate::currentDate(),
                                const int num_plan  = 1);
        void setNumberItem( const int number );
        void setDateEdit( const QDate date );
        void setNumberPlan( const int number );
        void setMaxNumberPlan( const int number );
        int getNumberItem( void ) const;
        QDate getDateEdit( void ) const;
        int getNumberPlan( void ) const;
        int getMaxNumberPlan( void ) const;
    signals:
    public slots:
        void setMaxPlan(const int);
    };
}
#endif // CREATEHOLIDAYS_H
