#include "map.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Map::Map() : Map(1, 1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Map::Map(int width, int height) : QObject()
{
    if (width == 0)
    {
        width = 1;
    }
    if (height == 0)
    {
        height = 1;
    }
    setHeight(height);
    setWidth(width);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Map::~Map()
{
    // Delete
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        delete it->second;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::list<int> Map::modificationGenerations() const
{
    std::list<int> generations;
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        generations.push_back(it->first);
    }
    generations.sort();
    return generations;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier* Map::modification(int generation)
{
    if (m_modifications.count(generation) == 1)
    {
        return m_modifications[generation];
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::addModification(int generation)
{
    // If the modification doesn't already exists
    if (m_modifications.count(generation) == 0)
    {
        // Add a new modification
        m_modifications[generation] = new MapStructureModifier(generation, width(), height());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::removeModification(int generation)
{
    if (m_modifications.count(generation) == 1)
    {
        delete m_modifications[generation];
        m_modifications.erase(generation);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::removeAllModifications()
{
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        delete it->second;
    }
    m_modifications.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const std::map<Deme*, std::map<Deme*, double> >& Map::routes() const
{
    return m_routes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Map::setRoute(int fromX, int fromY, int toX, int toY, double factor)
{
    Deme *fromDeme = deme(fromX, fromY);
    Deme *toDeme = deme(toX, toY);
    int absXDiff = abs(toX - fromX);
    int absYDiff = abs(toY - fromY);

    // Check if route goes between two adjacent demes
    if (fromDeme == nullptr || toDeme == nullptr || fromDeme == toDeme ||
        absXDiff > 1 || absYDiff > 1 || absXDiff == absYDiff)
    {
        return false;
    }

    m_routes[fromDeme][toDeme] = factor;
    emit routesChanged();
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Map::deleteRoute(int fromX, int fromY, int toX, int toY)
{
    Deme *fromDeme = deme(fromX, fromY);
    Deme *toDeme = deme(toX, toY);

    // Check if the route exists
    if (fromDeme == nullptr || toDeme == nullptr ||
        m_routes.count(fromDeme) == 0 || m_routes[fromDeme].count(toDeme) == 0)
    {
        return false;
    }

    // Remove the route
    m_routes[fromDeme].erase(toDeme);
    if (m_routes[fromDeme].empty())
    {
        m_routes.erase(fromDeme);
    }
    emit routesChanged();
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int Map::width() const
{
    return m_demes[0].size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int Map::height() const
{
    return m_demes.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Deme* Map::deme(int x, int y)
{
    // Check if coordinates are within the map
    if (x >= 0 && x < width() && y >= 0 && y < height())
    {
        return m_demes[y][x];
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::map<int, std::vector<Deme*> > Map::groups()
{
    std::map<int, std::vector<Deme*> > results;

    for (int y = 0; y < height(); y++)
    {
        for (int x = 0; x < width(); x++)
        {
            // If the group of the deme isn't already registered, create its entry
            if (results.count(m_demes[y][x]->group()) == 0)
            {
                results[m_demes[y][x]->group()] = std::vector<Deme*>();
            }
            results[m_demes[y][x]->group()].push_back(m_demes[y][x]);
        }
    }
    return results;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int Map::maxInitialPopulation() const
{
    int max = 0;
    for (int y = 0; y < height(); y++)
    {
        for (int x = 0; x < width(); x++)
        {
            Deme *deme = m_demes[y][x];
            if (deme->isEnabled() && deme->initialPopulation() > max)
            {
                max = deme->initialPopulation().maximum();
            }
        }
    }
    return max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int Map::maxCarryingCapacity() const
{
    int max = 0;
    for (int y = 0; y < height(); y++)
    {
        for (int x = 0; x < width(); x++)
        {
            Deme *deme = m_demes[y][x];
            if (deme->isEnabled() && deme->carryingCapacity() > max)
            {
                max = deme->carryingCapacity().maximum();
            }
        }
    }
    return max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
double Map::maxGrowthRate() const
{
    double max = 0.0;
    for (int y = 0; y < height(); y++)
    {
        for (int x = 0; x < width(); x++)
        {
            Deme *deme = m_demes[y][x];
            if (deme->isEnabled() && deme->growthRate() > max)
            {
                max = deme->growthRate().maximum();
            }
        }
    }
    return max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
double Map::maxMigrationRate() const
{
    double max = 0.0;
    for (int y = 0; y < height(); y++)
    {
        for (int x = 0; x < width(); x++)
        {
            Deme *deme = m_demes[y][x];
            if (deme->isEnabled() && deme->migrationRate() > max)
            {
                max = deme->migrationRate().maximum();
            }
        }
    }
    return max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int Map::maxSampleSize() const
{
    int max = 0;
    for (int y = 0; y < height(); y++)
    {
        for (int x = 0; x < width(); x++)
        {
            Deme *deme = m_demes[y][x];
            if (deme->isEnabled() && deme->sampleSize() > max)
            {
                max = deme->sampleSize();
            }
        }
    }
    return max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::dispatchDemeChange(Deme *changedDeme)
{
    emit changed();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::setWidth(int width)
{
    // Width must be 1 or greater
    if (width < 1)
    {
        return;
    }

    int oldWidth = this->width();

    // Check if the width really needs to be modified
    if (oldWidth == width)
    {
        return;
    }

    // Remove the routes including by the removed demes
    for (int y = 0; y < height(); y++)
    {
        for (int x = width; x < oldWidth; x++)
        {
            _removeRoutes(m_demes[y][x]);
        }
    }

    // Set the width for each row
    for (int y = 0; y < height(); y++)
    {
        _setRowWidth(m_demes[y], y, width);
    }

    // Set width of modifications maps
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        it->second->setWidth(width);
    }

    emit changed();
    emit widthChanged(this->width());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::setHeight(int height)
{
    // Height must be 1 or greater
    if (height < 1)
    {
        return;
    }

    int oldHeight = this->height();

    // Check if the height really needs to be modified
    if (height == oldHeight)
    {
        return;
    }

    // Remove the routes including removed demes and delete de demes
    for (int y = height; y < oldHeight; y++)
    {
        for (int x = 0; x < width(); x++)
        {
            _removeRoutes(m_demes[y][x]);
            m_demes[y][x]->deleteLater();
        }
    }

    m_demes.resize(height);

    // If new rows have been added, set the correct width for them
    for (int y = oldHeight; y < height; y++)
    {
        _setRowWidth(m_demes[y], y, width());
    }

    // Set height of modifications maps
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        it->second->setHeight(height);
    }

    emit changed();
    emit heightChanged(this->height());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::_setRowWidth(std::vector<Deme*>& row, int rowY, int width)
{
    // If the new width is lower, demes are deleted
    if (width < (int)row.size())
    {
        for (int x = width; x < (int)row.size(); x++)
        {
            m_demes[rowY][x]->deleteLater();
        }
        row.resize(width);
    }
    else
    {
        // Else, demes are added to the row
        for (int x = row.size(); x < width; x++)
        {
            Deme *newDeme = new Deme(this, x, rowY);
            row.push_back(newDeme);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::_removeRoutes(Deme *deme)
{
    // Remove routes starting from deme
    m_routes.erase(deme);

    // Remove routes finishing to deme
    for (auto it = m_routes.begin(); it != m_routes.end(); it++)
    {
        it->second.erase(deme);
    }
}
