#include "createdayplan.h"

using namespace UnitDeyPlan;

CreateDayPlan::CreateDayPlan(QWidget *parent,
                             const int index,
                             const QTime &time,
                             const int val1,
                             const int val2) : InterfaceForms(parent)
{
    lbText = new QLabel(QString::number(index)+": ",parent);
    timeEdit = new QTimeEdit(time,parent);
    value1 = new QSpinBox(parent);
    value1->setValue(val1);value1->setRange(0,100); //100 %
    value2 = new QSpinBox(parent);
    value2->setValue(val2);value2->setRange(0,100); //100%
    horizontalSpacer = new QSpacerItem(217, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout*hLayout = new QHBoxLayout(parent);
    hLayout->addWidget(lbText);
    QLabel *text1 = new QLabel(tr("Time (h:m): "),parent);
    hLayout->addWidget(text1);
    hLayout->addWidget(timeEdit);
    QLabel *text2 = new QLabel(tr("Speaker 1 (%): "),parent);
    hLayout->addWidget(text2);
    hLayout->addWidget(value1);
    QLabel *text3 = new QLabel(tr("Speaker 2 (%): "),parent);
    hLayout->addWidget(text3);
    hLayout->addWidget(value2);
    hLayout->addSpacerItem(horizontalSpacer);
    this->setLayout(hLayout);
    this->setMinimumHeight(30);
}
/**
 * @brief CreateDayPlan::setNumberItem
 * @param number
 */
void CreateDayPlan::setNumberItem(const int number)
{
    lbText->setText(QString::number(number)+": ");
}
