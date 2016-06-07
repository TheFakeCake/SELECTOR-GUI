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

private slots:

    void on_activatedCheckBox_clicked();
    void on_activatedCheckBox_stateChanged(int state);
    void on_initialPopulationSpinBox_valueChanged(int value);
    void on_carryingCapacitySpinBox_valueChanged(int value);
    void on_growthRateDoubleSpinBox_valueChanged(double value);
    void on_migrationRateDoubleSpinBox_valueChanged(double value);
    void on_sampleSizeSpinBox_valueChanged(int value);
    void on_groupComboBox_currentTextChanged(const QString &text);

private:

    Ui::DemesParamWidget *ui;
    MapWidget *m_mapWidget;
};

#endif // DEMESPARAMWIDGET_H
