#include "RoutesTableWidget.h"

#include <QHeaderView>

RoutesTableWidget::RoutesTableWidget(QWidget *parent) : QTableWidget(parent)
{
    m_map = nullptr;
    setColumnCount(3);
    setHorizontalHeaderLabels(QStringList() << "Source" << "Destination" << "Migration factor");
    setEditTriggers(NoEditTriggers);
    setSelectionBehavior(SelectRows);
    verticalHeader()->hide();
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT());
}

void RoutesTableWidget::setMap(Map *map)
{
    if (m_map != nullptr)
    {
        disconnect(m_map, SIGNAL(routesChanged()), this, SLOT(updateView()));
    }
    m_map = map;
    connect(m_map, SIGNAL(routesChanged()), this, SLOT(updateView()));
    updateView();
}

void RoutesTableWidget::updateView()
{
    const std::map<Deme*, std::map<Deme*, double> > &routes = m_map->routes();
    int rowCount = 0;
    for (auto srcIt = routes.begin(); srcIt != routes.end(); srcIt++)
    {
        for (auto dstIt = srcIt->second.begin(); dstIt != srcIt->second.end(); dstIt++)
        {
            insertRow(rowCount);

            setItem(rowCount, 0, new QTableWidgetItem(QString("%1, %2").arg(srcIt->first->x()).arg(srcIt->first->y())));
            setItem(rowCount, 1, new QTableWidgetItem(QString("%1, %2").arg(dstIt->first->x()).arg(dstIt->first->y())));
            setItem(rowCount, 2, new QTableWidgetItem(QString("%1").arg(dstIt->second)));

            rowCount++;
        }
    }
    // Remove unused rows
    while (this->rowCount() > rowCount)
    {
        removeRow(rowCount);
    }
}

