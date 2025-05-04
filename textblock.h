#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "settings.h"
#include "gameobject.h"

//关卡内的文字方块元素
class TextBlock : public GameObject
{
    Q_OBJECT
public:
    TextBlock(QPoint xypos = {0,0},QString word = "",QGraphicsItem *parent = nullptr);
    ~TextBlock();

    void setWord(QString newWord);
    QString Word();
    void setxy(QPoint);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    bool isTarget();
    void setTarget(bool);
    QPoint getXY();

signals:
    void clicked(TextBlock*);
private:

    QPoint xy;
    QGraphicsTextItem* word;

    //点击事件标记
    bool isClicked;
    //根据变换模式，被选中标记(以及金色框框)
    bool target;
    QGraphicsRectItem* goldRect;

};



#endif // TEXTBLOCK_H
