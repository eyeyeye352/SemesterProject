#include "gamesidebar.h"
#include "animation.h"


SideBar::SideBar(QGraphicsItem *parent)
    :GameObject(parent),
    opened(false)
{
    //展开按钮
    btn = new FunctionBtn(QPixmap(":/item/src/item/sideBarBtn.png"),this);
    btn->setPos(0,(Settings::screenHeight - btn->boundingRect().height())/2);
    btn->setTransformOriginPoint(btn->boundingRect().center());

    bg = new QGraphicsRectItem(btn->boundingRect().right(),0,
                               Settings::sideBarWidth,Settings::screenHeight,this);
    bg->setBrush(Qt::black);
    bg->setOpacity(0.6);

    //打开或关闭侧边栏
    connect(btn,&GameBtn::clicked,this,&SideBar::openOrCloseSideBar);

}

QRectF SideBar::boundingRect() const
{
    return bg->boundingRect();
}


void SideBar::openOrCloseSideBar()
{
    opened = !opened;
    Animation::sideBarAnime(this,opened)->start(QAbstractAnimation::DeleteWhenStopped);
}








MySideBar::MySideBar(QGraphicsItem *parent):
    SideBar(parent)
{
    //变换模式选择icons(classicMode)
    TranslateIcons* rowt = new TranslateIcons(QPixmap(":/transIcons/src/item/translatesIcon/rowTranslate.png"),TranslateIcons::ROWS,this);
    TranslateIcons* colt = new TranslateIcons(QPixmap(":/transIcons/src/item/translatesIcon/colTranslate.png"),TranslateIcons::COLS,this);
    TranslateIcons* crosst = new TranslateIcons(QPixmap(":/transIcons/src/item/translatesIcon/crossTranslate.png"),TranslateIcons::CROSS,this);
    TranslateIcons* hexlinet = new TranslateIcons(QPixmap(":/transIcons/src/item/translatesIcon/rotateTranslate.png"),TranslateIcons::ROTATE,this);
    TranslateIcons* hext = new TranslateIcons(QPixmap(":/transIcons/src/item/translatesIcon/hexTranslate.png"),TranslateIcons::HEX,this);

    classicIcons.append(rowt);
    classicIcons.append(colt);
    classicIcons.append(crosst);

    hexIcons.append(hexlinet);
    hexIcons.append(hext);

    //案件排版

    for (int n = 0; n < classicIcons.size(); ++n) {
        qDebug() << n;
        classicIcons[n]->setPos(
            /* x:  */ btn->boundingRect().width() + (boundingRect().width() - classicIcons[n]->boundingRect().width())/2,
            /* y:  */ Settings::translateIconInterval * (n+1) + classicIcons[n]->boundingRect().height() * n);


        QObject::connect(classicIcons[n],&TranslateIcons::selected,[this,n](TranslateIcons::Type t){
            makeIconGold(classicIcons[n]);
            emit SelectTransType(t);
        });
    }
    for (int n = 0; n < hexIcons.size(); ++n) {
        hexIcons[n]->setPos(
            /* x:  */ btn->boundingRect().width() + (boundingRect().width() - hexIcons[n]->boundingRect().width())/2,
            /* y:  */ Settings::translateIconInterval * (n+1) + hexIcons[n]->boundingRect().height() * n);

        QObject::connect(hexIcons[n],&TranslateIcons::selected,[this,n](TranslateIcons::Type t){
            makeIconGold(hexIcons[n]);
            emit SelectTransType(t);
        });
    }
    qDebug() << "sidebar 无异常" ;


}

void MySideBar::setGameMode(int m)
{
    gameMode = m;
    for (TranslateIcons* t : hexIcons) {
        t->removeGold();
        gameMode == CLASSIC ? t->hide() : t->show();
    }
    for (TranslateIcons* t : classicIcons) {
        t->removeGold();
        gameMode == HEX ? t->hide() : t->show();
    }

}

void MySideBar::makeIconGold(TranslateIcons * target)
{

    if(gameMode == CLASSIC){
        for (TranslateIcons* t : classicIcons) {
            if(t == target){
                t->makeGold();
            }else{
                t->removeGold();
            }
        }
    }
    else if(gameMode == HEX){
        for (TranslateIcons* t : hexIcons) {
            if(t == target){
                t->makeGold();
            }else{
                t->removeGold();
            }
        }
    }
}
