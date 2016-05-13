#ifndef MODIFIERDEME_H
#define MODIFIERDEME_H


class ModifierDeme
{
public:

    ModifierDeme(unsigned long x, unsigned long y);

    unsigned long x() const;
    unsigned long y() const;

    bool isCarryingCapacityModified() const;
    bool isGrowthRateModified() const;
    bool isMigrationRateModified() const;

    unsigned long absCarryingCapacity() const;
    double absGrowthRate() const;
    double absMigrationRate() const;
    double relCarryingCapacity() const;
    double relGrowthRate() const;
    double relMigrationRate() const;

    void setAbsCarryingCapacity(unsigned long capacity);
    void setAbsGrowthRate(double rate);
    void setAbsMigrationRate(double rate);

    void setRelCarryingCapacity(double factor);
    void setRelGrowthRate(double factor);
    void setRelMigrationRate(double factor);

    void resetCarryingCapacity();
    void resetGrowthRate();
    void resetMigrationRate();
    void reset();

protected:

    unsigned long m_x;
    unsigned long m_y;

    bool m_carryingCapacityModified;
    bool m_growthRateModified;
    bool m_migrationRateModified;

    unsigned long m_absCarryingCapacity;
    double m_absGrowthRate;
    double m_absMigrationRate;

    double m_relCarryingCapacity;
    double m_relGrowthRate;
    double m_relMigrationRate;
};

#endif // MODIFIERDEME_H
