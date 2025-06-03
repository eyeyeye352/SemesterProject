#ifndef HEX_H
#define HEX_H

#include "settings.h"
#include "gameobject.h"

//六边形
class Hex : public QObject , public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    Hex(QGraphicsItem *parent = nullptr);
    //外接圆半径
    Hex(int size,QGraphicsItem* parent = nullptr);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override{}
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override{}
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override{}
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override{
        emit clicked();
    }

signals:
    void clicked();

protected:
    int size;
};



#endif // HEX_H
