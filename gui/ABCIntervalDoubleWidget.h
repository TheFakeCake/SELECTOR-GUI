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
    void blockSignals(bool b);

public slots:

    void setValue(const ABCInterval<double> &value);

signals:

    void valueChanged(ABCInterval<double> newValue);
    void singleValueChanged(double newValue);
    void firstBoundChanged(double newFirstBound);
    void secondBoundChanged(double newLastBound);
    void distributionChanged(int newDistribution);

private slots:

    void on_ABC_toggled(bool checked);
    void on_singleValue_valueChanged(double value);
    void on_firstBound_valueChanged(double value);
    void on_secondBound_valueChanged(double value);
    void on_distribution_currentIndexChanged(int index);

protected:

    Ui::ABCIntervalDoubleWidget *ui;
};

#endif // ABCINTERVALDOUBLEWIDGET_H
