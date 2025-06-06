#include "animation.h"
#include "settings.h"
#include "gamesys.h"

Animation::Animation() {}

//切换场景的通用接口
QSequentialAnimationGroup* Animation::changeScene(Scene *origin, Scene *dest,QGraphicsView* view,int loadingTime)
{
    //先用黑布盖住
    QPointer<BlackOverlay> black = new BlackOverlay;
    QPointer<LoadScene> loadScene = new LoadScene;
    black->setZValue(999);

    origin->addItem(black);

    QSequentialAnimationGroup* anime = new QSequentialAnimationGroup;

    //黑幕淡入淡出2次
    anime->addAnimation(Animation::MakeAnime(black,"opacity",500,0,1));
    anime->addAnimation(Animation::MakeAnime(black,"opacity",500,1,0));
    anime->addPause(loadingTime);
    anime->addAnimation(Animation::MakeAnime(black,"opacity",500,0,1));
    anime->addAnimation(Animation::MakeAnime(black,"opacity",500,1,0));


    //暂停期间更换scene到loadscene，然后在换到gamescene
    QObject::connect(anime->animationAt(0),&QPropertyAnimation::finished,[=]{
        loadScene->addItem(black);
        view->setScene(loadScene);
    });
    QObject::connect(anime->animationAt(3),&QPropertyAnimation::finished,[=]{
        dest->addItem(black);
        view->setScene(dest);
    });

    //释放和黑幕资源
    QObject::connect(anime,&QSequentialAnimationGroup::finished,[=]{
        origin->removeItem(black);
        dest->removeItem(black);

        if(black){
            delete black;
            qDebug() << "black过度删除";
        }
        if(loadScene){
            delete loadScene;
            qDebug() << "loadscene过度删除";
        }
    });
    return anime;
}



//打开如设置、排行榜等功能的动画
//打开：加黑幕-tempscene飞入
QPropertyAnimation * Animation::TempPagein(QGraphicsView *bgView, QGraphicsView *tempView)
{
    tempView->setProperty("pos",QPointF(0,Settings::screenHeight));
    tempView->show();

    BlackOverlay* black = new BlackOverlay;
    black->setData(0,"tempBlackOverlay");
    black->setOpacity(0.5);
    black->setZValue(999);
    bgView->scene()->addItem(black);

    QPropertyAnimation* anime = Animation::MakeAnime(tempView,"pos",500,
                                                     QPointF(0,Settings::screenHeight),QPointF(0,0));
    anime->setEasingCurve(QEasingCurve::OutBack);
    return anime;
}

//关闭：tempscene飞出，去掉黑幕）
QPropertyAnimation * Animation::TempPageout(QGraphicsView *bgView, QGraphicsView *tempView)
{
    QPropertyAnimation* anime = Animation::MakeAnime(tempView,"pos",500,
                                                     QPointF(0,0),QPointF(0,Settings::screenHeight));
    //先播动画在删除黑幕。然后在清除tempview
    QObject::connect(anime,&QPropertyAnimation::finished,[bgView,tempView]{

        for (QGraphicsItem* item : bgView->items()) {
            if(item->data(0).toString() == "tempBlackOverlay"){
                bgView->scene()->removeItem(item);
            }
        }

        tempView->hide();
    });
    anime->setEasingCurve(QEasingCurve::InBack);
    return anime;
}



QParallelAnimationGroup *Animation::sideBarAnime(SideBar *sideBar, bool toOpen)
{
    QParallelAnimationGroup * anime = new QParallelAnimationGroup;

    //打开
    if(toOpen){
        anime->addAnimation(Animation::MakeAnime(sideBar,"pos",250,
                                     sideBar->pos(),
                                     sideBar->pos() + QPointF(-Settings::sideBarWidth,0)));
    }
    //关闭
    else{
        anime->addAnimation(Animation::MakeAnime(sideBar,"pos",250,
                                                 sideBar->pos(),
                                                 sideBar->pos() + QPointF(Settings::sideBarWidth,0)));
    }

    anime->addAnimation(Animation::MakeAnime(sideBar->btn,"rotation",250,
                                             sideBar->btn->rotation(),
                                             sideBar->btn->rotation()+180));


    return anime;
}








