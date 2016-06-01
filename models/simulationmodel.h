#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <QObject>
#include <QString>
#include "selection.h"
#include "map.h"

class SimulationModel : public QObject
{
    Q_OBJECT

public:

    SimulationModel(QObject *parent = nullptr);

    int nbSimulations() const;
    int nbGenerations() const;
    int initialAllelePoolSize() const;
    bool uniqueAllelePool() const;
    double firstAlleleFrequency() const;
    double mutationRate() const;
    Selection* selection();
    bool outputCompleteHistory() const;
    Map* map();

public slots:

    void setNbSimulations(int n);
    void setNbGenerations(int n);
    void setInitialAllelePoolSize(int n);
    void setUniqueAllelePool(bool b);
    void enableFirstAlleleFrequency(bool b);
    void setFirstAlleleFrequency(double frequency);
    void setMutationRate(double mutationRate);
    void setOutputCompleteHistory(bool b);

signals:

    void nbSimulationsChanged(int newNbSimulations);
    void nbGenerationsChanged(int newNbGenerations);
    void initialAllelePoolSizeChanged(int newPoolSize);
    void uniqueAllelePoolChanged(bool newUniqueAllelePool);
    void firstAlleleFrequencyEnabled(bool enabled);
    void firstAlleleFrequencyChanged(double newFrequency);
    void mutationRateChanged(double newRate);
    void outputCompleteHistoryChanged(bool newOutput);

protected:

    int m_nbSimulations;
    int m_nbGenerations;

    int m_initialAllelePoolSize;
    bool m_uniqueAllelePool;
    double m_firstAlleleFrequency;
    double m_mutationRate;

    Selection m_selection;

    double m_outputCompleteHistory;

    Map m_map;
};

#endif // SIMULATIONMODEL_H
