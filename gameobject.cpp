#include "gameobject.h"
#include "musicplayer.h"


GameObject::GameObject(QGraphicsItem *parent):QGraphicsPixmapItem{parent}{}

GameObject::GameObject(QPixmap pixmap, QGraphicsItem *parent)
    :QGraphicsPixmapItem{pixmap,parent}{}


//按键可接受hover事件。
GameBtn::GameBtn(QPixmap pixmap, QGraphicsItem *parent)
    :GameObject(pixmap,parent)
{
    setAcceptHoverEvents(true);
}

//缩放并调整按键位置，保证中心不变
void GameBtn::resizing(double scale)
{
    QPointF center = sceneBoundingRect().center();
    setScale(scale);
    QPointF dir = (center - sceneBoundingRect().center());
    moveBy(dir.x(),dir.y());
}



//经典模式按钮
ClassicBtn::ClassicBtn(QGraphicsItem *parent):
    GameBtn{QPixmap(":/item/src/item/classic.png"),parent}{}


void ClassicBtn::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    MusicPlayer::getMPlayer()->startBtnSound();
    setPixmap(QPixmap(":/item/src/item/classic_hover.png"));
}

void ClassicBtn::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setPixmap(QPixmap(":/item/src/item/classic.png"));
}

void ClassicBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::startBtnOriginScale*0.9);
}

void ClassicBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::startBtnOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked(Mode::CLASSIC);
}


//hex模式按钮
HexBtn::HexBtn(QGraphicsItem *parent):
    GameBtn{QPixmap(":/item/src/item/hexagon.png"),parent}{}

void HexBtn::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    MusicPlayer::getMPlayer()->startBtnSound();
    setPixmap(QPixmap(":/item/src/item/hexagon_hover.png"));
}

void HexBtn::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setPixmap(QPixmap(":/item/src/item/hexagon.png"));
}

void HexBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::startBtnOriginScale*0.9);
}

void HexBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::startBtnOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked(Mode::HEX);
}





FunctionBtn::FunctionBtn(QPixmap pixmap, QGraphicsItem *parent):
    GameBtn{pixmap,parent}
{
    setOpacity(Settings::funcBtnOriginOpacity);
}

void FunctionBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::funcBtnOriginScale*0.9);
}

void FunctionBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::funcBtnOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked();
}




LevelBlock::LevelBlock(int l, QGraphicsItem *parent)
    :GameBtn{QPixmap(":/item/src/item/levelBlock.png"),parent},
    levelNum(l)
{
    setScale(Settings::levelBlockOriginScale);

    QGraphicsTextItem * text = new QGraphicsTextItem(QString::number(levelNum),this);

    //关卡数字字体
    text->setFont(QFont("Tw Cen MT Condensed Extra Bold",20));
    text->setPos(
        (sceneBoundingRect().width() - text->sceneBoundingRect().width())/2,
        (sceneBoundingRect().height() - text->sceneBoundingRect().height())/2
        );

}

void LevelBlock::loadFile(QString filepath)
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);


    info = file.readAll();

    //调试用
    qDebug() << "Level" << levelNum << ": " << info;
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
    resizing(Settings::levelBlockOriginScale*0.9);
}

void LevelBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::levelBlockOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit selected(info);
}








//黑幕
BlackOverlay::BlackOverlay(QGraphicsItem *parent):
    GameObject{QPixmap(":/background/src/background/BlackOverLay.png"),parent}
{
    setOpacity(0);
}
