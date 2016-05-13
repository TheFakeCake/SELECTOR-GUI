#include "MapStructureModifier.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::MapStructureModifier(unsigned long generation, unsigned long width, unsigned long height)
{
    m_carryingCapacityMode = Absolute;
    m_growthRateMode = Absolute;
    m_migrationRateMode = Absolute;
    m_generation = generation;
    setHeight(height);
    setWidth(width);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long MapStructureModifier::width()
{
    return m_demes[0].size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long MapStructureModifier::height()
{
    return m_demes.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long MapStructureModifier::generation()
{
    return m_generation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::ModificationMode MapStructureModifier::carryingCapacityMode()
{
    return m_carryingCapacityMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::ModificationMode MapStructureModifier::growRateMode()
{
    return m_growthRateMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::ModificationMode MapStructureModifier::migrationRateMode()
{
    return m_migrationRateMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ModifierDeme* MapStructureModifier::deme(unsigned long x, unsigned long y)
{
    if (x < width() && y < height())
    {
        return &m_demes[y][x];
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setWidth(unsigned long width)
{
    // Width must be 1 or greater
    if (width < 1)
    {
        return;
    }

    // Set the width on each row
    for (unsigned long y = 0; y < height(); y++)
    {
        _setRowWidth(m_demes[y], y, width);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setHeight(unsigned long height)
{
    // Height must be 1 or greater
    if (height < 1)
    {
        return;
    }

    unsigned long oldHeight = this->height();
    m_demes.resize(height);

    // If new rows have been added, set the correct width for them
    for (unsigned long y = oldHeight; y < height; y++)
    {
        _setRowWidth(m_demes[y], y, width());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::_setRowWidth(std::vector<ModifierDeme>& row, unsigned long rowY, unsigned long width)
{
    // If the new width is lower, demes are removed
    if (row.size() > width)
    {
        for (unsigned long x = row.size(); x > width; x--)
        {
            row.pop_back();
        }
    }
    else // Else, demes are added to the row
    {
        for (unsigned long x = row.size(); x < width; x++)
        {
            row.push_back(ModifierDeme(x, rowY));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setGeneration(unsigned long generation)
{
    m_generation = generation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setCarryingCapacityMode(ModificationMode mode)
{
    m_carryingCapacityMode = mode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setGrowRateMode(ModificationMode mode)
{
    m_growthRateMode = mode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setMigrationRateMode(ModificationMode mode)
{
    m_migrationRateMode = mode;
}
