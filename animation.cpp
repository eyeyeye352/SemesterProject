#include "animation.h"

Animation::Animation() {}

void Animation::switchToLevelPage(StartScene * startScene)
{


    QPropertyAnimation * classicBtnAnime = Animation::out(startScene->classicBtn,500);
    QPropertyAnimation * hexBtnAnime = Animation::out(startScene->hexBtn,500);
    QPropertyAnimation * backBtnAnime = Animation::in(startScene->backBtn,500);
    backBtnAnime->setEndValue(0.5);

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
    QPropertyAnimation * backBtnAnime = Animation::out(startScene->hexBtn,500);
    backBtnAnime->setStartValue(0.5);
    backBtnAnime->setEndValue(0);

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


QPropertyAnimation * Animation::out(QObject *obj, int time)
{
    QPropertyAnimation * outAnime = new QPropertyAnimation(obj,"opacity");
    outAnime->setDuration(time);
    outAnime->setStartValue(1);
    outAnime->setEndValue(0);
    return outAnime;
}

QPropertyAnimation * Animation::in(QObject *obj, int time)
{
    QPropertyAnimation * inAnime = new QPropertyAnimation(obj,"opacity");
    inAnime->setDuration(time);
    inAnime->setStartValue(0);
    inAnime->setEndValue(1);
    return inAnime;
}
