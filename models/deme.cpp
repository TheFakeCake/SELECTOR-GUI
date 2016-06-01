#include "deme.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Deme::Deme(Map *parentMap, int x, int y) :
    QObject(parentMap),
    m_parentMap(parentMap),
    m_enabled(true),
    m_x(x),
    m_y(y),
    m_initialPopulation(0),
    m_carryingCapacity(0),
    m_growthRate(0.0),
    m_migrationRate(0.0),
    m_group(1),
    m_sampleSize(0)
{
}

///////////////////////////////////////////// GETTERS //////////////////////////////////////////////
bool Deme::isEnabled() const
{
    return m_enabled;
}

int Deme::x() const
{
    return m_x;
}

int Deme::y() const
{
    return m_y;
}

int Deme::initialPopulation() const
{
    return m_initialPopulation;
}

int Deme::carryingCapacity() const
{
    return m_carryingCapacity;
}

double Deme::growthRate() const
{
    return m_growthRate;
}

double Deme::migrationRate() const
{
    return m_migrationRate;
}

int Deme::group() const
{
    return m_group;
}

int Deme::sampleSize() const
{
    return m_sampleSize;
}

////////////////////////////////////////////// SETTERS /////////////////////////////////////////////
void Deme::enable(bool b)
{
    if (b != m_enabled)
    {
        m_enabled = b;
        emit enabledChanged(m_enabled);
        m_parentMap->dispatchDemeChange(this);
    }
}

void Deme::setInitialPopulation(int population)
{
    if (population != m_initialPopulation)
    {
        if (population >= 0)
        {
            m_initialPopulation = population;
        }
        else
        {
            m_initialPopulation = 0;
        }
        emit initialPopulationChanged(m_initialPopulation);
        m_parentMap->dispatchDemeChange(this);
    }
}

void Deme::setCarryingCapacity(int capacity)
{
    if (capacity != m_carryingCapacity)
    {
        if (capacity >= 0)
        {
            m_carryingCapacity = capacity;
        }
        else
        {
            m_carryingCapacity = 0;
        }
        emit carryingCapacityChanged(m_carryingCapacity);
        m_parentMap->dispatchDemeChange(this);
    }
}

void Deme::setGrowthRate(double rate)
{
    if (! qFuzzyCompare(1.0 + rate, 1.0 + m_growthRate))
    {
        if (rate < 0.0)
        {
            m_growthRate = 0.0;
        }
        else if (rate > 1.0)
        {
            m_growthRate = 1.0;
        }
        else
        {
            m_growthRate = rate;
        }
        emit growthRateChanged(m_growthRate);
        m_parentMap->dispatchDemeChange(this);
    }
}

void Deme::setMigrationRate(double rate)
{
    if (! qFuzzyCompare(1.0 + rate, 1 + m_migrationRate))
    {
        if (rate < 0.0)
        {
            m_migrationRate = 0.0;
        }
        else if (rate > 1.0)
        {
            m_migrationRate = 1.0;
        }
        else
        {
            m_migrationRate = rate;
        }
        emit migrationRateChanged(m_migrationRate);
        m_parentMap->dispatchDemeChange(this);
    }
}

void Deme::setGroup(int group)
{
    if (group != m_group)
    {
        if (group > 0)
        {
            m_group = group;
        }
        else
        {
            m_group = 1;
        }
        emit groupChanged(m_group);
        m_parentMap->dispatchDemeChange(this);
    }
}

void Deme::setSampleSize(int size)
{
    if (size != m_sampleSize)
    {
        if (size >= 0)
        {
            m_sampleSize = size;
        }
        else
        {
            m_sampleSize = 0;
        }
        emit sampleSizeChanged(m_sampleSize);
        m_parentMap->dispatchDemeChange(this);
    }
}
