#include "BoxSelectTool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
BoxSelectTool::BoxSelectTool() :
    AbstractSelectTool(),
    m_selectionMotion(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool BoxSelectTool::handleMousePress(QMouseEvent *event)
{
    bool res = false;

    // Select demes with left button of the mouse
    if (event->button() == Qt::LeftButton)
    {
        m_initialCoord.setX(event->pos().x() / m_mapWidget->demeSize());
        m_initialCoord.setY(event->pos().y() / m_mapWidget->demeSize());
        m_mouseCoord = m_initialCoord;

        // Check if clicked deme is inside the map
        if (m_initialCoord.x() >= m_mapWidget->map()->width() || m_initialCoord.y() >= m_mapWidget->map()->height())
        {
            if (! m_mapWidget->selection().empty())
            {
                m_mapWidget->selection().clear();
                return true;
            }
            return false;
        }

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
            if (! m_mapWidget->selection().empty())
            {
                m_mapWidget->selection().clear();
                res = true;
            }
        }
    }
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool BoxSelectTool::handleMouseMove(QMouseEvent *event)
{
    QPoint mouseCoord(event->pos().x() / m_mapWidget->demeSize(), event->pos().y() / m_mapWidget->demeSize());
    Map *map = m_mapWidget->map();

    // Check if mouse coord is inside the map
    if (mouseCoord.x() < 0)
    {
        mouseCoord.setX(0);
    }
    else if (mouseCoord.x() >= map->width())
    {
        mouseCoord.setX(map->width() - 1);
    }

    if (mouseCoord.y() < 0)
    {
        mouseCoord.setY(0);
    }
    else if (mouseCoord.y() >= map->height())
    {
        mouseCoord.setY(map->height() - 1);
    }

    // If the mouse moved onto another deme
    if (mouseCoord != m_mouseCoord)
    {
        m_mouseCoord = mouseCoord;
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool BoxSelectTool::handleMouseRelease(QMouseEvent *event)
{
    bool res = false;

    // If the left button is released ...
    if (event->button() == Qt::LeftButton)
    {
        // ... while selecting
        if (m_selectionMotion)
        {
            QList<QPoint> &selection = m_mapWidget->selection();

            // Add / remove all the demes inside the selection box to / from the selection
            QPoint first(qMin(m_initialCoord.x(), m_mouseCoord.x()), qMin(m_initialCoord.y(), m_mouseCoord.y()));
            QPoint last(qMax(m_initialCoord.x(), m_mouseCoord.x()), qMax(m_initialCoord.y(), m_mouseCoord.y()));
            QPoint cursor = first;

            while (cursor.y() <= last.y())
            {
                cursor.setX(first.x());
                while (cursor.x() <= last.x())
                {
                    if (m_selecting)
                    {
                        // Check if deme is not already in the selection when adding it
                        if (! selection.contains(cursor))
                        {
                            selection << cursor;
                            res = true;
                        }
                    }
                    else
                    {
                        if (selection.contains(cursor))
                        {
                            selection.removeOne(cursor);
                            res = true;
                        }
                    }

                    cursor.setX(cursor.x() + 1);
                }
                cursor.setY(cursor.y() + 1);
            }
            m_selectionMotion = false;
        }
    }
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void BoxSelectTool::draw(QPainter &painter)
{
    // Paint the rectangle box if user is selecting something
    if (m_selectionMotion)
    {
        double demeSize = m_mapWidget->demeSize();
        QRectF rect(qMin(m_initialCoord.x(), m_mouseCoord.x()) * demeSize,
                    qMin(m_initialCoord.y(), m_mouseCoord.y()) * demeSize,
                    (qAbs(m_mouseCoord.x() - m_initialCoord.x()) + 1) * demeSize,
                    (qAbs(m_mouseCoord.y() - m_initialCoord.y()) + 1) * demeSize);
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(rect);
    }
}
