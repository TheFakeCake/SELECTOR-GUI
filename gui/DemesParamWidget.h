#ifndef DEMESPARAMWIDGET_H
#define DEMESPARAMWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include "gui/MapWidget.h"

namespace Ui {
class DemesParamWidget;
}

class DemesParamWidget : public QWidget
{
    Q_OBJECT

public:

    explicit DemesParamWidget(MapWidget *mapWidget, QWidget *parent = 0);
    ~DemesParamWidget();

public slots:

    void updateView();
    void updateInitialPopulationView();

private slots:

    void on_activatedCheckBox_clicked();
    void on_activatedCheckBox_stateChanged(int state);

    // TODO : minimun max slots
    /*
    void on_initialPopulationABC_valueChanged(ABCInterval<int> value);
    void on_carryingCapacityABC_valueChanged(ABCInterval<int> value);
    void on_growthRateABC_valueChanged(ABCInterval<double> value);
    void on_migrationRateABC_valueChanged(ABCInterval<double> value);

    void on_initialPopulationABC_singleValueChanged(int value);
    void on_initialPopulationABC_minimumChanged(int value);
    void on_initialPopulationABC_maximumChanged(int value);
    void on_initialPopulationABC_distributionChanged(int value);
    */

    void on_sampleSizeSpinBox_valueChanged(int value);
    void on_groupComboBox_currentTextChanged(const QString &text);

    void on_initialPopulationCheckbox_toggled(bool checked);
    void on_initialPopulationSingleValue_valueChanged(int value);
    void on_initialPopulationMinValue_valueChanged(int value);
    void on_initialPopulationMaxValue_valueChanged(int value);
    void on_initialPopulationDistribution_currentIndexChanged(int index);

private:

    template<typename ValueType, class IntervalType>
    void setIntervalValueToSelection(ValueType value, ValueType varyingValue,
                                     IntervalType (Deme::*demeGetter)() const,
                                     void (Deme::*demeSetter)(IntervalType),
                                     void (IntervalType::*intervalSetter)(ValueType));

    void blockUISignals(bool b);

    Ui::DemesParamWidget *ui;
    MapWidget *m_mapWidget;
};

#endif // DEMESPARAMWIDGET_H
