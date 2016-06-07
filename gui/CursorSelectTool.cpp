#include "CursorSelectTool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CursorSelectTool::CursorSelectTool() :
    AbstractSelectTool(),
    m_selectionMotion(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::handleMousePress(QMouseEvent *event)
{
    bool res = false;

    // Select demes with left mouse button
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
            m_mapWidget->selection().clear();
            res = true;
        }

        if (selectDeme(event->pos()))
        {
            res = true;
        }
    }
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::handleMouseMove(QMouseEvent *event)
{
    if (m_selectionMotion)
    {
        return selectDeme(event->pos());
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::handleMouseRelease(QMouseEvent *event)
{
    // If the left mouse button is released, the selection motion stops
    if (event->button() == Qt::LeftButton)
    {
        m_selectionMotion = false;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CursorSelectTool::selectDeme(const QPoint& mousePos)
{
    QList<QPoint> &selection = m_mapWidget->selection();
    Deme *deme = m_mapWidget->map()->deme(mousePos.x() / m_mapWidget->demeSize(),
                                          mousePos.y() / m_mapWidget->demeSize());

    // Check if the mouse was over the map
    if (deme == nullptr)
    {
        return false;
    }

    QPoint demeCoord(deme->x(), deme->y());

    // If the user is selecting, the deme coordinate is added to the selection if it's not already in it.
    if (m_selecting)
    {
        if (! selection.contains(demeCoord))
        {
            selection << demeCoord;
            return true;
        }
    }
    else // Else, the deme coordinate is removed from the selection
    {
        if (selection.contains(demeCoord))
        {
            selection.removeOne(demeCoord);
            return true;
        }
    }
    return false;
}
