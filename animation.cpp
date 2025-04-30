#include "animation.h"
#include "settings.h"

Animation::Animation() {}

//切换场景的通用接口
void Animation::changeScene(Scene *origin, Scene *dest,QGraphicsView* view,int loadingTime)
{
    //先用黑布盖住
    BlackOverlay* black = new BlackOverlay;
    LoadScene* loadScene = new LoadScene;
    origin->addItem(black);

    QSequentialAnimationGroup* anime = new QSequentialAnimationGroup;

    //黑幕淡入淡出2次
    anime->addAnimation(Animation::MakeAnime(black,"opacity",300,0,1));
    anime->addAnimation(Animation::MakeAnime(black,"opacity",300,1,0));
    anime->addPause(loadingTime);
    anime->addAnimation(Animation::MakeAnime(black,"opacity",300,0,1));
    anime->addAnimation(Animation::MakeAnime(black,"opacity",300,1,0));


    //暂停期间更换scene到loadscene，然后在换到gamescene
    QObject::connect(anime->animationAt(0),&QPropertyAnimation::finished,[=]{
        loadScene->addItem(black);
        view->setScene(loadScene);
    });
    QObject::connect(anime->animationAt(3),&QPropertyAnimation::finished,[=]{
        dest->addItem(black);
        view->setScene(dest);
        delete loadScene;
    });
    anime->start(QAbstractAnimation::DeleteWhenStopped);
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






void Animation::changeMusic(QUrl url)
{
    //音乐淡入淡出，更换
    QMediaPlayer* bgm = MusicPlayer::getMPlayer()->bgm;

    QSequentialAnimationGroup * anime = new QSequentialAnimationGroup;
    anime->addAnimation(Animation::MakeAnime(bgm->audioOutput(),"volumn",300,Settings::musicVol,0));
    anime->addPause(1000);
    anime->addAnimation(Animation::MakeAnime(bgm->audioOutput(),"volumn",300,0,Settings::musicVol));



    QObject::connect(anime->animationAt(0),&QPropertyAnimation::finished,[bgm,url]{
        bgm->setSource(QUrl(url));
    });
    QObject::connect(anime->animationAt(1),&QPropertyAnimation::finished,[bgm,url]{
        bgm->setLoops(QMediaPlayer::Infinite);
        bgm->play();
    });

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

