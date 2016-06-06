#include "MapWidget.h"

#include <QPainter>
#include "CursorSelectTool.h"
#include "BoxSelectTool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
const QColor MapWidget::GROUP_COLORS[] = {
    QColor::fromRgb(40, 40, 40),
    QColor::fromRgb(191,0,10),
    QColor::fromRgb(157,125,255),
    QColor::fromRgb(38,128,0),
    QColor::fromRgb(255,125,131),
    QColor::fromRgb(0,153,255),
    QColor::fromRgb(76,255,0),
    QColor::fromRgb(255,0,229),
    QColor::fromRgb(62,101,128),
    QColor::fromRgb(64,54,0),
    QColor::fromRgb(48,0,191),
    QColor::fromRgb(31,64,49),
    QColor::fromRgb(191,163,0),
    QColor::fromRgb(79,62,128),
    QColor::fromRgb(125,255,196),
    QColor::fromRgb(64,31,31)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
MapWidget::MapWidget(Map *map, QWidget *parent) :
    QWidget(parent),
    m_map(map),
    m_mode(Group),
    m_img(1, 1, QImage::Format_ARGB32),
    m_selectionMotion(false)
{
    setAttribute(Qt::WA_StaticContents);
    m_selectionMode = Cursor;
    m_selectTool = new CursorSelectTool();
    connect(m_map, SIGNAL(changed()), this, SLOT(updateWidget()));
    updateImage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapWidget::~MapWidget()
{
    delete m_selectTool;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_selectTool->handleMousePress(event, m_map, m_selection, m_demeSize))
    {
        emit selectionChanged(m_selection);
        updateImage();
    }
    /*
    if (event->button() == Qt::LeftButton)
    {
        m_selectionMotion = true;
        m_selecting = true;

        if (event->modifiers() & Qt::ControlModifier)
        {
            m_selecting = false;
        }
        else if (! (event->modifiers() & Qt::ShiftModifier))
        {
            m_selection.clear();
            emit selectionChanged(m_selection);
        }

        selectDeme(event->pos());
        updateImage();
    }
    */
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_selectTool->handleMouseMove(event, m_map, m_selection, m_demeSize))
    {
        emit selectionChanged(m_selection);
        updateImage();
    }
    /*
    if (m_selectionMotion)
    {
        selectDeme(event->pos());
    }
    */
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_selectTool->handleMouseRelease(event, m_map, m_selection, m_demeSize))
    {
        emit selectionChanged(m_selection);
        updateImage();
    }
    /*
    switch (m_selectionMode)
    {
    case Cursor:
        m_selectionMotion = false;
        break;

    case Box:

        break;

    case Magic: break;
    default: break;
    }
    */
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::selectDeme(const QPoint& mousePos)
{
    Deme *deme = m_map->deme(mousePos.x() / m_demeSize,
                             mousePos.y() / m_demeSize);
    if (deme == nullptr)
    {
        return;
    }

    QPoint demeCoord(deme->x(), deme->y());

    if (m_selecting)
    {
        if (! m_selection.contains(demeCoord))
        {
            m_selection.append(demeCoord);
            updateImage();
            emit selectionChanged(m_selection);
        }
    }
    else
    {
        if (m_selection.contains(demeCoord))
        {
            m_selection.removeOne(demeCoord);
            updateImage();
            emit selectionChanged(m_selection);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_img, dirtyRect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::resizeEvent(QResizeEvent *event)
{
    // Check if image is big enough
    if (width() > m_img.width() || height() > m_img.height())
    {
        int newWidth = qMax(width() + 128, m_img.width());
        int newHeight = qMax(height() + 128, m_img.height());
        resizeImage(&m_img, QSize(newWidth, newHeight));
    }
    updateImage();
    QWidget::resizeEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MapWidget::DisplayMode MapWidget::displayMode() const
{
    return m_mode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::updateWidget()
{
    int oldSelectionSize = m_selection.size();
    for (int i = 0; i < m_selection.size(); i++)
    {
        if (m_selection[i].x() >= m_map->width() || m_selection[i].y() >= m_map->height())
        {
            m_selection.removeAt(i);
            i--;
        }
    }
    if (oldSelectionSize != m_selection.size())
    {
        emit selectionChanged(m_selection);
    }
    updateImage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::updateImage()
{
    // New deme size
    m_demeSize = qMin((double)width() / m_map->width(), (double)height() / m_map->height());
    if (m_demeSize < 1.0)
    {
        m_demeSize = 1.0;
    }

    QPainter painter(&m_img);

    m_img.fill(QWidget::palette().color(QWidget::backgroundRole()));

    double maxValue;
    switch (m_mode)
    {
    case Population:        maxValue = m_map->maxInitialPopulation(); break;
    case CarryingCapacity:  maxValue = m_map->maxCarryingCapacity(); break;
    case GrowthRate:        maxValue = m_map->maxGrowthRate(); break;
    case MigrationRate:     maxValue = m_map->maxMigrationRate(); break;
    case SampleSize:        maxValue = m_map->maxSampleSize(); break;
    default: break;
    }

    for (int y = 0; y < m_map->height(); y++)
    {
        for (int x = 0; x < m_map->width(); x++)
        {
           painter.fillRect(QRectF(x * m_demeSize, y * m_demeSize, m_demeSize, m_demeSize),
                            getDemeColor(m_map->deme(x, y), maxValue));
        }
    }
    for (int y = 0; y < m_map->height(); y++)
    {
        painter.drawLine(0, y * m_demeSize, m_map->width() * m_demeSize, y * m_demeSize);
    }
    painter.drawLine(0, m_map->height() * m_demeSize - 0.5, m_map->width() * m_demeSize, m_map->height() * m_demeSize - 0.5);
    for (int x = 0; x < m_map->width(); x++)
    {
        painter.drawLine(x * m_demeSize, 0, x * m_demeSize,  m_map->height() * m_demeSize);
    }
    painter.drawLine(m_map->width() * m_demeSize - 0.5, 0, m_map->width() * m_demeSize - 0.5,  m_map->height() * m_demeSize);
    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(3);
    painter.setPen(pen);
    QListIterator<QPoint> it(m_selection);
    while (it.hasNext())
    {
        QPoint demeCoord = it.next();
        painter.drawRect(QRectF(demeCoord.x() * m_demeSize, demeCoord.y() * m_demeSize, m_demeSize, m_demeSize));
    }

    m_selectTool->draw(painter, m_demeSize);

    update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::setDisplayMode(DisplayMode mode)
{
    setDisplayMode((int)mode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::setDisplayMode(int mode)
{
    if (mode != m_mode)
    {
        if (mode >= 0 && mode < LastDisplayMode)
        {
            m_mode = (DisplayMode)mode;
            updateImage();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::setSelectionMode(SelectionMode mode)
{
    setSelectionMode((int)mode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::setSelectionMode(int mode)
{
    if (mode != m_selectionMode)
    {
        if (mode >= 0 && mode <= LastSelectionMode)
        {
            m_selectionMode = (SelectionMode)mode;
            delete m_selectTool;

            switch (m_selectionMode)
            {
            case Box:
                m_selectTool = new BoxSelectTool();
                break;
            case Magic:
            case Cursor:
            default:
                m_selectTool = new CursorSelectTool();
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MapWidget::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
    {
        return;
    }
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
QColor MapWidget::getDemeColor(Deme* deme, double maxValue)
{
    if (! deme->isEnabled())
    {
        return GROUP_COLORS[0];
    }
    double value;
    switch (m_mode)
    {
    case Group:
        // TODO : random color when group > 21
        return GROUP_COLORS[deme->group()];
    case Population:        value = deme->initialPopulation(); break;
    case CarryingCapacity:  value = deme->carryingCapacity(); break;
    case GrowthRate:        value = deme->growthRate(); break;
    case MigrationRate:     value = deme->migrationRate(); break;
    case SampleSize:        value = deme->sampleSize(); break;
    default: return Qt::white;
    }

    if (qFuzzyCompare(1.0, 1.0 + maxValue))
    {
        maxValue = 1.0;
    }
    if (qFuzzyCompare(1.0, 1.0 + value))
    {
        return Qt::white;
    }
    return QColor(value / maxValue * 128,
                  value / maxValue * 128,
                  value / maxValue * 255);
}
