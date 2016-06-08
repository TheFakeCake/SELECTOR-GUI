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

public slots:

    void setValue(const ABCInterval<int> &value);

signals:

    void valueChanged(ABCInterval<int> newValue);

private slots:

    void on_ABC_toggled(bool checked);

private:

    void blockUISignals(bool b);

    Ui::ABCIntervalIntWidget *ui;
};

#endif // ABCINTERVALINTWIDGET_H
