#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>
#include <list>
#include "deme.h"
#include "MapStructureModifier.h"

class Map
{
public:

    typedef std::map<Deme*, std::map<Deme*, double> > Routes;

    Map();
    Map(unsigned long width, unsigned long height);
    ~Map();

    std::list<unsigned long> modificationGenerations() const;
    MapStructureModifier* modification(unsigned long generation);
    void addModification(unsigned long generation);
    void removeModification(unsigned long generation);
    void removeAllModifications();

    const Routes& routes() const;
    bool setRoute(unsigned long fromX, unsigned long fromY, unsigned long toX, unsigned long toY, double factor);
    bool deleteRoute(unsigned long fromX, unsigned long fromY, unsigned long toX, unsigned long toY);

    unsigned long width();
    unsigned long height();
    Deme* deme(unsigned long x, unsigned long y);
    std::map<unsigned long, std::vector<Deme*> > groups();

    bool setWidth(unsigned long width);
    bool setHeight(unsigned long height);

protected:

    void _setRowWidth(std::vector<Deme>& row, unsigned long rowY, unsigned long width);
    void _removeRoutes(Deme *deme);

    std::vector<std::vector<Deme> > m_demes;
    std::map<unsigned long, MapStructureModifier*> m_modifications;
    Routes m_routes;
};

#endif // MAP_H
