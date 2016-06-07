#ifndef MAGICSELECTTOOL_H
#define MAGICSELECTTOOL_H

#include "AbstractSelectTool.h"

class MagicSelectTool : public AbstractSelectTool
{
public:

    MagicSelectTool();

    bool handleMousePress(QMouseEvent *event);
    bool handleMouseMove(QMouseEvent *event);
    bool handleMouseRelease(QMouseEvent *event);
    void draw(QPainter &painter) {}

private:


};

#endif // MAGICSELECTTOOL_H
