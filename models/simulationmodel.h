#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <QObject>
#include <QString>
#include "selection.h"
#include "map.h"
#include "ABCInterval.h"

class SimulationModel : public QObject
{
    Q_OBJECT

public:

    SimulationModel(QObject *parent = nullptr);

    int nbSimulations() const;
    ABCInterval<int> nbGenerations() const;
    ABCInterval<int> initialAllelePoolSize() const;
    bool uniqueAllelePool() const;
    ABCInterval<double> firstAlleleFrequency() const;
    ABCInterval<double> mutationRate() const;
    Selection* selection();
    bool outputCompleteHistory() const;
    Map* map();

public slots:

    void setNbSimulations(int n);
    void setNbGenerations(ABCInterval<int> n);
    void setInitialAllelePoolSize(ABCInterval<int> n);
    void setUniqueAllelePool(bool b);
    void enableFirstAlleleFrequency(bool b);
    void setFirstAlleleFrequency(ABCInterval<double> frequency);
    void setMutationRate(ABCInterval<double> mutationRate);
    void setOutputCompleteHistory(bool b);

signals:

    void nbSimulationsChanged(int newNbSimulations);
    void nbGenerationsChanged(ABCInterval<int> newNbGenerations);
    void initialAllelePoolSizeChanged(ABCInterval<int> newPoolSize);
    void uniqueAllelePoolChanged(bool newUniqueAllelePool);
    void firstAlleleFrequencyEnabled(bool enabled);
    void firstAlleleFrequencyChanged(ABCInterval<double> newFrequency);
    void mutationRateChanged(ABCInterval<double> newRate);
    void outputCompleteHistoryChanged(bool newOutput);

protected:

    int m_nbSimulations;
    ABCInterval<int> m_nbGenerations;

    ABCInterval<int> m_initialAllelePoolSize;
    bool m_uniqueAllelePool;
    ABCInterval<double> m_firstAlleleFrequency;
    ABCInterval<double> m_mutationRate;

    Selection m_selection;

    bool m_outputCompleteHistory;

    Map m_map;
};

#endif // SIMULATIONMODEL_H
