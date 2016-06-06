#include "BoxSelectTool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
BoxSelectTool::BoxSelectTool() :
    m_selectionMotion(false)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool BoxSelectTool::handleMousePress(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize)
{
    bool res = false;

    // Select demes with left button of the mouse
    if (event->button() == Qt::LeftButton)
    {
        m_initialCoord.setX(event->pos().x() / demeSize);
        m_initialCoord.setY(event->pos().y() / demeSize);
        m_mouseCoord = m_initialCoord;

        if (m_initialCoord.x() >= map->width() || m_initialCoord.y() >= map->height())
        {
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
            selection.clear();
            res = true;
        }

/*
        if (selectDeme(event->pos(), map, selection, demeSize))
        {
            res = true;
        }
*/
    }
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool BoxSelectTool::handleMouseMove(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize)
{
    QPoint mouseCoord(event->pos().x() / demeSize, event->pos().y() / demeSize);

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

    if (mouseCoord != m_mouseCoord)
    {
        m_mouseCoord = mouseCoord;
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool BoxSelectTool::handleMouseRelease(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_selectionMotion)
        {
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
                        if (selection.indexOf(cursor) == -1)
                        {
                            selection << cursor;
                        }
                    }
                    else
                    {
                        selection.removeOne(cursor);
                    }

                    cursor.setX(cursor.x() + 1);
                }
                cursor.setY(cursor.y() + 1);
            }
            m_selectionMotion = false;
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void BoxSelectTool::draw(QPainter &painter, double demeSize)
{
    if (m_selectionMotion)
    {
        qDebug(QString("Init coord : %1, %2\nCurr coord : %3 %4").arg(m_initialCoord.x()).arg(m_initialCoord.y()).arg(m_mouseCoord.x()).arg(m_mouseCoord.y()).toLatin1().data());

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
