#include "ABCIntervalDoubleWidget.h"
#include "ui_ABCIntervalDoubleWidget.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
ABCIntervalDoubleWidget::ABCIntervalDoubleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ABCIntervalDoubleWidget)
{
    ui->setupUi(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ABCIntervalDoubleWidget::~ABCIntervalDoubleWidget()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ABCInterval<double> ABCIntervalDoubleWidget::value() const
{
    if (ui->ABC->isChecked())
    {
        return ABCInterval<double>(ui->firstBound->value(),
                                   ui->secondBound->value(),
                                   (ABCInterval<double>::Distribution)ui->distribution->currentIndex());
    }
    return ABCInterval<double>(ui->singleValue->value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::setMinimumValue(double min)
{
    ui->singleValue->setMinimum(min);
    ui->firstBound->setMinimum(min);
    ui->secondBound->setMinimum(min);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::setMaximumValue(double max)
{
    ui->singleValue->setMaximum(max);
    ui->firstBound->setMaximum(max);
    ui->secondBound->setMaximum(max);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::blockSignals(bool b)
{
    ui->ABC->blockSignals(b);
    ui->firstBound->blockSignals(b);
    ui->secondBound->blockSignals(b);
    ui->singleValue->blockSignals(b);
    ui->distribution->blockSignals(b);
    QWidget::blockSignals(b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::setValue(const ABCInterval<double> &value)
{
    ABCInterval<double> oldValue = this->value();

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

        ABCInterval<double> newValue = this->value();

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
void ABCIntervalDoubleWidget::on_ABC_toggled(bool checked)
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
void ABCIntervalDoubleWidget::on_singleValue_valueChanged(double value)
{
    emit singleValueChanged(value);
    emit valueChanged(this->value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::on_firstBound_valueChanged(double value)
{
    emit firstBoundChanged(value);
    emit valueChanged(this->value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::on_secondBound_valueChanged(double value)
{
    emit secondBoundChanged(value);
    emit valueChanged(this->value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::on_distribution_currentIndexChanged(int index)
{
    emit distributionChanged(index);
    emit valueChanged(this->value());
}
