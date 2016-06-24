#ifndef ROUTESTABLEWIDGET_H
#define ROUTESTABLEWIDGET_H

#include <QTableWidget>
#include "models/map.h"

class RoutesTableWidget : public QTableWidget
{
    Q_OBJECT

public:

    RoutesTableWidget(QWidget *parent = nullptr);
    void setMap(Map *map);

public slots:

    void updateView();

private:

    Map *m_map;
};

#endif // ROUTESTABLEWIDGET_H
