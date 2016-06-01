#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QPlainTextEdit>

class LogWidget : public QPlainTextEdit
{
    Q_OBJECT

public:

    enum MessageType { Info, Warning, Error };

    static const QString COLORS[];

    LogWidget(QWidget *parent = nullptr);

public slots:

    void log(const QString & msg, int type = 0);
    void logInfo(const QString & msg);
    void logWarning(const QString & msg);
    void logError(const QString & msg);
};

#endif // LOGWIDGET_H
