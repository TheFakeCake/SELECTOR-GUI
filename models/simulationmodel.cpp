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

int SimulationModel::nbGenerations() const
{
    return m_nbGenerations;
}

int SimulationModel::initialAllelePoolSize() const
{
    return m_initialAllelePoolSize;
}

bool SimulationModel::uniqueAllelePool() const
{
    return m_uniqueAllelePool;
}

double SimulationModel::firstAlleleFrequency() const
{
    return m_firstAlleleFrequency;
}

double SimulationModel::mutationRate() const
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

void SimulationModel::setNbGenerations(int n)
{
    if (n != m_nbGenerations)
    {
        if (n > 0)
        {
            m_nbGenerations = n;
        }
        else
        {
            m_nbGenerations = 1;
        }
        emit nbGenerationsChanged(m_nbGenerations);
    }
}

void SimulationModel::setInitialAllelePoolSize(int n)
{
    if (n != m_initialAllelePoolSize)
    {
        if (n > 0)
        {
            m_initialAllelePoolSize = n;
        }
        else
        {
            m_initialAllelePoolSize = 1;
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

void SimulationModel::setFirstAlleleFrequency(double frequency)
{
    if (frequency != m_firstAlleleFrequency)
    {
        if (frequency < 0.0)
        {
            m_firstAlleleFrequency = 0.0;
        }
        else if(frequency > 1.0)
        {
            m_firstAlleleFrequency = 1.0;
        }
        else
        {
            m_firstAlleleFrequency = frequency;
        }
        emit firstAlleleFrequencyChanged(m_firstAlleleFrequency);
    }
    emit firstAlleleFrequencyEnabled(m_firstAlleleFrequency != 0.0);
}

void SimulationModel::setMutationRate(double mutationRate)
{
    if (mutationRate != m_mutationRate)
    {
        if (mutationRate >= 0.0)
        {
            m_mutationRate = mutationRate;
        }
        else
        {
            m_mutationRate = 0.0;
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
