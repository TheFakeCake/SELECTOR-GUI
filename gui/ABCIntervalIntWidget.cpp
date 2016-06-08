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
                                (ABCInterval::Distribution)ui->distribution->currentIndex());
    }
    return ABCInterval<int>(ui->singleValue->value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalIntWidget::setValue(const ABCInterval<int> &value)
{
    if (value != this->value())
    {
        blockUISignals(true);
        if (value.isFixedValue())
        {
            ui->ABC->setChecked(false);
            ui->singleValue->setValue(value.maximum());
        }
        else
        {
            ui->ABC->setChecked(true);
            ui->minValue->setValue(value.minimum());
            ui->maxValue->setValue(value.maximum());
        }
        blockUISignals(false);
        emit valueChanged(this->value());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ABCIntervalIntWidget::on_ABC_toggled(bool checked)
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
void ABCIntervalIntWidget::blockUISignals(bool b)
{
    ui->ABC->blockSignals(b);
    ui->minValue->blockSignals(b);
    ui->maxValue->blockSignals(b);
    ui->singleValue->blockSignals(b);
    ui->distribution->blockSignals(b);
}
