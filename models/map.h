#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <vector>
#include <map>
#include <list>
#include "MapStructureModifier.h"
#include "RoutesTableModel.h"

class Deme;

#include "deme.h"

class Map : public QObject
{
    Q_OBJECT

public:

    Map();
    Map(int width, int height);
    ~Map();

    std::list<int> modificationGenerations() const;
    MapStructureModifier* modification(int generation);
    void addModification(int generation);
    void removeModification(int generation);
    void removeAllModifications();

    RoutesTableModel* routes();

    int width() const;
    int height() const;
    Deme* deme(int x, int y);
    std::map<int, std::vector<Deme*> > groups();
    int maxInitialPopulation() const;
    int maxCarryingCapacity() const;
    double maxGrowthRate() const;
    double maxMigrationRate() const;
    int maxSampleSize() const;

    void dispatchDemeChange(Deme *changedDeme);

public slots:

    void setWidth(int width);
    void setHeight(int height);

signals:

    void widthChanged(int newWidth);
    void heightChanged(int newHeight);
    void demeChanged(Deme *changedDeme);
    void changed();

protected:

    void _setRowWidth(std::vector<Deme*>& row, int rowY, int width);

    std::vector<std::vector<Deme*> > m_demes;
    std::map<int, MapStructureModifier*> m_modifications;
    RoutesTableModel *m_routes;
};

#endif // MAP_H
