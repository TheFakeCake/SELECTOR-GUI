#include "MapStructureModifier.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::MapStructureModifier(int generation, int width, int height)
{
    m_carryingCapacityMode = Absolute;
    m_growthRateMode = Absolute;
    m_migrationRateMode = Absolute;
    m_generation = generation;
    setHeight(height);
    setWidth(width);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int MapStructureModifier::width()
{
    return m_demes[0].size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int MapStructureModifier::height()
{
    return m_demes.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int MapStructureModifier::generation()
{
    return m_generation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::ModificationMode MapStructureModifier::carryingCapacityMode()
{
    return m_carryingCapacityMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::ModificationMode MapStructureModifier::growthRateMode()
{
    return m_growthRateMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier::ModificationMode MapStructureModifier::migrationRateMode()
{
    return m_migrationRateMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ModifierDeme* MapStructureModifier::deme(int x, int y)
{
    if (x < width() && y < height())
    {
        return &m_demes[y][x];
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setWidth(int width)
{
    // Width must be 1 or greater
    if (width < 1)
    {
        return;
    }

    // Set the width on each row
    for (int y = 0; y < height(); y++)
    {
        _setRowWidth(m_demes[y], y, width);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setHeight(int height)
{
    // Height must be 1 or greater
    if (height < 1)
    {
        return;
    }

    int oldHeight = this->height();
    m_demes.resize(height);

    // If new rows have been added, set the correct width for them
    for (int y = oldHeight; y < height; y++)
    {
        _setRowWidth(m_demes[y], y, width());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::_setRowWidth(std::vector<ModifierDeme>& row, int rowY, int width)
{
    // If the new width is lower, demes are removed
    if ((int)row.size() > width)
    {
        for (int x = row.size(); x > width; x--)
        {
            row.pop_back();
        }
    }
    else // Else, demes are added to the row
    {
        for (int x = row.size(); x < width; x++)
        {
            row.push_back(ModifierDeme(x, rowY));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setGeneration(int generation)
{
    m_generation = generation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setCarryingCapacityMode(ModificationMode mode)
{
    m_carryingCapacityMode = mode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setGrowthRateMode(ModificationMode mode)
{
    m_growthRateMode = mode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapStructureModifier::setMigrationRateMode(ModificationMode mode)
{
    m_migrationRateMode = mode;
}
