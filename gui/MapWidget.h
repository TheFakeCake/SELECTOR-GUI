#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QColor>
#include <QList>
#include "models/map.h"

class MapWidget : public QWidget
{
    Q_OBJECT

public:

    enum DisplayMode {
        Group = 0,
        Population,
        CarryingCapacity,
        GrowthRate,
        MigrationRate,
        SampleSize,
        LastDisplayMode
    };

    enum SelectionMode {
        Cursor = 0,
        Box,
        Magic,
        LastSelectionMode
    };

    static const int NB_GROUP_COLORS = 17;
    static const QColor GROUP_COLORS[NB_GROUP_COLORS];

    explicit MapWidget(Map *map, QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    DisplayMode displayMode() const;
    QList<QPoint> & selection() const;

signals:

    void selectionChanged(QList<QPoint> & newSelection);

public slots:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Update the map image.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void updateWidget();
    void setDisplayMode(DisplayMode mode);
    void setDisplayMode(int mode);
    void setSelectionMode(SelectionMode mode);
    void setSelectionMode(int mode);

private:

    void resizeImage(QImage *image, const QSize &newSize);
    void updateImage();
    QColor getDemeColor(Deme* deme, double maxValue);
    void selectDeme(const QPoint& mousePos);

    Map *m_map;
    DisplayMode m_mode;
    double m_demeSize;
    QImage m_img;

    QList<QPoint> m_selection;
    bool m_selectionMotion;
    bool m_selecting;
};

#endif // MAPWIDGET_H
