#ifndef DEME_H
#define DEME_H

#include <QObject>
#include <QString>
#include "ABCInterval.h"

class Map;

#include "map.h"

class Deme : public QObject
{
    Q_OBJECT

public:

    Deme(Map *parentMap, int x, int y);

    bool isEnabled() const;
    int x() const;
    int y() const;
    ABCInterval<int> initialPopulation() const;
    ABCInterval<int> carryingCapacity() const;
    ABCInterval<double> growthRate() const;
    ABCInterval<double> migrationRate() const;
    int group() const;
    int sampleSize() const;

    void enable(bool b);
    void setInitialPopulation(ABCInterval<int> population);
    void setCarryingCapacity(ABCInterval<int> capacity);
    void setGrowthRate(ABCInterval<double> rate);
    void setMigrationRate(ABCInterval<double> rate);
    void setGroup(int group);
    void setSampleSize(int size);

signals:

    void enabledChanged(bool b);
    void initialPopulationChanged(ABCInterval<int> newPopulation);
    void carryingCapacityChanged(ABCInterval<int> newCarryingCapacity);
    void growthRateChanged(ABCInterval<double> newRate);
    void migrationRateChanged(ABCInterval<double> newRate);
    void groupChanged(int newGroup);
    void sampleSizeChanged(int newSampleSize);

protected:

    Map *m_parentMap;
    bool m_enabled;
    int m_x;
    int m_y;
    ABCInterval<int> m_initialPopulation;
    ABCInterval<int> m_carryingCapacity;
    ABCInterval<double> m_growthRate;
    ABCInterval<double> m_migrationRate;
    int m_group;
    int m_sampleSize;
};

#endif // DEME_H
