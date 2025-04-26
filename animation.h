#ifndef ANIMATION_H
#define ANIMATION_H

#include "levels.h"

#include "scene.h"

class Animation
{
    Animation();
public:

    static void switchToLevelPage(StartScene*);
    static void backToHost(StartScene*);

    //make opacity 淡出淡入
    static QPropertyAnimation* out(QObject* obj,int time);
    static QPropertyAnimation* in(QObject* obj,int time);

};

#endif // ANIMATION_H
