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
    void blockSignals(bool b);

public slots:

    void setValue(const ABCInterval<int> &value);

signals:

    void valueChanged(ABCInterval<int> newValue);
    void singleValueChanged(int newValue);
    void firstBoundChanged(int newFirstBound);
    void secondBoundChanged(int newLastBound);
    void distributionChanged(int newDistribution);

protected slots:

    void on_ABC_toggled(bool checked);
    void on_singleValue_valueChanged(int value);
    void on_firstBound_valueChanged(int value);
    void on_secondBound_valueChanged(int value);
    void on_distribution_currentIndexChanged(int index);

protected:

    Ui::ABCIntervalIntWidget *ui;
};

#endif // ABCINTERVALINTWIDGET_H
