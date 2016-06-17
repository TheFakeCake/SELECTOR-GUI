#include "DemesParamWidget.h"
#include "ui_DemesParamWidget.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
DemesParamWidget::DemesParamWidget(MapWidget *mapWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemesParamWidget),
    m_mapWidget(mapWidget)
{
    ui->setupUi(this);

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
    bool viewInitialPopulationABC = false;
    bool viewCarryingCapacityABC = false;
    bool viewGrowthRateABC = false;
    bool viewMigrationRateABC = false;
    int viewSampleSize = 0;
    int viewGroup = 1;

    bool initialPopulationVaryingDistribution = false;

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
            viewInitialPopulationABC = ! viewInitialPopulation.isFixedValue();
            viewCarryingCapacity = deme->carryingCapacity();
            viewCarryingCapacityABC = ! viewCarryingCapacity.isFixedValue();
            viewGrowthRate = deme->growthRate();
            viewGrowthRateABC = ! viewGrowthRate.isFixedValue();
            viewMigrationRate = deme->migrationRate();
            viewMigrationRateABC = ! viewMigrationRate.isFixedValue();
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

        // Check initial population
        if (deme->initialPopulation().isFixedValue() != !viewInitialPopulationABC)
        {
            viewInitialPopulationABC = false;
        }
        if (deme->initialPopulation().firstBound() != viewInitialPopulation.firstBound())
        {
            viewInitialPopulation.setFirstBound(-1);
        }
        if (deme->initialPopulation().secondBound() != viewInitialPopulation.secondBound())
        {
            viewInitialPopulation.setSecondBound(-1);
        }
        if (deme->initialPopulation().distribution() != viewInitialPopulation.distribution())
        {
            initialPopulationVaryingDistribution = true;
        }

        // Check carrying capacity
        if (deme->carryingCapacity().isFixedValue() != !viewCarryingCapacityABC)
        {
            viewCarryingCapacityABC = false;
        }
        if (deme->carryingCapacity().minimum() != viewCarryingCapacity.minimum())
        {
            viewCarryingCapacity.setMinimum(-1);
        }
        if (deme->carryingCapacity().maximum() != viewCarryingCapacity.maximum())
        {
            viewCarryingCapacity.setMaximum(-1);
        }

        // Check growth rate
        if (deme->growthRate().isFixedValue() != !viewGrowthRateABC)
        {
            viewGrowthRateABC = false;
        }
        if (deme->growthRate().minimum() != viewGrowthRate.minimum())
        {
            viewGrowthRate.setMinimum(-0.001);
        }
        if (deme->growthRate().maximum() != viewGrowthRate.maximum())
        {
            viewGrowthRate.setMaximum(-0.001);
        }

        // Check migration rate
        if (deme->migrationRate().isFixedValue() != !viewMigrationRateABC)
        {
            viewMigrationRateABC = false;
        }
        if (deme->migrationRate().minimum() != viewMigrationRate.minimum())
        {
            viewMigrationRate.setMinimum(-0.001);
        }
        if (deme->migrationRate().maximum() != viewMigrationRate.maximum())
        {
            viewMigrationRate.setMaximum(-0.001);
        }

        // Check sample size
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
    blockUISignals(true);

    if (viewInitialPopulation.firstBound() == -1)
    {
        ui->initialPopulationSingleValue->setMinimum(-1);
        ui->initialPopulationSingleValue->setSpecialValueText("Varying");
        ui->initialPopulationMinValue->setMinimum(-1);
        ui->initialPopulationMinValue->setSpecialValueText("Varying");
    }
    else
    {
        ui->initialPopulationSingleValue->setMinimum(0);
        ui->initialPopulationSingleValue->setSpecialValueText("");
        ui->initialPopulationMinValue->setMinimum(0);
        ui->initialPopulationMinValue->setSpecialValueText("");
    }
    if (viewInitialPopulation.secondBound() == -1)
    {
        ui->initialPopulationMaxValue->setMinimum(-1);
        ui->initialPopulationMaxValue->setSpecialValueText("Varying");
    }
    else
    {
        ui->initialPopulationMaxValue->setMinimum(0);
        ui->initialPopulationMaxValue->setSpecialValueText("");
    }

    ui->activatedCheckBox->setCheckState(viewActivatedState);


    ui->initialPopulationDistribution->removeItem(3);
    if (initialPopulationVaryingDistribution)
    {
        ui->initialPopulationDistribution->addItem("Varying");
        ui->initialPopulationDistribution->setCurrentIndex(3);
    }
    else
    {
        ui->initialPopulationDistribution->setCurrentIndex(viewInitialPopulation.distribution());
    }
    ui->initialPopulationSingleValue->setValue(viewInitialPopulation.firstBound());
    ui->initialPopulationMinValue->setValue(viewInitialPopulation.firstBound());
    ui->initialPopulationMaxValue->setValue(viewInitialPopulation.secondBound());
    ui->initialPopulationCheckbox->setChecked(viewInitialPopulationABC);
    /* /!\ */
    ui->initialPopulationStackedWidget->setCurrentIndex(viewInitialPopulationABC ? 1 : 0);
    /* /!\ */

    ui->carryingCapacitySingleValue->setValue(viewCarryingCapacity.minimum());
    ui->carryingCapacityMinValue->setValue(viewCarryingCapacity.minimum());
    ui->carryingCapacityMaxValue->setValue(viewCarryingCapacity.maximum());
    ui->carryingCapacityCheckbox->setChecked(viewCarryingCapacityABC);
    ui->carryingCapacityStackedWidget->setCurrentIndex(viewCarryingCapacityABC ? 1 : 0);

    ui->growthRateSingleValue->setValue(viewGrowthRate.minimum());
    ui->growthRateMinValue->setValue(viewGrowthRate.minimum());
    ui->growthRateMaxValue->setValue(viewGrowthRate.maximum());
    ui->growthRateCheckbox->setChecked(viewGrowthRateABC);
    ui->growthRateStackedWidget->setCurrentIndex(viewGrowthRateABC ? 1 : 0);

    ui->migrationRateSingleValue->setValue(viewMigrationRate.minimum());
    ui->migrationRateMinValue->setValue(viewMigrationRate.minimum());
    ui->migrationRateMaxValue->setValue(viewMigrationRate.maximum());
    ui->migrationRateCheckbox->setChecked(viewMigrationRateABC);
    ui->migrationRateStackedWidget->setCurrentIndex(viewMigrationRateABC ? 1 : 0);

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

    blockUISignals(false);

    // TODO : Check if group must be 1 to ....
}

