#include "createholidays.h"
using namespace UnitHolidays;
/**
 * @brief CreateHolidays::CreateHolidays
 * @param parent
 * @param index
 * @param mounth
 * @param day
 * @param maxPlan - maximum plan
 */
CreateHolidays::CreateHolidays(QWidget *parent,
                               const int index,
                               const QDate date,
                               const int maxPlan) : InterfaceForms(parent)
{
    lbText = new QLabel(QString::number(index)+": ",parent);
    dataEdit = new QDateEdit(date,parent);
    dataEdit->setCalendarPopup(true);
    dataEdit->setDisplayFormat("dd.MM");
    sbNumberPlan = new QSpinBox(parent);
    sbNumberPlan->setMaximum(maxPlan);
    //btDelete = new QPushButton(tr("REMOVE"),parent);
    horizontalSpacer = new QSpacerItem(217, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout*hLayout = new QHBoxLayout(parent);
    hLayout->addWidget(lbText);
    QLabel *text1 = new QLabel(tr("Day.Mounth: "),parent);
    hLayout->addWidget(text1);
    hLayout->addWidget(dataEdit);
    QLabel *text2 = new QLabel(tr("Plan: "),parent);
    hLayout->addWidget(text2);
    hLayout->addWidget(sbNumberPlan);
    //hLayout->addWidget(btDelete);
    hLayout->addSpacerItem(horizontalSpacer);
    this->setLayout(hLayout);
    this->setMinimumSize(0,30);
    // create connect
}
/**
 * @brief CreateHolidays::setNumberItem
 * @param number
 */
void CreateHolidays::setNumberItem(const int number)
{
    lbText->setText(QString::number(number)+": ");
}
/**
 * @brief CreateHolidays::setDateEdit
 * @param date
 */
void CreateHolidays::setDateEdit(const QDate date)
{
    dataEdit->setDate(date);
}
/**
 * @brief CreateHolidays::setNumberPlan
 * @param number
 */
void CreateHolidays::setNumberPlan(const int number)
{
    sbNumberPlan->setValue(number);
}
/**
 * @brief CreateHolidays::setMaxNumberPlan
 * @param number
 */
void CreateHolidays::setMaxNumberPlan(const int number)
{
    sbNumberPlan->setMaximum(number);
}
/**
 * @brief CreateHolidays::getNumberItem
 * @return
 */
int CreateHolidays::getNumberItem() const
{
    return lbText->text().toInt();
}
/**
 * @brief CreateHolidays::getDateEdit
 * @return
 */
QDate CreateHolidays::getDateEdit() const
{
    return dataEdit->date();
}
/**
 * @brief CreateHolidays::getNumberPlan
 * @return
 */
int CreateHolidays::getNumberPlan() const
{
    return sbNumberPlan->value();
}
/**
 * @brief CreateHolidays::getMaxNumberPlan
 * @return
 */
int CreateHolidays::getMaxNumberPlan() const
{
    return sbNumberPlan->maximum();
}

void CreateHolidays::setMaxPlan(const int maxPlan)
{
    sbNumberPlan->setMaximum(maxPlan);
}
