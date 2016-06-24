#ifndef ROUTESTABLEMODEL_H
#define ROUTESTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QPoint>

class Map;

class RoutesTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RoutesTableModel(Map *map, QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    const QList<QPair<QPoint, QPoint> > & getAll() const;
    const QPair<QPoint, QPoint> & getAt(int index) const;
    double routeFactor(const QPoint &from, const QPoint &to) const;
    double routeFactorAt(int index) const;
    void set(const QPoint &from, const QPoint &to, double factor);
    bool remove(const QPoint &from, const QPoint &to);

private slots:

    void handleMapWidthChange(int newWidth);
    void handleMapHeightChange(int newHeight);

private:

    void _removeRoute(int index);
    void _addRoute(QPair<QPoint,QPoint> &route, double factor);

    Map *m_map;
    int m_mapWidth;
    int m_mapHeight;
    QList<QPair<QPoint, QPoint> > m_routes;
    QList<double> m_factors;
    QMap<QPoint, QList<int> > m_from;
    QMap<QPoint, QList<int> > m_to;
};

bool operator <(QPoint p1, QPoint p2);

#endif // ROUTESTABLEMODEL_H