void DemesParamWidget::updateInitialPopulationView()
{
    QList<QPoint> selection = m_mapWidget->selection();

    // Data that will be displayed in the view widgets
    bool viewDataIsInitialized = false;
    ABCInterval<int> viewInterval(0);
    bool viewABC = false;
    bool varyingDistribution = false;

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
            viewInterval = deme->initialPopulation();
            viewABC = ! viewInterval.isFixedValue();
            viewDataIsInitialized = true;
            continue;
        }

        // Compares the values of the deme parameters with the view data. If they are different,
        // this means that at least 2 selected demes do not share the same value for the particular
        // parameter and thus, the "Varying" value is set.

        // Ignore data from inactive demes
        if (! deme->isEnabled())
        {
            continue;
        }

        if (deme->initialPopulation().isFixedValue() != !viewABC)
        {
            viewABC = false;
        }
        if (deme->initialPopulation().firstBound() != viewInterval.firstBound())
        {
            viewInterval.setFirstBound(-1);
        }
        if (deme->initialPopulation().secondBound() != viewInterval.secondBound())
        {
            viewInterval.setSecondBound(-1);
        }
        if (deme->initialPopulation().distribution() != viewInterval.distribution())
        {
            varyingDistribution = true;
        }
    }

    // Display the data in the views
    blockUISignals(true);

    if (viewInterval.firstBound() == -1)
    {
        ui->initialPopulationSingleValue->setMinimum(-1);
        ui->initialPopulationSingleValue->setSpecialValueText("Varying");
        ui->initialPopulationMinValue->setMinimum(-1);
        ui->initialPopulationMinValue->setSpecialValueText("Varying");
    }
    else
    {
        ui->initialPopulationSingleValue->setMinimum(0);
        ui->initialPopulationSingleValue->setSpecialValueText("");
        ui->initialPopulationMinValue->setMinimum(0);
        ui->initialPopulationMinValue->setSpecialValueText("");
    }

    if (viewInterval.secondBound() == -1)
    {
        ui->initialPopulationMaxValue->setMinimum(-1);
        ui->initialPopulationMaxValue->setSpecialValueText("Varying");
    }
    else
    {
        ui->initialPopulationMaxValue->setMinimum(0);
        ui->initialPopulationMaxValue->setSpecialValueText("");
    }

    ui->initialPopulationDistribution->removeItem(3);
    if (varyingDistribution)
    {
        ui->initialPopulationDistribution->addItem("Varying");
        ui->initialPopulationDistribution->setCurrentIndex(3);
    }
    else
    {
        ui->initialPopulationDistribution->setCurrentIndex(viewInterval.distribution());
    }

    ui->initialPopulationSingleValue->setValue(viewInterval.firstBound());
    ui->initialPopulationMinValue->setValue(viewInterval.firstBound());
    ui->initialPopulationMaxValue->setValue(viewInterval.secondBound());
    ui->initialPopulationCheckbox->setChecked(viewABC);

    blockUISignals(false);
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
    ui->initialPopulationStackedWidget->setEnabled(b);
    ui->initialPopulationCheckbox->setEnabled(b);
    ui->carryingCapacityLabel->setEnabled(b);
    ui->carryingCapacityStackedWidget->setEnabled(b);
    ui->carryingCapacityCheckbox->setEnabled(b);
    ui->growthRateLabel->setEnabled(b);
    ui->growthRateStackedWidget->setEnabled(b);
    ui->growthRateCheckbox->setEnabled(b);
    ui->migrationRateLabel->setEnabled(b);
    ui->migrationRateStackedWidget->setEnabled(b);
    ui->migrationRateCheckbox->setEnabled(b);
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
*/

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
void DemesParamWidget::on_initialPopulationCheckbox_toggled(bool checked)
{
    if (checked)
    {
        blockUISignals(true);
        ui->initialPopulationMinValue->setValue(ui->initialPopulationSingleValue->value());
        ui->initialPopulationMaxValue->setValue(ui->initialPopulationSingleValue->value());
        ui->initialPopulationStackedWidget->setCurrentIndex(1);
        blockUISignals(false);
    }
    else
    {
        ui->initialPopulationSingleValue->setValue(ui->initialPopulationMinValue->value());
        ui->initialPopulationStackedWidget->setCurrentIndex(0);

        if (ui->initialPopulationMinValue->value() != ui->initialPopulationMaxValue->value())
        {
            setIntervalValueToSelection<int, ABCInterval<int> >(ui->initialPopulationSingleValue->value(), -1,
                                                                &Deme::initialPopulation,
                                                                &Deme::setInitialPopulation,
                                                                &ABCInterval<int>::setValue);
        }
    }
}

