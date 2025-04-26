#include "levels.h"

LevelBlock::LevelBlock(int l, QGraphicsItem *parent)
    :QGraphicsPixmapItem{QPixmap(":/item/src/item/levelBlock.png"),parent},levelNum(l)
{
    setScale(1);
    QGraphicsTextItem * text = new QGraphicsTextItem(QString::number(levelNum),this);

    //排版
    text->setFont(QFont("Tw Cen MT Condensed Extra Bold",20));
    text->setPos(
        (sceneBoundingRect().width() - text->sceneBoundingRect().width())/2,
        (sceneBoundingRect().height() - text->sceneBoundingRect().height())/2
    );

    setAcceptHoverEvents(true);
}

void LevelBlock::loadFile(QString filepath)
{
    QFile * file = new QFile(filepath);
    info = file->readAll();
    delete file;
}

QString LevelBlock::getInfo() const
{
    return info;
}

void LevelBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsColorizeEffect* fx = new QGraphicsColorizeEffect(this);
    fx->setColor(Qt::black);
    fx->setStrength(1);
    setGraphicsEffect(fx);
}

void LevelBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setGraphicsEffect(nullptr);
}

void LevelBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(0.9);
    QPointF dir = (center - sceneBoundingRect().center());
    moveBy(dir.x(),dir.y());
}

void LevelBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(1);
    QPointF dir = center - sceneBoundingRect().center();
    moveBy(dir.x(),dir.y());
    MusicPlayer::getMPlayer()->clickSound();
    emit selected(levelNum);
}



