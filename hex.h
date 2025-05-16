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




/*关卡内的六边形文字方块
 *
 * 六边形坐标系：
 * 共有r、x，y三个数，
 * 中心六边形为0，0，0，
 * r代表到中心六边形的距离，
 * x以正右为正向，y以左上为正向，
 * 该坐标可以唯一的表示大六边形中的一个小六边形，
 * 相邻六边形的相对位置按逆时针顺序是（右、右上、左上、左、左下、右下）。
 */
struct HPoint{
    HPoint():x(0),y(0),r(0){}
    HPoint(int x,int y,int r):x(x),y(y),r(r){}
    int x;
    int y;
    int r;

    bool operator==(HPoint h){
        return (x == h.x) &&
               (y == h.y) &&
               (r == h.r);
    }
};




#endif // HEX_H
