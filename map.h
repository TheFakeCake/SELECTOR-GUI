#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>
#include <list>
#include "deme.h"

class Map
{
public:

    typedef std::map<Deme*, std::map<Deme*, double> > Routes;

    Map();
    Map(unsigned long width, unsigned long height);

    std::list<unsigned long> eventList() const;
    void addEvent(unsigned long generation);
    void removeEvent(unsigned long generation);

    const std::map<Deme*, std::map<Deme*, double> >& routes() const;
    bool setRoute(unsigned long fromX, unsigned long fromY, unsigned long toX, unsigned long toY, double factor);
    bool deleteRoute(unsigned long fromX, unsigned long fromY, unsigned long toX, unsigned long toY);

    unsigned long width();
    unsigned long height();
    Deme* deme(unsigned long x, unsigned long y, unsigned long generation = 0);
    bool groupDemography();
    std::map<unsigned long, std::vector<Deme*> > groups();

    bool setWidth(unsigned long width);
    bool setHeight(unsigned long height);
    bool enableGroupDemography(bool b);

protected:

    void _setRowWidth(std::vector<Deme>& row, unsigned long rowY, unsigned long width);
    void _removeRoutes(Deme *deme);

    std::map<unsigned long,  std::vector<std::vector<Deme> > >  m_demes;
    Routes m_routes;
    bool m_groupDemography;
};

#endif // MAP_H
