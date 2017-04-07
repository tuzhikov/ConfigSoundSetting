#include "createholidays.h"
using namespace UnitHolidays;

CreateHolidays::CreateHolidays(QWidget *parent,
                               const QString &text,
                               const int mounth,
                               const int day,
                               const int num_plan) : QWidget(parent)
{
    lbText = new QLabel(text,this);
    sbMounth = new QSpinBox(this);
    sbMounth->setValue(mounth);
    sbDay = new QSpinBox(this);
    sbDay->setValue(day);
    sbNumberPlan = new QSpinBox(this);
    sbNumberPlan->setValue(num_plan);
    btDelete = new QPushButton(tr("REMOVE"),this);

    QHBoxLayout*hLayout = new QHBoxLayout(this);
    hLayout->addWidget(lbText);
    hLayout->addWidget(sbMounth);
    hLayout->addWidget(sbDay);
    hLayout->addWidget(sbNumberPlan);
    hLayout->addWidget(btDelete);
    this->setLayout(hLayout);

    connect(btDelete,SIGNAL(clicked(bool)),this,SIGNAL(click(bool)));
}
