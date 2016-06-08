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

public slots:

    void setValue(const ABCInterval<double> &value);

signals:

    void valueChanged(ABCInterval<double> newValue);

private slots:

    void on_ABC_toggled(bool checked);

private:

    void blockUISignals(bool b);

    Ui::ABCIntervalDoubleWidget *ui;
};

#endif // ABCINTERVALDOUBLEWIDGET_H
