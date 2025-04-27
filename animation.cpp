#include "animation.h"
#include "settings.h"

Animation::Animation() {}

void Animation::switchToLevelPage(StartScene * startScene)
{


    QPropertyAnimation * classicBtnAnime = Animation::out(startScene->classicBtn,500);
    QPropertyAnimation * hexBtnAnime = Animation::out(startScene->hexBtn,500);
    QPropertyAnimation * backBtnAnime = Animation::in(startScene->backBtn,500,0,Settings::funcBtnOriginOpacity);

    QParallelAnimationGroup * outs = new QParallelAnimationGroup;
    outs->addAnimation(classicBtnAnime);
    outs->addAnimation(hexBtnAnime);


    QParallelAnimationGroup * ins = new QParallelAnimationGroup;
    ins->addAnimation(backBtnAnime);

    for (int i = 0; i < startScene->levels.size(); ++i) {
        QPropertyAnimation* in = Animation::in(startScene->levels[i],500);
        ins->addAnimation(in);
    }


    QSequentialAnimationGroup * allAnime = new QSequentialAnimationGroup;
    allAnime->addAnimation(outs);
    allAnime->addAnimation(ins);
    allAnime->start(QAbstractAnimation::DeleteWhenStopped);

}

void Animation::backToHost(StartScene * startScene)
{

    QPropertyAnimation * classicBtnAnime = Animation::in(startScene->classicBtn,500);
    QPropertyAnimation * hexBtnAnime = Animation::in(startScene->hexBtn,500);
    QPropertyAnimation * backBtnAnime = Animation::out(startScene->backBtn,500,Settings::funcBtnOriginOpacity,0);

    QParallelAnimationGroup * ins = new QParallelAnimationGroup;
    ins->addAnimation(classicBtnAnime);
    ins->addAnimation(hexBtnAnime);


    QParallelAnimationGroup * outs = new QParallelAnimationGroup;
    outs->addAnimation(backBtnAnime);

    for (int i = 0; i < startScene->levels.size(); ++i) {
        QPropertyAnimation* out = Animation::out(startScene->levels[i],500);
        outs->addAnimation(out);
    }


    QSequentialAnimationGroup * allAnime = new QSequentialAnimationGroup;
    allAnime->addAnimation(outs);
    allAnime->addAnimation(ins);
    allAnime->start(QAbstractAnimation::DeleteWhenStopped);
}

void Animation::backToHost(LevelScene * gamescene, StartScene * startscene, QGraphicsView* view)
{
    BlackOverlay* black = new BlackOverlay;
    LoadScene* loadScene = new LoadScene;
    gamescene->addItem(black);

    QPropertyAnimation* blackin = Animation::in(black,300);
    QPropertyAnimation* blackin2 = Animation::in(black,300);
    QPropertyAnimation* blackout = Animation::out(black,300);
    QPropertyAnimation* blackout2 = Animation::out(black,300);

    QSequentialAnimationGroup* allAnime = new QSequentialAnimationGroup;
    allAnime->addAnimation(blackin);
    allAnime->addAnimation(blackout);
    allAnime->addPause(1000);
    allAnime->addAnimation(blackin2);
    allAnime->addAnimation(blackout2);

    //暂停期间更换scene到loadscene，然后在换到gamescene
    QObject::connect(blackin,&QPropertyAnimation::finished,[=]{
        loadScene->addItem(black);
        view->setScene(loadScene);
    });
    QObject::connect(blackin2,&QPropertyAnimation::finished,[=]{
        startscene->addItem(black);
        view->setScene(startscene);
    });
    QObject::connect(blackout2,&QPropertyAnimation::finished,[=]{
        startscene->removeItem(black);
        gamescene->removeItem(black);
        delete loadScene;
    });

    allAnime->start(QAbstractAnimation::DeleteWhenStopped);
}


void Animation::transToGame(StartScene * startScene, LevelScene * gameScene,QGraphicsView* view)
{

    BlackOverlay* black = new BlackOverlay;
    LoadScene* loadScene = new LoadScene;
    startScene->addItem(black);

    QPropertyAnimation* blackin = Animation::in(black,500);
    QPropertyAnimation* blackin2 = Animation::in(black,500);
    QPropertyAnimation* blackout = Animation::out(black,500);
    QPropertyAnimation* blackout2 = Animation::out(black,500);

    QSequentialAnimationGroup* allAnime = new QSequentialAnimationGroup;
    allAnime->addAnimation(blackin);
    allAnime->addAnimation(blackout);
    allAnime->addPause(2000);
    allAnime->addAnimation(blackin2);
    allAnime->addAnimation(blackout2);

    //暂停期间更换scene到loadscene，然后在换到gamescene
    QObject::connect(blackin,&QPropertyAnimation::finished,[=]{
        loadScene->addItem(black);
        view->setScene(loadScene);
    });
    QObject::connect(blackin2,&QPropertyAnimation::finished,[=]{
        gameScene->addItem(black);
        view->setScene(gameScene);
    });
    QObject::connect(blackout2,&QPropertyAnimation::finished,[=]{
        startScene->removeItem(black);
        gameScene->removeItem(black);
        delete loadScene;
    });

    allAnime->start(QAbstractAnimation::DeleteWhenStopped);
}





QPropertyAnimation *Animation::out(QObject *obj, int time, double sVal, double eVal)
{
    QPropertyAnimation * outAnime = new QPropertyAnimation(obj,"opacity");
    outAnime->setDuration(time);
    outAnime->setStartValue(sVal);
    outAnime->setEndValue(eVal);
    return outAnime;
}


QPropertyAnimation * Animation::in(QObject *obj, int time, double sVal, double eVal)
{
    QPropertyAnimation * inAnime = new QPropertyAnimation(obj,"opacity");
    inAnime->setDuration(time);
    inAnime->setStartValue(sVal);
    inAnime->setEndValue(eVal);
    return inAnime;
}


void Animation::changeMusic(QUrl url)
{
    //音乐淡入淡出，更换
    QMediaPlayer* bgm = MusicPlayer::getMPlayer()->getBgmPlayer();
    QAudioOutput* bgmOutPut = bgm->audioOutput();

    QPropertyAnimation* mFadeout = new QPropertyAnimation(bgmOutPut,"volume");
    QPropertyAnimation* mFadein = new QPropertyAnimation(bgmOutPut,"volume");

    mFadeout->setDuration(300);
    mFadeout->setStartValue(Settings::musicVol);
    mFadeout->setEndValue(0);

    mFadein->setDuration(300);
    mFadein->setStartValue(0);
    mFadein->setEndValue(Settings::musicVol);

    QSequentialAnimationGroup * anime = new QSequentialAnimationGroup;
    anime->addAnimation(mFadeout);
    anime->addPause(1000);
    anime->addAnimation(mFadein);

    QObject::connect(mFadeout,&QPropertyAnimation::finished,[bgm,url]{
        bgm->setSource(QUrl(url));
    });
    QObject::connect(mFadein,&QPropertyAnimation::finished,[bgm,url]{
        bgm->setLoops(QMediaPlayer::Infinite);
        bgm->play();
    });

    anime->start(QAbstractAnimation::DeleteWhenStopped);
}

