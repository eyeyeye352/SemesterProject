#include "scene.h"
#include "animation.h"


Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
{}

StartScene::StartScene(QObject *parent):Scene{parent}
{
    setSceneRect(QRectF(0,0,Settings::screenWidth,Settings::screenHeight));
    bg1 = new QGraphicsPixmapItem(QPixmap(":/background/src/background/start_background.png"));
    bg2 = new QGraphicsPixmapItem(QPixmap(":/background/src/background/start_background.png"));
    bg2->setX(Settings::screenWidth - 1);

    addItem(bg1);
    addItem(bg2);

    //btns
    classicBtn = new ClassicBtn();
    hexBtn = new HexBtn();

    classicBtn->setScale(2);
    hexBtn->setScale(2);

    classicBtn->setPos(Settings::screenWidth/2 - classicBtn->sceneBoundingRect().width()/2,Settings::classicBtnY);
    hexBtn->setPos(Settings::screenWidth/2 - hexBtn->sceneBoundingRect().width()/2,Settings::hexBtnY);

    addItem(classicBtn);
    addItem(hexBtn);


    //title
    title = new QGraphicsPixmapItem(QPixmap(":/item/src/item/title.png"));
    title->setScale(1.3);
    title->setPos((Settings::screenWidth - title->sceneBoundingRect().width())/2,50);
    addItem(title);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"));
    rankBtn = new FunctionBtn(QPixmap(":/item/src/item/rankIcon.png"));
    createModeBtn = new FunctionBtn(QPixmap(":/item/src/item/createModeIcon.png"));
    backBtn = new FunctionBtn(QPixmap(":/item/src/item/backIcon.png"));

    settingBtn->setPos(5,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    rankBtn->setPos(10 + settingBtn->sceneBoundingRect().width() , Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    createModeBtn->setPos(15 + 2*settingBtn->sceneBoundingRect().width() ,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    backBtn->setPos(20 + 3*settingBtn->sceneBoundingRect().width() ,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    backBtn->setOpacity(0);

    addItem(settingBtn);
    addItem(rankBtn);
    addItem(createModeBtn);
    addItem(backBtn);


    //levels
    for (int n = 1; n <= 5; ++n) {
        LevelBlock * l = new LevelBlock(n);
        //排版
        l->moveBy(40*n + l->sceneBoundingRect().width()*(n-1),(Settings::screenHeight - l->sceneBoundingRect().height())/2);
        levels.push_back(l);
        l->setOpacity(0);
        addItem(l);
    }



}

StartScene::~StartScene(){}


void StartScene::moveBG()
{

    bg1->moveBy(-Settings::backgroundMoveSpeed,0);
    bg2->moveBy(-Settings::backgroundMoveSpeed,0);

    // - 1 优化边界闪烁问题
    if(bg1->sceneBoundingRect().right() < 0){
        bg1->setX(bg2->sceneBoundingRect().right() - 1);
    }
    if(bg2->sceneBoundingRect().right() < 0){
        bg2->setX(bg1->sceneBoundingRect().right() - 1);
    }

}




//经典模式按钮
ClassicBtn::ClassicBtn(QGraphicsItem *parent):
    QGraphicsPixmapItem{QPixmap(":/item/src/item/classic.png"),parent}
{
    setAcceptHoverEvents(true);
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
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(scale()*0.9);
    QPointF dir = (center - sceneBoundingRect().center());
    moveBy(dir.x(),dir.y());
}

void ClassicBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(2);
    QPointF dir = center - sceneBoundingRect().center();
    moveBy(dir.x(),dir.y());
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked();


}

//hex模式按钮设置
HexBtn::HexBtn(QGraphicsItem *parent):
    QGraphicsPixmapItem{QPixmap(":/item/src/item/hexagon.png"),parent}
{
    setAcceptHoverEvents(true);
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
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(scale()*0.9);
    QPointF dir = (center - sceneBoundingRect().center());
    moveBy(dir.x(),dir.y());
}

void HexBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(2);
    QPointF dir = center - sceneBoundingRect().center();
    moveBy(dir.x(),dir.y());
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked();
}





FunctionBtn::FunctionBtn(QPixmap pixmap, QGraphicsItem *parent):
    QGraphicsPixmapItem{pixmap,parent}
{
    setAcceptHoverEvents(true);
    setOpacity(0.5);
}

void FunctionBtn::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setOpacity(1);
}

void FunctionBtn::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setOpacity(0.5);
}

void FunctionBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(0.9);
    QPointF dir = (center - sceneBoundingRect().center());
    moveBy(dir.x(),dir.y());
}

void FunctionBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //保证中心缩放后位置不变
    QPointF center = sceneBoundingRect().center();
    setScale(1);
    QPointF dir = center - sceneBoundingRect().center();
    moveBy(dir.x(),dir.y());
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked();
}


