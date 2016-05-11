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
    QFile paramFile(directory + "/" + SELECTOR_PARAM_FILE);

    if (!paramFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return false;
    }

    QTextStream out(&paramFile);
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
    out << groups.size() << "\t// Number of population groups\n";

    // TODO: Demes from same group

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
    std::list<unsigned long> eventList = m_map.eventList();
    out << eventList.size() << "\t// Number of different population structures to load\n";
    i = 1;
    for (auto it = eventList.begin(); it != eventList.end(); it++, i++)
    {
        out << i << "\t" << *it << "\n";
    }

    paramFile.close();

    QFile structureFile(directory + "/" + QString(SELECTOR_STRUCTURE_FILE).arg(1));
    if (!structureFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return false;
    }
    out.setDevice(&structureFile);

    for (unsigned long y = 0; y < m_map.height(); y++)
    {
        for (unsigned long x = 0; x < m_map.width(); x++)
        {
            out << m_map.deme(x, y)->toString() << "\n";
        }
    }
    structureFile.close();

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
