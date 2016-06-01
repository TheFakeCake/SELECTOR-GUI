#include "LogWidget.h"

LogWidget::LogWidget(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setReadOnly(true);
}

void LogWidget::log(const QString &msg, int type)
{
    appendHtml(QString("<span color=\"%1\">%2</span>").arg("red", msg));
}

void LogWidget::logInfo(const QString &msg)
{
    log(msg, Info);
}

void LogWidget::logWarning(const QString &msg)
{
    log(msg, Warning);
}

void LogWidget::logError(const QString &msg)
{
    log(msg, Error);
}
