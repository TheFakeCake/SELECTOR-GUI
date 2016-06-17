#include "DemesParamWidget.h"
#include "ui_DemesParamWidget.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
DemesParamWidget::DemesParamWidget(MapWidget *mapWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemesParamWidget),
    m_mapWidget(mapWidget)
{
    ui->setupUi(this);

    ui->initialPopulationVaryingWidget->setMinimumValue(0);
    ui->initialPopulationVaryingWidget->setMaximumValue(99999999);
    ui->initialPopulationVaryingWidget->init(m_mapWidget, &Deme::initialPopulation, &Deme::setInitialPopulation);

    ui->carryingCapacityVaryingWidget->setMinimumValue(0);
    ui->carryingCapacityVaryingWidget->setMaximumValue(99999999);
    ui->carryingCapacityVaryingWidget->init(m_mapWidget, &Deme::carryingCapacity, &Deme::setCarryingCapacity);

    ui->growthRateVaryingWidget->setMinimumValue(0.0);
    ui->growthRateVaryingWidget->setMaximumValue(1.0);
    ui->growthRateVaryingWidget->init(m_mapWidget, &Deme::growthRate, &Deme::setGrowthRate);

    ui->migrationRateVaryingWidget->setMinimumValue(0.0);
    ui->migrationRateVaryingWidget->setMaximumValue(1.0);
    ui->migrationRateVaryingWidget->init(m_mapWidget, &Deme::migrationRate, &Deme::setMigrationRate);

    connect(m_mapWidget, SIGNAL(selectionChanged()), this, SLOT(updateView()));

    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
DemesParamWidget::~DemesParamWidget()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::updateView()
{
    QList<QPoint> selection = m_mapWidget->selection();

    // If no demes are selected, the widget is disabled
    if (selection.empty())
    {
        setEnabled(false);
        return;
    }
    setEnabled(true);

    // Data that will be displayed in the view widgets
    bool viewDataIsInitialized = false;
    Qt::CheckState viewActivatedState = Qt::Unchecked;
    int viewSampleSize = 0;
    int viewGroup = 1;

    // For each selected deme ...
    QListIterator<QPoint> it(selection);
    for (int i = 0; i < selection.size(); i++)
    {
        QPoint demeCoord = it.next();
        Deme *deme = m_mapWidget->map()->deme(demeCoord.x(), demeCoord.y());

        // If the view data is not initialized yet, it is initialized with the current deme parameters
        // if it is enabled.
        if (!viewDataIsInitialized && deme->isEnabled())
        {
            viewActivatedState = (i > 0 ? Qt::PartiallyChecked : Qt::Checked);
            viewSampleSize = deme->sampleSize();
            viewGroup = deme->group();
            viewDataIsInitialized = true;
            continue;
        }

        // Compares the values of the deme parameters with the view data. If they are different,
        // this means that at least 2 selected demes do not share the same value for the particular
        // parameter and thus, the "Varying" value is set.

        if (deme->isEnabled() != (bool)viewActivatedState)
        {
            viewActivatedState = Qt::PartiallyChecked;
        }

        // Ignore data from inactive demes
        if (! deme->isEnabled())
        {
            continue;
        }

        // Check sample size
        if (deme->sampleSize() != viewSampleSize)
        {
            viewSampleSize = -1;
        }
        // Check group
        if (deme->group() != viewGroup)
        {
            viewGroup = 0;
        }
    }

    // Display the data in the views
    blockUISignals(true);

    ui->activatedCheckBox->setCheckState(viewActivatedState);

    ui->initialPopulationVaryingWidget->updateView();
    ui->carryingCapacityVaryingWidget->updateView();
    ui->growthRateVaryingWidget->updateView();
    ui->migrationRateVaryingWidget->updateView();

    ui->sampleSizeSpinBox->setValue(viewSampleSize);

    // The "group" combo box needs to be filled with the values from the map
    ui->groupComboBox->clear();

    if (viewGroup == 0)
    {
        ui->groupComboBox->addItem("Varying");
    }

    std::map<int, std::vector<Deme*> > groups = m_mapWidget->map()->groups();
    for (auto it = groups.begin(); it != groups.end(); it++)
    {
        ui->groupComboBox->addItem(QString::number(it->first));
    }
    ui->groupComboBox->addItem("New group");
    ui->groupComboBox->setCurrentText(QString::number(viewGroup));

    blockUISignals(false);

    // TODO : Check if group must be 1 to ....
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_activatedCheckBox_clicked()
{
    if (ui->activatedCheckBox->checkState() == Qt::PartiallyChecked)
    {
        ui->activatedCheckBox->setCheckState(Qt::Checked);
    }

    Map *map = m_mapWidget->map();
    m_mapWidget->setPreventUpdates(true);
    QListIterator<QPoint> it(m_mapWidget->selection());
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());
        deme->enable(ui->activatedCheckBox->isChecked());
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_activatedCheckBox_stateChanged(int state)
{
    // Enabled / disables the widgets if the "Active" checkbox is checked / unchecked
    bool b = (state != Qt::Unchecked);
    ui->initialPopulationLabel->setEnabled(b);
    ui->initialPopulationVaryingWidget->setEnabled(b);
    ui->carryingCapacityLabel->setEnabled(b);
    ui->carryingCapacityVaryingWidget->setEnabled(b);
    ui->growthRateLabel->setEnabled(b);
    ui->growthRateVaryingWidget->setEnabled(b);
    ui->migrationRateLabel->setEnabled(b);
    ui->migrationRateVaryingWidget->setEnabled(b);
    ui->sampleSizeLabel->setEnabled(b);
    ui->sampleSizeSpinBox->setEnabled(b);
    ui->groupLabel->setEnabled(b);
    ui->groupComboBox->setEnabled(b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_sampleSizeSpinBox_valueChanged(int value)
{
    if (value == -1)
    {
        return;
    }
    Map *map = m_mapWidget->map();
    m_mapWidget->setPreventUpdates(true);
    QListIterator<QPoint> it(m_mapWidget->selection());
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());
        if (deme->isEnabled())
        {
            deme->setSampleSize(value);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_groupComboBox_currentTextChanged(const QString &text)
{
    if (text == "Varying")
    {
        return;
    }
    Map *map = m_mapWidget->map();
    int group = 1;

    if (text == "New group")
    {
        group = map->groups().rbegin()->first + 1;
    }
    else
    {
        group = text.toInt();
    }

    m_mapWidget->setPreventUpdates(true);
    QListIterator<QPoint> it(m_mapWidget->selection());
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());
        if (deme->isEnabled())
        {
            deme->setGroup(group);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::blockUISignals(bool b)
{
    ui->sampleSizeSpinBox->blockSignals(b);
    ui->groupComboBox->blockSignals(b);
}
