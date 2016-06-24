#include "SelectorConfigFiles.h"

#include <QFile>
#include <QTextStream>

////////////////////////////////////////////////////////////////////////////////////////////////////
const QString SelectorConfigFiles::SELECTOR_PARAM_FILE       = QString("selector_param.txt");
const QString SelectorConfigFiles::SELECTOR_STRUCTURE_FILE   = QString("selector_structure_%1.txt");
const QString SelectorConfigFiles::SELECTOR_ALLDEMES_FILE    = QString("selector_alldemes_%1.txt");
const QString SelectorConfigFiles::DISTRIBUTIONS[] = { "UNIFORM", "LOG_UNIFORM", "UNIFORM_DISCRETE" };

const QString SPACE         = QString("[ \\t]");
const QString COMMENT       = QString("(?:\\/\\/[^\\n]*)");
const QString INT           = QString("(?:[0-9]+)");
const QString FLOAT         = QString("(?:[0-9]+(?:\\.[0-9]+)?)");
const QString DISTRIBUTION  = QString("(?:UNIFORM|LOG_UNIFORM|UNIFORM_DISCRETE)");
const QString INTERVAL_INT  = QString("\\(\\*("+DISTRIBUTION+")\\/("+INT+")\\/("+INT+")\\)");
const QString INTERVAL_FLOAT= QString("\\(\\*("+DISTRIBUTION+")\\/("+FLOAT+")\\/("+FLOAT+")\\)");
const QString ROUTE         = QString("(?:\\(("+INT+"),("+INT+"),("+FLOAT+")\\))");
const QString LINE_END      = SPACE+"*"+COMMENT+"?";

const int SELECTOR_PARAM_NB_LINES = 25;
const QString SELECTOR_PARAM_REGEX[] = {
/*01*/  LINE_END,
/*02*/  "("+INT+")"+LINE_END,
/*03*/  "(?:("+INT+")|"+INTERVAL_INT+")"+LINE_END,
/*04*/  LINE_END,
/*05*/  "(?:("+INT+")|"+INTERVAL_INT+")"+LINE_END,
/*06*/  "("+INT+")"+LINE_END,
/*07*/  "(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+LINE_END,
/*08*/  "(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+LINE_END,
/*09*/  LINE_END,
/*10*/  "("+INT+")"+LINE_END,
/*11*/  "("+INT+")"+LINE_END,
/*12*/  "(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+LINE_END,
/*13*/  "(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+LINE_END,
/*14*/  "(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+LINE_END,
/*15*/  LINE_END,
/*16*/  "("+INT+")"+LINE_END,
/*17*/  LINE_END,
/*18*/  "("+INT+")"+LINE_END,
/*19*/  "("+INT+")"+LINE_END,
/*20*/  "("+INT+")"+LINE_END,
/*21*/  "("+INT+")"+LINE_END,
/*22*/  "("+INT+")"+LINE_END,
/*23*/  "(?:"+ROUTE+SPACE+"*)*"+LINE_END,
/*24*/  "("+INT+")"+LINE_END,
/*25*/  "1"+SPACE+"+0"+LINE_END,
/*26+*/ "("+INT+")"+SPACE+"+("+INT+")"+LINE_END
};

