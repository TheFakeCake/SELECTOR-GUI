#ifndef ABCINTERVALINTWIDGET_H
#define ABCINTERVALINTWIDGET_H

#include <QWidget>
#include "../models/ABCInterval.h"

namespace Ui {
class ABCIntervalIntWidget;
}

class ABCIntervalIntWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ABCIntervalIntWidget(QWidget *parent = 0);
    ~ABCIntervalIntWidget();

    ABCInterval<int> value() const;

    void setMinimumValue(int min);
    void setMaximumValue(int max);
    void setSpecialValueText(const QString &text);
    void blockSignals(bool b);

public slots:

    void setValue(const ABCInterval<int> &value);

signals:

    void valueChanged(ABCInterval<int> newValue);
    void singleValueChanged(int newValue);
    void maximumChanged(int newMax);
    void minimumChanged(int newMin);
    void distributionChanged(int newDistribution);

private slots:

    void on_ABC_toggled(bool checked);
    void on_singleValue_valueChanged(int arg1);
    void on_minValue_valueChanged(int arg1);
    void on_maxValue_valueChanged(int arg1);
    void on_distribution_currentIndexChanged(int index);

private:

    Ui::ABCIntervalIntWidget *ui;
};

#endif // ABCINTERVALINTWIDGET_H
