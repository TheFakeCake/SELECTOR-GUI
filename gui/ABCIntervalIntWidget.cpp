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
        return ABCInterval<int>(ui->firstBound->value(),
                                ui->secondBound->value(),
                                (ABCInterval<int>::Distribution)ui->distribution->currentIndex());
    }
    return ABCInterval<int>(ui->singleValue->value());
}

void ABCIntervalIntWidget::setMinimumValue(int min)
{
    ui->singleValue->setMinimum(min);
    ui->firstBound->setMinimum(min);
    ui->secondBound->setMinimum(min);
}

void ABCIntervalIntWidget::setMaximumValue(int max)
{
    ui->singleValue->setMaximum(max);
    ui->firstBound->setMaximum(max);
    ui->secondBound->setMaximum(max);
}

void ABCIntervalIntWidget::blockSignals(bool b)
{
    ui->ABC->blockSignals(b);
    ui->firstBound->blockSignals(b);
    ui->secondBound->blockSignals(b);
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
            ui->singleValue->setValue(value.firstBound());
        }
        else
        {
            ui->ABC->setChecked(true);
            ui->stackedWidget->setCurrentWidget(ui->intervalPage);
            ui->firstBound->setValue(value.firstBound());
            ui->secondBound->setValue(value.secondBound());
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
            if (newValue.firstBound() != oldValue.firstBound())
            {
                emit firstBoundChanged(newValue.firstBound());
            }
            if (newValue.secondBound() != oldValue.secondBound())
            {
                emit secondBoundChanged(newValue.secondBound());
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
    if (checked)
    {
        blockSignals(true);
        ui->firstBound->setValue(ui->singleValue->value());
        ui->secondBound->setValue(ui->singleValue->value());
        ui->stackedWidget->setCurrentIndex(1);
        blockSignals(false);
    }
    else
    {
        ui->singleValue->setValue(ui->firstBound->value());
        ui->stackedWidget->setCurrentIndex(0);

        // Emit a valueChanged signal only if the interval wasn't a single value
        if (ui->firstBound->value() != ui->secondBound->value())
        {
            emit singleValueChanged(ui->singleValue->value());
            emit valueChanged(this->value());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalIntWidget::on_singleValue_valueChanged(int arg1)
{
    emit singleValueChanged(arg1);
    emit valueChanged(this->value());
}

void ABCIntervalIntWidget::on_firstBound_valueChanged(int arg1)
{
    emit firstBoundChanged(arg1);
    emit valueChanged(this->value());
}

void ABCIntervalIntWidget::on_secondBound_valueChanged(int arg1)
{
    emit secondBoundChanged(arg1);
    emit valueChanged(this->value());
}

void ABCIntervalIntWidget::on_distribution_currentIndexChanged(int index)
{
    emit distributionChanged(index);
    emit valueChanged(this->value());
}
