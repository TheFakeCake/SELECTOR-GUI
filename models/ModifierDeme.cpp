#include "ModifierDeme.h"

ModifierDeme::ModifierDeme(unsigned long x, unsigned long y) :
    m_x(x),
    m_y(y),
    m_carryingCapacityModified(false),
    m_growthRateModified(false),
    m_migrationRateModified(false),
    m_absCarryingCapacity(0),
    m_absGrowthRate(0.0),
    m_absMigrationRate(0.0),
    m_relCarryingCapacity(0.0),
    m_relGrowthRate(0.0),
    m_relMigrationRate(0.0)
{
}

unsigned long ModifierDeme::x() const                   { return m_x; }
unsigned long ModifierDeme::y() const                   { return m_y; }
bool ModifierDeme::isCarryingCapacityModified() const   { return m_carryingCapacityModified; }
bool ModifierDeme::isGrowthRateModified() const         { return m_growthRateModified; }
bool ModifierDeme::isMigrationRateModified() const      { return m_migrationRateModified; }
unsigned long ModifierDeme::absCarryingCapacity() const { return m_absCarryingCapacity; }
double ModifierDeme::absGrowthRate() const              { return m_absGrowthRate; }
double ModifierDeme::absMigrationRate() const           { return m_absMigrationRate; }
double ModifierDeme::relCarryingCapacity() const        { return m_relCarryingCapacity; }
double ModifierDeme::relGrowthRate() const              { return m_relGrowthRate; }
double ModifierDeme::relMigrationRate() const           { return m_relMigrationRate; }

void ModifierDeme::setAbsCarryingCapacity(unsigned long capacity)
{
    m_absCarryingCapacity = capacity;
    m_carryingCapacityModified = true;
}

void ModifierDeme::setAbsGrowthRate(double rate)
{
    m_absGrowthRate = rate;
    m_growthRateModified = true;
}

void ModifierDeme::setAbsMigrationRate(double rate)
{
    m_absMigrationRate = rate;
    m_migrationRateModified = true;
}

void ModifierDeme::setRelCarryingCapacity(double factor)
{
    if (factor >= 0)
    {
        m_relCarryingCapacity = factor;
        m_carryingCapacityModified = true;
    }
}

void ModifierDeme::setRelGrowthRate(double factor)
{
    if (factor >= 0)
    {
        m_relGrowthRate = factor;
        m_growthRateModified = true;
    }
}

void ModifierDeme::setRelMigrationRate(double factor)
{
    if (factor >= 0)
    {
        m_relMigrationRate = factor;
        m_migrationRateModified = true;
    }
}

void ModifierDeme::resetCarryingCapacity()
{
    m_carryingCapacityModified = false;
}

void ModifierDeme::resetGrowthRate()
{
    m_growthRateModified = false;
}

void ModifierDeme::resetMigrationRate()
{
    m_migrationRateModified = false;
}

void ModifierDeme::reset()
{
    resetCarryingCapacity();
    resetCarryingCapacity();
    resetMigrationRate();
}