void Animation::goLevelSelection(StartScene * startScene)
{
    QParallelAnimationGroup* outs = new QParallelAnimationGroup;
    QParallelAnimationGroup* ins = new QParallelAnimationGroup;

    outs->addAnimation(Animation::MakeAnime(startScene->classicBtn,"opacity",500,1,0));
    outs->addAnimation(Animation::MakeAnime(startScene->hexBtn,"opacity",500,1,0));

    ins->addAnimation(Animation::MakeAnime(startScene->backBtn,"opacity",500,0,Settings::funcBtnOriginOpacity));

    for (int i = 0; i < startScene->levels.size(); ++i) {
        ins->addAnimation(Animation::MakeAnime(startScene->levels[i],"opacity",500,0,1));
    }


    QSequentialAnimationGroup * anime = new QSequentialAnimationGroup;
    anime->addAnimation(outs);
    anime->addAnimation(ins);
    anime->start(QAbstractAnimation::DeleteWhenStopped);

}

//选择关卡页面回到起始页面
void Animation::backModeSelection(StartScene * startScene)
{

    QParallelAnimationGroup* outs = new QParallelAnimationGroup;
    QParallelAnimationGroup* ins = new QParallelAnimationGroup;

    outs->addAnimation(Animation::MakeAnime(startScene->backBtn,"opacity",500,Settings::funcBtnOriginOpacity,0));

    for (int i = 0; i < startScene->levels.size(); ++i) {
        outs->addAnimation(Animation::MakeAnime(startScene->levels[i],"opacity",500,1,0));
    }


    ins->addAnimation(Animation::MakeAnime(startScene->classicBtn,"opacity",500,0,1));
    ins->addAnimation(Animation::MakeAnime(startScene->hexBtn,"opacity",500,0,1));


    QSequentialAnimationGroup * anime = new QSequentialAnimationGroup;
    anime->addAnimation(outs);
    anime->addAnimation(ins);
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}






QPropertyAnimation *Animation::MakeAnime(QObject *obj, QByteArray property, int time, QVariant sval, QVariant eval)
{
    QPropertyAnimation* anime = new QPropertyAnimation(obj,property);
    anime->setDuration(time);
    anime->setStartValue(sval);
    anime->setEndValue(eval);
    return anime;
}

QSequentialAnimationGroup *Animation::bgMoveLoopAnime(GameObject *bg)
{

    bg->setTransformOriginPoint(bg->sceneBoundingRect().center());
    QSequentialAnimationGroup* anime = new QSequentialAnimationGroup;
    QPointF curPos = bg->pos();

    //右50 左100 右50上50 下100 上50
    QPropertyAnimation *a1,*a2,*a3,*a4,*a5;
    a1 = MakeAnime(bg,"pos",2000,curPos,curPos+QPointF(50,0));
    curPos.rx() += 50;
    a2 = MakeAnime(bg,"pos",1500,curPos,curPos+QPointF(-100,0));
    curPos.rx() -= 100;
    a3 = MakeAnime(bg,"pos",2000,curPos,curPos+QPointF(50,-50));
    curPos += QPointF(50,-50);
    a4 = MakeAnime(bg,"pos",1500,curPos,curPos+QPointF(0,100));
    curPos.ry() += 100;
    a5 = MakeAnime(bg,"pos",1500,curPos,curPos+QPointF(0,-50));


    a1->setEasingCurve(QEasingCurve::InOutCubic);
    a2->setEasingCurve(QEasingCurve::InOutCubic);
    a3->setEasingCurve(QEasingCurve::InOutCubic);
    a4->setEasingCurve(QEasingCurve::InOutCubic);
    a5->setEasingCurve(QEasingCurve::InOutCubic);


    anime->addAnimation(a1);
    anime->addAnimation(a2);
    anime->addAnimation(a3);
    anime->addAnimation(a4);
    anime->addAnimation(a5);
    anime->setLoopCount(-1);
    return anime;
}

