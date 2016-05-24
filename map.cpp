#include "map.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Map::Map() : Map(1, 1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Map::Map(unsigned long width, unsigned long height)
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
std::list<unsigned long> Map::modificationGenerations() const
{
    std::list<unsigned long> generations;
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        generations.push_back(it->first);
    }
    generations.sort();
    return generations;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapStructureModifier* Map::modification(unsigned long generation)
{
    if (m_modifications.count(generation) == 1)
    {
        return m_modifications[generation];
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::addModification(unsigned long generation)
{
    // If the modification doesn't already exists
    if (m_modifications.count(generation) == 0)
    {
        // Add a new modification
        m_modifications[generation] = new MapStructureModifier(generation, width(), height());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::removeModification(unsigned long generation)
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
bool Map::setRoute(unsigned long fromX, unsigned long fromY, unsigned long toX, unsigned long toY, double factor)
{
    Deme *fromDeme = deme(fromX, fromY);
    Deme *toDeme = deme(toX, toY);
    unsigned long absXDiff = abs((long)toX - (long)fromX);
    unsigned long absYDiff = abs((long)toY - (long)fromY);

    // Check if route goes between two adjacent demes
    if (fromDeme == nullptr || toDeme == nullptr || fromDeme == toDeme ||
        absXDiff > 1 || absYDiff > 1 || absXDiff == absYDiff)
    {
        return false;
    }

    m_routes[fromDeme][toDeme] = factor;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Map::deleteRoute(unsigned long fromX, unsigned long fromY, unsigned long toX, unsigned long toY)
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
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long Map::width()
{
    return m_demes[0].size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long Map::height()
{
    return m_demes.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Deme* Map::deme(unsigned long x, unsigned long y)
{
    // Check if coordinates are within the map
    if (x < width() && y < height())
    {
        return &(m_demes[y][x]);
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::map<unsigned long, std::vector<Deme*> > Map::groups()
{
    std::map<unsigned long, std::vector<Deme*> > results;

    for (unsigned long y = 0; y < height(); y++)
    {
        for (unsigned long x = 0; x < width(); x++)
        {
            if (results.count(m_demes[y][x].group()) == 0)
            {
                results[m_demes[y][x].group()] = std::vector<Deme*>();
            }
            results[m_demes[y][x].group()].push_back(&m_demes[y][x]);
        }
    }
    return results;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Map::setWidth(unsigned long width)
{
    // Width must be 1 or greater
    if (width < 1)
    {
        return false;
    }

    unsigned long oldWidth = this->width();

    // Check if the width really needs to be modified
    if (oldWidth == width)
    {
        return true;
    }

    // Remove the routes including by the removed demes
    for (unsigned long y = 0; y < height(); y++)
    {
        for (unsigned long x = width; x < oldWidth; x++)
        {
            _removeRoutes(&(m_demes[y][x]));
        }
    }

    // Set the width for each row
    for (unsigned long y = 0; y < height(); y++)
    {
        _setRowWidth(m_demes[y], y, width);
    }

    // Set width of modifications maps
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        it->second->setWidth(width);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Map::setHeight(unsigned long height)
{
    // Height must be 1 or greater
    if (height < 1)
    {
        return false;
    }

    unsigned long oldHeight = this->height();

    // Check if the height really needs to be modified
    if (height == oldHeight)
    {
        return true;
    }

    // Remove the routes including removed demes
    for (unsigned long y = height; y < oldHeight; y++)
    {
        for (unsigned long x = 0; x < width(); x++)
        {
            _removeRoutes(&(m_demes[y][x]));
        }
    }

    m_demes.resize(height);

    // If new rows have been added, set the correct width for them
    for (unsigned long y = oldHeight; y < height; y++)
    {
        _setRowWidth(m_demes[y], y, width());
    }

    // Set height of modifications maps
    for (auto it = m_modifications.begin(); it != m_modifications.end(); it++)
    {
        it->second->setHeight(height);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::_setRowWidth(std::vector<Deme>& row, unsigned long rowY, unsigned long width)
{
    // If the new width is lower, demes are removed
    if (row.size() > width)
    {
        row.resize(width);
    }
    else
    {
        // Else, demes are added to the row
        for (unsigned long x = row.size(); x < width; x++)
        {
            row.push_back(Deme(x, rowY));
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
