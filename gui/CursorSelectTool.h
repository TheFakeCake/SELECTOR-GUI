#ifndef CURSORSELECTTOOL_H
#define CURSORSELECTTOOL_H

#include "MapWidget.h"

class CursorSelectTool : public MapWidget::SelectTool
{
public:

    CursorSelectTool();

    bool handleMousePress(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize);
    bool handleMouseMove(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize);
    bool handleMouseRelease(QMouseEvent *event, Map *map, QList<QPoint> &selection, double demeSize);
    void draw(QPainter &painter, double demeSize) {}

private:

    bool selectDeme(const QPoint& mousePos, Map *map, QList<QPoint> &selection, double demeSize);

    bool m_selectionMotion;
    bool m_selecting;
};

#endif // CURSORSELECTTOOL_H
