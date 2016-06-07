#ifndef ABSTRACTSELECTTOOL_H
#define ABSTRACTSELECTTOOL_H

class MapWidget;

#include <QMouseEvent>
#include <QList>
#include <QPoint>
#include <QPainter>
#include "MapWidget.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The AbstractSelectTool class
///
/// This abstract class contains the basic interface for a selection tool used by the MapWidget class.
/// A selection tool offers to the used a way of selecting demes with the mouse in a MapWidget.
////////////////////////////////////////////////////////////////////////////////////////////////////
class AbstractSelectTool
{
public:

    AbstractSelectTool();
    virtual ~AbstractSelectTool() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Mouse button press event handling on the MapWidget.
    ///
    /// This method is called by the MapWidget which uses the AbstractSelectTool when a mouse press
    /// event occurs on the MapWidget.
    ///
    /// \param event : Pointer to the QMouseEvent.
    /// \return If the content of the selection has been modified, TRUE must be returned, else FALSE.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    virtual bool handleMousePress(QMouseEvent *event) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Mouse move event handling on the MapWidget.
    ///
    /// This method is called by the MapWidget when the mouse moves over it.
    ///
    /// \param event : Pointer to the QMouseEvent.
    /// \return If the content of the selection has been modified, TRUE must be returned, else FALSE.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    virtual bool handleMouseMove(QMouseEvent *event) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Mouse button release event handling on the MapWidget.
    ///
    /// This method is called by the MapWidget when a mouse button is released.
    ///
    /// \param event : Pointer to the QMouseEvent.
    /// \return If the content of the selection has been modified, TRUE must be returned, else FALSE.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    virtual bool handleMouseRelease(QMouseEvent *event) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Draw the selection.
    ///
    /// In some cases, a select tool may require to show something on the MapWidget, like the rectangle
    /// box of the BoxSelectTool. This method is called by the MapWidget when it updates its appearance.
    ///
    /// \param painter : Reference to the QPainter instance to be used.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    virtual void draw(QPainter &painter) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Set the MapWidget that uses the select tool.
    /// \param mapWidget : Pointer to the MapWidget instance.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    virtual void setMapWidget(MapWidget *mapWidget);

protected:

    MapWidget *m_mapWidget; ///< Pointer to the MapWidget instance that uses the tool.
};

#endif // ABSTRACTSELECTTOOL_H
