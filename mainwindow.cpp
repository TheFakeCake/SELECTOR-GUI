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
    ((QVBoxLayout*)ui->demesParamTab->layout())->insertWidget(0, m_demesParamWidget);

    // General UI configuration
    ui->firstAlleleFrequencyABC->layout()->setContentsMargins(25, 0, 0, 0);
    ui->splitter->setCollapsible(0, false);
    ui->splitter->setSizes(QList<int>() << 1 << 0);

    // Prevent the QSpinBox and QDoubleSpinBox widgets inside the scroll area to catch mouse wheel events
    // so they don't interrupt scrolling
    Q_FOREACH(QAbstractSpinBox *sp, ui->settingsScrollArea->findChildren<QAbstractSpinBox*>())
    {
        sp->installEventFilter(this);
        sp->setFocusPolicy(Qt::StrongFocus);
    }
    Q_FOREACH(QComboBox *sp, ui->settingsScrollArea->findChildren<QComboBox*>())
    {
        sp->installEventFilter(this);
        sp->setFocusPolicy(Qt::StrongFocus);
    }

    // ABCIntervalWidgets configuration
    ui->numberOfGenerationsABC->setMinimumValue(1);
    ui->numberOfGenerationsABC->setMaximumValue(99999999);
    ui->initialAllelePoolSizeABC->setMinimumValue(1);
    ui->initialAllelePoolSizeABC->setMaximumValue(99999999);
    ui->mutationRateABC->setMinimumValue(0.0);
    ui->firstAlleleFrequencyABC->setMinimumValue(0.0);
    ui->firstAlleleFrequencyABC->setMaximumValue(1.0);
    ui->uniformCoefficientABC->setMinimumValue(0.0);
    ui->northCoefficientABC->setMinimumValue(0.0);
    ui->northCoefficientABC->setMaximumValue(1.0);
    ui->southCoefficientABC->setMinimumValue(0.0);
    ui->southCoefficientABC->setMaximumValue(1.0);
    ui->eastCoefficientABC->setMinimumValue(0.0);
    ui->eastCoefficientABC->setMaximumValue(1.0);
    ui->westCoefficientABC->setMinimumValue(0.0);
    ui->westCoefficientABC->setMaximumValue(1.0);

    // Config files
    m_simulationFromConfigFiles = false;
    connect(&m_configFiles, SIGNAL(info(QString)), ui->logWidget, SLOT(logInfo(QString)));
    connect(&m_configFiles, SIGNAL(warning(QString)), ui->logWidget, SLOT(logWarning(QString)));
    connect(&m_configFiles, SIGNAL(error(QString)), ui->logWidget, SLOT(logError(QString)));

    // SimulationModel -> View connections
    connect(&m_simulation, SIGNAL(nbSimulationsChanged(int)), ui->numberOfSimulationsSpinBox, SLOT(setValue(int)));
    connect(&m_simulation, SIGNAL(nbGenerationsChanged(ABCInterval<int>)), ui->numberOfGenerationsABC, SLOT(setValue(ABCInterval<int>)));
    connect(&m_simulation, SIGNAL(initialAllelePoolSizeChanged(ABCInterval<int>)), ui->initialAllelePoolSizeABC, SLOT(setValue(ABCInterval<int>)));
    connect(&m_simulation, SIGNAL(uniqueAllelePoolChanged(bool)), ui->uniquePoolRadioButton, SLOT(setChecked(bool)));
    connect(&m_simulation, SIGNAL(firstAlleleFrequencyEnabled(bool)), ui->initialFrequencyCheckBox, SLOT(setChecked(bool)));
    connect(&m_simulation, SIGNAL(firstAlleleFrequencyChanged(ABCInterval<double>)), ui->firstAlleleFrequencyABC, SLOT(setValue(ABCInterval<double>)));
    connect(&m_simulation, SIGNAL(mutationRateChanged(ABCInterval<double>)), ui->mutationRateABC, SLOT(setValue(ABCInterval<double>)));
    connect(&m_simulation, SIGNAL(outputCompleteHistoryChanged(bool)), ui->completeHistoryCheckBox, SLOT(setChecked(bool)));

    // View -> SimulationModel connections
    connect(ui->numberOfSimulationsSpinBox, SIGNAL(valueChanged(int)), &m_simulation, SLOT(setNbSimulations(int)));
    connect(ui->numberOfGenerationsABC, SIGNAL(valueChanged(ABCInterval<int>)), &m_simulation, SLOT(setNbGenerations(ABCInterval<int>)));
    connect(ui->initialAllelePoolSizeABC, SIGNAL(valueChanged(ABCInterval<int>)), &m_simulation, SLOT(setInitialAllelePoolSize(ABCInterval<int>)));
    connect(ui->uniquePoolRadioButton, SIGNAL(toggled(bool)), &m_simulation, SLOT(setUniqueAllelePool(bool)));
    connect(ui->initialFrequencyCheckBox, SIGNAL(toggled(bool)), &m_simulation, SLOT(enableFirstAlleleFrequency(bool)));
    connect(ui->firstAlleleFrequencyABC, SIGNAL(valueChanged(ABCInterval<double>)), &m_simulation, SLOT(setFirstAlleleFrequency(ABCInterval<double>)));
    connect(ui->mutationRateABC, SIGNAL(valueChanged(ABCInterval<double>)), &m_simulation, SLOT(setMutationRate(ABCInterval<double>)));
    connect(ui->completeHistoryCheckBox, SIGNAL(toggled(bool)), &m_simulation, SLOT(setOutputCompleteHistory(bool)));

    // Simulation Selection -> View connections
    connect(m_simulation.selection(), SIGNAL(modelChanged(int)), ui->selectionModelComboBox, SLOT(setCurrentIndex(int)));
    connect(m_simulation.selection(), SIGNAL(heterogeneityChanged(int)), ui->heterogeneityComboBox, SLOT(setCurrentIndex(int)));
    connect(m_simulation.selection(), SIGNAL(uniformCoefficientChanged(ABCInterval<double>)), ui->uniformCoefficientABC, SLOT(setValue(ABCInterval<double>)));
    connect(m_simulation.selection(), SIGNAL(northEastCoefficientChanged(ABCInterval<double>)), ui->northCoefficientABC, SLOT(setValue(ABCInterval<double>)));
    connect(m_simulation.selection(), SIGNAL(northEastCoefficientChanged(ABCInterval<double>)), ui->eastCoefficientABC, SLOT(setValue(ABCInterval<double>)));
    connect(m_simulation.selection(), SIGNAL(southWestCoefficientChanged(ABCInterval<double>)), ui->southCoefficientABC, SLOT(setValue(ABCInterval<double>)));
    connect(m_simulation.selection(), SIGNAL(southWestCoefficientChanged(ABCInterval<double>)), ui->westCoefficientABC, SLOT(setValue(ABCInterval<double>)));

    // View -> Simulation Selection connections
    connect(ui->selectionModelComboBox, SIGNAL(currentIndexChanged(int)), m_simulation.selection(), SLOT(setModel(int)));
    connect(ui->heterogeneityComboBox, SIGNAL(currentIndexChanged(int)), m_simulation.selection(), SLOT(setHeterogeneity(int)));
    connect(ui->uniformCoefficientABC, SIGNAL(valueChanged(ABCInterval<double>)), m_simulation.selection(), SLOT(setUniformCoefficient(ABCInterval<double>)));
    connect(ui->northCoefficientABC, SIGNAL(valueChanged(ABCInterval<double>)), m_simulation.selection(), SLOT(setNorthEastCoefficient(ABCInterval<double>)));
    connect(ui->southCoefficientABC, SIGNAL(valueChanged(ABCInterval<double>)), m_simulation.selection(), SLOT(setSouthWestCoefficient(ABCInterval<double>)));
    connect(ui->eastCoefficientABC, SIGNAL(valueChanged(ABCInterval<double>)), m_simulation.selection(), SLOT(setNorthEastCoefficient(ABCInterval<double>)));
    connect(ui->westCoefficientABC, SIGNAL(valueChanged(ABCInterval<double>)), m_simulation.selection(), SLOT(setSouthWestCoefficient(ABCInterval<double>)));

    // Simulation Map -> Vuew connections
    connect(m_simulation.map(), SIGNAL(widthChanged(int)), ui->mapWidthSpinBox, SLOT(setValue(int)));
    connect(m_simulation.map(), SIGNAL(heightChanged(int)), ui->mapHeightSpinBox, SLOT(setValue(int)));

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
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    // Ignore the event if it is coming from the mouse wheel and if the object is a QAbstractSpinBox
    if (event->type() == QEvent::Wheel && (qobject_cast<QAbstractSpinBox*>(object) || qobject_cast<QComboBox*>(object)))
    {
        event->ignore();
        return true;
    }
    return QWidget::eventFilter(object, event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::updateView()
{
    ui->numberOfSimulationsSpinBox->setValue(m_simulation.nbSimulations());
    ui->numberOfGenerationsABC->setValue(m_simulation.nbGenerations());
    ui->initialAllelePoolSizeABC->setValue(m_simulation.initialAllelePoolSize());
    ui->uniquePoolRadioButton->setChecked(m_simulation.uniqueAllelePool());
    ui->multiplePoolRadioButton->setChecked(!m_simulation.uniqueAllelePool());
    ui->initialFrequencyCheckBox->setChecked(m_simulation.firstAlleleFrequency() > 0);
    ui->firstAlleleFrequencyABC->setValue(m_simulation.firstAlleleFrequency());
    ui->mutationRateABC->setValue(m_simulation.mutationRate());
    ui->selectionModelComboBox->setCurrentText(m_simulation.selection()->modelName());
    ui->heterogeneityComboBox->setCurrentText(m_simulation.selection()->heterogeneityName());
    ui->uniformCoefficientABC->setValue(m_simulation.selection()->uniformCoefficient());
    ui->northCoefficientABC->setValue(m_simulation.selection()->northEastCoefficient());
    ui->eastCoefficientABC->setValue(m_simulation.selection()->northEastCoefficient());
    ui->southCoefficientABC->setValue(m_simulation.selection()->southWestCoefficient());
    ui->westCoefficientABC->setValue(m_simulation.selection()->southWestCoefficient());
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
        QMessageBox::warning(this, tr("SELECTOR-GUI"),
                             tr("Something happened while saving the simulation configuration. See the log for more details."));
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
        QMessageBox::warning(this, tr("SELECTOR-GUI"),
                             tr("Something happened while saving the simulation configuration. See the log for more details."));
    }

    m_simulationFromConfigFiles = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
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

void MainWindow::on_initialFrequencyCheckBox_toggled(bool checked)
{
    ui->firstAlleleFrequencyABC->setEnabled(checked);
}
