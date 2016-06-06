#ifndef BOXSELECTTOOL_H
#define BOXSELECTTOOL_H

#include "MapWidget.h"
#include <QPainter>

class BoxSelectTool : public MapWidget::SelectTool
{
public:

    BoxSelectTool();

    bool handleMousePress(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize);
    bool handleMouseMove(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize);
    bool handleMouseRelease(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize);
    void draw(QPainter &painter, double demeSize);

private:

    bool m_selectionMotion;
    bool m_selecting;
    QPoint m_initialCoord;
    QPoint m_mouseCoord;
};

#endif // BOXSELECTTOOL_H
