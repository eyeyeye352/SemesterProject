#ifndef OTHERITEMS_H
#define OTHERITEMS_H

#include "gameobject.h"
#include "settings.h"

//可拖动的图形
class DragableObj : public GameObject{
public:
    DragableObj(QPixmap pixmap,QGraphicsItem *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void setOnlyDragX(bool newOnlyDragX);
    void setOnlyDragY(bool newOnlyDragY);
    void setRange(QRectF);

private:
    bool isDragging;
    QPointF mouseOriginPos;

    bool onlyDragX;
    bool onlyDragY;

    //movable range
    QRectF range;
};



//黑幕转场特效
class BlackOverlay : public GameObject{

    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    BlackOverlay(QGraphicsItem *parent = nullptr);
};





//一组三角形按钮+数字文本（数值调整器）
class ValSets : public GameObject{

    Q_OBJECT

public:
    ValSets(QGraphicsItem *parent = nullptr);
    ~ValSets();

    FunctionBtn* upBtn, *downBtn;
    QGraphicsTextItem* valText;

    QRectF boundingRect() const override;

signals:
    void valueChanged(double);

private:
    double val;
};

#endif // OTHERITEMS_H
