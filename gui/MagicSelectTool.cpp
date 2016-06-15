#include "MagicSelectTool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
MagicSelectTool::MagicSelectTool() :
    AbstractSelectTool()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool MagicSelectTool::handleMousePress(QMouseEvent *event)
{
    m_mapWidget->selection().clear();
    Deme *deme = m_mapWidget->map()->deme(event->pos().x() / m_mapWidget->demeSize(),
                                          event->pos().y() / m_mapWidget->demeSize());

    // Check if the press event occured over a deme of the map
    if (deme == nullptr)
    {
        return true;
    }

    // Select all the deme sharing the same value that is being displayed
    switch (m_mapWidget->displayMode())
    {
    case MapWidget::Group:
        return selectDemes<int>(deme->group(), &Deme::group);
    case MapWidget::Population:
        return selectDemes<ABCInterval<int> >(deme->initialPopulation(), &Deme::initialPopulation);
    case MapWidget::CarryingCapacity:
        return selectDemes<ABCInterval<int> >(deme->carryingCapacity(), &Deme::carryingCapacity);
    case MapWidget::GrowthRate:
        return selectDemes<ABCInterval<double> >(deme->growthRate(), &Deme::growthRate);
    case MapWidget::MigrationRate:
        return selectDemes<ABCInterval<double> >(deme->migrationRate(), &Deme::migrationRate);
    case MapWidget::SampleSize:
        return selectDemes<int>(deme->sampleSize(), &Deme::sampleSize);
    default:
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool MagicSelectTool::handleMouseMove(QMouseEvent *event)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool MagicSelectTool::handleMouseRelease(QMouseEvent *event)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool MagicSelectTool::selectDemes(T value, T (Deme::*methodPtr)() const)
{
    Map *map = m_mapWidget->map();
    QList<QPoint> &selection = m_mapWidget->selection();

    // Check the value of each deme in the map ...
    for (int y = 0; y < map->height(); y++)
    {
        for (int x = 0; x < map->width(); x++)
        {
            // ... if the deme is active and its value is the same, the deme is added to the selection
            if (map->deme(x, y)->isEnabled() && (map->deme(x, y)->*methodPtr)() == value)
            {
                selection << QPoint(x, y);
            }
        }
    }
    return true;
}
