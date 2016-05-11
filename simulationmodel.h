#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <QString>
#include "selection.h"
#include "map.h"

class SimulationModel
{
public:

    static const QString SELECTOR_PARAM_FILE;
    static const QString SELECTOR_STRUCTURE_FILE;
    static const QString SELECTOR_ALLDEMES_FILE;

    SimulationModel();

    bool loadFromFiles(QString directory);
    bool writeToFiles(QString directory);

    unsigned long nbSimulations() const;
    unsigned long nbGenerations() const;
    unsigned long initialAllelePoolSize() const;
    bool uniqueAllelePool() const;
    double firstAlleleFrequency() const;
    double mutationRate() const;
    Selection* selection();
    bool outputCompleteHistory() const;
    Map* map();

    bool setNbSimulations(unsigned long n);
    bool setNbGenerations(unsigned long n);
    bool setInitialAllelePoolSize(unsigned long n);
    bool setUniqueAllelePool(bool b);
    bool setFirstAlleleFrequency(double frequency);
    bool setMutationRate(double mutationRate);
    bool setOutputCompleteHistory(bool b);

protected:

    unsigned long m_nbSimulations;
    unsigned long m_nbGenerations;

    unsigned long m_initialAllelePoolSize;
    bool m_uniqueAllelePool;
    double m_firstAlleleFrequency;
    double m_mutationRate;

    Selection m_selection;

    double m_outputCompleteHistory;

    Map m_map;
};

#endif // SIMULATIONMODEL_H
