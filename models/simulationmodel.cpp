#include "simulationmodel.h"

#include <QFile>
#include <QTextStream>

////////////////////////////////////////////////////////////////////////////////////////////////////
SimulationModel::SimulationModel(QObject *parent) :
    QObject(parent),
    m_nbSimulations(1),
    m_nbGenerations(1),
    m_initialAllelePoolSize(20),
    m_uniqueAllelePool(true),
    m_firstAlleleFrequency(0.0),
    m_mutationRate(0.0),
    m_outputCompleteHistory(false)
{
}

//////////////////////////////////////////// GETTERS ///////////////////////////////////////////////
int SimulationModel::nbSimulations() const
{
    return m_nbSimulations;
}

ABCInterval<int> SimulationModel::nbGenerations() const
{
    return m_nbGenerations;
}

ABCInterval<int> SimulationModel::initialAllelePoolSize() const
{
    return m_initialAllelePoolSize;
}

bool SimulationModel::uniqueAllelePool() const
{
    return m_uniqueAllelePool;
}

ABCInterval<double> SimulationModel::firstAlleleFrequency() const
{
    return m_firstAlleleFrequency;
}

ABCInterval<double> SimulationModel::mutationRate() const
{
    return m_mutationRate;
}

Selection* SimulationModel::selection()
{
    return &m_selection;
}

bool SimulationModel::outputCompleteHistory() const
{
    return m_outputCompleteHistory;
}

Map* SimulationModel::map()
{
    return &m_map;
}

/////////////////////////////////////////// SETTERS ////////////////////////////////////////////////
void SimulationModel::setNbSimulations(int n)
{
    if (n != m_nbSimulations)
    {
        if (n > 0)
        {
            m_nbSimulations = n;
        }
        else
        {
            m_nbSimulations = 1;
        }
        emit nbSimulationsChanged(m_nbSimulations);
    }
}

void SimulationModel::setNbGenerations(ABCInterval<int> n)
{
    if (n != m_nbGenerations)
    {
        m_nbGenerations = n;

        if (m_nbGenerations < 1)
        {
            n.setMinimum(1);
        }
        emit nbGenerationsChanged(m_nbGenerations);
    }
}

void SimulationModel::setInitialAllelePoolSize(ABCInterval<int> n)
{
    if (n != m_initialAllelePoolSize)
    {
        m_initialAllelePoolSize = n;

        if (m_initialAllelePoolSize < 1)
        {
            m_initialAllelePoolSize.setMinimum(1);
        }
        emit initialAllelePoolSizeChanged(m_initialAllelePoolSize);
    }
}

void SimulationModel::setUniqueAllelePool(bool b)
{
    if (b != m_uniqueAllelePool)
    {
        m_uniqueAllelePool = b;
        emit uniqueAllelePoolChanged(m_uniqueAllelePool);
    }
}

void SimulationModel::enableFirstAlleleFrequency(bool b)
{
    if (!b)
    {
        setFirstAlleleFrequency(0.0);
    }
}

void SimulationModel::setFirstAlleleFrequency(ABCInterval<double> frequency)
{
    if (frequency != m_firstAlleleFrequency)
    {
        m_firstAlleleFrequency = frequency;

        if (m_firstAlleleFrequency < 0.0)
        {
            m_firstAlleleFrequency.setMinimum(0.0);
        }
        if (m_firstAlleleFrequency > 1.0)
        {
            m_firstAlleleFrequency.setMaximum(1.0);
        }
        emit firstAlleleFrequencyChanged(m_firstAlleleFrequency);
    }
    emit firstAlleleFrequencyEnabled(m_firstAlleleFrequency != 0.0);
}

void SimulationModel::setMutationRate(ABCInterval<double> mutationRate)
{
    if (mutationRate != m_mutationRate)
    {
        m_mutationRate = mutationRate;

        if (m_mutationRate < 0.0)
        {
            m_mutationRate.setMinimum(0.0);
        }
        emit mutationRateChanged(m_mutationRate);
    }
}

void SimulationModel::setOutputCompleteHistory(bool b)
{
    if (b != m_outputCompleteHistory)
    {
        m_outputCompleteHistory = b;
        emit outputCompleteHistoryChanged(m_outputCompleteHistory);
    }
}
