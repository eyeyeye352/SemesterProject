#include "hex.h"
#include "myalgorithms.h"

Hex::Hex(QGraphicsItem *parent)
    :QGraphicsPolygonItem(parent),size(0){}

//注意：六边形的（0，0）坐标位于中心。
Hex::Hex(int s, QGraphicsItem *parent):
    QGraphicsPolygonItem(parent),size(s)
{
    QPolygonF hexagon = MyAlgorithms::makeHex(s);
    setPolygon(hexagon);
    setBrush(Qt::blue);
    setPen(QPen(Qt::black,2));

}




