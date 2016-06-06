#include "CursorSelectTool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CursorSelectTool::CursorSelectTool() :
    m_selectionMotion(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::handleMousePress(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize)
{
    bool res = false;

    // Select demes with left button of the mouse
    if (event->button() == Qt::LeftButton)
    {
        m_selectionMotion = true;
        m_selecting = true;

        // If CTRL is held down, demes are deselected
        if (event->modifiers() & Qt::ControlModifier)
        {
            m_selecting = false;
        }
        // Else if SHIFT is NOT held down, reset the selection
        else if (! (event->modifiers() & Qt::ShiftModifier))
        {
            selection.clear();
            res = true;
        }

        if (selectDeme(event->pos(), map, selection, demeSize))
        {
            res = true;
        }
    }
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::handleMouseMove(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize)
{
    if (m_selectionMotion)
    {
        return selectDeme(event->pos(), map, selection, demeSize);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::handleMouseRelease(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize)
{
    if (event->button() == Qt::LeftButton)
    {
        m_selectionMotion = false;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::selectDeme(const QPoint& mousePos, Map *map, QList<QPoint> &selection, double demeSize)
{
    Deme *deme = map->deme(mousePos.x() / demeSize,
                             mousePos.y() / demeSize);
    if (deme == nullptr)
    {
        return false;
    }

    QPoint demeCoord(deme->x(), deme->y());

    if (m_selecting)
    {
        if (! selection.contains(demeCoord))
        {
            selection.append(demeCoord);
            return true;
        }
    }
    else
    {
        if (selection.contains(demeCoord))
        {
            selection.removeOne(demeCoord);
            return true;
        }
    }
    return false;
}
