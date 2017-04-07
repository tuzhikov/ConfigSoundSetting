#ifndef CREATEHOLIDAYS_H
#define CREATEHOLIDAYS_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>

class QLabel;
class QSpinBox;
class QPushButton;
class QHBoxLayout;

namespace UnitHolidays{

class CreateHolidays : public QWidget
{
    Q_OBJECT
    QLabel *lbText;
    QSpinBox *sbMounth;
    QSpinBox *sbDay;
    QSpinBox *sbNumberPlan;
    QPushButton *btDelete;
public:
    explicit CreateHolidays(QWidget *parent = 0,
                            const QString &text = "1",
                            const int mounth = 1,
                            const int day = 1,
                            const int num_plan = 1);

signals:
    void click(bool);

public slots:
};
}
#endif // CREATEHOLIDAYS_H
