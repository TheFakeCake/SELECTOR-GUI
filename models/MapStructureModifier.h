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

    MapStructureModifier(int generation, int width, int height);

    int width();
    int height();
    int generation();
    ModificationMode carryingCapacityMode();
    ModificationMode growthRateMode();
    ModificationMode migrationRateMode();
    ModifierDeme* deme(int x, int y);

    void setWidth(int width);
    void setHeight(int height);
    void setGeneration(int generation);
    void setCarryingCapacityMode(ModificationMode mode);
    void setGrowthRateMode(ModificationMode mode);
    void setMigrationRateMode(ModificationMode mode);

protected:

    void _setRowWidth(std::vector<ModifierDeme>& row, int rowY, int width);

    int m_generation;
    ModificationMode m_carryingCapacityMode;
    ModificationMode m_growthRateMode;
    ModificationMode m_migrationRateMode;
    std::vector<std::vector<ModifierDeme> > m_demes;
};

#endif // MAPSTRUCTUREMODIFIER_H
