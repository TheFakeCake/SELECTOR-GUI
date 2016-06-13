#ifndef ABCINTERVALDOUBLEWIDGET_H
#define ABCINTERVALDOUBLEWIDGET_H

#include <QWidget>
#include "../models/ABCInterval.h"

namespace Ui {
class ABCIntervalDoubleWidget;
}

class ABCIntervalDoubleWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ABCIntervalDoubleWidget(QWidget *parent = 0);
    ~ABCIntervalDoubleWidget();

    ABCInterval<double> value() const;

    void setMinimumValue(double min);
    void setMaximumValue(double max);
    void setSpecialValueText(const QString &text);

public slots:

    void setValue(const ABCInterval<double> &value);

signals:

    void valueChanged(ABCInterval<double> newValue);
    void maximumChanged(double newMax);
    void minimumChanged(double newMin);
    void distributionChanged(int newDistribution);

private slots:

    void on_ABC_toggled(bool checked);
    void on_singleValue_valueChanged(double arg1);
    void on_minValue_valueChanged(double arg1);
    void on_maxValue_valueChanged(double arg1);
    void on_distribution_currentIndexChanged(int index);

private:

    void blockUISignals(bool b);

    Ui::ABCIntervalDoubleWidget *ui;
};

#endif // ABCINTERVALDOUBLEWIDGET_H
