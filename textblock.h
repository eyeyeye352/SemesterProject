#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "settings.h"
#include "gameobject.h"

//关卡内的文字方块元素
class TextBlock : public GameObject
{
    Q_OBJECT
public:
    TextBlock(QPoint xypos,QString word,QGraphicsItem *parent);
    ~TextBlock();

    void reset();

    void setWord(QString newWord);
    QString Word();
    void setxy(QPoint);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void showGoldRect();
    void hideGoldRect();
    QPoint getXY();

    //static函数，交换两个方块的文字
    static void switchWord(TextBlock* t1,TextBlock* t2);

signals:
    void clicked(TextBlock*);
private:

    QPoint xy;
    QGraphicsTextItem* word;

    //根据变换模式，被选中标记(金色框框)
    QGraphicsRectItem* goldRect;

};



#endif // TEXTBLOCK_H
