#include "textblock.h"



TextBlock::TextBlock(QGraphicsItem *parent)
    : GameObject{parent}
{
    setPixmap(QPixmap(":/item/src/item/wordBlock.png"));
}
