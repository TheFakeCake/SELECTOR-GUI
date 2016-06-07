#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QColor>
#include <QList>
#include "models/map.h"

class AbstractSelectTool;

#include "AbstractSelectTool.h"

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
    ~MapWidget();

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    DisplayMode displayMode() const;
    QList<QPoint> & selection();
    double demeSize() const;
    Map *map() const;

    void setPreventUpdates(bool b);
    bool isPreventingUpdate();

signals:

    void selectionChanged();

public slots:

    void updateWidget();
    void setDisplayMode(DisplayMode mode);
    void setDisplayMode(int mode);
    void setSelectTool(AbstractSelectTool *tool);

private:

    void resizeImage(QImage *image, const QSize &newSize);
    void updateImage();
    QColor getDemeColor(Deme* deme, double maxValue);

    Map *m_map;
    DisplayMode m_displayMode;
    double m_demeSize;
    QImage m_img;
    bool m_preventingUpdates;
    bool m_needsUpdate;

    QList<QPoint> m_selection;
    AbstractSelectTool *m_selectTool;
};

#endif // MAPWIDGET_H
