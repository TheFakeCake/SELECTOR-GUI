#ifndef BOXSELECTTOOL_H
#define BOXSELECTTOOL_H

#include "AbstractSelectTool.h"
#include <QPoint>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The BoxSelectTool class
///
/// This selection tool is used to select groups of deme from a MapWidget. The selection is rectangle
/// shaped (a box). The user must hold the left mouse button and drag th cursor over the map to create
/// the selection box.
/// Holding SHIFT while doing so keeps the old selection and adds the newly selected
/// demes to it.
/// Holding CTRL will also keep the old selection but the newly selected demes will be removed from it.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BoxSelectTool : public AbstractSelectTool
{
public:

    BoxSelectTool();

    bool handleMousePress(QMouseEvent *event);
    bool handleMouseMove(QMouseEvent *event);
    bool handleMouseRelease(QMouseEvent *event);
    void draw(QPainter &painter);

private:

    bool m_selectionMotion;
    bool m_selecting;
    QPoint m_initialCoord;
    QPoint m_mouseCoord;
};

#endif // BOXSELECTTOOL_H
