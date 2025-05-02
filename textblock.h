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
    void setxy(QPoint);

private:

    QPoint xy;
    QGraphicsTextItem* word;
};



#endif // TEXTBLOCK_H
