#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "gui/CursorSelectTool.h"
#include "gui/BoxSelectTool.h"
#include "gui/MagicSelectTool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add the map widget
    m_mapWidget = new MapWidget(m_simulation.map());
    connect(ui->displayModeComboBox, SIGNAL(currentIndexChanged(int)), m_mapWidget, SLOT(setDisplayMode(int)));
    ui->mapWidgetFrame->layout()->addWidget(m_mapWidget);

    // Add the demes parameter widget
    m_demesParamWidget = new DemesParamWidget(m_mapWidget);
    ((QVBoxLayout*)ui->demesParamFrame->layout())->insertWidget(0, m_demesParamWidget);

    // Config files
    m_simulationFromConfigFiles = false;
    connect(&m_configFiles, SIGNAL(info(QString)), ui->logWidget, SLOT(logInfo(QString)));
    connect(&m_configFiles, SIGNAL(warning(QString)), ui->logWidget, SLOT(logWarning(QString)));
    connect(&m_configFiles, SIGNAL(error(QString)), ui->logWidget, SLOT(logError(QString)));

    // SimulationModel -> View connections
    connect(&m_simulation, SIGNAL(nbSimulationsChanged(int)), ui->numberOfSimulationsSpinBox, SLOT(setValue(int)));
    connect(&m_simulation, SIGNAL(nbGenerationsChanged(int)), ui->numberOfGenerationsSpinBox, SLOT(setValue(int)));
    connect(&m_simulation, SIGNAL(initialAllelePoolSizeChanged(int)), ui->numberOfInitialAllelesSpinBox, SLOT(setValue(int)));
    connect(&m_simulation, SIGNAL(uniqueAllelePoolChanged(bool)), ui->uniquePoolRadioButton, SLOT(setChecked(bool)));
    connect(&m_simulation, SIGNAL(firstAlleleFrequencyEnabled(bool)), ui->initialFrequencyCheckBox, SLOT(setChecked(bool)));
    connect(&m_simulation, SIGNAL(firstAlleleFrequencyChanged(double)), ui->fixedFrequencyDoubleSpinBox, SLOT(setValue(double)));
    connect(&m_simulation, SIGNAL(mutationRateChanged(double)), ui->mutationRateDoubleSpinBox, SLOT(setValue(double)));
    connect(&m_simulation, SIGNAL(outputCompleteHistoryChanged(bool)), ui->completeHistoryCheckBox, SLOT(setChecked(bool)));

    // View -> SimulationModel connections
    connect(ui->numberOfSimulationsSpinBox, SIGNAL(valueChanged(int)), &m_simulation, SLOT(setNbSimulations(int)));
    connect(ui->numberOfGenerationsSpinBox, SIGNAL(valueChanged(int)), &m_simulation, SLOT(setNbGenerations(int)));
    connect(ui->numberOfInitialAllelesSpinBox, SIGNAL(valueChanged(int)), &m_simulation, SLOT(setInitialAllelePoolSize(int)));
    connect(ui->uniquePoolRadioButton, SIGNAL(toggled(bool)), &m_simulation, SLOT(setUniqueAllelePool(bool)));
    connect(ui->initialFrequencyCheckBox, SIGNAL(toggled(bool)), &m_simulation, SLOT(enableFirstAlleleFrequency(bool)));
    connect(ui->fixedFrequencyDoubleSpinBox, SIGNAL(valueChanged(double)), &m_simulation, SLOT(setFirstAlleleFrequency(double)));
    connect(ui->mutationRateDoubleSpinBox, SIGNAL(valueChanged(double)), &m_simulation, SLOT(setMutationRate(double)));
    connect(ui->completeHistoryCheckBox, SIGNAL(toggled(bool)), &m_simulation, SLOT(setOutputCompleteHistory(bool)));

    // Simulation Selection -> View connections
    connect(m_simulation.selection(), SIGNAL(modelChanged(int)), ui->selectionModelComboBox, SLOT(setCurrentIndex(int)));
    connect(m_simulation.selection(), SIGNAL(heterogeneityChanged(int)), ui->heterogeneityComboBox, SLOT(setCurrentIndex(int)));
    connect(m_simulation.selection(), SIGNAL(uniformCoefficientChanged(double)), ui->uniformCoefficientDoubleSpinBox, SLOT(setValue(double)));
    connect(m_simulation.selection(), SIGNAL(northEastCoefficientChanged(double)), ui->northEastCoefficientDoubleSpinBox, SLOT(setValue(double)));
    connect(m_simulation.selection(), SIGNAL(southWestCoefficientChanged(double)), ui->southWestCoefficientDoubleSpinBox, SLOT(setValue(double)));

    // View -> Simulation Selection connections
    connect(ui->selectionModelComboBox, SIGNAL(currentIndexChanged(int)), m_simulation.selection(), SLOT(setModel(int)));
    connect(ui->heterogeneityComboBox, SIGNAL(currentIndexChanged(int)), m_simulation.selection(), SLOT(setHeterogeneity(int)));
    connect(ui->uniformCoefficientDoubleSpinBox, SIGNAL(valueChanged(double)), m_simulation.selection(), SLOT(setUniformCoefficient(double)));
    connect(ui->northEastCoefficientDoubleSpinBox, SIGNAL(valueChanged(double)), m_simulation.selection(), SLOT(setNorthEastCoefficient(double)));
    connect(ui->southWestCoefficientDoubleSpinBox, SIGNAL(valueChanged(double)), m_simulation.selection(), SLOT(setSouthWestCoefficient(double)));

    // Simulation Map -> Vuew connections
    connect(m_simulation.map(), SIGNAL(widthChanged(int)), ui->mapWidthSpinBox, SLOT(setValue(int)));
    connect(m_simulation.map(), SIGNAL(heightChanged(int)), ui->mapHeightSpinBox, SLOT(setValue(int)));
    connect(m_simulation.map(), SIGNAL(changed()), m_mapWidget, SLOT(repaint()));

    // View -> Simulation Map connections
    connect(ui->mapWidthSpinBox, SIGNAL(valueChanged(int)), m_simulation.map(), SLOT(setWidth(int)));
    connect(ui->mapHeightSpinBox, SIGNAL(valueChanged(int)), m_simulation.map(), SLOT(setHeight(int)));

    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::updateView()
{
    ui->numberOfSimulationsSpinBox->setValue(m_simulation.nbSimulations());
    ui->numberOfGenerationsSpinBox->setValue(m_simulation.nbGenerations());
    ui->numberOfInitialAllelesSpinBox->setValue(m_simulation.initialAllelePoolSize());
    ui->uniquePoolRadioButton->setChecked(m_simulation.uniqueAllelePool());
    ui->multiplePoolRadioButton->setChecked(!m_simulation.uniqueAllelePool());
    ui->initialFrequencyCheckBox->setChecked(m_simulation.firstAlleleFrequency() > 0);
    ui->fixedFrequencyDoubleSpinBox->setValue(m_simulation.firstAlleleFrequency());
    ui->mutationRateDoubleSpinBox->setValue(m_simulation.mutationRate());
    ui->selectionModelComboBox->setCurrentText(m_simulation.selection()->modelName());
    ui->heterogeneityComboBox->setCurrentText(m_simulation.selection()->heterogeneityName());
    ui->uniformCoefficientDoubleSpinBox->setValue(m_simulation.selection()->uniformCoefficient());
    ui->northEastCoefficientDoubleSpinBox->setValue(m_simulation.selection()->northEastCoefficient());
    ui->southWestCoefficientDoubleSpinBox->setValue(m_simulation.selection()->southWestCoefficient());
    ui->completeHistoryCheckBox->setChecked(m_simulation.outputCompleteHistory());

    // Map
    ui->mapWidthSpinBox->setValue(m_simulation.map()->width());
    ui->mapHeightSpinBox->setValue(m_simulation.map()->height());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_OpenSimulation_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this);

    if (directory.isEmpty())
    {
        return;
    }

    m_configFiles.setDirectory(directory);

    if (! m_configFiles.read(&m_simulation))
    {
        QMessageBox::warning(this, tr("SELECTOR-GUI"),
                             tr("Some errors happened. See the log for more details."));
        m_simulationFromConfigFiles = false;
    }
    else
    {
        m_simulationFromConfigFiles = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_Save_triggered()
{
    if (! m_simulationFromConfigFiles)
    {
        QString directory = QFileDialog::getExistingDirectory(this);

        if (directory.isEmpty())
        {
            return;
        }
        m_configFiles.setDirectory(directory);
    }
    if (! m_configFiles.write(&m_simulation))
    {
        QMessageBox::warning(this, tr("Error while saving configuration"),
                             tr("Something happened while waving the simulation configuration :\n\n") + m_configFiles.errorMessages().join('\n'));
    }
    m_simulationFromConfigFiles = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_SaveAs_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this);

    if (directory.isEmpty())
    {
        return;
    }

    m_configFiles.setDirectory(directory);

    if (! m_configFiles.write(&m_simulation))
    {
        QMessageBox::warning(this, tr("Error while saving configuration"),
                             tr("Something happened while waving the simulation configuration :\n\n") + m_configFiles.errorMessages().join('\n'));
    }

    m_simulationFromConfigFiles = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_fixedFrequencyRadioButton_toggled(bool b)
{
    ui->firstAlleleFrequencyStackedWidget->setCurrentIndex((int)(!b));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_heterogeneityComboBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->coefficientStackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->coefficientStackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_penToolPushButton_toggled(bool b)
{
    if (b)
    {
        m_mapWidget->setSelectTool(new CursorSelectTool());
    }
}

void MainWindow::on_boxToolPushButton_toggled(bool b)
{
    if (b)
    {
        m_mapWidget->setSelectTool(new BoxSelectTool());
    }
}

void MainWindow::on_magicToolPushButton_toggled(bool b)
{
    if (b)
    {
        m_mapWidget->setSelectTool(new MagicSelectTool());
    }
}
