#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "settings.h"
#include "gameobject.h"

//关卡内的文字方块元素
class TextBlock : public GameObject
{
    Q_OBJECT
public:
    TextBlock(QGraphicsItem *parent = nullptr);

    QPoint xy;
    QString word;
};



#endif // TEXTBLOCK_H
