#include "deme.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Deme::Deme() : Deme(0, 0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Deme::Deme(unsigned long x, unsigned long y) : Deme(x, y, 0, 0, 0.0, 0.0, 1, 0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Deme::Deme(unsigned long x, unsigned long y, unsigned long initialPopulation, unsigned long carryingCapacity,
           double growthRate, double migrationRate, unsigned long group, unsigned long sampleSize)
{
    m_enabled = true;
    move(x, y);
    setInitialPopulation(initialPopulation);
    setCarryingCapacity(carryingCapacity);
    setGrowRate(growthRate);
    setMigrationRate(migrationRate);
    setGroup(group);
    setSampleSize(sampleSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
QString Deme::toString() const
{
    return QString("%1 %2 %3 %4 %5 %6").arg(m_initialPopulation)
                                       .arg(m_enabled ? m_carryingCapacity : -1)
                                       .arg(m_growRate)
                                       .arg(m_migrationRate)
                                       .arg(m_group)
                                       .arg(m_sampleSize);
}

///////////////////////////////////////////// GETTERS //////////////////////////////////////////////
bool Deme::isEnabled() const
{
    return m_enabled;
}

unsigned long Deme::x() const
{
    return m_x;
}

unsigned long Deme::y() const
{
    return m_y;
}

unsigned long Deme::initialPopulation() const
{
    return m_initialPopulation;
}

unsigned long Deme::carryingCapacity() const
{
    return m_carryingCapacity;
}

double Deme::growRate() const
{
    return m_growRate;
}

double Deme::migrationRate() const
{
    return m_migrationRate;
}

unsigned long Deme::group() const
{
    return m_group;
}

unsigned long Deme::sampleSize() const
{
    return m_sampleSize;
}

////////////////////////////////////////////// SETTERS /////////////////////////////////////////////
bool Deme::enable(bool b)
{
    m_enabled = b;
    return true;
}

bool Deme::move(unsigned long x, unsigned long y)
{
    m_x = x;
    m_y = y;
    return true;
}

bool Deme::setInitialPopulation(unsigned long population)
{
    m_initialPopulation = population;
    return true;
}

bool Deme::setCarryingCapacity(unsigned long capacity)
{
    m_carryingCapacity = capacity;
    return true;
}

bool Deme::setGrowRate(double rate)
{
    if (rate >= 0.0 && rate <= 1.0)
    {
        m_growRate = rate;
        return true;
    }
    return false;
}

bool Deme::setMigrationRate(double rate)
{
    if (rate >= 0.0 && rate <= 1.0)
    {
        m_migrationRate = rate;
        return true;
    }
    return false;
}

bool Deme::setGroup(unsigned long group)
{
    if (group > 0)
    {
        m_group = group;
        return true;
    }
    return false;
}

bool Deme::setSampleSize(unsigned long size)
{
    m_sampleSize = size;
    return true;
}
