#ifndef SELECTORCONFIGFILES_H
#define SELECTORCONFIGFILES_H

#include <QString>
#include "models/simulationmodel.h"

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

    void _handleParamLine(SimulationModel *sim, int line, QRegExp *regex,
                          bool &groupedDemes, int &nbRoutes, int &nbStructures);
    void _readAllDemesFiles(SimulationModel *sim);
    void _readStructureFiles(SimulationModel *sim);

    QString m_directory;
    QStringList m_errorMessages;
};

#endif // SELECTORCONFIGFILES_H
