#ifndef VARYINGABCINTERVALDOUBLEWIDGET_H
#define VARYINGABCINTERVALDOUBLEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include "MapWidget.h"

namespace Ui {
class ABCIntervalDoubleWidget;
}

class VaryingABCIntervalDoubleWidget : public QWidget
{
    Q_OBJECT

public:

    explicit VaryingABCIntervalDoubleWidget(QWidget *parent = nullptr);
    ~VaryingABCIntervalDoubleWidget();

    void setMinimumValue(double min);
    void setMaximumValue(double max);
    void init(MapWidget *mapWidget, ABCInterval<double> (Deme::*demeGetter)() const, void (Deme::*demeSetter)(ABCInterval<double>));

    void updateView(bool keepCurrentStackIndex = false);

private slots:

    void on_ABC_toggled(bool checked);
    void on_singleValue_valueChanged(double value);
    void on_firstBound_valueChanged(double value);
    void on_secondBound_valueChanged(double value);
    void on_distribution_currentIndexChanged(int index);

private:

    template<typename T>
    void setIntervalValueToSelection(T value, T varyingValue, void (ABCInterval<double>::*intervalSetter)(T));
    void blockUISignals(bool b);

    Ui::ABCIntervalDoubleWidget *ui;
    MapWidget *m_mapWidget;
    double m_minimumValue;
    double m_varyingValue;
    ABCInterval<double> (Deme::*m_demeGetter)() const;
    void (Deme::*m_demeSetter)(ABCInterval<double>);
};

#endif // VARYINGABCINTERVALDOUBLEWIDGET_H
