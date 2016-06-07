#ifndef MAGICSELECTTOOL_H
#define MAGICSELECTTOOL_H

#include "AbstractSelectTool.h"
#include "../models/deme.h"

class MagicSelectTool : public AbstractSelectTool
{
public:

    MagicSelectTool();

    bool handleMousePress(QMouseEvent *event);
    bool handleMouseMove(QMouseEvent *event);
    bool handleMouseRelease(QMouseEvent *event);
    void draw(QPainter &painter) {}

private:

    template<typename T>
    bool selectDemes(T value, T (Deme::*methodPtr)() const);
};

#endif // MAGICSELECTTOOL_H
