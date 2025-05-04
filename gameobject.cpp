#include "gameobject.h"
#include "musicplayer.h"
#include "otheritems.h"


GameObject::GameObject(QGraphicsItem *parent):QGraphicsPixmapItem{parent}{}

GameObject::GameObject(QPixmap pixmap, QGraphicsItem *parent)
    :QGraphicsPixmapItem{pixmap,parent}
{}


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
    GameBtn{QPixmap(":/item/src/item/classic.png"),parent}
{
    resizing(Settings::startBtnOriginScale);
}


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
    //模式按键大小调整
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
    GameBtn{QPixmap(":/item/src/item/hexagon.png"),parent}
{
    resizing(Settings::startBtnOriginScale);
}

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
{}


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




LevelSelectBlock::LevelSelectBlock(int l, QGraphicsItem *parent)
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



void LevelSelectBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsColorizeEffect* fx = new QGraphicsColorizeEffect(this);
    fx->setColor(Qt::black);
    fx->setStrength(0.8);
    setGraphicsEffect(fx);
}

void LevelSelectBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setGraphicsEffect(nullptr);
}

void LevelSelectBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::levelBlockOriginScale*0.9);
}

void LevelSelectBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::levelBlockOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit selected(levelNum);
}







TranslateIcons::TranslateIcons(QPixmap pixmap, Type t,QGraphicsItem *parent):
    GameObject(pixmap,parent),
    type(t)
{
    goldRect = new QGraphicsRectItem(boundingRect(),this);
    goldRect->setPen(QPen(QColor(255,215,0),2));
    goldRect->hide();
}

void TranslateIcons::makeGold()
{
    goldRect->show();
}

void TranslateIcons::removeGold()
{
    goldRect->hide();
}

void TranslateIcons::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit selected(type);
}





