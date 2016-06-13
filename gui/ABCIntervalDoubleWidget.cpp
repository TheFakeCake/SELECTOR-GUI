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
        return ABCInterval<double>(ui->minValue->value(),
                                   ui->maxValue->value(),
                                   (ABCInterval<double>::Distribution)ui->distribution->currentIndex());
    }
    return ABCInterval<double>(ui->singleValue->value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::setMinimumValue(double min)
{
    ui->singleValue->setMinimum(min);
    ui->minValue->setMinimum(min);
    ui->maxValue->setMinimum(min);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::setMaximumValue(double max)
{
    ui->singleValue->setMaximum(max);
    ui->minValue->setMaximum(max);
    ui->maxValue->setMaximum(max);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::setSpecialValueText(const QString &text)
{
    ui->singleValue->setSpecialValueText(text);
    ui->minValue->setSpecialValueText(text);
    ui->maxValue->setSpecialValueText(text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::setValue(const ABCInterval<double> &value)
{
    if (value != this->value())
    {
        blockUISignals(true);
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
        blockUISignals(false);
        emit valueChanged(this->value());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::on_ABC_toggled(bool checked)
{
    blockUISignals(true);
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
            emit valueChanged(this->value());
        }
    }
    blockUISignals(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalDoubleWidget::blockUISignals(bool b)
{
    ui->ABC->blockSignals(b);
    ui->minValue->blockSignals(b);
    ui->maxValue->blockSignals(b);
    ui->singleValue->blockSignals(b);
    ui->distribution->blockSignals(b);
}

void ABCIntervalDoubleWidget::on_singleValue_valueChanged(double arg1)
{
    emit minimumChanged(arg1);
    emit maximumChanged(arg1);
    emit valueChanged(this->value());
}

void ABCIntervalDoubleWidget::on_minValue_valueChanged(double arg1)
{
    emit minimumChanged(arg1);
    emit valueChanged(this->value());
}

void ABCIntervalDoubleWidget::on_maxValue_valueChanged(double arg1)
{
    emit maximumChanged(arg1);
    emit valueChanged(this->value());
}

void ABCIntervalDoubleWidget::on_distribution_currentIndexChanged(int index)
{
    emit distributionChanged(index);
    emit valueChanged(this->value());
}
