#include "ABCIntervalIntWidget.h"
#include "ui_ABCIntervalIntWidget.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
ABCIntervalIntWidget::ABCIntervalIntWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ABCIntervalIntWidget)
{
    ui->setupUi(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ABCIntervalIntWidget::~ABCIntervalIntWidget()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ABCInterval<int> ABCIntervalIntWidget::value() const
{
    if (ui->ABC->isChecked())
    {
        return ABCInterval<int>(ui->minValue->value(),
                                ui->maxValue->value(),
                                (ABCInterval<int>::Distribution)ui->distribution->currentIndex());
    }
    return ABCInterval<int>(ui->singleValue->value());
}

void ABCIntervalIntWidget::setMinimumValue(int min)
{
    ui->singleValue->setMinimum(min);
    ui->minValue->setMinimum(min);
    ui->maxValue->setMinimum(min);
}

void ABCIntervalIntWidget::setMaximumValue(int max)
{
    ui->singleValue->setMaximum(max);
    ui->minValue->setMaximum(max);
    ui->maxValue->setMaximum(max);
}

void ABCIntervalIntWidget::setSpecialValueText(const QString &text)
{
    ui->singleValue->setSpecialValueText(text);
    ui->minValue->setSpecialValueText(text);
    ui->maxValue->setSpecialValueText(text);
}

void ABCIntervalIntWidget::blockSignals(bool b)
{
    ui->ABC->blockSignals(b);
    ui->minValue->blockSignals(b);
    ui->maxValue->blockSignals(b);
    ui->singleValue->blockSignals(b);
    ui->distribution->blockSignals(b);
    QWidget::blockSignals(b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalIntWidget::setValue(const ABCInterval<int> &value)
{
    ABCInterval<int> oldValue = this->value();

    if (value != oldValue)
    {
        blockSignals(true);
        if (value.isFixedValue())
        {
            ui->ABC->setChecked(false);
            ui->stackedWidget->setCurrentWidget(ui->singleValuePage);
            ui->singleValue->setValue(value.maximum());
        }
        else
        {
            ui->ABC->setChecked(true);
            ui->stackedWidget->setCurrentWidget(ui->intervalPage);
            ui->minValue->setValue(value.minimum());
            ui->maxValue->setValue(value.maximum());
        }
        blockSignals(false);

        ABCInterval<int> newValue = this->value();

        // Signals emmiting
        if (newValue.isFixedValue())
        {
            emit singleValueChanged(newValue.minimum());
        }
        else
        {
            if (newValue.minimum() != oldValue.minimum())
            {
                emit minimumChanged(newValue.minimum());
            }
            if (newValue.maximum() != oldValue.maximum())
            {
                emit maximumChanged(newValue.maximum());
            }
            if (newValue.distribution() != newValue.distribution())
            {
                emit distributionChanged(newValue.distribution());
            }
        }
        emit valueChanged(newValue);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalIntWidget::on_ABC_toggled(bool checked)
{
    blockSignals(true);
    if (checked)
    {
        ui->minValue->setValue(ui->singleValue->value());
        ui->maxValue->setValue(ui->singleValue->value());
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->singleValue->setValue(ui->minValue->value());
        ui->stackedWidget->setCurrentIndex(0);

        // Emit a valueChanged signal only if the interval wasn't a single value
        if (ui->minValue->value() != ui->maxValue->value())
        {
            emit singleValueChanged(ui->singleValue->value());
            emit valueChanged(this->value());
        }
    }
    blockSignals(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalIntWidget::on_singleValue_valueChanged(int arg1)
{
    emit singleValueChanged(arg1);
    emit valueChanged(this->value());
}

void ABCIntervalIntWidget::on_minValue_valueChanged(int arg1)
{

    if (arg1 > ui->maxValue->value())
    {
        ui->minValue->setValue(ui->maxValue->value());
    }
    else
    {
        emit minimumChanged(arg1);
        emit valueChanged(this->value());
    }
}

void ABCIntervalIntWidget::on_maxValue_valueChanged(int arg1)
{
    if (arg1 < ui->minValue->value())
    {
        ui->maxValue->setValue(ui->minValue->value());
    }
    else
    {
        emit maximumChanged(arg1);
        emit valueChanged(this->value());
    }
}

void ABCIntervalIntWidget::on_distribution_currentIndexChanged(int index)
{
    emit distributionChanged(index);
    emit valueChanged(this->value());
}
