#include "simulationmodel.h"

#include <QFile>
#include <QTextStream>

////////////////////////////////////////////////////////////////////////////////////////////////////
const QString SimulationModel::SELECTOR_PARAM_FILE = QString("selector_param.txt");
const QString SimulationModel::SELECTOR_STRUCTURE_FILE = QString("selector_structure_%1.txt");
const QString SimulationModel::SELECTOR_ALLDEMES_FILE = QString("selector_alldemes_%1.txt");

////////////////////////////////////////////////////////////////////////////////////////////////////
SimulationModel::SimulationModel() : m_nbSimulations(1), m_nbGenerations(1), m_initialAllelePoolSize(20),
    m_uniqueAllelePool(false), m_firstAlleleFrequency(0.0), m_mutationRate(0.0), m_outputCompleteHistory(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimulationModel::loadFromFiles(QString directory)
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimulationModel::writeToFiles(QString directory)
{
    int i;
    QFile file(directory + "/" + SELECTOR_PARAM_FILE);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return false;
    }

    QTextStream out(&file);
    out << "// GENERAL SETTINGS\n"
        << m_nbSimulations << "\t// Number of simulations\n"
        << m_nbGenerations << "\t// Number of generations\n"
        << "// GENETIC SETTINGS\n"
        << m_initialAllelePoolSize << "\t// Size of the initial allele pool\n"
        << (m_uniqueAllelePool ? "0" : "1") << "\t// 0: One unique allele pool | 1: Independant allele pools for each population\n"
        << m_firstAlleleFrequency << "\t// Initial frequency for allele \"a1\"\n"
        << m_mutationRate << "\t// Mutation rate\n"
        << "// SELECTION SETTINGS\n";
    switch (m_selection.model())
    {
    case Selection::SOS:
        out << "1";
        break;
    case Selection::FDS:
        out << "2";
        break;
    case Selection::DPS:
        out << "3";
        break;
    }
    out << "\t// Selection model. 1: SOS | 2: FDS | 3: DPS\n"
        << m_selection.heterogeneity() << "\t// Selection heterogeneity. 0: Uniform | 1: Latitudinal | 2: Longitudinal\n"
        << m_selection.uniformCoefficient() << "\t// Uniform selection coefficient\n"
        << m_selection.northEastCoefficient() << "\t// Northern or eastern selection coefficient\n"
        << m_selection.southWestCoefficient() << "\t// Southern or western selection coefficient\n"
        << "// OUTPUT SETTINGS\n"
        << (m_outputCompleteHistory ? "1" : "0") << "\t// Generate allele frequency files for last simulation. 1: Yes | 0: No\n"
        << "// WORLD STRUCTURE AND DEMOGRAPHY SETTINGS\n"
        << m_map.width() << "\t// Number of demes along the X axis\n"
        << m_map.height() << "\t// Number of demes along the Y axis\n";

    // Groups
    std::map<unsigned long, std::vector<Deme*> > groups = m_map.groups();
    out << groups.size() << "\t// Number of population groups\n"
        << "0\t// All demes within a group have the same demography. 0: No | 1: Yes\n";

    // Routes
    Map::Routes routes = m_map.routes();
    int nbRoutes = 0;
    QString routesLine = "";

    for (auto fromIt = routes.begin(); fromIt != routes.end(); fromIt++)
    {
        for (auto toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
        {
            nbRoutes++;
            routesLine += QString("(%1,%2,%3) ").arg(fromIt->first->x() + fromIt->first->y() * m_map.width())
                                                .arg(toIt->first->x() + toIt->first->y() * m_map.width())
                                                .arg(toIt->second);
        }
    }
    out << nbRoutes << "\t// Number of routes\n"
        << routesLine << "\n";

    // Poulation structures
    std::list<unsigned long> generations = m_map.modificationGenerations();
    out << generations.size() + 1 << "\t// Number of different population structures to load\n"
        << "1\t0\n";
    i = 2;
    for (auto it = generations.begin(); it != generations.end(); it++, i++)
    {
        out << i << "\t" << *it << "\n";
    }
    file.close();

    // Structure file 1
    file.setFileName(directory + "/" + QString(SELECTOR_STRUCTURE_FILE).arg(1));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return false;
    }
    out << "// Population characteristics\n"
        << "// Initial size (positive int), Carrying capacity K (positive int), growth rate r ([0,1]),migration rate m ([0,1]), group of demes, sample size\n";
    for (unsigned long y = 0; y < m_map.height(); y++)
    {
        for (unsigned long x = 0; x < m_map.width(); x++)
        {
            Deme *deme = m_map.deme(x, y);
            out << QString("%1 %2 %3 %4 %5 %6\n").arg(deme->initialPopulation())
                                                 .arg(deme->isEnabled() ? deme->carryingCapacity() : -1)
                                                 .arg(deme->growRate())
                                                 .arg(deme->migrationRate())
                                                 .arg(deme->group())
                                                 .arg(deme->sampleSize());
        }
    }
    file.close();

    // Next structure files
    i = 2;
    for (auto generationIt = generations.begin()++; generationIt != generations.end(); generationIt++)
    {
        file.setFileName(QString(SELECTOR_STRUCTURE_FILE).arg(i));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            return false;
        }
        MapStructureModifier *modification = m_map.modification(*generationIt);

        out << "// K (positive int), r ([0,1]), m ([0,1])\n"
            << (modification->carryingCapacityMode() == MapStructureModifier::Absolute ? "abs\t" : "rel\t")
            << (modification->growRateMode() == MapStructureModifier::Absolute ? "abs\t" : "rel\t")
            << (modification->migrationRateMode() == MapStructureModifier::Absolute ? "abs\n" : "rel\n");

        for (unsigned long y = 0; y < modification->height(); y++)
        {
            for (unsigned long x = 0; x < modification->width(); x++)
            {
                if (!modification->deme(x, y)->isCarryingCapacityModified())
                {
                    out << "-1\t";
                }
                else if (modification->carryingCapacityMode() == MapStructureModifier::Absolute)
                {
                    out << modification->deme(x, y)->absCarryingCapacity() << "\t";
                }
                else
                {
                    out << modification->deme(x, y)->relCarryingCapacity() << "\t";
                }

                if (!modification->deme(x, y)->isGrowthRateModified())
                {
                    out << "-1\t";
                }
                else if (modification->growRateMode() == MapStructureModifier::Absolute)
                {
                    out << modification->deme(x, y)->absGrowthRate() << "\t";
                }
                else
                {
                    out << modification->deme(x, y)->relGrowthRate() << "\t";
                }

                if (!modification->deme(x, y)->isMigrationRateModified())
                {
                    out << "-1\n";
                }
                else if (modification->migrationRateMode() == MapStructureModifier::Absolute)
                {
                    out << modification->deme(x, y)->absMigrationRate() << "\n";
                }
                else
                {
                    out << modification->deme(x, y)->relMigrationRate() << "\n";
                }
            }
        }
        file.close();
        i++;
    }

    return true;
}

