#include "DemesParamWidget.h"
#include "ui_DemesParamWidget.h"

DemesParamWidget::DemesParamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemesParamWidget)
{
    ui->setupUi(this);
}

DemesParamWidget::~DemesParamWidget()
{
    delete ui;
}
