#ifndef CREATEDAYPLAN_H
#define CREATEDAYPLAN_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimeEdit>
class CreateDayPlan : public QWidget
{
    Q_OBJECT
    QLabel *lbText;
    QTimeEdit *time;
    QSpinBox *value1;
    QSpinBox *value2;
    QPushButton *btDelete;

public:
    explicit CreateDayPlan(QWidget *parent = 0,
                           const QString &text = "1");

signals:
    void click(bool);

public slots:
};

#endif // CREATEDAYPLAN_H
