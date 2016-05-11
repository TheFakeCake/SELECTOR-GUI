#ifndef DEME_H
#define DEME_H

#include <QString>

class Deme
{
public:

    Deme();
    Deme(unsigned long x, unsigned long y);
    Deme(unsigned long x, unsigned long y, unsigned long initialPopulation, unsigned long carryingCapacity,
         double growthRate, double migrationRate, unsigned long group, unsigned long sampleSize);

    QString toString() const;

    bool isEnabled() const;
    unsigned long x() const;
    unsigned long y() const;
    unsigned long initialPopulation() const;
    unsigned long carryingCapacity() const;
    double growRate() const;
    double migrationRate() const;
    unsigned long group() const;
    unsigned long sampleSize() const;

    bool move(unsigned long x, unsigned long y);
    bool setInitialPopulation(unsigned long population);
    bool setCarryingCapacity(unsigned long capacity);
    bool setGrowRate(double rate);
    bool setMigrationRate(double rate);
    bool setGroup(unsigned long group);
    bool setSampleSize(unsigned long size);

protected:

    unsigned long m_x;
    unsigned long m_y;
    unsigned long m_initialPopulation;
    unsigned long m_carryingCapacity;
    double m_growRate;
    double m_migrationRate;
    unsigned long m_group;
    unsigned long m_sampleSize;
};

#endif // DEME_H
