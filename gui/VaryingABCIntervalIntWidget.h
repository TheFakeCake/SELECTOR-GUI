#ifndef VARYINGABCINTERVALINTWIDGET_H
#define VARYINGABCINTERVALINTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include "MapWidget.h"

namespace Ui {
class ABCIntervalIntWidget;
}

class VaryingABCIntervalIntWidget : public QWidget
{
    Q_OBJECT

public:

    explicit VaryingABCIntervalIntWidget(QWidget *parent = nullptr);
    ~VaryingABCIntervalIntWidget();

    void setMinimumValue(int min);
    void setMaximumValue(int max);
    void init(MapWidget *mapWidget, ABCInterval<int> (Deme::*demeGetter)() const, void (Deme::*demeSetter)(ABCInterval<int>));

    void updateView(bool keepCurrentStackIndex = false);

private slots:

    void on_ABC_toggled(bool checked);
    void on_singleValue_valueChanged(int value);
    void on_firstBound_valueChanged(int value);
    void on_secondBound_valueChanged(int value);
    void on_distribution_currentIndexChanged(int index);

private:

    void setIntervalValueToSelection(int value, int varyingValue, void (ABCInterval<int>::*intervalSetter)(int));
    void blockUISignals(bool b);

    Ui::ABCIntervalIntWidget *ui;
    MapWidget *m_mapWidget;
    int m_minimumValue;
    int m_varyingValue;
    ABCInterval<int> (Deme::*m_demeGetter)() const;
    void (Deme::*m_demeSetter)(ABCInterval<int>);
};

#endif // VARYINGABCINTERVALINTWIDGET_H
