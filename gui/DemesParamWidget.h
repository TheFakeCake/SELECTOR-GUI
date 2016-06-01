#ifndef DEMESPARAMWIDGET_H
#define DEMESPARAMWIDGET_H

#include <QWidget>

namespace Ui {
class DemesParamWidget;
}

class DemesParamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DemesParamWidget(QWidget *parent = 0);
    ~DemesParamWidget();

private:
    Ui::DemesParamWidget *ui;
};

#endif // DEMESPARAMWIDGET_H
