#ifndef ANIMATION_H
#define ANIMATION_H

#include "scene.h"

class Animation
{
    Animation();
public:

    static void switchToLevelPage(StartScene*);
    static void backToHost(StartScene*);
    static void backToHost(LevelScene*,StartScene*,QGraphicsView*);

    //startscene 与 gamescene 过度
    static void transToGame(StartScene*,LevelScene*,QGraphicsView*);

    //make opacity 淡出淡入
    static QPropertyAnimation* out(QObject* obj,int time,double sVal = 1,double eVal = 0);
    static QPropertyAnimation* in(QObject* obj,int time,double sVal = 0,double eVal = 1);

    static void changeMusic(QUrl);

};



#endif // ANIMATION_H
