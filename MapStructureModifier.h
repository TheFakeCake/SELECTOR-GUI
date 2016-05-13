#ifndef MAPSTRUCTUREMODIFIER_H
#define MAPSTRUCTUREMODIFIER_H

#include <vector>
#include "ModifierDeme.h"

class MapStructureModifier
{
public:

    enum ModificationMode {
        Absolute,
        Relative
    };

    MapStructureModifier(unsigned long generation, unsigned long width, unsigned long height);

    unsigned long width();
    unsigned long height();
    unsigned long generation();
    ModificationMode carryingCapacityMode();
    ModificationMode growRateMode();
    ModificationMode migrationRateMode();
    ModifierDeme* deme(unsigned long x, unsigned long y);

    void setWidth(unsigned long width);
    void setHeight(unsigned long height);
    void setGeneration(unsigned long generation);
    void setCarryingCapacityMode(ModificationMode mode);
    void setGrowRateMode(ModificationMode mode);
    void setMigrationRateMode(ModificationMode mode);

protected:

    void _setRowWidth(std::vector<ModifierDeme>& row, unsigned long rowY, unsigned long width);

    unsigned long m_generation;
    ModificationMode m_carryingCapacityMode;
    ModificationMode m_growthRateMode;
    ModificationMode m_migrationRateMode;
    std::vector<std::vector<ModifierDeme> > m_demes;
};

#endif // MAPSTRUCTUREMODIFIER_H
