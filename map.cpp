#include "map.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Map::Map() : Map(1, 1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Map::Map(unsigned long width, unsigned long height)
{
    m_demes[0] = std::vector<std::vector<Deme> >();
    setHeight(height);
    setWidth(width);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::list<unsigned long> Map::eventList() const
{
    std::list<unsigned long> events;
    for (auto it = m_demes.begin(); it != m_demes.end(); it++)
    {
        events.push_back(it->first);
    }
    return events;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::addEvent(unsigned long generation)
{
    // If the event doesn't already exists
    if (m_demes.count(generation) == 0)
    {
        // Add a new deme grid
        m_demes[generation] = std::vector<std::vector<Deme> >(height(), std::vector<Deme>());

        // Fill the new grid with demes
        for (unsigned long y = 0; y < height(); y++)
        {
            for (unsigned long x = 0; x < width(); x++)
            {
                m_demes[generation][y].push_back(Deme(x, y));
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Map::removeEvent(unsigned long generation)
{
    m_demes.erase(generation);
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
    return m_demes[0][0].size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long Map::height()
{
    return m_demes[0].size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Deme* Map::deme(unsigned long x, unsigned long y, unsigned long generation)
{
    // Check if coordinates are within the map
    if (x >= width() || y >= height())
    {
        return nullptr;
    }
    return &(m_demes[generation][y][x]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Map::groupDemography()
{
    return m_groupDemography;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::map<unsigned long, std::vector<Deme*> > Map::groups()
{
    std::map<unsigned long, std::vector<Deme*> > results;

    for (unsigned long y = 0; y < height(); y++)
    {
        for (unsigned long x = 0; x < width(); x++)
        {
            if (results.count(m_demes[0][y][x].group()) == 0)
            {
                results[m_demes[0][y][x].group()] = std::vector<Deme*>();
            }
            results[m_demes[0][y][x].group()].push_back(&m_demes[0][y][x]);
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
            _removeRoutes(&(m_demes[0][y][x]));
        }
    }

    // Set the width for each events ...
    for (auto eventIt = m_demes.begin(); eventIt != m_demes.end(); eventIt++)
    {
        // ... on each row
        for (unsigned long y = 0; y < height(); y++)
        {
            _setRowWidth(eventIt->second[y], y, width);
        }
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
            _removeRoutes(&(m_demes[0][y][x]));
        }
    }

    // Set the height for each events
    for (auto eventIt = m_demes.begin(); eventIt != m_demes.end(); eventIt++)
    {
        eventIt->second.resize(height);

        // If new rows have been added, set the correct width for them
        for (unsigned long y = oldHeight; y < height; y++)
        {
            _setRowWidth(eventIt->second[y], y, width());
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Map::enableGroupDemography(bool b)
{
    if (b && !m_groupDemography)
    {
       // std::map<unsigned long, std::vector<Deme*> > groups = groups();
    }

    m_groupDemography = b;
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
