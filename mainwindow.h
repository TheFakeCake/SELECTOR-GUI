#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "models/simulationmodel.h"
#include "SelectorConfigFiles.h"
#include "gui/MapWidget.h"
#include "gui/DemesParamWidget.h"

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

    void on_penToolPushButton_toggled(bool b);
    void on_boxToolPushButton_toggled(bool b);
    void on_magicToolPushButton_toggled(bool b);

    void on_initialFrequencyCheckBox_toggled(bool checked);

private:

    Ui::MainWindow *ui;
    MapWidget *m_mapWidget;
    DemesParamWidget *m_demesParamWidget;
    SimulationModel m_simulation;
    SelectorConfigFiles m_configFiles;
    bool m_simulationFromConfigFiles;
};

#endif // MAINWINDOW_H
