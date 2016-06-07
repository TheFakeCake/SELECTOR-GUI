#ifndef CURSORSELECTTOOL_H
#define CURSORSELECTTOOL_H

#include "AbstractSelectTool.h"

class CursorSelectTool : public AbstractSelectTool
{
public:

    CursorSelectTool();

    bool handleMousePress(QMouseEvent *event);
    bool handleMouseMove(QMouseEvent *event);
    bool handleMouseRelease(QMouseEvent *event);
    void draw(QPainter &painter) {}

private:

    bool selectDeme(const QPoint& mousePos);

    bool m_selectionMotion;
    bool m_selecting;
};

#endif // CURSORSELECTTOOL_H