//////////////////////////////////////////// GETTERS ///////////////////////////////////////////////
unsigned long SimulationModel::nbSimulations() const
{
    return m_nbSimulations;
}

unsigned long SimulationModel::nbGenerations() const
{
    return m_nbGenerations;
}

unsigned long SimulationModel::initialAllelePoolSize() const
{
    return m_initialAllelePoolSize;
}

bool SimulationModel::uniqueAllelePool() const
{
    return m_uniqueAllelePool;
}

double SimulationModel::firstAlleleFrequency() const
{
    return m_firstAlleleFrequency;
}

double SimulationModel::mutationRate() const
{
    return m_mutationRate;
}

Selection* SimulationModel::selection()
{
    return &m_selection;
}

bool SimulationModel::outputCompleteHistory() const
{
    return m_outputCompleteHistory;
}

Map* SimulationModel::map()
{
    return &m_map;
}

/////////////////////////////////////////// SETTERS ////////////////////////////////////////////////
bool SimulationModel::setNbSimulations(unsigned long n)
{
    if (n > 0)
    {
        m_nbSimulations = n;
        return true;
    }
    return false;
}

bool SimulationModel::setNbGenerations(unsigned long n)
{
    if (n > 0)
    {
        m_nbGenerations = n;
        return true;
    }
    return false;
}

bool SimulationModel::setInitialAllelePoolSize(unsigned long n)
{
    if (n > 0)
    {
        m_initialAllelePoolSize = n;
        return true;
    }
    return false;
}

bool SimulationModel::setUniqueAllelePool(bool b)
{
    m_uniqueAllelePool = b;
    return true;
}

bool SimulationModel::setFirstAlleleFrequency(double frequency)
{
    if (frequency >= 0.0 && frequency <= 1.0)
    {
        m_firstAlleleFrequency = frequency;
        return true;
    }
    return false;
}

bool SimulationModel::setMutationRate(double mutationRate)
{
    if (mutationRate >= 0.0)
    {
        m_mutationRate = mutationRate;
        return false;
    }
    return false;
}

bool SimulationModel::setOutputCompleteHistory(bool b)
{
    m_outputCompleteHistory = b;
    return true;
}
