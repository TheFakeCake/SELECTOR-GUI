#include "RoutesTableModel.h"

#include "map.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
RoutesTableModel::RoutesTableModel(Map *map, QObject *parent) :
    QAbstractTableModel(parent),
    m_map(map)
{
    m_mapWidth = m_map->width();
    m_mapHeight = m_map->height();
    connect(m_map, SIGNAL(widthChanged(int)), this, SLOT(handleMapWidthChange(int)));
    connect(m_map, SIGNAL(heightChanged(int)), this, SLOT(handleMapHeightChange(int)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
QVariant RoutesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    switch (section)
    {
    case 0: return "Source";
    case 1: return "Destination";
    case 2: return "Migration Factor";
    default: return QVariant();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int RoutesTableModel::rowCount(const QModelIndex &parent) const
{
    return m_routes.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int RoutesTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
QVariant RoutesTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    {
        QPair<QPoint,QPoint> route = m_routes[index.row()];

        switch (index.column())
        {
        case 0: return QString("%1, %2").arg(route.first.x()).arg(route.first.y());
        case 1: return QString("%1, %2").arg(route.second.x()).arg(route.second.y());
        case 2: return QString::number(m_factors[index.row()]);
        }
    }

    return QVariant();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const QList<QPair<QPoint, QPoint> > & RoutesTableModel::getAll() const
{
    return m_routes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const QPair<QPoint, QPoint> & RoutesTableModel::getAt(int index) const
{
    if (index < m_routes.size())
    {
        return m_routes[index];
    }
    return QPair<QPoint, QPoint>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
double RoutesTableModel::routeFactor(const QPoint &from, const QPoint &to) const
{
    QPair<QPoint,QPoint> route(from, to);
    int routeIndex = m_routes.indexOf(route);

    if (routeIndex == -1)
    {
        return 1.0;
    }
    return m_factors[routeIndex];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
double RoutesTableModel::routeFactorAt(int index) const
{
    if (index < m_routes.size())
    {
        return m_factors[index];
    }
    return 1.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void RoutesTableModel::set(const QPoint &from, const QPoint &to, double factor)
{
    QPair<QPoint,QPoint> route(from, to);
    int routeIndex = m_routes.indexOf(route);

    if (routeIndex == -1)
    {
        _addRoute(route, factor);
    }
    else
    {
        m_factors[routeIndex] = factor;
        emit dataChanged(index(routeIndex, 3), index(routeIndex, 3));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool RoutesTableModel::remove(const QPoint &from, const QPoint &to)
{
    int routeIndex = m_routes.indexOf(QPair<QPoint,QPoint>(from, to));

    if (routeIndex != -1)
    {
        _removeRoute(routeIndex);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void RoutesTableModel::handleMapWidthChange(int newWidth)
{
    if (newWidth < m_mapWidth)
    {
       for (int i = 0; i < m_routes.size(); i++)
       {
            QPair<QPoint,QPoint> &route = m_routes[i];

            if (route.first.x() >= newWidth || route.second.x() >= newWidth)
            {
                _removeRoute(i);
                i--;
            }
        }
    }
    m_mapWidth = newWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void RoutesTableModel::handleMapHeightChange(int newHeight)
{
    if (newHeight < m_mapHeight)
    {
       for (int i = 0; i < m_routes.size(); i++)
       {
            QPair<QPoint,QPoint> &route = m_routes[i];

            if (route.first.y() >= newHeight || route.second.y() >= newHeight)
            {
                _removeRoute(i);
                i--;
            }
        }
    }
    m_mapHeight = newHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void RoutesTableModel::_removeRoute(int index)
{
    beginRemoveRows(QModelIndex(), index, index);

    QPoint from = m_routes[index].first;
    QPoint to = m_routes[index].second;

    m_from[from].removeOne(index);
    if (m_from[from].empty())
    {
        m_from.remove(from);
    }

    m_to[to].removeOne(index);
    if (m_to[to].empty())
    {
        m_to.remove(to);
    }

    m_routes.removeAt(index);
    m_factors.removeAt(index);

    endRemoveRows();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void RoutesTableModel::_addRoute(QPair<QPoint,QPoint> &route, double factor)
{
    int index = m_routes.size();

    beginInsertRows(QModelIndex(), index, index);

    m_routes.append(route);
    m_factors.append(factor);

    if (!m_from.contains(route.first))
    {
        m_from[route.first] = QList<int>();
    }
    m_from[route.first].append(index);

    if (!m_to.contains(route.second))
    {
        m_to[route.second] = QList<int>();
    }
    m_to[route.second].append(index);

    endInsertRows();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool operator <(QPoint p1, QPoint p2)
{
    return p1.x() < p2.x() || p1.y() < p2.y();
}
