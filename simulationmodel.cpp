#include "simulationmodel.h"

#include <QFile>
#include <QTextStream>

////////////////////////////////////////////////////////////////////////////////////////////////////
const QString SimulationModel::SELECTOR_PARAM_FILE = QString("selector_param.txt");
const QString SimulationModel::SELECTOR_STRUCTURE_FILE = QString("selector_structure_%1.txt");
const QString SimulationModel::SELECTOR_ALLDEMES_FILE = QString("selector_alldemes_%1.txt");

////////////////////////////////////////////////////////////////////////////////////////////////////
SimulationModel::SimulationModel() : m_nbSimulations(1), m_nbGenerations(1), m_initialAllelePoolSize(20),
    m_uniqueAllelePool(false), m_firstAlleleFrequency(0.0), m_mutationRate(0.0), m_outputCompleteHistory(false)
{
}

//////////////////////////////////////////// GETTERS ///////////////////////////////////////////////
unsigned long SimulationModel::nbSimulations() const
{
    return m_nbSimulations;
}

unsigned long SimulationModel::nbGenerations() const
{
    return m_nbGenerations;
}

unsigned long SimulationModel::initialAllelePoolSize() const
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
bool SimulationModel::setNbSimulations(unsigned long n)
{
    if (n > 0)
    {
        m_nbSimulations = n;
        return true;
    }
    return false;
}

bool SimulationModel::setNbGenerations(unsigned long n)
{
    if (n > 0)
    {
        m_nbGenerations = n;
        return true;
    }
    return false;
}

bool SimulationModel::setInitialAllelePoolSize(unsigned long n)
{
    if (n > 0)
    {
        m_initialAllelePoolSize = n;
        return true;
    }
    return false;
}

bool SimulationModel::setUniqueAllelePool(bool b)
{
    m_uniqueAllelePool = b;
    return true;
}

bool SimulationModel::setFirstAlleleFrequency(double frequency)
{
    if (frequency >= 0.0 && frequency <= 1.0)
    {
        m_firstAlleleFrequency = frequency;
        return true;
    }
    return false;
}

bool SimulationModel::setMutationRate(double mutationRate)
{
    if (mutationRate >= 0.0)
    {
        m_mutationRate = mutationRate;
        return false;
    }
    return false;
}

bool SimulationModel::setOutputCompleteHistory(bool b)
{
    m_outputCompleteHistory = b;
    return true;
}
