#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "models/simulationmodel.h"
#include "SelectorConfigFiles.h"
#include "gui/MapWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateView();

private slots:
    void on_action_OpenSimulation_triggered();
    void on_action_Save_triggered();
    void on_action_SaveAs_triggered();

    void on_fixedFrequencyRadioButton_toggled(bool b);
    void on_heterogeneityComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    MapWidget *m_mapWidget;
    SimulationModel m_simulation;
    SelectorConfigFiles m_configFiles;
    bool m_simulationFromConfigFiles;
};

#endif // MAINWINDOW_H
