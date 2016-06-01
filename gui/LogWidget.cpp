#include "LogWidget.h"

const QString LogWidget::COLORS[] = {
    QString("#0033cc"),
    QString("#cc9900"),
    QString("#cc0000")
};

LogWidget::LogWidget(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setReadOnly(true);
}

void LogWidget::log(const QString &msg, int type)
{
    if (type < 0 || type > 2)
    {
        type = 0;
    }
    appendHtml(QString("<font color=\"%1\">%2</font>").arg(COLORS[type], msg));
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
