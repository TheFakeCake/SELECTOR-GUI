#ifndef SELECTORCONFIGFILES_H
#define SELECTORCONFIGFILES_H

#include <QObject>
#include <QString>
#include "models/simulationmodel.h"

class SelectorConfigFiles : public QObject
{
    Q_OBJECT

public:

    static QString ABCIntervalToStr(ABCInterval<int> interval);
    static QString ABCIntervalToStr(ABCInterval<double> interval);

    SelectorConfigFiles();
    SelectorConfigFiles(QString directory);

    bool read(SimulationModel *sim);
    bool write(SimulationModel *sim);

    QString directory() const;
    void setDirectory(QString directory);

    const QStringList& errorMessages() const;

signals:

    void info(const QString & msg);
    void warning(const QString & msg);
    void error(const QString & msg);

private:

    static const QString SELECTOR_PARAM_FILE;
    static const QString SELECTOR_STRUCTURE_FILE;
    static const QString SELECTOR_ALLDEMES_FILE;
    static const QString DISTRIBUTIONS[];

    template<typename T>
    static ABCInterval<T> toInterval(const QString &singleValueCap, const QString &distributionCap,
                                     const QString &firstBoundCap, const QString &secondBoundCap);

    void _handleParamLine(SimulationModel *sim, int line, QRegExp *regex,
                          bool &groupedDemes, int &nbRoutes, int &nbStructures);
    void _readAllDemesFiles(SimulationModel *sim);
    void _readStructureFiles(SimulationModel *sim);

    QString m_directory;
    QStringList m_errorMessages;
};

#endif // SELECTORCONFIGFILES_H
