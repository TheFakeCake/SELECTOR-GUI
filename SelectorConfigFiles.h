#ifndef SELECTORCONFIGFILES_H
#define SELECTORCONFIGFILES_H

#include <QString>
#include "simulationmodel.h"

class SelectorConfigFiles
{
public:

    SelectorConfigFiles();
    SelectorConfigFiles(QString directory);

    bool read(SimulationModel *sim);
    bool write(SimulationModel *sim);

    QString directory() const;
    void setDirectory(QString directory);

    const QStringList& errorMessages() const;

private:

    static const QString SELECTOR_PARAM_FILE;
    static const QString SELECTOR_STRUCTURE_FILE;
    static const QString SELECTOR_ALLDEMES_FILE;

    void _handleParamLine(SimulationModel *sim, unsigned long line, QRegExp *regex,
                          bool &groupedDemes, long &nbRoutes, long &nbStructures);
    void _readAllDemesFiles(SimulationModel *sim);
    void _readStructureFiles(SimulationModel *sim);

    QString m_directory;
    QStringList m_errorMessages;
};

#endif // SELECTORCONFIGFILES_H
