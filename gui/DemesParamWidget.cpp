#include "DemesParamWidget.h"
#include "ui_DemesParamWidget.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
DemesParamWidget::DemesParamWidget(MapWidget *mapWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemesParamWidget),
    m_mapWidget(mapWidget)
{
    ui->setupUi(this);

    ui->initialPopulationABC->setMinimumValue(-1);
    ui->initialPopulationABC->setMaximumValue(999999999);
    ui->initialPopulationABC->setSpecialValueText("Varying");
    ui->carryingCapacityABC->setMinimumValue(-1);
    ui->carryingCapacityABC->setMaximumValue(999999999);
    ui->carryingCapacityABC->setSpecialValueText("Varying");
    ui->growthRateABC->setMinimumValue(-0.001);
    ui->growthRateABC->setSpecialValueText("Varying");
    ui->migrationRateABC->setMinimumValue(-0.001);
    ui->migrationRateABC->setSpecialValueText("Varying");

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
    ABCInterval<int> viewInitialPopulation(0);
    ABCInterval<int> viewCarryingCapacity(0);
    ABCInterval<double> viewGrowthRate(0.0);
    ABCInterval<double> viewMigrationRate(0.0);
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
            viewInitialPopulation = deme->initialPopulation();
            viewCarryingCapacity = deme->carryingCapacity();
            viewGrowthRate = deme->growthRate();
            viewMigrationRate = deme->migrationRate();
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

        if (deme->initialPopulation().minimum() != viewInitialPopulation.minimum())
        {
            viewInitialPopulation.setMinimum(-1);
        }
        if (deme->initialPopulation().maximum() != viewInitialPopulation.maximum())
        {
            viewInitialPopulation.setMaximum(-1);
        }

        if (deme->carryingCapacity().minimum() != viewCarryingCapacity.minimum())
        {
            viewCarryingCapacity.setMinimum(-1);
        }
        if (deme->carryingCapacity().maximum() != viewCarryingCapacity.maximum())
        {
            viewCarryingCapacity.setMaximum(-1);
        }

        if (deme->growthRate().minimum() != viewGrowthRate.minimum())
        {
            viewGrowthRate.setMinimum(-0.001);
        }
        if (deme->growthRate().maximum() != viewGrowthRate.maximum())
        {
            viewGrowthRate.setMaximum(-0.001);
        }

        if (deme->migrationRate().minimum() != viewMigrationRate.minimum())
        {
            viewMigrationRate.setMinimum(-0.001);
        }
        if (deme->migrationRate().maximum() != viewMigrationRate.maximum())
        {
            viewMigrationRate.setMaximum(-0.001);
        }

        if (deme->sampleSize() != viewSampleSize)
        {
            viewSampleSize = -1;
        }
        if (deme->group() != viewGroup)
        {
            viewGroup = 0;
        }
    }

    // Display the data in the views
    ui->activatedCheckBox->setCheckState(viewActivatedState);
    ui->initialPopulationABC->setValue(viewInitialPopulation);
    ui->carryingCapacityABC->setValue(viewCarryingCapacity);
    ui->growthRateABC->setValue(viewGrowthRate);
    ui->migrationRateABC->setValue(viewMigrationRate);
    ui->sampleSizeSpinBox->setValue(viewSampleSize);

    // The "group" combo box needs to be filled with the values from the map
    ui->groupComboBox->blockSignals(true);
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
    ui->groupComboBox->blockSignals(false);

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
    ui->initialPopulationABC->setEnabled(b);
    ui->carryingCapacityLabel->setEnabled(b);
    ui->carryingCapacityABC->setEnabled(b);
    ui->growthRateLabel->setEnabled(b);
    ui->growthRateABC->setEnabled(b);
    ui->migrationRateLabel->setEnabled(b);
    ui->migrationRateABC->setEnabled(b);
    ui->sampleSizeLabel->setEnabled(b);
    ui->sampleSizeSpinBox->setEnabled(b);
    ui->groupLabel->setEnabled(b);
    ui->groupComboBox->setEnabled(b);
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_initialPopulationABC_valueChanged(ABCInterval<int> value)
{
    if (value == -1)
    {
        return;
    }
    Map *map = m_mapWidget->map();
    m_mapWidget->setPreventUpdates(true);
    QListIterator<QPoint> it(m_mapWidget->selection());
    ABCInterval<int> newValue = value;
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());
        if (deme->isEnabled())
        {
            if (value.minimum() == -1)
            {
                newValue.setMinimum(deme->initialPopulation().minimum());
            }
            else if (value.maximum() == -1)
            {
                newValue.setMaximum(deme->initialPopulation().maximum());
            }
            deme->setInitialPopulation(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_carryingCapacityABC_valueChanged(ABCInterval<int> value)
{
    if (value == -1)
    {
        return;
    }
    Map *map = m_mapWidget->map();
    m_mapWidget->setPreventUpdates(true);
    QListIterator<QPoint> it(m_mapWidget->selection());
    ABCInterval<int> newValue = value;
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());
        if (deme->isEnabled())
        {
            if (value.minimum() == -1)
            {
                newValue.setMinimum(deme->carryingCapacity().minimum());
            }
            else if (value.maximum() == -1)
            {
                newValue.setMaximum(deme->carryingCapacity().maximum());
            }
            deme->setCarryingCapacity(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_growthRateABC_valueChanged(ABCInterval<double> value)
{
    if (value == -0.001)
    {
        return;
    }
    Map *map = m_mapWidget->map();
    m_mapWidget->setPreventUpdates(true);
    QListIterator<QPoint> it(m_mapWidget->selection());
    ABCInterval<double> newValue = value;
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());
        if (deme->isEnabled())
        {
            if (value.minimum() == -0.001)
            {
                newValue.setMinimum(deme->growthRate().minimum());
            }
            else if (value.maximum() == -0.001)
            {
                newValue.setMaximum(deme->growthRate().maximum());
            }
            deme->setGrowthRate(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_migrationRateABC_valueChanged(ABCInterval<double> value)
{
    if (value == -0.001)
    {
        return;
    }
    Map *map = m_mapWidget->map();
    m_mapWidget->setPreventUpdates(true);
    QListIterator<QPoint> it(m_mapWidget->selection());
    ABCInterval<double> newValue = value;
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());
        if (deme->isEnabled())
        {
            if (value.minimum() == -0.001)
            {
                newValue.setMinimum(deme->migrationRate().minimum());
            }
            else if (value.maximum() == -0.001)
            {
                newValue.setMaximum(deme->migrationRate().maximum());
            }
            deme->setMigrationRate(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_initialPopulationABC_singleValueChanged(int value)
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
            ABCInterval<int> newValue = deme->initialPopulation();
            newValue.setValue(value);
            deme->setInitialPopulation(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_initialPopulationABC_minimumChanged(int value)
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
            ABCInterval<int> newValue = deme->initialPopulation();
            newValue.setMinimum(value);
            deme->setInitialPopulation(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DemesParamWidget::on_initialPopulationABC_maximumChanged(int value)
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
            ABCInterval<int> newValue = deme->initialPopulation();
            newValue.setMaximum(value);
            deme->setInitialPopulation(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
}

void DemesParamWidget::on_initialPopulationABC_distributionChanged(int value)
{
    if (value < 0 || value > ABCInterval<int>::LastDistribution)
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
            ABCInterval<int> newValue = deme->initialPopulation();
            newValue.setDistribution((ABCInterval<int>::Distribution)value);
            deme->setInitialPopulation(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
    updateView();
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

template<typename T>
void DemesParamWidget::setValueInModel(T value, void (Deme::*setterMethod)(T))
{

}
