#include "VaryingABCIntervalIntWidget.h"
#include "ui_ABCIntervalIntWidget.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
VaryingABCIntervalIntWidget::VaryingABCIntervalIntWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ABCIntervalIntWidget)
{
    ui->setupUi(this);
    m_minimumValue = ui->singleValue->minimum();
    m_varyingValue = m_minimumValue - 1;
    m_demeGetter = &Deme::initialPopulation;
    m_demeSetter = &Deme::setInitialPopulation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
VaryingABCIntervalIntWidget::~VaryingABCIntervalIntWidget()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::setMinimumValue(int min)
{
    ui->singleValue->setMinimum(min);
    ui->firstBound->setMinimum(min);
    ui->secondBound->setMinimum(min);
    m_minimumValue = min;
    m_varyingValue = min - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::setMaximumValue(int max)
{
    ui->singleValue->setMaximum(max);
    ui->firstBound->setMaximum(max);
    ui->secondBound->setMaximum(max);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::init(MapWidget *mapWidget, ABCInterval<int> (Deme::*demeGetter)() const, void (Deme::*demeSetter)(ABCInterval<int>))
{
    m_mapWidget = mapWidget;
    m_demeGetter = demeGetter;
    m_demeSetter = demeSetter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::on_ABC_toggled(bool checked)
{
    if (checked)
    {
        blockUISignals(true);
        ui->firstBound->setValue(ui->singleValue->value());
        ui->secondBound->setValue(ui->singleValue->value());
        ui->stackedWidget->setCurrentIndex(1);
        blockUISignals(false);
    }
    else
    {
        ui->singleValue->setValue(ui->firstBound->value());
        ui->stackedWidget->setCurrentIndex(0);

        if (ui->firstBound->value() != ui->secondBound->value())
        {
            setIntervalValueToSelection(ui->singleValue->value(), m_varyingValue, &ABCInterval<int>::setValue);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::on_singleValue_valueChanged(int value)
{
    setIntervalValueToSelection(value, m_varyingValue, &ABCInterval<int>::setValue);
    updateView(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::on_firstBound_valueChanged(int value)
{
    setIntervalValueToSelection(value, m_varyingValue, &ABCInterval<int>::setFirstBound);
    updateView(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::on_secondBound_valueChanged(int value)
{
    setIntervalValueToSelection(value, m_varyingValue, &ABCInterval<int>::setSecondBound);
    updateView(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::on_distribution_currentIndexChanged(int index)
{
    setIntervalValueToSelection(index, 3, &ABCInterval<int>::setDistribution);
    updateView(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::updateView(bool keepCurrentStackIndex)
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
            viewInterval = (deme->*m_demeGetter)();
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

        ABCInterval<int> demeValue = (deme->*m_demeGetter)();

        if (demeValue.isFixedValue() != !viewABC)
        {
            viewABC = false;
        }
        if (demeValue.firstBound() != viewInterval.firstBound())
        {
            viewInterval.setFirstBound(m_varyingValue);
        }
        if (demeValue.secondBound() != viewInterval.secondBound())
        {
            viewInterval.setSecondBound(m_varyingValue);
        }
        if (demeValue.distribution() != viewInterval.distribution())
        {
            varyingDistribution = true;
        }
    }

    // Display the data in the views
    blockUISignals(true);

    // Set the current index of the stacked widget
    if (! keepCurrentStackIndex)
    {
        ui->stackedWidget->setCurrentIndex(viewABC ? 1 : 0);
    }

    // Single value and first bound widgets. If the value is varying, the special text and minimum
    // value is applied.
    if (viewInterval.firstBound() == m_varyingValue)
    {
        ui->singleValue->setMinimum(m_varyingValue);
        ui->singleValue->setSpecialValueText("Varying");
        ui->firstBound->setMinimum(m_varyingValue);
        ui->firstBound->setSpecialValueText("Varying");
    }
    else
    {
        ui->singleValue->setMinimum(m_minimumValue);
        ui->singleValue->setSpecialValueText("");
        ui->firstBound->setMinimum(m_minimumValue);
        ui->firstBound->setSpecialValueText("");
    }

    // Second bound widget. If the value is varying, the special text and minimum value is applied.
    if (viewInterval.secondBound() == m_varyingValue)
    {
        ui->secondBound->setMinimum(m_varyingValue);
        ui->secondBound->setSpecialValueText("Varying");
    }
    else
    {
        ui->secondBound->setMinimum(m_minimumValue);
        ui->secondBound->setSpecialValueText("");
    }

    // Distribution widget. If the value is varying, the "Varying" item is added to the combobox.
    ui->distribution->removeItem(3);
    if (varyingDistribution)
    {
        ui->distribution->addItem("Varying");
        ui->distribution->setCurrentIndex(3);
    }
    else
    {
        ui->distribution->setCurrentIndex(viewInterval.distribution());
    }

    // Put data into the widgets
    ui->singleValue->setValue(viewInterval.firstBound());
    ui->firstBound->setValue(viewInterval.firstBound());
    ui->secondBound->setValue(viewInterval.secondBound());
    ui->ABC->setChecked(viewABC);

    blockUISignals(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::setIntervalValueToSelection(int value, int varyingValue, void (ABCInterval<int>::*intervalSetter)(int))
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
            ABCInterval<int> newValue = (deme->*m_demeGetter)();
            (newValue.*intervalSetter)(value);
            (deme->*m_demeSetter)(newValue);
        }
    }
    m_mapWidget->setPreventUpdates(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void VaryingABCIntervalIntWidget::blockUISignals(bool b)
{
    ui->singleValue->blockSignals(b);
    ui->firstBound->blockSignals(b);
    ui->secondBound->blockSignals(b);
    ui->distribution->blockSignals(b);
    ui->ABC->blockSignals(b);
}
