#ifndef ANIMATION_H
#define ANIMATION_H

#include "scene.h"

class Animation
{
    Animation();
public:

    static void goLevelSelection(StartScene*);
    static void backModeSelection(StartScene*);

    static QSequentialAnimationGroup* changeScene(Scene * origin,Scene * dest,QGraphicsView* view,int loadingTime);

    //打开、关闭temp页面的动画，返回整个动画组（打开：加黑幕-tempscene飞入，关闭：tempscene飞出，去掉黑幕）
    static QPropertyAnimation * TempPagein(QGraphicsView* bgView,QGraphicsView* tempView);
    static QPropertyAnimation * TempPageout(QGraphicsView* bgView,QGraphicsView* tempView);

    //打开/关闭sidebar
    static QParallelAnimationGroup* sideBarAnime(SideBar* sideBar,bool toOpen);


    static QPropertyAnimation* MakeAnime(QObject* obj,QByteArray property,int time,QVariant sval,QVariant eval);

    //createmode的背景移动效果
    static QSequentialAnimationGroup* bgMoveLoopAnime(GameObject* bg);


};



#endif // ANIMATION_H
