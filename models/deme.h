#ifndef DEME_H
#define DEME_H

#include <QObject>
#include <QString>

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
    int initialPopulation() const;
    int carryingCapacity() const;
    double growthRate() const;
    double migrationRate() const;
    int group() const;
    int sampleSize() const;

    void enable(bool b);
    void setInitialPopulation(int population);
    void setCarryingCapacity(int capacity);
    void setGrowthRate(double rate);
    void setMigrationRate(double rate);
    void setGroup(int group);
    void setSampleSize(int size);

signals:

    void enabledChanged(bool b);
    void initialPopulationChanged(int newPopulation);
    void carryingCapacityChanged(int newCarryingCapacity);
    void growthRateChanged(double newRate);
    void migrationRateChanged(double newRate);
    void groupChanged(int newGroup);
    void sampleSizeChanged(int newSampleSize);

protected:

    Map *m_parentMap;
    bool m_enabled;
    int m_x;
    int m_y;
    int m_initialPopulation;
    int m_carryingCapacity;
    double m_growthRate;
    double m_migrationRate;
    int m_group;
    int m_sampleSize;
};

#endif // DEME_H