void DemesParamWidget::on_initialPopulationSingleValue_valueChanged(int value)
{
    setIntervalValueToSelection<int, ABCInterval<int> >(value, -1,
                                                        &Deme::initialPopulation,
                                                        &Deme::setInitialPopulation,
                                                        &ABCInterval<int>::setValue);
    updateInitialPopulationView();
}

void DemesParamWidget::on_initialPopulationMinValue_valueChanged(int value)
{
    setIntervalValueToSelection<int, ABCInterval<int> >(value, -1,
                                                        &Deme::initialPopulation,
                                                        &Deme::setInitialPopulation,
                                                        &ABCInterval<int>::setFirstBound);
    updateInitialPopulationView();
}

void DemesParamWidget::on_initialPopulationMaxValue_valueChanged(int value)
{
    setIntervalValueToSelection<int, ABCInterval<int> >(value, -1,
                                                        &Deme::initialPopulation,
                                                        &Deme::setInitialPopulation,
                                                        &ABCInterval<int>::setSecondBound);
    updateInitialPopulationView();
}

void DemesParamWidget::on_initialPopulationDistribution_currentIndexChanged(int index)
{
    setIntervalValueToSelection<int, ABCInterval<int> >(index, 3,
                                                        &Deme::initialPopulation,
                                                        &Deme::setInitialPopulation,
                                                        &ABCInterval<int>::setDistribution);
    updateInitialPopulationView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType, class IntervalType>
void DemesParamWidget::setIntervalValueToSelection(ValueType value, ValueType varyingValue,
                                                   IntervalType (Deme::*demeGetter)() const,
                                                   void (Deme::*demeSetter)(IntervalType),
                                                   void (IntervalType::*intervalSetter)(ValueType))
{
    if (value == varyingValue)
    {
        return;
    }

    Map *map = m_mapWidget->map();
    QListIterator<QPoint> it(m_mapWidget->selection());

    m_mapWidget->setPreventUpdates(true);
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        Deme *deme = map->deme(demeCoord.x(), demeCoord.y());

        if (deme == nullptr)
        {
            continue;
        }

        if (deme->isEnabled())
        {
            IntervalType newValue = (deme->*demeGetter)();
            (newValue.*intervalSetter)(value);
            (deme->*demeSetter)(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
}


void DemesParamWidget::blockUISignals(bool b)
{
    ui->initialPopulationCheckbox->blockSignals(b);
    ui->initialPopulationDistribution->blockSignals(b);
    ui->initialPopulationMaxValue->blockSignals(b);
    ui->initialPopulationMinValue->blockSignals(b);
    ui->initialPopulationSingleValue->blockSignals(b);

    ui->carryingCapacityCheckbox->blockSignals(b);
    ui->carryingCapacityDistribution->blockSignals(b);
    ui->carryingCapacityMaxValue->blockSignals(b);
    ui->carryingCapacityMinValue->blockSignals(b);
    ui->carryingCapacitySingleValue->blockSignals(b);

    ui->growthRateCheckbox->blockSignals(b);
    ui->growthRateDistribution->blockSignals(b);
    ui->growthRateMaxValue->blockSignals(b);
    ui->growthRateMinValue->blockSignals(b);
    ui->growthRateSingleValue->blockSignals(b);

    ui->migrationRateCheckbox->blockSignals(b);
    ui->migrationRateDistribution->blockSignals(b);
    ui->migrationRateMaxValue->blockSignals(b);
    ui->migrationRateMinValue->blockSignals(b);
    ui->migrationRateSingleValue->blockSignals(b);

    ui->sampleSizeSpinBox->blockSignals(b);

    ui->groupComboBox->blockSignals(b);
}