const QString SELECTOR_STRUCTURE_1_REGEX[] {
/*01*/  LINE_END,
/*02*/  LINE_END,
/*03+*/ "(?:("+INT+")|"+INTERVAL_INT+")"+SPACE+"+(?:("+INT+"|-1)|"+INTERVAL_INT+")"+SPACE+"+(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+SPACE+"+(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+SPACE+"+("+INT+")"+SPACE+"+("+INT+")"+LINE_END
};

const QString SELECTOR_STRUCTURE_N_REGEX[] {
/*01*/  LINE_END,
/*02*/  "(abs|rel)"+SPACE+"+(abs|rel)"+SPACE+"+(abs|rel)"+LINE_END,
};

const QString SELECTOR_ALLDEMES_REGEX[] {
/*01*/  LINE_END,
/*02*/  "(?:("+INT+"|-1)|"+INTERVAL_INT+")"+SPACE+"+(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+SPACE+"+(?:("+FLOAT+")|"+INTERVAL_FLOAT+")"+LINE_END,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
QString SelectorConfigFiles::ABCIntervalToStr(ABCInterval<int> interval)
{
    if (interval.isFixedValue())
    {
        return QString::number(interval.minimum());
    }
    return QString("(*%1/%2/%3)").arg(DISTRIBUTIONS[interval.distribution()])
                                 .arg(interval.minimum())
                                 .arg(interval.maximum());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
QString SelectorConfigFiles::ABCIntervalToStr(ABCInterval<double> interval)
{
    if (interval.isFixedValue())
    {
        return QString::number(interval.minimum());
    }
    return QString("(*%1/%2/%3)").arg(DISTRIBUTIONS[interval.distribution()])
                                 .arg(interval.minimum())
                                 .arg(interval.maximum());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
SelectorConfigFiles::SelectorConfigFiles() : SelectorConfigFiles(".")
{
}

SelectorConfigFiles::SelectorConfigFiles(QString directory) : QObject()
{
    setDirectory(directory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SelectorConfigFiles::read(SimulationModel *sim)
{
    emit info(QString("Reading SELECTOR configuration from directory %1").arg(m_directory));
    m_errorMessages.clear();

    // Open selector_param.txt file
    QFile file(m_directory + "/" + SELECTOR_PARAM_FILE);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString msg = QString("Failed to open %1").arg(file.fileName());
        m_errorMessages << msg;
        emit error(msg);
        return false;
    }

    QTextStream in(&file);
    QRegExp regex;
    bool groupedDemes;
    int nbRoutes;
    int nbStructures;

    // Read the first 25 lines of selector_param.txt
    for (int i = 0; i < SELECTOR_PARAM_NB_LINES; i++)
    {
        // Check if end of files is prematurely reached
        if (in.atEnd())
        {
            QString msg = QString("Reached the end of file %1. Incomplete configuration.").arg(SELECTOR_PARAM_FILE);
            m_errorMessages << msg;
            emit warning(msg);
            break;
        }

        regex.setPattern(SELECTOR_PARAM_REGEX[i]);
        QString line = in.readLine().trimmed();

        if (regex.exactMatch(line))
        {
            _handleParamLine(sim, i, &regex, groupedDemes, nbRoutes, nbStructures);
        }
        else
        {
            QString msg = QString("Error on line %1 of file %2. Line ignored.").arg(i+1).arg(SELECTOR_PARAM_FILE);
            m_errorMessages << msg;
            emit warning(msg);
        }
    }

    // Read the last lines of selector_param.txt (map structure modifications)
    sim->map()->removeAllModifications();
    regex.setPattern(SELECTOR_PARAM_REGEX[SELECTOR_PARAM_NB_LINES]);
    for (int i = 0; i < nbStructures - 1; i++)
    {
        // Check if end of files is prematurely reached
        if (in.atEnd())
        {
            QString msg = QString("Reached the end of file %1. Missing structure files.").arg(SELECTOR_PARAM_FILE);
            m_errorMessages << msg;
            emit warning(msg);
            break;
        }
        if (regex.exactMatch(in.readLine().trimmed()))
        {
            sim->map()->addModification(regex.cap(2).toInt());
        }
        else
        {
            QString msg = QString("Error on line %1 of file %2. Line ignored.").arg(SELECTOR_PARAM_NB_LINES + i).arg(SELECTOR_PARAM_FILE);
            m_errorMessages << msg;
            emit warning(msg);
        }
    }

    file.close();
    file.setFileName(m_directory + "/" + QString(SELECTOR_STRUCTURE_FILE).arg(1));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString msg = QString("Failed to open %1").arg(file.fileName());
        m_errorMessages << msg;
        emit error(msg);
        return false;
    }
    in.seek(0);

    // Read selector_structure_1.txt
    in.readLine(); // Ignore the first 2 comment lines
    in.readLine();

    regex.setPattern(SELECTOR_STRUCTURE_1_REGEX[2]);

    // For each deme of the map ...
    for (int y = 0; y < sim->map()->height(); y++)
    {
        for (int x = 0; x < sim->map()->width(); x++)
        {
            // Check if end of files is prematurely reached
            if (in.atEnd())
            {
                QString msg = QString("Reached the end of file %1. Incomplete map data.")
                                     .arg(QString(SELECTOR_STRUCTURE_FILE).arg(1));
                m_errorMessages << msg;
                emit warning(msg);
                y = sim->map()->height();
                break;
            }
            // Check if the line format is correct
            if (regex.exactMatch(in.readLine().trimmed()))
            {
                // Set deme parameters
                sim->map()->deme(x, y)->setInitialPopulation(toInterval<int>(regex.cap(1), regex.cap(2), regex.cap(3), regex.cap(4)));
                if (regex.cap(5) == "-1")
                {
                    sim->map()->deme(x, y)->enable(false);
                }
                else
                {
                    sim->map()->deme(x, y )->enable(true);
                    sim->map()->deme(x, y)->setCarryingCapacity(toInterval<int>(regex.cap(5), regex.cap(6), regex.cap(7), regex.cap(8)));
                }
                sim->map()->deme(x, y)->setGrowthRate(toInterval<double>(regex.cap(9), regex.cap(10), regex.cap(11), regex.cap(12)));
                sim->map()->deme(x, y)->setMigrationRate(toInterval<double>(regex.cap(13), regex.cap(14), regex.cap(15), regex.cap(16)));
                sim->map()->deme(x, y)->setGroup(regex.cap(17).toInt());
                sim->map()->deme(x, y)->setSampleSize(regex.cap(18).toInt());
            }
            else
            {
                QString msg = QString("Error on line %1 of file %2. Line ignored.").arg(3 + x + y * sim->map()->width())
                                     .arg(QString(SELECTOR_STRUCTURE_FILE).arg(1));
                m_errorMessages << msg;
                emit warning(msg);
            }
        }
    }
    file.close();

    if (groupedDemes)
    {
        _readAllDemesFiles(sim);
    }
    else
    {
        _readStructureFiles(sim);
    }

    if (! m_errorMessages.empty())
    {
        emit warning("End of SELECTOR configuration parsing. Some errors occured.");
        return false;
    }

    emit info("SELECTOR configuration successfuly read.");

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SelectorConfigFiles::_readAllDemesFiles(SimulationModel *sim)
{
    std::list<int> modificationGenerations = sim->map()->modificationGenerations();
    MapStructureModifier *modif;
    QFile file;
    QTextStream in(&file);
    QRegExp regex;
    std::map<int, std::vector<Deme*> > groups = sim->map()->groups();

    // Open selector_alldemes_1.txt file
    file.setFileName(m_directory + "/" + QString(SELECTOR_ALLDEMES_FILE).arg(1));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString msg = QString("Failed to open %1.").arg(file.fileName());
        m_errorMessages << msg;
        emit error(msg);
        return;
    }

    int lineCount = 2;

    // Ignore first line
    in.readLine();

    regex.setPattern(SELECTOR_ALLDEMES_REGEX[1]);

    for (auto groupIt = groups.begin(); groupIt != groups.end(); groupIt++, lineCount++)
    {
        // Check if end of files is prematurely reached
        if (in.atEnd())
        {
            QString msg = QString("Reached the end of file %1. Incomplete configuration.").arg(QString(SELECTOR_ALLDEMES_FILE).arg(1));
            m_errorMessages << msg;
            emit warning(msg);
            break;
        }
        // Check line format and retrieve data from it
        if (regex.exactMatch(in.readLine().trimmed()))
        {
            for (auto demeIt = groupIt->second.begin(); demeIt != groupIt->second.end(); demeIt++)
            {
                if (regex.cap(1) == "-1")
                {
                    (*demeIt)->enable(false);
                    continue;
                }
                else
                {
                    (*demeIt)->setCarryingCapacity(toInterval<int>(regex.cap(1), regex.cap(2), regex.cap(3), regex.cap(4)));
                    (*demeIt)->setGrowthRate(toInterval<double>(regex.cap(5), regex.cap(6), regex.cap(7), regex.cap(8)));
                    (*demeIt)->setMigrationRate(toInterval<double>(regex.cap(9), regex.cap(10), regex.cap(11), regex.cap(12)));
                }
            }
        }
        else
        {
            QString msg = QString("Error on line %1 of file %2. Line ignored.").arg(lineCount)
                                 .arg(QString(SELECTOR_ALLDEMES_FILE).arg(1));
            m_errorMessages << msg;
            emit warning(msg);
        }
    }
    file.close();

    // Reads the selector_alldemes_X.txt files
    int i = 2;
    for (auto it = modificationGenerations.begin(); it != modificationGenerations.end(); it++, i++)
    {
        // Open selector_alldemes_X.txt file
        file.setFileName(m_directory + "/" + QString(SELECTOR_ALLDEMES_FILE).arg(i));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString msg = QString("Failed to open %1.").arg(file.fileName());
            m_errorMessages << msg;
            emit error(msg);
            continue;
        }
        in.seek(0);

        modif = sim->map()->modification(*it);

        // Ignore first line (comments)
        in.readLine();

        // Check format of line 2
        regex.setPattern(SELECTOR_STRUCTURE_N_REGEX[1]);
        if (!regex.exactMatch(in.readLine().trimmed()))
        {
            QString msg = QString("Error on line 2 of file %1. Line ignored.").arg(QString(SELECTOR_ALLDEMES_FILE).arg(i));
            m_errorMessages << msg;
            emit warning(msg);
            file.close();
            continue;
        }

        // Analyse line 2 to set modification modes and create the regex for the next data lines
        QString dataRegex;

        if (regex.cap(1) == "rel")
        {
            modif->setCarryingCapacityMode(MapStructureModifier::Relative);
            dataRegex = "("+FLOAT+"|-1)"+SPACE+"+";
        }
        else
        {
            modif->setCarryingCapacityMode(MapStructureModifier::Absolute);
            dataRegex = "("+INT+"|-1)"+SPACE+"+";
        }

        if (regex.cap(2) == "rel")
        {
            modif->setGrowthRateMode(MapStructureModifier::Relative);
            dataRegex += "("+FLOAT+"|-1)"+SPACE+"+";
        }
        else
        {
            modif->setGrowthRateMode(MapStructureModifier::Absolute);
            dataRegex += "("+FLOAT+"|-1)"+SPACE+"+";
        }

        if (regex.cap(3) == "rel")
        {
            modif->setMigrationRateMode(MapStructureModifier::Relative);
            dataRegex += "("+FLOAT+"|-1)"+LINE_END;
        }
        else
        {
            modif->setMigrationRateMode(MapStructureModifier::Absolute);
            dataRegex += "("+FLOAT+"|-1)"+LINE_END;
        }

        regex.setPattern(dataRegex);

        // Read data lines
        lineCount = 2;
        for (auto groupIt = groups.begin(); groupIt != groups.end(); groupIt++, lineCount++)
        {
            if (! regex.exactMatch(in.readLine().trimmed()))
            {
                QString msg = QString("Error on line %1 of file %2. Line ignored").arg(lineCount)
                                     .arg(QString(SELECTOR_ALLDEMES_FILE).arg(i));
                m_errorMessages << msg;
                emit warning(msg);
                continue;
            }
            for (auto demeIt = groupIt->second.begin(); demeIt != groupIt->second.end(); demeIt++)
            {
                if (regex.cap(1) != "-1")
                {
                    if (modif->carryingCapacityMode() == MapStructureModifier::Relative)
                    {
                        modif->deme((*demeIt)->x(), (*demeIt)->y())->setRelCarryingCapacity(regex.cap(1).toDouble());
                    }
                    else
                    {
                        modif->deme((*demeIt)->x(), (*demeIt)->y())->setAbsCarryingCapacity(regex.cap(1).toInt());
                    }
                }
                if (regex.cap(2) != "-1")
                {
                    if (modif->growthRateMode() == MapStructureModifier::Relative)
                    {
                        modif->deme((*demeIt)->x(), (*demeIt)->y())->setRelGrowthRate(regex.cap(2).toDouble());
                    }
                    else
                    {
                        modif->deme((*demeIt)->x(), (*demeIt)->y())->setAbsGrowthRate(regex.cap(2).toDouble());
                    }
                }
                if (regex.cap(3) != "-1")
                {
                    if (modif->growthRateMode() == MapStructureModifier::Relative)
                    {
                        modif->deme((*demeIt)->x(), (*demeIt)->y())->setRelMigrationRate(regex.cap(3).toDouble());
                    }
                    else
                    {
                        modif->deme((*demeIt)->x(), (*demeIt)->y())->setAbsMigrationRate(regex.cap(3).toDouble());
                    }
                }
            }
        }

        file.close();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SelectorConfigFiles::_readStructureFiles(SimulationModel *sim)
{
    std::list<int> modificationGenerations = sim->map()->modificationGenerations();
    MapStructureModifier *modif;
    QFile file;
    QTextStream in(&file);
    QRegExp regex;
    int i = 2;

    for (auto it = modificationGenerations.begin(); it != modificationGenerations.end(); it++, i++)
    {
        // Open selector_structure_X.txt file
        file.setFileName(m_directory + "/" + QString(SELECTOR_STRUCTURE_FILE).arg(i));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString msg = QString("Failed to open %1.").arg(file.fileName());
            m_errorMessages << msg;
            emit error(msg);
            continue;
        }
        in.seek(0);

        modif = sim->map()->modification(*it);

        // Ignore first line (comments)
        in.readLine();

        // Check format of line 2
        regex.setPattern(SELECTOR_STRUCTURE_N_REGEX[1]);
        if (!regex.exactMatch(in.readLine().trimmed()))
        {
            QString msg = QString("Error on line 2 of file %1. Line ignored.").arg(QString(SELECTOR_STRUCTURE_FILE).arg(i));
            m_errorMessages << msg;
            emit warning(msg);
            file.close();
            continue;
        }

        // Analyse line 2 to set modification modes and create the regex for the next data lines
        QString dataRegex;

        if (regex.cap(1) == "rel")
        {
            modif->setCarryingCapacityMode(MapStructureModifier::Relative);
            dataRegex = "("+FLOAT+"|-1)"+SPACE+"+";
        }
        else
        {
            modif->setCarryingCapacityMode(MapStructureModifier::Absolute);
            dataRegex = "("+INT+"|-1)"+SPACE+"+";
        }

        if (regex.cap(2) == "rel")
        {
            modif->setGrowthRateMode(MapStructureModifier::Relative);
            dataRegex += "("+FLOAT+"|-1)"+SPACE+"+";
        }
        else
        {
            modif->setGrowthRateMode(MapStructureModifier::Absolute);
            dataRegex += "("+FLOAT+"|-1)"+SPACE+"+";
        }

        if (regex.cap(3) == "rel")
        {
            modif->setMigrationRateMode(MapStructureModifier::Relative);
            dataRegex += "("+FLOAT+"|-1)"+LINE_END;
        }
        else
        {
            modif->setMigrationRateMode(MapStructureModifier::Absolute);
            dataRegex += "("+FLOAT+"|-1)"+LINE_END;
        }

        regex.setPattern(dataRegex);

        // Read data lines
        for (int y = 0; y < modif->height(); y++)
        {
            for (int x = 0; x < modif->width(); x++)
            {
                if (! regex.exactMatch(in.readLine().trimmed()))
                {
                    QString msg = QString("Error on line %1 of file %2. Line ignored").arg(2 + x + y * modif->width())
                                         .arg(QString(SELECTOR_STRUCTURE_FILE).arg(i));
                    m_errorMessages << msg;
                    emit warning(msg);
                    continue;
                }
                if (regex.cap(1) != "-1")
                {
                    if (modif->carryingCapacityMode() == MapStructureModifier::Relative)
                    {
                        modif->deme(x, y)->setRelCarryingCapacity(regex.cap(1).toDouble());
                    }
                    else
                    {
                        modif->deme(x, y)->setAbsCarryingCapacity(regex.cap(1).toInt());
                    }
                }
                if (regex.cap(2) != "-1")
                {
                    if (modif->growthRateMode() == MapStructureModifier::Relative)
                    {
                        modif->deme(x, y)->setRelGrowthRate(regex.cap(2).toDouble());
                    }
                    else
                    {
                        modif->deme(x, y)->setAbsGrowthRate(regex.cap(2).toDouble());
                    }
                }
                if (regex.cap(3) != "-1")
                {
                    if (modif->growthRateMode() == MapStructureModifier::Relative)
                    {
                        modif->deme(x, y)->setRelMigrationRate(regex.cap(3).toDouble());
                    }
                    else
                    {
                        modif->deme(x, y)->setAbsMigrationRate(regex.cap(3).toDouble());
                    }
                }
            }
        }

        file.close();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SelectorConfigFiles::write(SimulationModel *sim)
{
    emit info(QString("Writing SELECTOR configuration to directory %1").arg(m_directory));
    m_errorMessages.clear();

    int i;
    QFile file(m_directory + "/" + SELECTOR_PARAM_FILE);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        m_errorMessages << QString("Failed to open %1").arg(file.fileName());
        return false;
    }

    QTextStream out(&file);
    out << "// GENERAL SETTINGS\n"
        << sim->nbSimulations() << "\t// Number of simulations\n"
        << ABCIntervalToStr(sim->nbGenerations()) << "\t// Number of generations\n"
        << "// GENETIC SETTINGS\n"
        << ABCIntervalToStr(sim->initialAllelePoolSize()) << "\t// Size of the initial allele pool\n"
        << (sim->uniqueAllelePool() ? "0" : "1") << "\t// 0: One unique allele pool | 1: Independant allele pools for each population\n"
        << ABCIntervalToStr(sim->firstAlleleFrequency()) << "\t// Initial frequency for allele \"a1\"\n"
        << ABCIntervalToStr(sim->mutationRate()) << "\t// Mutation rate\n"
        << "// SELECTION SETTINGS\n";
    switch (sim->selection()->model())
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
        << sim->selection()->heterogeneity() << "\t// Selection heterogeneity. 0: Uniform | 1: Latitudinal | 2: Longitudinal\n"
        << ABCIntervalToStr(sim->selection()->uniformCoefficient()) << "\t// Uniform selection coefficient\n"
        << ABCIntervalToStr(sim->selection()->northEastCoefficient()) << "\t// Northern or eastern selection coefficient\n"
        << ABCIntervalToStr(sim->selection()->southWestCoefficient()) << "\t// Southern or western selection coefficient\n"
        << "// OUTPUT SETTINGS\n"
        << (sim->outputCompleteHistory() ? "1" : "0") << "\t// Generate allele frequency files for last simulation. 1: Yes | 0: No\n"
        << "// WORLD STRUCTURE AND DEMOGRAPHY SETTINGS\n"
        << sim->map()->width() << "\t// Number of demes along the X axis\n"
        << sim->map()->height() << "\t// Number of demes along the Y axis\n";

    // Groups
    std::map<int, std::vector<Deme*> > groups = sim->map()->groups();
    out << groups.size() << "\t// Number of population groups\n"
        << "0\t// All demes within a group have the same demography. 0: No | 1: Yes\n";

    // Routes
    const QList<QPair<QPoint, QPoint> > &routes = sim->map()->routes()->getAll();
    QString routesLine = "";

    for (int i = 0; i < routes.size(); i++)
    {
        routesLine += QString("(%1,%2,%3) ").arg(routes[i].first.x() + routes[i].first.y() * sim->map()->width())
                                            .arg(routes[i].second.x() + routes[i].second.y() * sim->map()->width())
                                            .arg(sim->map()->routes()->routeFactorAt(i));
    }

    out << routes.size() << "\t// Number of routes\n"
        << routesLine << "\n";

    // Poulation structures
    std::list<int> generations = sim->map()->modificationGenerations();
    out << generations.size() + 1 << "\t// Number of different population structures to load\n"
        << "1\t0\n";
    i = 2;
    for (auto it = generations.begin(); it != generations.end(); it++, i++)
    {
        out << i << "\t" << *it << "\n";
    }
    file.close();

    // Structure file 1
    file.setFileName(m_directory + "/" + QString(SELECTOR_STRUCTURE_FILE).arg(1));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        m_errorMessages << QString("Failed to open %1").arg(file.fileName());
        return false;
    }
    out << "// Population characteristics\n"
        << "// Initial size (positive int), Carrying capacity K (positive int), growth rate r ([0,1]),migration rate m ([0,1]), group of demes, sample size\n";
    for (int y = 0; y < sim->map()->height(); y++)
    {
        for (int x = 0; x < sim->map()->width(); x++)
        {
            Deme *deme = sim->map()->deme(x, y);
            out << QString("%1\t%2\t%3\t%4\t%5\t%6\n").arg(ABCIntervalToStr(deme->initialPopulation()))
                                                      .arg(deme->isEnabled() ? ABCIntervalToStr(deme->carryingCapacity()) : "-1")
                                                      .arg(ABCIntervalToStr(deme->growthRate()))
                                                      .arg(ABCIntervalToStr(deme->migrationRate()))
                                                      .arg(deme->group())
                                                      .arg(deme->sampleSize());
        }
    }
    file.close();

    // Next structure files
    i = 2;
    for (auto generationIt = generations.begin(); generationIt != generations.end(); generationIt++, i++)
    {
        file.setFileName(m_directory + "/" + QString(SELECTOR_STRUCTURE_FILE).arg(i));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            m_errorMessages << QString("Failed to open %1").arg(file.fileName());
            continue;
        }
        MapStructureModifier *modification = sim->map()->modification(*generationIt);

        out << "// K (positive int), r ([0,1]), m ([0,1])\n"
            << (modification->carryingCapacityMode() == MapStructureModifier::Absolute ? "abs\t" : "rel\t")
            << (modification->growthRateMode() == MapStructureModifier::Absolute ? "abs\t" : "rel\t")
            << (modification->migrationRateMode() == MapStructureModifier::Absolute ? "abs\n" : "rel\n");

        for (int y = 0; y < modification->height(); y++)
        {
            for (int x = 0; x < modification->width(); x++)
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
                else if (modification->growthRateMode() == MapStructureModifier::Absolute)
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
    }
    return m_errorMessages.empty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
QString SelectorConfigFiles::directory() const                  { return m_directory; }
void SelectorConfigFiles::setDirectory(QString directory)       { m_directory = directory; }
const QStringList& SelectorConfigFiles::errorMessages() const   { return m_errorMessages; }

////////////////////////////////////////////////////////////////////////////////////////////////////
void SelectorConfigFiles::_handleParamLine(SimulationModel *sim, int line, QRegExp *regex,
                                           bool &groupedDemes, int &nbRoutes, int &nbStructures)
{
    QRegExp routeRegex(ROUTE);
    int pos = 0;

    switch (line)
    {
    case 0: break;
    case 1: sim->setNbSimulations(regex->cap(1).toInt()); break;
    case 2: sim->setNbGenerations(toInterval<int>(regex->cap(1), regex->cap(2), regex->cap(3), regex->cap(4))); break;
    case 3: break;
    case 4: sim->setInitialAllelePoolSize(toInterval<int>(regex->cap(1), regex->cap(2), regex->cap(3), regex->cap(4))); break;
    case 5: sim->setUniqueAllelePool(! regex->cap(1).toInt()); break;
    case 6: sim->setFirstAlleleFrequency(toInterval<double>(regex->cap(1), regex->cap(2), regex->cap(3), regex->cap(4))); break;
    case 7: sim->setMutationRate(toInterval<double>(regex->cap(1), regex->cap(2), regex->cap(3), regex->cap(4))); break;
    case 8: break;
    case 9:
        switch (regex->cap(1).toInt())
        {
        case 2: sim->selection()->setModel(Selection::FDS); break;
        case 3: sim->selection()->setModel(Selection::DPS); break;
        default: sim->selection()->setModel(Selection::SOS); break;
        }
        break;
    case 10:
        switch (regex->cap(1).toInt())
        {
        case 1: sim->selection()->setHeterogeneity(Selection::Latitudinal); break;
        case 2: sim->selection()->setHeterogeneity(Selection::Longitudinal); break;
        default: sim->selection()->setHeterogeneity(Selection::Uniform); break;
        }
        break;
    case 11: sim->selection()->setUniformCoefficient(toInterval<double>(regex->cap(1), regex->cap(2), regex->cap(3), regex->cap(4))); break;
    case 12: sim->selection()->setNorthEastCoefficient(toInterval<double>(regex->cap(1), regex->cap(2), regex->cap(3), regex->cap(4))); break;
    case 13: sim->selection()->setSouthWestCoefficient(toInterval<double>(regex->cap(1), regex->cap(2), regex->cap(3), regex->cap(4))); break;
    case 14: break;
    case 15: sim->setOutputCompleteHistory(regex->cap(1).toInt()); break;
    case 16: break;
    case 17: sim->map()->setWidth(regex->cap(1).toInt()); break;
    case 18: sim->map()->setHeight(regex->cap(1).toInt()); break;
    case 19: break;
    case 20: groupedDemes = regex->cap(1).toInt(); break;
    case 21: nbRoutes = regex->cap(1).toInt(); break;
    case 22:
        while ((pos = routeRegex.indexIn(regex->cap(0), pos)) != -1 && nbRoutes > 0)
        {
            int fromDemeIndex = routeRegex.cap(1).toInt();
            int toDemeIndex = routeRegex.cap(2).toInt();
            double factor = routeRegex.cap(3).toDouble();
            sim->map()->routes()->set(QPoint(fromDemeIndex % sim->map()->width(), fromDemeIndex / sim->map()->width()),
                                      QPoint(toDemeIndex % sim->map()->width(), toDemeIndex / sim->map()->width()),
                                      factor);
            pos += routeRegex.matchedLength();
            nbRoutes--;
        }
        if (nbRoutes > 0 || pos != -1)
        {
            m_errorMessages << QString("Number of routes specified on line %1 in file %2 doesn't match the number of routes.")
                               .arg(line + 1).arg(SELECTOR_PARAM_FILE);
        }
        break;
    case 23: nbStructures = regex->cap(1).toInt(); break;
    case 24: break;
    default: break;
    }
}

template<typename T>
ABCInterval<T> SelectorConfigFiles::toInterval(const QString &singleValueCap, const QString &distributionCap,
                                               const QString &firstBoundCap, const QString &secondBoundCap)
{
    ABCInterval<T> interval;

    if (! singleValueCap.isEmpty())
    {
        interval.setValue(singleValueCap.toDouble());
    }
    else
    {
        if (distributionCap == "UNIFORM")
        {
            interval.setDistribution(ABCInterval<T>::Uniform);
        }
        else if (distributionCap == "LOG_UNIFORM")
        {
            interval.setDistribution(ABCInterval<T>::LogUniform);
        }
        else if (distributionCap == "UNIFORM_DISCRETE")
        {
            interval.setDistribution(ABCInterval<T>::UniformDiscrete);
        }
        interval.setFirstBound(firstBoundCap.toDouble());
        interval.setSecondBound(secondBoundCap.toDouble());
    }
    return interval;
}
